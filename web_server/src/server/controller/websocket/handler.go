package websocket

import (
	"encoding/json"
	"log"

	"vrs_web_server/src/model/entity"

	"github.com/labstack/echo"
	"golang.org/x/net/websocket"
)

//Handler websocket处理器
func Handler(c echo.Context) error {
	websocket.Handler(func(ws *websocket.Conn) {
		defer ws.Close()
		for {
			var msg string
			err := websocket.Message.Receive(ws, &msg)
			if err != nil {
				log.Println("接收websocket出错，有可能客户端主动断开了连接")
				log.Println(err)
				return
			}
			log.Printf(msg)

			var event entity.RequestEvent
			err = json.Unmarshal([]byte(msg), &event)
			if err != nil {
				log.Println("websocket接收得到的json字符串转换出错:")
				log.Println(err)
				return
			}

			switch event.Type {
			case entity.RequestProcessVideo:
				//视频处理请求
				if val, ok := event.Data["videoID"].(float64); ok {
					videoID := uint32(val)
					processVideoHandler(videoID, ws)
				} else {
					log.Println("非法的视频处理请求，找不到VideoID")
				}

			case entity.RequestMergeIndex:
				//视频索引合并请求
				mergeIndexHandler(ws)

			case entity.RequestTrainFaceClassifier:
				//人脸训练器训练请求
				trainFaceClassifier(ws)

			case entity.RequestObjectDetectVideo:
				if val, ok := event.Data["videoID"].(float64); ok {
					videoID := uint32(val)
					objectDetectVideoHandler(videoID, ws)
				} else {
					log.Println("非法的视频物体检测请求，找不到VideoID")
				}

			case entity.RequestFaceRecognizeVideo:
				if val, ok := event.Data["videoID"].(float64); ok {
					videoID := uint32(val)
					faceRecognizeVideoHandler(videoID, ws)
				} else {
					log.Println("非法的视频人脸识别请求，找不到VideoID")
				}

			default:
				log.Printf("未定义的请求事件 事件类型:%d 请求数据:%s", event.Type, msg)
			}
		}
	}).ServeHTTP(c.Response(), c.Request())
	return nil
}
