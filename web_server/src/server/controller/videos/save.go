package videos

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"net/http"
	"os"
	"path"
	"time"
	"vrs_web_server/src/config"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/model/response"

	"github.com/labstack/echo"
)

//Save 保存视频
func Save(c echo.Context) error {
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
	distPath := fmt.Sprintf("%s/%s%s", config.VideosStoragePath, newFilename, fileExt)
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

	//存入数据库
	db := model.GetDb()
	defer db.Close()
	var resp response.Response
	var video entity.Video
	video.Name = file.Filename
	video.Path = distPath
	video.CreatedAt = time.Now()
	video.UpdatedAt = video.CreatedAt
	video.Status = entity.VideoStatusUnprocess //默认为未处理状态

	db.Create(&video)
	resp.Code = response.Success

	return c.JSON(http.StatusOK, resp)
}
