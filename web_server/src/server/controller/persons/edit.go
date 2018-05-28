package persons

import (
	"net/http"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/model/response"

	"github.com/labstack/echo"
)

//Edit 编辑预标注人物数据
func Edit(c echo.Context) error {
	var person entity.Person
	c.Bind(&person)
	resp := response.NewResponse()
	if person.Name == "" {
		resp.Message = "标注人物姓名不能为空"
	} else {
		db := model.GetDb()
		defer db.Close()
		db.Model(&person).Where("person_id = ?", person.PersonID).Updates(person)
	}
	resp.Code = response.Success
	return c.JSON(http.StatusOK, resp)
}
