package videos

import (
	"log"
	"net/http"
	"strconv"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//Video 获取视频文件
func Video(c echo.Context) error {
	videoID, err := strconv.Atoi(c.QueryParam("video_id"))
	if err != nil {
		log.Print(err)
		return c.String(http.StatusBadRequest, "")
	}
	db := model.GetDb()
	defer db.Close()
	var video entity.Video
	db.Where("video_id = ?", videoID).Find(&video)
	return c.File(video.Path)
}
