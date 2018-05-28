package entity

//Frame 视频帧数据库实体
type Frame struct {
	FrameID     uint64 `gorm:"primary_key"`
	VideoID     uint32
	ImgPath     string
	FeaturePath string
	IsShotFrame bool
	Second      uint32
}
