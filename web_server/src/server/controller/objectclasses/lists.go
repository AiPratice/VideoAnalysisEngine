package objectclasses

import (
	"net/http"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//List 获取物体检测类别列表
func List(c echo.Context) error {
	db := model.GetDb()
	defer db.Close()

	var objectClasses []entity.ObjectClass
	db.Find(&objectClasses)
	return c.JSON(http.StatusOK, &objectClasses)
}
