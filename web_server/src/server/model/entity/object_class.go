package entity

import (
	"time"
)

//ObjectClass 物体类型
type ObjectClass struct {
	ObjectClassID uint32    `gorm:"primary_key" json:"object_class_id"`
	CnName        string    `json:"cn_name"`
	EnName        string    `json:"en_name"`
	CreatedAt     time.Time `json:"created_at"`
	UpdatedAt     time.Time `json:"updated_at"`
}
