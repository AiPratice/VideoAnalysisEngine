package frames

import (
	"net/http"
	"unicode/utf8"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//GetFrameImg 获取帧图像
func GetFrameImg(c echo.Context) error {
	frameID := c.QueryParam("frame_id")
	if utf8.RuneCountInString(frameID) == 0 {
		return c.NoContent(http.StatusNoContent)
	}
	db := model.GetDb()
	defer db.Close()

	var frame entity.Frame
	db.First(&frame, frameID)

	return c.File(frame.ImgPath)
}
