#include "base_model.h"
#include "../config.h"
#include <iostream>
#include <pqxx/pqxx>

using namespace std;

namespace vrs {
namespace models{
base_model::base_model() {
	db_con = new pqxx::connection(config::get_db_conn());
	//cout << "base_model" << endl;;
}

base_model::~base_model() {
	if(db_con){
		delete db_con;
	}
	//cout << "~base_model" << endl;
}
}
} /* namespace vrs */

