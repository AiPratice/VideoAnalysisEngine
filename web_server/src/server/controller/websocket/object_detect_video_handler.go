package websocket

import (
	"context"
	"encoding/json"
	"io"
	"log"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/vrs_rpc"

	"golang.org/x/net/websocket"
)

//objectDetectVideoHandler 视频物体检测
func objectDetectVideoHandler(videoID uint32, ws *websocket.Conn) {
	log.Printf("请求对视频%d进行物体检测\n", videoID)
	db := model.GetDb()
	defer db.Close()

	var frames []entity.Frame
	db.Where("video_id = ?", videoID).Where("is_shot_frame = ?", true).Find(&frames)

	client := vrs_rpc.GetObjectClientInstance()

	//清理一存在的帧物体检测结果
	db.Where("video_id = ?", videoID).Delete(entity.FrameObject{})

	//设置视频帧处理状态为处理中
	db.Model(entity.Video{}).Where("video_id = ?", videoID).Update("object_status", 1)

	for _, frame := range frames {
		//对关键帧进行物体检测
		req := vrs_rpc.DetectObjectInImgRequest{Path: frame.ImgPath}
		stream, err := client.DetectObjectInImg(context.Background(), &req)
		if err != nil {
			log.Printf("%v %v\n", client, err)
		}

		for {
			result, err := stream.Recv()
			if err == io.EOF {
				break
			} else if err != nil {
				log.Printf("%v %v\n", client, err)
			}

			//查找并插入物体类型
			objectClass := entity.ObjectClass{ObjectClassID: result.GetClassID() + 1, EnName: result.GetClassName()}
			db.Where("object_class_id = ?", objectClass.ObjectClassID).FirstOrCreate(&objectClass)

			//插入帧物体信息到数据库
			frameObject := entity.FrameObject{
				FrameID:       frame.FrameID,
				ObjectClassID: objectClass.ObjectClassID,
				VideoID:       frame.VideoID,
				Left:          result.GetLeft(),
				Top:           result.GetTop(),
				Width:         result.GetWidth(),
				Height:        result.GetHeight(),
				Prob:          result.GetProb(),
			}

			db.Create(&frameObject)

		}
	}

	//修改视频物体检测状态为已处理
	db.Model(entity.Video{}).Where("video_id = ?", videoID).Update("object_status", 2)

	respEvent := entity.ResponseEvent{
		StatusCode: entity.SUCCEED,
		Type:       entity.ResponseObjectDetectVideo,
	}

	jsonByteArray, err := json.Marshal(&respEvent)
	if err != nil {
		log.Printf("格式化视频物体检测响应json出错")
		log.Print(err)
	}

	jsonStr := string(jsonByteArray[:])
	err = websocket.Message.Send(ws, jsonStr)
	if err != nil {
		log.Printf("websocket数据发送失败。数据:%s", jsonStr)
		log.Print(err)
	}
}
