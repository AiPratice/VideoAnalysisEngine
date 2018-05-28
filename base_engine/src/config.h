#pragma once

#include <boost/log/trivial.hpp>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/parse.h>

namespace vrs
{

class config {
private:
	static std::string db_conn; //postgresql 数据库连接
	static std::string rpc_server_address; //rpc服务器地址
	static std::string videos_dir_path; //视频存储文件夹路径
	static std::string keyframes_dir_path; //视频帧图像存储文件夹路径
	static std::string local_descriptors_dir_path; //局部描述算子存储文件夹路径
	static std::string global_descriptors_dir_path; //全局描述算子存储文件夹路径
	static std::string gdindex_filename; //全局描述算子文件名
	static std::string gdindex_parameters_path; //全局描述算子参数文件夹路径
	static std::string gdindex_path; //全局描述算子文件路径
	static std::string temp_dir_path; //临时文件存储文件夹路径
	static std::string local_descriptor_extension; //局部描述算子文件拓展名

public:
	/**
	 * 初始化配置文件
	 * @param config_file_path 配置文件路径
	 */
	static void init(const std::string &config_file_path);

	static inline const std::string& get_db_conn() {
		return db_conn;
	}

	static inline const std::string& get_global_descriptors_dir_path(){
		return global_descriptors_dir_path;
	}

	static inline const std::string& get_keyframes_dir_path(){
		return keyframes_dir_path;
	}

	static inline const std::string& get_local_descriptors_dir_path() {
		return local_descriptors_dir_path;
	}

	static inline const std::string& get_videos_dir_path() {
		return videos_dir_path;
	}

	static inline const std::string& get_local_descriptor_extension(){
		return local_descriptor_extension;
	}

	static inline const std::string& get_temp_dir_path(){
		return temp_dir_path;
	}
	
	static inline const std::string& get_gdindex_filename(){
		return gdindex_filename;
	}
	
	static inline const std::string& get_gdindex_path(){
		return gdindex_path;
	}

	static inline const std::string& get_gdindex_parameters_path(){
		return gdindex_parameters_path;
	}

	static inline const std::string& get_rpc_server_address(){
		return rpc_server_address;
	}
};

}

