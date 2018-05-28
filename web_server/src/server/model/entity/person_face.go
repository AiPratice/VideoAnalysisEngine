package entity

import (
	"time"
)

//PersonFace 用于训练的人脸数据
type PersonFace struct {
	PersonFaceID uint32    `gorm:"primary_key" json:"person_face_id"`
	PersonID     uint32    `json:"person_id"`
	Path         string    `json:"-"`
	CreatedAt    time.Time `json:"created_at"`
}
