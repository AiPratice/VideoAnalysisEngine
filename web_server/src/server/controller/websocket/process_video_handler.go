package websocket

import (
	"context"
	"encoding/json"
	"log"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/vrs_rpc"

	"golang.org/x/net/websocket"
)

//processVideoHandler 视频处理
func processVideoHandler(videoID uint32, ws *websocket.Conn) {
	if ws == nil {
		return
	}
	log.Printf("请求处理视频 VideoID:%d", videoID)
	client := vrs_rpc.GetVrsClientInstance()
	req := vrs_rpc.ProcessVideoRequest{VideoID: uint32(videoID)}
	result, err := client.ProcessVideo(context.Background(), &req)

	respEvent := entity.ResponseEvent{Type: entity.ResponseProcessVideo}
	if err != nil {
		log.Printf("视频处理rpc调用失败 VideoID:%d", videoID)
		log.Print(err)
		respEvent.StatusCode = entity.FAILED
	} else {
		if result.Code == 200 {
			respEvent.StatusCode = entity.SUCCEED
			log.Printf("视频处理完成 VideoID:%d", videoID)
		} else {
			respEvent.StatusCode = entity.FAILED
			log.Printf("视频处理失败 VideoID:%d", videoID)
		}
	}

	jsonByteArray, err := json.Marshal(&respEvent)
	if err != nil {
		log.Printf("格式化视频响应json出错")
		log.Print(err)
	}

	jsonStr := string(jsonByteArray[:])
	err = websocket.Message.Send(ws, jsonStr)
	if err != nil {
		log.Printf("websocket数据发送失败。数据:%s", jsonStr)
		log.Print(err)
	}
}
