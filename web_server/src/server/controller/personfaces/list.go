package personfaces

import (
	"net/http"
	"strconv"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//List 获取人脸列表
func List(c echo.Context) error {
	personID, err := strconv.Atoi(c.QueryParam("person_id"))
	if err != nil {
		return err
	}
	db := model.GetDb()
	defer db.Close()

	var personFaces []entity.PersonFace
	db.Where("person_id = ?", personID).Find(&personFaces)
	return c.JSON(http.StatusOK, personFaces)
}
