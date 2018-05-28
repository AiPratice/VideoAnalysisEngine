#pragma once


#include <vector>
#include <string>

using namespace std;

namespace vrs
{
namespace components
{
class sift_extractor
{
public:
	static constexpr int PROGRESS_INTERVAL = 50;

	sift_extractor()
	{

	}

	~sift_extractor()
	{

	}

	/*
	 * 批量处理图像以生成关键点-描述符文件
	 * unsigned int start_i images向量的开始下标
	 * unsigned int num_images 需要处理的图像数量
	 * const vector<string> &images 图像文件名向量
	 * const char *in_folder_path 输入图像文件夹的路径
	 * const char *out_folder_path 输出文件夹的路径
	 * bool is_skip_existing 是否跳过已存在的文件
	 */
	static void process_images(unsigned int start_i, unsigned int num_images,
	                           const vector<string> &images, const char *in_folder_path,
	                           const char *out_folder_path, unsigned int thread_index,
	                           bool is_skip_existing);

	/*
	 * 多线程提取sift特征文件
	 * const char *in_folder_path 输入图像文件夹的路径
	 * unsigned int num_threads 线程数
	 * bool is_skip_existing 是否跳过已存在的特征文件
	 */
	static void extract(const char *in_folder_path,const char *out_folder_path, unsigned int num_least_threads,
	                    bool is_skip_existing);
};

}
}
