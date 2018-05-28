package entity

import (
	"time"
)

//视频状态常亮定义
const (
	VideoStatusUnprocess  = 0 //未处理
	VideoStatusProcessing = 1 //处理中
	VideoStatusProcessed  = 2 //处理完成

	VideoObjectStatusUnprocess  = 0
	VideoObjectStatusProcessing = 1
	VideoObjectStatusProcessed  = 2
)

//Video 视频实体结构体
type Video struct {
	//	gorm.Model
	VideoID      uint32    `gorm:"primary_key" json:"video_id"`
	Name         string    `json:"name"`
	Description  string    `json:"description"`
	Path         string    `json:"-"`
	GdIndexPath  string    `gorm:"column:gdindex_path" json:"-"`
	Status       uint16    `json:"status"`
	ObjectStatus uint16    `json:"object_status"`
	FaceStatus   uint16    `json:"face_status"`
	CreatedAt    time.Time `json:"created_at"`
	UpdatedAt    time.Time `json:"updated_at"`
}
