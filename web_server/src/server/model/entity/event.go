package entity

//请求及响应事件类型
const (
	//RequestProcessVideo 视频处理请求事件
	RequestProcessVideo uint32 = 1

	//ResponseProcessVideo 视频处理响应事件
	ResponseProcessVideo uint32 = 2

	//RequestMergeIndex 合并全局描述算子索引请求事件
	RequestMergeIndex uint32 = 3

	//ResponseMergeIndex 合并全局描述算子索引响应事件
	ResponseMergeIndex uint32 = 4

	//RequestTrainClassifier 训练人脸分类器请求
	RequestTrainFaceClassifier uint32 = 5

	//ResponseTrainClassifier 训练人脸分类器响应
	ResponseTrainFaceClassifier uint32 = 6

	//RequestObjectDetectVideo 请求对视频进行物体检测
	RequestObjectDetectVideo uint32 = 7

	//ResponseObjectDetectVideo 响应对视频的物体检测
	ResponseObjectDetectVideo uint32 = 8

	//RequestFaceRecognizeVideo 请求对视频进行人脸识别
	RequestFaceRecognizeVideo uint32 = 9

	//ResponseFaceRecognizeVideo 响应对视频的人脸识别
	ResponseFaceRecognizeVideo uint32 = 10
)

//响应事件状态码常量
const (
	//SUCCEED 成功
	SUCCEED uint32 = 200

	//FAILED 失败
	FAILED uint32 = 400
)

//RequestEvent Websocket上所传输的客户端请求事件
type RequestEvent struct {
	Type uint32                 //请求事件类型
	Data map[string]interface{} //数据
}

//ResponseEvent Websocket上所传输的服务端响应事件
type ResponseEvent struct {
	Type       uint32                 //响应事件类型
	StatusCode uint32                 //响应状态码
	Data       map[string]interface{} //数据
}
