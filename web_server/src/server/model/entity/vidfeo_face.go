package entity

import (
	"time"
)

//FrameFace 视频人脸模型
type FrameFace struct {
	FrameFaceID uint32    `gorm:"primary_key" json:"frame_face_id"`
	FrameID     uint64    `json:"frame_id"`
	VideoID     uint32    `json:"video_id"`
	PersonID    uint32    `json:"person_id"`
	Top         int32     `json:"top"`
	Left        int32     `json:"left"`
	Width       int32     `json:"width"`
	Height      int32     `json:"height"`
	CreatedAt   time.Time `json:"created_at"`
}
