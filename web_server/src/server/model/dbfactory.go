package model

import (
	"fmt"
	"server/config"

	"github.com/jinzhu/gorm"
)

var conStr string

//GetDb 获取gorm数据库对象
func GetDb() *gorm.DB {
	db, err := gorm.Open("postgres", config.DbConn)
	if err != nil {
		fmt.Println("数据库连接失败")
		return nil
	}

	return db
}
