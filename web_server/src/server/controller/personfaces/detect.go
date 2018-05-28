package personfaces

import (
	"context"
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"path"
	"time"
	"vrs_web_server/src/config"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/vrs_rpc"

	"github.com/labstack/echo"
)

//返回浏览器的检测结果
type retDetectInImgResult struct {
	PersonID uint32
	Name     string
	Top      int32
	Right    int32
	Bottom   int32
	Left     int32
}

//DetectInImg 在图像中检测人脸
func DetectInImg(c echo.Context) error {
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

	client := vrs_rpc.GetFaceClientInstance()
	req := vrs_rpc.DetectPersonInImgRequest{Path: distPath}

	stream, err := client.DetectPersonInImg(context.Background(), &req)
	if err != nil {
		log.Printf("%v %v\n", client, err)
		return c.String(http.StatusBadRequest, "")
	}

	results := make([]retDetectInImgResult, 0, 1)
	db := model.GetDb()
	defer db.Close()
	for {
		result, err := stream.Recv()
		if err == io.EOF {
			break
		} else if err != nil {
			log.Printf("%v %v\n", client, err)
			return c.String(http.StatusBadRequest, "")
		}

		var person entity.Person
		db.Where("person_id = ?", result.GetPersonID()).Find(&person)
		rDetectResult := retDetectInImgResult{
			PersonID: result.GetPersonID(),
			Name:     person.Name,
			Top:      result.GetTop(),
			Right:    result.GetRight(),
			Bottom:   result.GetBottom(),
			Left:     result.GetLeft(),
		}
		results = append(results, rDetectResult)
	}

	os.Remove(distPath)
	return c.JSON(http.StatusOK, results)
}
