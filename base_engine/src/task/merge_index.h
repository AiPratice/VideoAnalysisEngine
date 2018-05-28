#pragma once
#include "../models/videos.h"


namespace vrs
{

namespace task
{

class merge_index
{
private:
	models::videos videos_model;
public:
	/*
	 * @brief 合并所有视频的全局描述算子索引并生成全局索引文件
	 */
	void merge();
};

}

}
