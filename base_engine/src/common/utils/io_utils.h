#pragma once

#include <list>
#include <vector>
#include <string>

namespace vrs {
namespace common{

class io_utils {
public:
	/*
	 * 从文件夹中获取所有文件名
	 * const char* dir_path 文件夹路径
	 * const char* extension 文件拓展名，例如.jpg
	 * files 用于接收文件名的容器
	 */
	static bool get_dir_files(const char *dir_path,const char *extension,std::list<std::string> &files);
	static bool get_dir_files(const char *dir_path,const char *extension,std::vector<std::string> &files);

	/*
	 * 从文件夹中获取所有文件的路径
	 * const char *dir_path 文件夹路径
	 * const char *extension 文件拓展名
	 * file_paths 用于接收文件路径的容器
	 */
	static bool get_dir_file_paths(const char *dir_path,const char *extension,std::vector<std::string> &file_paths,bool is_order=false);

	/*
	 * 判断文件是否以及存在
	 * @param path 路径
	 */
	static bool is_file_exist(const char *path);
	static inline bool is_file_exist(const std::string &path){
		return is_file_exist(path.c_str());
	}

	/*
	 * 如果指定路径的文件夹不存在则创建新的文件夹
	 * @param path 文件夹路径
	 */
	static void notexist_createdir(const std::string &path);
};


}
}

