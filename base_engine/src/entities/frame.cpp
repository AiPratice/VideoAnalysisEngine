#include <string>
#include <sstream>
#include "frame.h"

using namespace std;

namespace vrs{
namespace entities{

string frame::to_string(){
	stringstream stream;
	stream << '{';
	stream << "frame_id:" << frame_id;
	stream << ",video_id:" << video_id;
	stream << ",img_path:" << img_path;
	stream << ",feature_path:" << feature_path;
	stream << ",is_shot_frame:" << is_shot_frame;
	stream << '}';
	return stream.str();
}

}
}
