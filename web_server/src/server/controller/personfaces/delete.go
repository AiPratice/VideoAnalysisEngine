package personfaces

import (
	"net/http"
	"os"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/model/response"

	"github.com/labstack/echo"
)

//Delete 删除人脸图片失败
func Delete(c echo.Context) error {
	var personFace entity.PersonFace
	c.Bind(&personFace)
	resp := response.NewResponse()
	db := model.GetDb()
	defer db.Close()
	db.Find(&personFace)
	if personFace.Path == "" {
		resp.Code = response.Error
		resp.Message = "找不到该人脸图片，删除失败"
	} else {
		err := os.Remove(personFace.Path)
		if err != nil {
			resp.Code = response.Error
			resp.Message = "删除人脸文件失败"
		} else {
			db.Delete(&personFace)
		}
	}
	return c.JSON(http.StatusOK, resp)
}
