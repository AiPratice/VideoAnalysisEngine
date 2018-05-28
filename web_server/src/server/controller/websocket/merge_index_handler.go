package websocket

import (
	"context"
	"encoding/json"
	"log"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/vrs_rpc"

	"golang.org/x/net/websocket"
)

//mergeIndexHandler 合并全局描述算子
func mergeIndexHandler(ws *websocket.Conn) {
	if ws == nil {
		return
	}
	client := vrs_rpc.GetVrsClientInstance()
	var req vrs_rpc.Empty
	result, err := client.Merge(context.Background(), &req)

	respEvent := entity.ResponseEvent{Type: entity.ResponseMergeIndex}
	if err != nil {
		log.Print("全局描述算子合并rpc调用失败")
		log.Print(err)
		respEvent.StatusCode = entity.FAILED
	} else {
		if result.Code == entity.SUCCEED {
			respEvent.StatusCode = entity.SUCCEED
		} else {
			respEvent.StatusCode = entity.FAILED
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
