package videos

import (
	"context"
	"fmt"
	"log"
	"net/http"
	"strconv"
	"vrs_web_server/src/model/response"
	"vrs_web_server/src/vrs_rpc"

	"github.com/labstack/echo"
)

//Process 视频处理
func Process(c echo.Context) error {
	logger := c.Logger()
	videoIDStr := c.FormValue("video_id")
	if videoIDStr == "" {
		logger.Error("空的video_id")
		return c.String(http.StatusBadRequest, "")
	}

	client := vrs_rpc.GetVrsClientInstance()
	videoID, err := strconv.Atoi(videoIDStr)
	if err != nil {
		log.Printf("video_id 转换失败:%s", videoIDStr)
		logger.Errorf("video_id 转换失败:%s", videoIDStr)
		return c.String(http.StatusBadRequest, "")
	}
	req := vrs_rpc.ProcessVideoRequest{VideoID: uint32(videoID)}
	result, err := client.ProcessVideo(context.Background(), &req)
	if err != nil {
		log.Printf("视频处理rpc调用失败 video_id:%s", videoIDStr)
		logger.Errorf("视频处理rpc调用失败 video_id:%s", videoIDStr)
		return c.String(http.StatusBadRequest, "")
	}

	fmt.Print(result)

	var resp response.Response

	if result.Code == 200 {
		resp.Code = response.Success
		return c.JSON(http.StatusOK, resp)
	}
	resp.Code = response.Error
	resp.Message = result.Msg
	return c.JSON(http.StatusOK, resp)
}
