#include "shot_detector.h"

#include <cmath>
#include <string>
#include <opencv/cv.hpp>
#include <boost/log/trivial.hpp>

using namespace std;
using namespace cv;

namespace vrs {
namespace components {

namespace shot_detector {
bool shot_bound(const Mat &curr_frame, const Mat &prev_frame,
		double threshold) {
	//转换为HSV直方图
	Mat hsv1, hsv2;
	bool is_shot_bound = false;
	//如有有必要则进行缩放
	if (curr_frame.cols < FRAME_WIDTH) {
		resize(curr_frame, curr_frame,
				Size(FRAME_WIDTH,
						round(
								(FRAME_WIDTH
										/ static_cast<float>(curr_frame.cols))
										* static_cast<float>(curr_frame.rows))));
	}
	if (prev_frame.cols < FRAME_WIDTH) {
		resize(prev_frame, prev_frame,
				Size(FRAME_WIDTH,
						round(FRAME_WIDTH / static_cast<float>(prev_frame.cols))
								* static_cast<float>(prev_frame.rows)));
	}

	//转换到HSV(HSV 为色相，饱和度，明度) 《图像处理、分析与机器视觉》P27
	cvtColor(curr_frame, hsv1, CV_BGR2HSV);
	cvtColor(prev_frame, hsv2, CV_BGR2HSV);

	//计算hsv直方图
	int hbins = 16, sbins = 16, vbins = 16;
	int hist_size[] = { hbins, sbins, vbins };
	//hue(色调)、saturation(饱和度),value(值)范围变化
	float hranges[] = { 0, 180 }, sranges[] = { 0, 256 },
			vranges[] = { 0, 256 };
	const float *ranges[] = { hranges, sranges, vranges };
	MatND hist1, hist2;
	int channels[] = { 0, 1, 2 };
	bool b_uniform = true, b_accumulate = false;

	calcHist(&hsv1, 1, channels, Mat(), hist1, 3, hist_size, ranges, b_uniform,
			b_accumulate);
	calcHist(&hsv2, 1, channels, Mat(), hist2, 3, hist_size, ranges, b_uniform,
			b_accumulate);

	normalize(hist1, hist1, 1, 0, NORM_L1, -1, Mat());
	normalize(hist2, hist2, 1, 0, NORM_L1, -1, Mat());

	//对比直方图
	double d_corr = compareHist(hist1, hist2, CV_COMP_INTERSECT);
	is_shot_bound = d_corr < threshold;
	BOOST_LOG_TRIVIAL(debug)<< "对比得分=" << d_corr;
	BOOST_LOG_TRIVIAL(debug)<< "是否找到一个镜头?=" << is_shot_bound;
	return is_shot_bound;
}
}

}
}
