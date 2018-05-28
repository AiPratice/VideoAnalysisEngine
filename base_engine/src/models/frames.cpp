/*
 * frames.cpp
 *
 *  Created on: 2018年1月13日
 *      Author: Yunquan Cheng
 */

#include "frames.h"
#include "base_model.h"

#include <sstream>
#include <cstring>
#include <boost/log/trivial.hpp>
#include <cstdint>
#include "../entities/frame.h"

using namespace std;

namespace vrs {
namespace models {

frames::frames() :
		base_model() {

}

frames::~frames() {
}

bool frames::insert_frame(const entities::frame &frame) {
	try {
		pqxx::work txn(*base_model::db_con);
		if (!txn.prepared("insert_frame").exists()) {
			const char *sql_template =
					"INSERT INTO public.frames(video_id,img_path,feature_path,is_shot_frame,second) VALUES ($1,$2,$3,$4,$5)";
			base_model::db_con->prepare("insert_frame", sql_template);
		}
		txn.prepared("insert_frame")(frame.video_id)(frame.img_path)(
				frame.feature_path)(frame.is_shot_frame)(frame.second).exec();
		txn.commit();
	} catch (const std::exception &e) {
		BOOST_LOG_TRIVIAL(error)<< e.what();
		return false;
	}
	return true;
}

vector<entities::frame> frames::get_all_frames() {
	const char *sql =
			"SELECT frame_id,video_id,img_path,feature_path,is_shot_frame from public.frames";
	vector<entities::frame> frames;
	BOOST_LOG_TRIVIAL(debug)<< "sql:" << sql;
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec(sql);
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		entities::frame frame;
		frame.frame_id = r[i][0].as<uint64_t>();
		frame.video_id = r[i][1].as<uint32_t>();
		frame.img_path = r[i][2].as<string>();
		frame.feature_path = r[i][3].as<string>();
		frame.is_shot_frame = r[i][4].as<bool>();
		frames.push_back(frame);
	}
	return frames;
}

void frames::truncate() {
	const char *sql = "TRUNCATE TABLE public.frames";
	BOOST_LOG_TRIVIAL(debug)<< "sql:" << sql;
	pqxx::work txn(*base_model::db_con);
	txn.exec(sql);
	txn.commit();
}

vector<string> frames::get_feature_paths() {
	vector<string> paths;
	const char *sql = "SELECT feature_path FROM frames";
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec(sql);
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		paths.push_back(r[i][0].as<string>());
	}
	return paths;
}

vector<string> frames::get_img_paths() {
	vector<string> paths;
	const char *sql = "SELECT img_path FROM frames";
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec(sql);
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		paths.push_back(r[i][0].as<string>());
	}
	return paths;
}

vector<uint64_t> frames::get_frame_ids() {
	vector<uint64_t> frame_ids;
	const char *sql = "SELECT frame_id FROM frames WHERE";
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec(sql);
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		frame_ids.push_back(r[i][0].as<uint64_t>());
	}
	return frame_ids;
}

vector<uint64_t> frames::get_shot_frame_ids(){
	vector<uint64_t> frame_ids;
	const char *sql = "SELECT frame_id FROM frames WHERE is_shot_frame=true ORDER BY video_id";
	//const char *sql = "SELECT frame_id FROM frames WHERE is_shot_frame=true AND video_id IN (3)";
	pqxx::work txn(*base_model::db_con);
	pqxx::result r = txn.exec(sql);
	for (pqxx::result::size_type i = 0; i < r.size(); i++) {
		frame_ids.push_back(r[i][0].as<uint64_t>());
	}
	return frame_ids;
}

vector<string> frames::get_feature_paths_by_video(uint32_t video_id) {
	const char *sql_template =
			"SELECT feature_path FROM frames WHERE video_id=$1";
	vector<string> paths;
	try {
		pqxx::work txn(*base_model::db_con);
		if (!txn.prepared("get_feature_paths_by_video").exists()) {
			db_con->prepare("get_feature_paths_by_video", sql_template);
		}
		pqxx::result r =
				txn.prepared("get_feature_paths_by_video")(video_id).exec();
		for (pqxx::result::size_type i = 0; i < r.size(); i++) {
			paths.push_back(r[i][0].as<string>());
		}
	} catch (const std::exception &e) {
		BOOST_LOG_TRIVIAL(error)<< "获取视频"<< video_id <<"特征路径失败";
		BOOST_LOG_TRIVIAL(error) << e.what();
	}

	return paths;
}

vector<string> frames::get_shot_feature_paths_by_video(uint32_t video_id) {
	const char *sql_template =
			"SELECT feature_path FROM frames WHERE video_id=$1 AND is_shot_frame=true";
	vector<string> paths;
	try {
		pqxx::work txn(*base_model::db_con);
		if (!txn.prepared("get_shot_feature_paths_by_video").exists()) {
			db_con->prepare("get_shot_feature_paths_by_video", sql_template);
		}
		pqxx::result r = txn.prepared("get_shot_feature_paths_by_video")(
				video_id).exec();
		for (pqxx::result::size_type i = 0; i < r.size(); i++) {
			paths.push_back(r[i][0].as<string>());
		}
	} catch (const std::exception &e) {
		BOOST_LOG_TRIVIAL(error)<< "获取视频"<< video_id <<"特征路径失败";
		BOOST_LOG_TRIVIAL(error) << e.what();
	}

	return paths;
}

}
} /* namespace vrs */
