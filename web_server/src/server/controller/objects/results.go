package objects

import (
	"net/http"
	"time"
	"unicode/utf8"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
)

//返回结果
type retResult struct {
	FrameObjectID uint32    `json:"frame_object_id"`
	FrameID       uint64    `json:"frame_id"`
	ObjectClassID uint32    `json:"object_class_id"`
	VideoID       uint32    `json:"video_id"`
	Left          int32     `json:"left"`
	Top           int32     `json:"top"`
	Width         int32     `json:"width"`
	Height        int32     `json:"height"`
	Prob          float32   `json:"prob"`
	Second        uint32    `json:"second"`
	CreatedAt     time.Time `json:"created_at"`
}

//Results 物体检测结果
func Results(c echo.Context) error {
	videoID := c.QueryParam("video_id")
	if utf8.RuneCountInString(videoID) == 0 {
		return c.NoContent(http.StatusNoContent)
	}

	db := model.GetDb()
	defer db.Close()

	var frameObjects []entity.FrameObject
	db.Where("video_id = ?", videoID).Find(&frameObjects)

	rResults := make([]retResult, 0, 100)
	for _, frameObject := range frameObjects {
		frame := entity.Frame{FrameID: frameObject.FrameID}
		db.First(&frame)

		rResult := retResult{
			FrameObjectID: frameObject.FrameObjectID,
			FrameID:       frameObject.FrameID,
			ObjectClassID: frameObject.ObjectClassID,
			VideoID:       frameObject.VideoID,
			Left:          frameObject.Left,
			Top:           frameObject.Top,
			Width:         frameObject.Width,
			Height:        frameObject.Height,
			Prob:          frameObject.Prob,
			Second:        frame.Second,
		}
		rResults = append(rResults, rResult)
	}

	return c.JSON(http.StatusOK, &rResults)
}
