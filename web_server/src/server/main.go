package main

import (
	"fmt"
	"log"
	"server/config"
	"server/controller/faces"
	"server/controller/frames"
	"server/controller/objectclasses"
	"server/controller/objects"
	"server/controller/personfaces"
	"server/controller/persons"
	"server/controller/videos"
	"server/controller/websocket"
	"server/vrs_rpc"

	_ "github.com/jinzhu/gorm/dialects/postgres"
	"github.com/labstack/echo"
	"github.com/labstack/echo/middleware"
)

func httpWebServe() {
	e := echo.New()
	e.Use(middleware.CORS()) //允许跨域
	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	//路由定义
	e.Static("/", "static") //静态目录

	//websocket
	e.GET("/ws", websocket.Handler)

	//视频路由
	videosGroup := e.Group("/videos")
	videosGroup.GET("/list", videos.List)
	videosGroup.POST("/search", videos.Search)
	videosGroup.POST("/save", videos.Save)
	videosGroup.GET("/capture", videos.Capture)
	videosGroup.GET("/video", videos.Video)
	videosGroup.POST("/process", videos.Process)
	videosGroup.POST("merge_index", videos.MergeIndex)
	videosGroup.GET("/find", videos.Find)
	videosGroup.POST("/edit", videos.Edit)
	videosGroup.POST("/tag-search", videos.TagSearch)

	//帧图像路由
	framesGroup := e.Group("/frames")
	framesGroup.GET("/frame_img", frames.GetFrameImg)
	framesGroup.GET("/shots", frames.Shots)

	//人物标注信息
	personsGroup := e.Group("/persons")
	personsGroup.POST("/add", persons.Add)
	personsGroup.POST("/edit", persons.Edit)
	personsGroup.GET("/list", persons.List)
	personsGroup.POST("/recognize", persons.Recognize)

	//人脸标注图片
	personFacesGroup := e.Group("/personfaces")
	personFacesGroup.POST("/save", personfaces.Save)
	personFacesGroup.GET("/list", personfaces.List)
	personFacesGroup.GET("/face", personfaces.Face)
	personFacesGroup.POST("/delete", personfaces.Delete)

	//物体检测
	objectsGroup := e.Group("/objects")
	objectsGroup.POST("/detect", objects.Detect)
	objectsGroup.GET("/results", objects.Results)

	//物体分类
	objectClassesGroup := e.Group("/objectclasses")
	objectClassesGroup.GET("/lists", objectclasses.List)

	//人脸识别
	facesGroup := e.Group("/faces")
	facesGroup.GET("/results", faces.Results)

	portStr := fmt.Sprintf(":%d", config.HTTPPort) //端口号字符串

	log.Printf("http服务开启，端口号:%d", config.HTTPPort)

	e.Logger.Fatal(e.Start(portStr))
}

/*视频检索系统WebApi服务端*/
func main() {
	config.InitConfig("config.json") //初始化配置

	vrs_rpc.InitVrsClient(config.RPCServerAddress) //初始化相似检索rpc客户端
	vrs_rpc.InitFaceClient("127.0.0.1:50052")      //初始化人脸检索rpc客户端
	vrs_rpc.InitObjectClient("127.0.0.1:50052")    //初始化物体检测rpc客户端

	httpWebServe()

}
