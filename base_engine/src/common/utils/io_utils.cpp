/*
 * ioutils.cpp
 *
 *  Created on: 2017年12月27日
 *      Author: Yunquan Cheng
 */

#include "io_utils.h"

#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <list>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

using namespace std;

namespace vrs {
namespace common{

bool io_utils::get_dir_files(const char *dir_path, const char *extension,
		list<string> &files) {
	DIR *dir = opendir(dir_path);
	if (!dir) {
		BOOST_LOG_TRIVIAL(error)<< "无法打开文件夹:" << dir_path;
		return false;;
	}
	struct dirent *ptr;
	while ((ptr = readdir(dir)) != NULL) {
		char *filename = ptr->d_name;
		if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
			continue;
		}
		if (extension != NULL) {
			char *_extension = strrchr(filename, '.');
			if (strcmp(extension, _extension) == 0) {
				files.push_back(filename); //判断文件后缀是否吻合
			}
		} else {
			files.push_back(filename);
		}
	}
	return true;
}

bool io_utils::get_dir_files(const char *dir_path, const char *extension,
		vector<string> &files) {
	DIR *dir = opendir(dir_path);
	if (!dir) {
		BOOST_LOG_TRIVIAL(error)<< "无法打开文件夹:" << dir_path;
		return false;;
	}
	struct dirent *ptr;
	while ((ptr = readdir(dir)) != NULL) {
		char *filename = ptr->d_name;
		if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
			continue;
		}
		if (extension != NULL) {
			char *_extension = strrchr(filename, '.');
			if (strcmp(extension, _extension) == 0) {
				files.push_back(filename); //判断文件后缀是否吻合
			}
		} else {
			files.push_back(filename);
		}
	}
	closedir(dir);
	return true;
}

bool io_utils::get_dir_file_paths(const char *dir_path, const char *extension,
		vector<string> &file_paths,bool is_order) {
	DIR *dir = opendir(dir_path);
	string new_dir_path = dir_path;
	if (!dir) {
		BOOST_LOG_TRIVIAL(error)<< "无法打开文件夹:" << dir_path;
		return false;;
	}
	struct dirent *ptr;
	if(dir_path[strlen(dir_path) - 1] != '/'){
		new_dir_path += '/';
	}
	while ((ptr = readdir(dir)) != NULL) {
		char *filename = ptr->d_name;
		if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
			continue;
		}
		if (extension != NULL) {
			char *_extension = strrchr(filename, '.');
			if (strcmp(extension, _extension) == 0) {
				file_paths.push_back(new_dir_path+filename); //判断文件后缀是否吻合
			}
		} else {
			file_paths.push_back(new_dir_path+filename);
		}
	}
	closedir(dir);
	if(is_order){
		//对vector进行字典排序
		std::sort(file_paths.begin(),file_paths.end());
	}
	return true;
}

bool io_utils::is_file_exist(const char *path) {
	struct stat stat_buffer;
	return path != nullptr && stat(path, &stat_buffer) == 0;
}

void io_utils::notexist_createdir(const string &path){
	boost::filesystem::path p(path);
	if(!boost::filesystem::exists(p)){
		boost::filesystem::create_directories(p);
	}
}

}
}