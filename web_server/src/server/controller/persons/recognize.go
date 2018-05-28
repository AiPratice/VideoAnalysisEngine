package persons

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

//响应识别结果
type retRecognizeResult struct {
	PersonID uint32 `json:"person_id"`
	Name     string `json:"name"`
	Top      int32  `json:"top"`
	Left     int32  `json:"left"`
	Width    int32  `json:"width"`
	Height   int32  `json:"height"`
}

//Recognize 人脸识别
func Recognize(c echo.Context) error {
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

	client := vrs_rpc.GetFaceClientInstance()
	req := vrs_rpc.DetectPersonInImgRequest{Path: distPath}
	stream, err := client.DetectPersonInImg(context.Background(), &req)
	if err != nil {
		log.Printf("%v %v\n", client, err)
		return c.String(http.StatusBadRequest, "")
	}

	retResults := make([]retRecognizeResult, 0, 2)

	for {
		result, err := stream.Recv()
		if err == io.EOF {
			break
		} else if err != nil {
			log.Printf("%v %v\n", client, err)
			return c.String(http.StatusBadRequest, "")
		}

		var retResult retRecognizeResult
		person := entity.Person{PersonID: result.GetPersonID()}
		retResult.PersonID = person.PersonID
		if person.PersonID == 0 {
			person.Name = "未知"
		} else {
			db.Where("person_id = ?", result.GetPersonID()).Find(&person)
		}
		retResult.Name = person.Name
		retResult.Top = result.GetTop()
		retResult.Left = result.GetLeft()
		retResult.Width = result.GetRight() - result.GetLeft()
		retResult.Height = result.GetBottom() - result.GetTop()

		retResults = append(retResults, retResult)
	}

	os.Remove(distPath) //删除临时上传的图像查询文件
	return c.JSON(http.StatusOK, retResults)
}
