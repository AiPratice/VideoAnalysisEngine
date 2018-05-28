package persons

import (
	"net/http"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/model/response"

	"github.com/labstack/echo"
)

//Add 添加标注人物
func Add(c echo.Context) error {
	person := new(entity.Person)
	err := c.Bind(person)
	if err != nil {
		return err
	}
	var resp response.Response
	resp.Code = response.Error
	if person.Name == "" {
		resp.Message = "姓名不能为空"
		return c.JSON(http.StatusOK, resp)
	}

	db := model.GetDb()
	defer db.Close()
	db.Create(person)
	if person.PersonID > 0 {
		resp.Code = response.Success
		return c.JSON(http.StatusOK, resp)
	}
	resp.Message = "添加标注人物失败"
	return c.JSON(http.StatusBadRequest, resp)
}

//List 获取预标注人物列表
func List(c echo.Context) error {
	db := model.GetDb()
	defer db.Close()

	var persons []entity.Person
	db.Order("created_at desc").Find(&persons)
	return c.JSON(http.StatusOK, persons)
}
