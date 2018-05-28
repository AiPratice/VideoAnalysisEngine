package faces

import (
	"net/http"
	"server/model"
	"server/model/entity"
	"time"
	"unicode/utf8"

	"github.com/labstack/echo"
)

//返回结果
type retResult struct {
	FrameFaceID uint32    `json:"frame_face_id"`
	FrameID     uint64    `json:"frame_id"`
	PersonID    uint32    `json:"person_id"`
	VideoID     uint32    `json:"video_id"`
	Left        int32     `json:"left"`
	Top         int32     `json:"top"`
	Width       int32     `json:"width"`
	Height      int32     `json:"height"`
	Second      uint32    `json:"second"`
	CreatedAt   time.Time `json:"created_at"`
}

//Results 人脸检测结果
func Results(c echo.Context) error {
	videoID := c.QueryParam("video_id")
	if utf8.RuneCountInString(videoID) == 0 {
		return c.NoContent(http.StatusNoContent)
	}

	db := model.GetDb()
	defer db.Close()

	var frameFaces []entity.FrameFace
	db.Where("video_id = ?", videoID).Find(&frameFaces)

	rResults := make([]retResult, 0, 100)
	for _, frameFace := range frameFaces {
		frame := entity.Frame{FrameID: frameFace.FrameID}
		db.First(&frame)

		rResult := retResult{
			FrameFaceID: frameFace.FrameFaceID,
			FrameID:     frameFace.FrameID,
			PersonID:    frameFace.PersonID,
			VideoID:     frameFace.VideoID,
			Left:        frameFace.Left,
			Top:         frameFace.Top,
			Width:       frameFace.Width,
			Height:      frameFace.Height,
			Second:      frame.Second,
		}
		rResults = append(rResults, rResult)
	}

	return c.JSON(http.StatusOK, &rResults)
}
