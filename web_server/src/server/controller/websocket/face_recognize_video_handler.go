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

//faceRecognizeVideoHandler 视频人脸识别
func faceRecognizeVideoHandler(videoID uint32, ws *websocket.Conn) {
	log.Printf("请求对视频%d进行人脸识别\n", videoID)
	db := model.GetDb()
	defer db.Close()

	var frames []entity.Frame
	db.Where("video_id = ?", videoID).Where("is_shot_frame = ?", true).Find(&frames)

	client := vrs_rpc.GetFaceClientInstance()

	//清理已经存在的帧物体检测结果
	db.Where("video_id = ?", videoID).Delete(entity.FrameFace{})

	//设置视频帧处理状态为处理中
	db.Model(entity.Video{}).Where("video_id = ?", videoID).Update("face_status", 1)

	for _, frame := range frames {
		//对关键帧进行物体检测
		req := vrs_rpc.DetectPersonInImgRequest{Path: frame.ImgPath}
		stream, err := client.DetectPersonInImg(context.Background(), &req)
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

			frameFace := entity.FrameFace{
				FrameID:  frame.FrameID,
				VideoID:  frame.VideoID,
				PersonID: result.GetPersonID(),
				Top:      result.GetTop(),
				Left:     result.GetLeft(),
				Width:    result.GetRight() - result.GetLeft(),
				Height:   result.GetBottom() - result.GetTop(),
			}

			db.Create(&frameFace)
		}
	}

	//修改视频物体检测状态为已处理
	db.Model(entity.Video{}).Where("video_id = ?", videoID).Update("face_status", 2)

	respEvent := entity.ResponseEvent{
		StatusCode: entity.SUCCEED,
		Type:       entity.ResponseFaceRecognizeVideo,
	}

	jsonByteArray, err := json.Marshal(&respEvent)
	if err != nil {
		log.Printf("格式化视频人脸识别响应json出错")
		log.Print(err)
	}

	jsonStr := string(jsonByteArray[:])
	err = websocket.Message.Send(ws, jsonStr)
	if err != nil {
		log.Printf("websocket数据发送失败。数据:%s", jsonStr)
		log.Print(err)
	}
}
