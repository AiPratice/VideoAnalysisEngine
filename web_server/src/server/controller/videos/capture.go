package videos

import (
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//Capture 视频截图
func Capture(c echo.Context) error {
	videoID := c.QueryParam("video_id")
	db := model.GetDb()
	defer db.Close()
	var frame entity.Frame
	db.Where("video_id = ?", videoID).Find(&frame)
	if frame.ImgPath == "" {
		return c.File("static/nopic.jpg")
	}
	return c.File(frame.ImgPath)
}
