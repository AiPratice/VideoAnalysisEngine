/*
 * semaphore.cpp
 *  信号量
 *  Created on: 2018年2月5日
 *      Author: pkdog
 */

#include "semaphore.h"

using namespace std;

namespace vrs {
namespace common {

semaphore::semaphore(int count) :
		count(count) {

}

semaphore::~semaphore() {

}

void semaphore::notify() {
	unique_lock<mutex> lock(mtx);
	count++;
	convar.notify_one();
}

void semaphore::wait() {
	unique_lock<mutex> lock(mtx);
	while (count == 0) {
		convar.wait(lock);
	}
	count--;
}

} /* namespace common */
} /* namespace vrs */
