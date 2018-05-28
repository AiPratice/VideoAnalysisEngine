package videos

import (
	"fmt"
	"net/http"
	"time"
	"vrs_web_server/src/model"
	"vrs_web_server/src/model/entity"
	"vrs_web_server/src/model/response"

	"github.com/labstack/echo"
)

//Edit 编辑视频信息
func Edit(c echo.Context) error {
	var video entity.Video
	c.Bind(&video)
	resp := response.NewResponse()

	if video.Name == "" {
		resp.Message = "视频名称不能为空"
	} else {
		db := model.GetDb()
		defer db.Close()

		db.Model(&video).Where("video_id = ?", video.VideoID).Update("name", video.Name).Update("description", video.Description).Update("updated_at", time.Now())
	}
	fmt.Println(video)
	resp.Code = response.Success
	return c.JSON(http.StatusOK, resp)
}
