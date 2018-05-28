#include "config.h"
#include <string>

using namespace std;

namespace vrs
{

string config::db_conn;
string config::rpc_server_address;
string config::videos_dir_path;
string config::keyframes_dir_path;
string config::local_descriptors_dir_path;
string config::global_descriptors_dir_path;
string config::local_descriptor_extension;
string config::temp_dir_path;
string config::gdindex_filename;
string config::gdindex_parameters_path;
string config::gdindex_path;

void config::init(const string &config_file_path)
{
	YAML::Node yaml_config = YAML::LoadFile(config_file_path);

	if (yaml_config["db_conn"]) {
		config::db_conn = yaml_config["db_conn"].as<string>();
	}
	if(yaml_config["rpc_server_address"]){
		config::rpc_server_address = yaml_config["rpc_server_address"].as<string>();
	}
	if (yaml_config["videos_dir_path"]) {
		config::videos_dir_path =
		    yaml_config["videos_dir_path"].as<string>();
	}
	if (yaml_config["keyframes_dir_path"]) {
		config::keyframes_dir_path = yaml_config["keyframes_dir_path"].as<
		                             string>();
	}
	if (yaml_config["local_descriptors_dir_path"]) {
		config::local_descriptors_dir_path =
		    yaml_config["local_descriptors_dir_path"].as<string>();
	}
	if (yaml_config["global_descriptors_dir_path"]) {
		config::global_descriptors_dir_path =
		    yaml_config["global_descriptors_dir_path"].as<string>();
	}
	if (yaml_config["local_descriptor_extension"]) {
		config::local_descriptor_extension =
		    yaml_config["local_descriptor_extension"].as<string>();
	}
	if(yaml_config["temp_dir_path"]) {
		config::temp_dir_path = yaml_config["temp_dir_path"].as<string>();
	}
	if(yaml_config["gdindex_filename"]) {
		config::gdindex_filename = yaml_config["gdindex_filename"].as<string>();
	}
	if(yaml_config["gdindex_parameters_path"]){
		config::gdindex_parameters_path = yaml_config["gdindex_parameters_path"].as<string>();
	}

	config::gdindex_path = global_descriptors_dir_path+string("/")+gdindex_filename;
}

}
