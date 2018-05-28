package videos

import (
	"context"
	"net/http"
	"vrs_web_server/src/model/response"
	"vrs_web_server/src/vrs_rpc"

	"github.com/labstack/echo"
)

//MergeIndex 索引合并
func MergeIndex(c echo.Context) error {
	logger := c.Logger()

	client := vrs_rpc.GetVrsClientInstance()

	req := vrs_rpc.Empty{}
	result, err := client.Merge(context.Background(), &req)
	if err != nil {
		logger.Errorf("索引合并rpc调用失败")
		return c.String(http.StatusBadRequest, "")
	}

	var resp response.Response
	if result.Code == 200 {
		resp.Code = response.Success
		return c.JSON(http.StatusOK, resp)
	}
	resp.Code = response.Error
	resp.Message = result.Msg
	return c.JSON(http.StatusOK, resp)
}
