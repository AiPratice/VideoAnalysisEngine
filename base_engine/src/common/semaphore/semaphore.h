#pragma once

#include <condition_variable>
#include <mutex>

namespace vrs {
namespace common {

/*
 * 信号量
 */
class semaphore {
private:
	std::mutex mtx;
	std::condition_variable convar;
	int count;

public:
	semaphore(int count = 0);
	virtual ~semaphore();

	inline void notify();

	inline void wait();
};

} /* namespace common */
} /* namespace vrs */
