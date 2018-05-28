package objects

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
	"vrs_web_server/src/vrs_rpc"

	"github.com/labstack/echo"
)

type retDetectInImgResult struct {
	ClassID   uint32  `json:"class_id"`
	ClassName string  `json:"class_name"`
	Prob      float32 `json:"prob"`
	Left      int32   `json:"left"`
	Top       int32   `json:"top"`
	Width     int32   `json:"width"`
	Height    int32   `json:"height"`
}

//Detect 物体检测
func Detect(c echo.Context) error {
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

	client := vrs_rpc.GetObjectClientInstance()
	req := vrs_rpc.DetectObjectInImgRequest{Path: distPath}
	stream, err := client.DetectObjectInImg(context.Background(), &req)
	if err != nil {
		log.Printf("%v %v\n", client, err)
		return c.String(http.StatusBadRequest, "")
	}

	retResults := make([]retDetectInImgResult, 0, 1)

	for {
		result, err := stream.Recv()
		if err == io.EOF {
			break
		} else if err != nil {
			log.Printf("%v %v\n", client, err)
			return c.String(http.StatusBadRequest, "")
		}

		var retResult retDetectInImgResult
		retResult.ClassID = result.GetClassID() + 1
		retResult.ClassName = result.GetClassName()
		retResult.Top = result.GetTop()
		retResult.Left = result.GetLeft()
		retResult.Width = result.GetWidth()
		retResult.Height = result.GetHeight()

		retResults = append(retResults, retResult)
	}

	os.Remove(distPath) //删除临时上传的图像查询文件
	return c.JSON(http.StatusOK, retResults)
}
