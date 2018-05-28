package videos

import (
	"context"
	"crypto/md5"
	"database/sql"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"path"
	"strings"
	"time"
	"vrs_web_server/src/config"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/vrs_rpc"

	"github.com/labstack/echo"
)

//返回浏览器的搜索结果
type retSearchResult struct {
	FrameID uint64  `json:"frame_id"`
	VideoID uint32  `json:"video_id"`
	Score   float32 `json:"score"`
	Second  uint32  `json:"second"`
}

//标签搜索返回结果
type retTagSearchResult struct {
	FrameID uint64 `json:"frame_id"`
	VideoID uint32 `json:"video_id"`
	Second  uint32 `json:"second"`
}

//标签搜索请求
type reqTagSearch struct {
	//Objects
	Objects []string `json:"objects"`
	People  []string `json:"people"`
}

//Search 根据用户上传的图片返回搜索结果
func Search(c echo.Context) error {
	/*文件上传处理*/
	file, err := c.FormFile("file")
	if err != nil {
		return err
	}

	src, err := file.Open()
	if err != nil {
		return err
	}
	defer src.Close()

	md5Ctx := md5.New()
	md5Ctx.Write([]byte(file.Filename + time.Now().Local().String()))
	fileExt := path.Ext(file.Filename)
	newFilename := hex.EncodeToString(md5Ctx.Sum(nil))
	distPath := fmt.Sprintf("%s/%s%s", config.TEMPDirPath, newFilename, fileExt)
	fmt.Printf("distPath:%s\n", distPath)
	dst, err := os.Create(distPath)
	if err != nil {
		fmt.Println("文件创建失败")
		return err
	}
	defer dst.Close()

	_, err = io.Copy(dst, src)
	if err != nil {
		return err
	}
	/*文件上传处理*/

	db := model.GetDb()
	defer db.Close()
	results := make([]retSearchResult, 0, 100)

	client := vrs_rpc.GetVrsClientInstance()
	req := vrs_rpc.SearchRequest{ImgPath: distPath}
	stream, err := client.Search(context.Background(), &req)
	if err != nil {
		log.Printf("%v %v\n", client, err)
		return c.String(http.StatusBadRequest, "")
	}

	for {
		result, err := stream.Recv()
		if err == io.EOF {
			break
		} else if err != nil {
			log.Printf("%v %v\n", client, err)
			return c.String(http.StatusBadRequest, "")
		}

		//组合搜索结果
		var frame entity.Frame
		var rSearchResult retSearchResult
		db.Where("frame_id = ?", result.GetFrameID()).Find(&frame)
		rSearchResult.FrameID = frame.FrameID
		rSearchResult.VideoID = frame.VideoID
		rSearchResult.Score = result.GetScore()
		rSearchResult.Second = frame.Second
		results = append(results, rSearchResult)
	}

	os.Remove(distPath) //删除临时上传的图像查询文件
	return c.JSON(http.StatusOK, results)
}

//TagSearch 根据用户上传的图片返回搜索结果
func TagSearch(c echo.Context) error {
	req := new(reqTagSearch)
	var err error
	if err = c.Bind(req); err != nil {
		return err
	}

	db := model.GetDb()
	defer db.Close()

	lenObjects := len(req.Objects)
	lenPeople := len(req.People)

	strObjects := strings.Join(req.Objects, ",")
	strPeople := strings.Join(req.People, ",")

	results := make([]retTagSearchResult, 0, 100)

	var rows *sql.Rows
	if lenObjects == 0 && lenPeople > 0 {
		sql := `
		SELECT DISTINCT ON (f.frame_id) f.*
		FROM frames f
			JOIN frame_faces ff on ff.frame_id = f.frame_id
		GROUP BY f.frame_id
		HAVING array_agg(ff.person_id) @> array[%s]
		`
		rows, err = db.Raw(fmt.Sprintf(sql, strPeople)).Rows()
		log.Println("a")
	} else if lenObjects > 0 && lenPeople == 0 {
		sql := `
		SELECT DISTINCT ON (f.frame_id) f.*
		FROM frames f
			JOIN frame_objects fo on fo.frame_id = f.frame_id
		GROUP BY f.frame_id
		HAVING array_agg(fo.object_class_id) @> array[%s]
		`
		rows, err = db.Raw(fmt.Sprintf(sql, strObjects)).Rows()
		log.Println("b")
	} else if lenObjects > 0 && lenPeople > 0 {
		sql := `
		SELECT DISTINCT ON (f.frame_id) f.*
		FROM frames f
			JOIN frame_faces ff on ff.frame_id = f.frame_id
			JOIN frame_objects fo on fo.frame_id = f.frame_id
		GROUP BY f.frame_id
		HAVING array_agg(ff.person_id) @> array[%s] AND array_agg(fo.object_class_id) @> array[%s]
		`
		rows, err = db.Raw(fmt.Sprintf(sql, strPeople, strObjects)).Rows()
		log.Println("c")
	}

	if err != nil {
		log.Println(err)
		return c.NoContent(http.StatusBadRequest)
	}

	if rows == nil {
		return c.JSON(http.StatusOK, nil)
	}

	defer rows.Close()
	for rows.Next() {
		frame := entity.Frame{}
		db.ScanRows(rows, &frame)

		db.Find(&frame)

		result := retTagSearchResult{
			FrameID: frame.FrameID,
			VideoID: frame.VideoID,
			Second:  frame.Second,
		}
		results = append(results, result)
	}

	return c.JSON(http.StatusOK, results)
}
