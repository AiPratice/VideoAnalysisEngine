#pragma once
/*
 * base_task.h
 *	任务基类
 *  Created on: 2018年1月14日
 *      Author: Yunquan Cheng
 */


#include <string>

namespace vrs {
namespace task{
class base_task {
protected:
	std::string task_name;//任务名称
public:
	base_task(const std::string &task_name){
		this->task_name = task_name;
	}

	virtual ~base_task(){

	}

	const std::string& get_task_name() const {
		return task_name;
	}

};

}
} /* namespace vrs */
