package personfaces

import (
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//Face 人脸图片获取
func Face(c echo.Context) error {
	db := model.GetDb()
	defer db.Close()

	personFaceID := c.QueryParam("person_face_id")
	var personFace entity.PersonFace
	db.Where("person_face_id = ?", personFaceID).Find(&personFace)
	if personFace.Path != "" {
		return c.File(personFace.Path)
	}
	return c.File("static/nopic.jpg")
}
