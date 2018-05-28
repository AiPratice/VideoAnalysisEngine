package frames

import (
	"net/http"
	"unicode/utf8"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

type retShot struct {
	FrameID uint64 `json:"frame_id"`
	VideoID uint32 `json:"video_id"`
	Second  uint32 `json:"second"`
}

//Shots 视频镜头列表
func Shots(c echo.Context) error {
	videoID := c.QueryParam("video_id")
	if utf8.RuneCountInString(videoID) == 0 {
		return c.NoContent(http.StatusNoContent)
	}

	db := model.GetDb()
	defer db.Close()

	var frames []entity.Frame
	db.Where("video_id = ?", videoID).Where("is_shot_frame = ?", true).Find(&frames)

	rShots := make([]retShot, 0, 20)

	for _, frame := range frames {
		rShot := retShot{
			FrameID: frame.FrameID,
			VideoID: frame.VideoID,
			Second:  frame.Second,
		}
		rShots = append(rShots, rShot)
	}

	return c.JSON(http.StatusOK, rShots)
}
