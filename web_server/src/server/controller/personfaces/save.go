package personfaces

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"net/http"
	"os"
	"path"
	"strconv"
	"time"
	"vrs_web_server/src/config"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/model/response"

	"github.com/labstack/echo"
)

//Save 上传保存人脸数据
func Save(c echo.Context) error {
	/*文件上传处理*/
	file, err := c.FormFile("file")
	if err != nil {
		return err
	}
	personIDStr := c.FormValue("person_id")

	personID, err := strconv.Atoi(personIDStr)
	if err != nil {
		return err
	}

	src, err := file.Open()
	if err != nil {
		return err
	}
	defer src.Close()

	md5Ctx := md5.New()
	md5Ctx.Write([]byte(file.Filename + time.Now().String()))
	fileExt := path.Ext(file.Filename)
	newFilename := hex.EncodeToString(md5Ctx.Sum(nil))
	distPath := fmt.Sprintf("%s/%s%s", config.FacesStoragePath, newFilename, fileExt)
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
	var resp response.Response
	var personFace entity.PersonFace
	personFace.PersonID = uint32(personID)
	personFace.Path = distPath
	personFace.CreatedAt = time.Now()
	db.Create(&personFace)
	if personFace.PersonFaceID != 0 {
		resp.Code = response.Success
	} else {
		resp.Code = response.Error
		resp.Message = "上传数据写入数据库失败"
	}

	return c.JSON(http.StatusOK, resp)
}
