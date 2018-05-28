package config

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
)

//Config server配置信息
type Config struct {
	HTTPPort          uint
	SocketIOPort      uint
	DbConn            string
	StoragePath       string //文件存储基础路径
	VideosStoragePath string //视频文件存储路径
	FacesStoragePath  string //人脸文件存储路径
	RPCServerAddress  string //rpc服务器地址
	TEMPDirPath       string //临时文件存储路径
}

var (
	//HTTPPort Http服务端口
	HTTPPort uint

	//SocketIOPort socketio服务端口
	SocketIOPort uint

	//DbConn 数据库连接
	DbConn string

	//StoragePath 文件存储基础路径
	StoragePath string

	//VideosStoragePath 视频文件存储路径
	VideosStoragePath string

	//FacesStoragePath 人脸文件存储路径
	FacesStoragePath string

	//RPCServerAddress rpc服务器地址
	RPCServerAddress string

	//TEMPDirPath 临时文件存储路径
	TEMPDirPath string
)

//InitConfig 根据json文件初始化配置
func InitConfig(path string) {
	data, err := ioutil.ReadFile(path)
	if err != nil {
		currentAbsPath, err := filepath.Abs(path)
		if err != nil {
			fmt.Println("无法获取当前路径")
			os.Exit(-1)
		}
		fmt.Printf("error:配置文件读取错误!配置文件路径:%s,当前路径:%s\n", path, currentAbsPath)

		fmt.Print(err)
		os.Exit(-1)
	}
	var config Config
	json.Unmarshal(data, &config)
	HTTPPort = config.HTTPPort
	SocketIOPort = config.SocketIOPort
	DbConn = config.DbConn
	StoragePath = config.StoragePath
	VideosStoragePath = config.VideosStoragePath
	FacesStoragePath = config.FacesStoragePath
	RPCServerAddress = config.RPCServerAddress
	TEMPDirPath = config.TEMPDirPath
}
