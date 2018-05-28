package response

//返回前端的状态码定义
const (
	//Success 操作成功
	Success uint = 200

	//Error 操作错误
	Error uint = 500
)

//Response 响应前端的结构体
type Response struct {
	Message string `json:"message"`
	Code    uint   `json:"code"`
}

//NewResponse 构造响应
func NewResponse() *Response {
	return &Response{Code: Error}
}
