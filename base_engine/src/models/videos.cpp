/*
 * videos.cpp
 *
 *  Created on: 2018年1月13日
 *      Author: Yunquan Cheng
 */

#include "videos.h"
#include "../entities/video.h"
#include "base_model.h"

#include <boost/log/trivial.hpp>
#include <cstdint>
#include <pqxx/pqxx>
#include <iostream>
#include <vector>
#include <list>

using namespace std;

namespace vrs {
namespace models {

videos::videos() :
		base_model() {
}

videos::~videos() {
}

vector<string> videos::get_all_video_paths() {
	vector<string> video_paths;
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec("SELECT path FROM public.videos");
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		string path = r[i][0].as<string>();
		video_paths.push_back(path);
	}
	return video_paths;
}

list<string> videos::get_all_gdindex_paths() {
	list<string> gdindex_paths;
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec("SELECT gdindex_path FROM public.videos WHERE status = 2 ORDER BY video_id");
	//pqxx::result r = txn.exec("SELECT gdindex_path FROM public.videos WHERE video_id IN (3)");
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		string path = r[i][0].as<string>();
		gdindex_paths.push_back(path);
	}
	return gdindex_paths;
}

list<entities::video> videos::get_all_videos() {
	list<entities::video> videos;
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec(
			"SELECT video_id,name,description,path,status FROM videos");
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		entities::video video;
		video.video_id = r[i][0].as<uint32_t>();
		video.name = r[i][1].as<string>();
		video.description = r[i][2].as<string>();
		video.path = r[i][3].as<string>();
		video.status = r[i][4].as<uint32_t>();
		videos.push_back(video);
	}
	return videos;
}

list<entities::video> videos::get_unprocess_videos() {
	list<entities::video> videos;
	pqxx::work txn(*base_model::db_con);
	const char *sql =
			"SELECT video_id,name,description,path,status FROM videos WHERE status=0";
	pqxx::result r = txn.exec(sql);
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		entities::video video;
		video.video_id = r[i][0].as<uint32_t>();
		video.name = r[i][1].as<string>();
		video.description = r[i][2].as<string>();
		video.path = r[i][3].as<string>();
		video.status = r[i][4].as<uint32_t>();
		videos.push_back(video);
	}
	return videos;
}

list<entities::video> videos::get_pending_videos() {
	list<entities::video> videos;
	const char *sql =
			"SELECT v.video_id,v.name,v.description,v.path,v.status FROM videos v,pending_videos pd WHERE v.video_id=pd.video_id";
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec(sql);
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		entities::video video;
		video.video_id = r[i][0].as<uint32_t>();
		video.name = r[i][1].as<string>();
		video.description = r[i][2].as<string>();
		video.path = r[i][3].as<string>();
		video.status = r[i][4].as<uint32_t>();
		videos.push_back(video);
	}
	return videos;
}

bool videos::insert_pending_video(uint32_t video_id){
	const char *sql_template = "INSERT INTO pending_videos (video_id) VALUES ($1)";
	try{
		pqxx::work txn(*base_model::db_con);
		if(!txn.prepared("insert_pending_video").exists()){
			db_con->prepare("insert_pending_video",sql_template);
		}
		txn.prepared("insert_pending_video")(video_id).exec();
		txn.commit();
	} catch (const std::exception &e) {
		BOOST_LOG_TRIVIAL(error)<< "插入视频id到视频待处理队列";
		BOOST_LOG_TRIVIAL(error) << e.what();
		return false;
	}

	return true;
}

bool videos::set_video_status(uint32_t video_id, uint16_t status) {
	try {
		pqxx::work txn(*base_model::db_con);
		const char *sql_template =
				"UPDATE videos SET status=$1 WHERE video_id=$2";
		if (!txn.prepared("set_video_status").exists()) {

			db_con->prepare("set_video_status", sql_template);
		}
		txn.prepared("set_video_status")(status)(video_id).exec();
		txn.commit();
	} catch (const std::exception &e) {
		BOOST_LOG_TRIVIAL(error)<< "设置视频处理状态失败";
		BOOST_LOG_TRIVIAL(error) << e.what();
		return false;
	}
	return true;
}

entities::video videos::get_video(uint32_t video_id) {
	entities::video video;
	const char *sql_template =
			"SELECT video_id,name,description,path,gdindex_path,status FROM videos WHERE video_id=$1";
	try {
		pqxx::work txn(*base_model::db_con);
		if (!txn.prepared("get_video").exists()) {
			db_con->prepare("get_video", sql_template);
		}
		pqxx::result r = txn.prepared("get_video")(video_id).exec();
		if (r.size() > 0) {
			video.video_id = r[0][0].as<uint32_t>();
			video.name = r[0][1].as<string>();
			video.description = r[0][2].as<string>();
			video.path = r[0][3].as<string>();
			video.gdindex_path = r[0][4].as<string>();
			video.status = r[0][5].as<uint32_t>();
		}
	} catch (const std::exception &e) {
		BOOST_LOG_TRIVIAL(error)<< "获取视频实体失败";
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
	return video;
}

bool videos::set_video(const entities::video &video) {
	const char *sql_template =
			"UPDATE videos SET name=$1,description=$2,path=$3,gdindex_path=$4,status=$5 WHERE video_id=$6";
	try {
		pqxx::work txn(*base_model::db_con);
		if (!txn.prepared("set_video").exists()) {
			db_con->prepare("set_video", sql_template);
		}
		txn.prepared("set_video")(video.name)(video.description)(video.path)(
				video.gdindex_path)(video.status)(video.video_id).exec();
		txn.commit();
	} catch (const std::exception &e) {
		BOOST_LOG_TRIVIAL(error)<< "更新视频数据失败";
		BOOST_LOG_TRIVIAL(error) << e.what();
		return false;
	}
	return true;
}

}
} /* namespace vrs */
