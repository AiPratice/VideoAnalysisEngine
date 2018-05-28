package videos

import (
	"log"
	"net/http"
	"strconv"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

type retVideo struct {
	VideoID uint32 `json:"video_id"`
	Name    string `json:"name"`
}

//Find 查找具体的视频信息
func Find(c echo.Context) error {
	videoID, err := strconv.Atoi(c.QueryParam("video_id"))
	if err != nil {
		log.Print(err)
		return c.String(http.StatusBadRequest, "")
	}
	db := model.GetDb()
	defer db.Close()
	var video entity.Video
	db.Where("video_id = ?", videoID).Find(&video)

	rVideo := retVideo{
		VideoID: video.VideoID,
		Name:    video.Name,
	}
	return c.JSON(http.StatusOK, rVideo)
}
