package entity

import (
	"time"
)

//FrameObject 视频帧物体
type FrameObject struct {
	FrameObjectID uint32    `gorm:"primary_key" json:"frame_object_id"`
	FrameID       uint64    `json:"frame_id"`
	ObjectClassID uint32    `json:"object_class_id"`
	VideoID       uint32    `json:"video_id"`
	Left          int32     `json:"left"`
	Top           int32     `json:"top"`
	Width         int32     `json:"width"`
	Height        int32     `json:"height"`
	Prob          float32   `json:"prob"`
	CreatedAt     time.Time `json:"created_at"`
}
