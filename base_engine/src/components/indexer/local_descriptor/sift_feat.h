#pragma once

#include <string>
#include <vector>
#include <sys/types.h>

namespace vrs
{
namespace components
{
/*
 * sift特征类
 */
class sift_feat
{
public:
	enum {
		SIFT_DESCRIPTOR_LENGTH = 128,
		SIFT_KEYPOINT_LENGTH = 4
	};

	/*
	 * 根据图像去获取其关键点和描述算子。如果获取成功，则返回true。
	 * @param image_path 图像路径
	 * @param divide_512 如果为真，则描述算子元素会处于0~1之间
	 * @param frames 关键点包含x,y,s,o信息。向量中每一个元素都是作为一个关键点
	 * @param descriptors 每一个关键点的128维sift组件。向量中每一个元素都是作为相对于关键点的描述算子。
	 */
	static bool get_keypoints_and_descriptors(const char *image_path,
	        bool divide_512, std::vector<float*>& frames,
	        std::vector<float*>& descriptors, uint& num_descriptor);

	/*
	 * 保存关键点-描述算子成为文件。如果保存成功，则返回true
	 * std::vector<float*> keypoints 关键点矩阵
	 * std::vector<float*> descriptors 描述算子矩阵
	 * const char *file_path 保存文件路径
	 */
	static bool save_keypoints_and_descriptor(const std::vector<float*>& keypoints,
	        const std::vector<float*>& descriptors, uint num_descriptor,
	        const char *file_path);

	/*
	 * 根据图像路径生成特征文件
	 * @param image_path 图像路径
	 * @param out_feature_path 输出特征的路径
	 * 生成成功返回true,否则返回false
	 */
	static bool image_to_feature(const std::string &image_path,const std::string &out_feature_path);
};

}
}
