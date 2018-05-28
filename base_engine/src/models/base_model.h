#pragma once

#include <pqxx/pqxx>
#include <sys/types.h>

namespace vrs
{

namespace models
{

class base_model {
protected:
	static constexpr uint SQL_BUF_SIZIE = 512;
	pqxx::connection *db_con = nullptr;
public:
	base_model();
	virtual ~base_model();
};

}

}

