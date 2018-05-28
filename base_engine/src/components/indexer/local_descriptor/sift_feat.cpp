#include <opencv/cv.hpp>
#include <boost/log/trivial.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include "sift_feat.h"
#include "../../../common/utils/io_utils.h"

extern "C" {
#include <vl/mathop.h>
#include <vl/sift.h>
}

using namespace std;
using namespace cv;

namespace vrs {
namespace components{

bool sift_feat::get_keypoints_and_descriptors(const char *image_path,
		bool divide_512, vector<float*>& frames, vector<float*>& descriptors,
		uint& num_descriptor) {
	Mat image = imread(image_path, CV_LOAD_IMAGE_GRAYSCALE);
	int im_width, im_height;
	chrono::time_point<std::chrono::system_clock> start_time =
			chrono::system_clock::now(); //开始时间
	if (!image.data) {
		BOOST_LOG_TRIVIAL(error)<< "无法加载图像:" << image_path;
		return false;
	}
	im_width = image.cols, im_height = image.rows;

//	string window_name = image_path;
//	namedWindow( window_name, WINDOW_AUTOSIZE );// Create a window for display.
//	imshow( window_name, image );               // Show our image inside it.
//	waitKey(0);                                 // Wait for a keystroke in the window

	//转换图像为vlfeat的数据结构
	uint num_pixels = im_width * im_height;
	vl_sift_pix *data = new vl_sift_pix[num_pixels];
	for (uint i = 0; i < num_pixels; i++) {
		data[i] = static_cast<vl_sift_pix>(image.data[i]);
	}

	//vfleat sift 参数
	int O = -1;
	int S = 3;
	int o_min = -1;
	double edge_thresh = 5;
	double peak_thresh = 10;
	double norm_thresh = -1;
	double magnif = -1;
	double window_size = -1;

	VlSiftFilt *filt = vl_sift_new(im_width, im_height, O, S, o_min);

	int nframes = 0;

	if (peak_thresh >= 0)
		vl_sift_set_peak_thresh(filt, peak_thresh);
	if (edge_thresh >= 0)
		vl_sift_set_edge_thresh(filt, edge_thresh);
	if (norm_thresh >= 0)
		vl_sift_set_norm_thresh(filt, norm_thresh);
	if (magnif >= 0)
		vl_sift_set_magnif(filt, magnif);
	if (window_size >= 0)
		vl_sift_set_window_size(filt, window_size);

//	BOOST_LOG_TRIVIAL(debug)<< "vl_sift 过滤器设置信息:";
//	BOOST_LOG_TRIVIAL(debug)<< "im_width=" << im_width;
//	BOOST_LOG_TRIVIAL(debug)<< "im_height=" << im_height;
//	BOOST_LOG_TRIVIAL(debug)<< "octaves (O)=" << vl_sift_get_noctaves(filt);
//	BOOST_LOG_TRIVIAL(debug)<< "levels (S)=" << vl_sift_get_nlevels(filt);
//	BOOST_LOG_TRIVIAL(debug)<< "first octave (o_min)=" << vl_sift_get_octave_first(filt);
//	BOOST_LOG_TRIVIAL(debug)<< "edge thresh=" << vl_sift_get_edge_thresh(filt);
//	BOOST_LOG_TRIVIAL(debug)<< "peak thresh=" << vl_sift_get_peak_thresh(filt);
//	BOOST_LOG_TRIVIAL(debug)<< "norm thresh=" << vl_sift_get_norm_thresh(filt);
//	BOOST_LOG_TRIVIAL(debug)<< "window size=" << vl_sift_get_window_size(filt);

	//处理每一个倍频程
	bool is_first = true;
	while (true) {
		int err;
		const VlSiftKeypoint *keys = nullptr;
		int nkeys = 0;

		//BOOST_LOG_TRIVIAL(debug)<< "正在处理octave:" << vl_sift_get_octave_index(filt);

		if (is_first) {
			err = vl_sift_process_first_octave(filt, data);
			is_first = false;
		} else {
			err = vl_sift_process_next_octave(filt);
		}

		if (err)
			break;

		//BOOST_LOG_TRIVIAL(debug)<< "GSS octave:" << vl_sift_get_octave_index(filt);

		//运行检测器
		vl_sift_detect(filt);
		keys = vl_sift_get_keypoints(filt);
		nkeys = vl_sift_get_nkeypoints(filt);
		//BOOST_LOG_TRIVIAL(debug)<<"检测到未定向关键点个数:" << nkeys;

		//遍历关键点
		for (int i = 0; i < nkeys; i++) {
			double angles[4];
			int nangles;
			const VlSiftKeypoint *k;

			//获得关键点的方向
			k = keys + i;
			nangles = vl_sift_calc_keypoint_orientations(filt, angles, k);

			//遍历所有方向
			for (int q = 0; q < nangles; q++) {
				vl_sift_pix rbuf[SIFT_DESCRIPTOR_LENGTH];
				float *this_frame = new float[SIFT_KEYPOINT_LENGTH];
				float *this_descr = new float[SIFT_DESCRIPTOR_LENGTH];

				//计算描述算子
				vl_sift_calc_keypoint_descriptor(filt, rbuf, k, angles[q]);

				this_frame[0] = k->x;
				this_frame[1] = k->y;
				this_frame[2] = k->sigma;
				this_frame[3] = angles[q];
				frames.push_back(this_frame);

				for (uint j = 0; j < SIFT_DESCRIPTOR_LENGTH; j++) {
					this_descr[j] = divide_512 ? rbuf[j] : 512.0f * rbuf[j];
				}
				descriptors.push_back(this_descr);
				nframes++;
			}
		}
	}

	//释放过滤器
	if (filt) {
		vl_sift_delete(filt);
		filt = nullptr;
	}
	//释放图像数据
	if (data) {
		delete[] data;
		data = nullptr;
	}

	num_descriptor = nframes;
	chrono::duration<double> elapsed_seconds = chrono::system_clock::now()
			- start_time;
	BOOST_LOG_TRIVIAL(debug)<< image_path << " 关键点数:" << nframes <<",耗时:" << elapsed_seconds.count() << "s";

	return true;
}

bool sift_feat::save_keypoints_and_descriptor(const vector<float*>& keypoints,
		const vector<float*>& descriptors, uint num_descriptor,
		const char *file_path) {
	ofstream out_file;
	out_file.open(file_path, ios::out | ios::binary); //以二进制文件的形式去输出
	if (!out_file.is_open()) {
		BOOST_LOG_TRIVIAL(error)<< "无法生成特征文件:" << file_path;
		return false;
	}

	//保存描述算子和关键点的数量
	out_file.write((char*) &num_descriptor, sizeof(uint));

	//保存每一个关键点及描述算子
	for (uint i = 0; i < num_descriptor; i++) {
		out_file.write((char*) keypoints[i],
				SIFT_KEYPOINT_LENGTH * sizeof(float));
		out_file.write((char*) descriptors[i],
				SIFT_DESCRIPTOR_LENGTH * sizeof(float));
	}

	out_file.close();
	return true;
}

bool sift_feat::image_to_feature(const string &image_path,
		const string &out_feature_path) {
	if(!common::io_utils::is_file_exist(image_path)) {
		BOOST_LOG_TRIVIAL(error) << "无法打开图像文件:" << image_path;
		return false;
	}
	vector<float*> keypoints, descriptors;
	uint num_descriptors;
	//这里不会除去512，因为在检索时默认关键点-描述符文件被乘上了512
	if(get_keypoints_and_descriptors(image_path.c_str(),false,keypoints,descriptors,num_descriptors)){
		if(!save_keypoints_and_descriptor(keypoints,descriptors,num_descriptors,out_feature_path.c_str())){
			BOOST_LOG_TRIVIAL(error)<< "无法从'" << image_path << "'生成特征文件'" << out_feature_path << "'";
		}
	}else{
		BOOST_LOG_TRIVIAL(error)<< "无法从" << image_path << "获取关键点-局部描述算子信息";
	}
	//释放空间
	for (auto iter = descriptors.begin(); iter != descriptors.end();
			iter++) {
		float *descriptor = *iter;
		if (descriptor) {
			delete[] descriptor;
			descriptor = nullptr;
		}
	}
	for (auto iter = keypoints.begin(); iter != keypoints.end(); iter++) {
		float *keypoint = *iter;
		if (keypoint) {
			delete[] keypoint;
			keypoint = nullptr;
		}
	}
	return true;
}

}
}
