package websocket

import (
	"context"
	"encoding/json"
	"log"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/vrs_rpc"

	"golang.org/x/net/websocket"
)

//TrainFaceClassifier 训练人脸分类器
func trainFaceClassifier(ws *websocket.Conn) {
	client := vrs_rpc.GetFaceClientInstance()
	emptyReq := vrs_rpc.Empty{}
	respEvent := entity.ResponseEvent{Type: entity.ResponseTrainFaceClassifier}
	result, err := client.BuildClassifer(context.Background(), &emptyReq)
	if err != nil {
		log.Println("人脸训练rpc请求出错:", err)
		return
	}

	if result.Code == entity.SUCCEED {
		//训练完成
		respEvent.StatusCode = entity.SUCCEED
	} else {
		respEvent.StatusCode = entity.FAILED
	}

	jsonByteArray, err := json.Marshal(&respEvent)
	if err != nil {
		log.Printf("格式化人脸训练响应json出错")
		log.Print(err)
	}

	jsonStr := string(jsonByteArray[:])
	err = websocket.Message.Send(ws, jsonStr)
	if err != nil {
		log.Printf("websocket数据发送失败。数据:%s", jsonStr)
		log.Print(err)
	}
}
