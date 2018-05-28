package videos

import (
	"net/http"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//List 获取视频列表
func List(c echo.Context) error {
	db := model.GetDb()
	defer db.Close()

	//db测试
	var videos []entity.Video
	db.Find(&videos)
	return c.JSON(http.StatusOK, videos)
}
