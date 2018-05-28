package entity

import (
	"time"
)

//Person 用于人脸检测的预标注人物
type Person struct {
	PersonID    uint32    `gorm:"primary_key" json:"person_id"`
	Name        string    `json:"name"`
	Description string    `json:"description"`
	CreatedAt   time.Time `json:"created_at"`
	UpdatedAt   time.Time `json:"updated_at"`
}

//TableName 表名
func (p Person) TableName() string {
	return "persons"
}
