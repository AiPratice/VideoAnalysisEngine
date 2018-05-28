#pragma once

#include "../indexer/global_descriptor/gdindex.h"
#include "../indexer/global_descriptor/definition.h"
#include <string>
#include <vector>

namespace vrs
{
namespace components
{

constexpr uint DEFAULT_NUM_OUTPUT_RESULTS = 100;
//constexpr const char *TEMP_FOLDER_PATH = "storage/temp";

class searcher
{
private:
	/*************配置信息*************/
	std::string gdindex_path; //全局描述算子索引路径
	std::string gdindex_trained_parameters_path = "./trained_parameters"; //预训练参数文件夹路径
	std::string query_index_path;
	int local_descriptor_mode = SIFT_LOCAL_DESCRIPTOR; //局部描述算子模式
	int shot_mode = -1; //镜头模式
	uint number_scenes_to_rerank = 0;
	uint number_gaussians = 512;
	uint number_gaussians_rerank = 0;
	int word_selection_mode = WORD_SOFT_ASSGN;
	float word_selection_thresh = 7;
	float word_selection_thresh_rerank = 6;
	uint min_number_words_visited = 0;
	std::string gdindex_path_rerank;
	bool avoid_redundant_scene_results = true;
	bool gd_intra_normalization = false;
	bool gd_unbinarized = false;
	int asym_scoring_mode = ASYM_QAGS;
	int asym_scoring_mode_rerank = ASYM_QAGS;
	float score_den_power_norm = 0.5;
	float score_den_power_norm_rerank = 0.5;
	/*************配置信息*************/

	gdindex *gi = nullptr; //全局描述算子数据库指针
	// Extra GDIndex database, pointer to it; This is used in the case we
	// are retrieving in two stages (eg, first with scenes signatures, then
	// with shot signatures)
	gdindex *gi_rerank = nullptr;
	gdindex *query_index = nullptr; //查询数据库索引指针
	int verbose_level = 3; //debug详细程度
	uint num_output_results = DEFAULT_NUM_OUTPUT_RESULTS; //输出搜索结果的数量
	//vector<std::string> db_list; //每一帧图像所在的路径
	// This std::vector is used when we need to assign different nums to the database instances, when doing evaluation; this has been used when dealing with different frame rates
	std::vector<uint> keyframe_ids_for_eval;
	// This std::vector is used when we need to load shot indices, to do retrieval using this information
	std::vector<uint> shot_first_frames;
	std::vector<std::vector<uint> > group_lists_rerank = std::vector<std::vector<uint> >();

	void get_vector_of_strs_from_file_lines(const char *file_path,
	                                        std::vector<std::string>& out);

	void get_vector_of_uints_from_file_lines(const char *file_path,
	        std::vector<uint> &out);

public:
	searcher(const std::string &gdindex_path,const std::vector<uint> &keyframe_ids_for_eval)
	{
		this->gdindex_path = gdindex_path;
		this->keyframe_ids_for_eval = keyframe_ids_for_eval;
	}
	virtual ~searcher();

	/*
	 * 初始化
	 */
	void init();

	/*
	 * 根据图像文件列表进行搜索
	 * const std::string &query_list 用于搜索的图像文件路径
	 */
	void search(const std::vector<std::string> &query_list);

	/*
	 * @brief 根据图像文件进行搜索
	 * @param query_img_path 图像文件路径
	 * @return 返回得分列表
	 */
	std::vector<std::pair<float, uint>> search(
	                                     const std::string &query_img_path);

	/*
	 * 输出搜索结果
	 * const std::vector<pair<float, uint> >& results 结果集
	 */
	void output_results(const std::vector<std::pair<float, uint> >& results);

	/**********getter&setter**************/
	// Set verbose level.
	void set_verbose_level(uint level);

	// Set num of results to output
	void set_num_output_results(uint n);

	const std::vector<uint>& get_keyframe_ids_for_eval() const
	{
		return keyframe_ids_for_eval;
	}

	void set_keyframe_ids_for_eval(const std::vector<uint>& keyframe_ids_for_eval)
	{
		this->keyframe_ids_for_eval = keyframe_ids_for_eval;
	}

	const std::vector<uint>& get_shot_first_frames() const
	{
		return shot_first_frames;
	}

	void set_shot_first_frames(const std::vector<uint>& shot_first_frames)
	{
		this->shot_first_frames = shot_first_frames;
	}

	int get_asym_scoring_mode() const
	{
		return asym_scoring_mode;
	}

	void set_asym_scoring_mode(int asym_scoring_mode = ASYM_QAGS)
	{
		this->asym_scoring_mode = asym_scoring_mode;
	}

	int get_asym_scoring_mode_rerank() const
	{
		return asym_scoring_mode_rerank;
	}

	void set_asym_scoring_mode_rerank(
	    int asym_scoring_mode_rerank = ASYM_QAGS)
	{
		this->asym_scoring_mode_rerank = asym_scoring_mode_rerank;
	}

	bool isavoid_redundant_scene_results() const
	{
		return avoid_redundant_scene_results;
	}

	void set_avoid_redundant_scene_results(bool avoid_redundant_scene_results =
	        true)
	{
		this->avoid_redundant_scene_results = avoid_redundant_scene_results;
	}

	bool isgd_intra_normalization() const
	{
		return gd_intra_normalization;
	}

	void set_gd_intra_normalization(bool gd_intra_normalization = false)
	{
		this->gd_intra_normalization = gd_intra_normalization;
	}

	bool isgd_unbinarized() const
	{
		return gd_unbinarized;
	}

	void set_gd_unbinarized(bool gd_unbinarized = false)
	{
		this->gd_unbinarized = gd_unbinarized;
	}

	const std::string& get_gdindex_path_rerank() const
	{
		return gdindex_path_rerank;
	}

	void set_gdindex_path_rerank(const std::string& gdindex_path_rerank)
	{
		this->gdindex_path_rerank = gdindex_path_rerank;
	}

	const std::string& get_gdindex_trained_parameters_path() const
	{
		return gdindex_trained_parameters_path;
	}

	void set_gdindex_trained_parameters_path(
	    const std::string& gdindex_trained_parameters_path =
	        "storage/trained_parameters")
	{
		this->gdindex_trained_parameters_path = gdindex_trained_parameters_path;
	}

	int get_local_descriptor_mode() const
	{
		return local_descriptor_mode;
	}

	void set_local_descriptor_mode(int local_descriptor_mode =
	                                   SIFT_LOCAL_DESCRIPTOR)
	{
		this->local_descriptor_mode = local_descriptor_mode;
	}

	uint get_min_number_words_visited() const
	{
		return min_number_words_visited;
	}

	void set_min_number_words_visited(uint min_number_words_visited = 0)
	{
		this->min_number_words_visited = min_number_words_visited;
	}

	uint get_num_output_results() const
	{
		return num_output_results;
	}

	uint get_number_gaussians() const
	{
		return number_gaussians;
	}

	void set_number_gaussians(uint number_gaussians = 512)
	{
		this->number_gaussians = number_gaussians;
	}

	uint get_number_gaussians_rerank() const
	{
		return number_gaussians_rerank;
	}

	void set_number_gaussians_rerank(uint number_gaussians_rerank = 0)
	{
		this->number_gaussians_rerank = number_gaussians_rerank;
	}

	uint get_number_scenes_to_rerank() const
	{
		return number_scenes_to_rerank;
	}

	void set_number_scenes_to_rerank(uint number_scenes_to_rerank = 0)
	{
		this->number_scenes_to_rerank = number_scenes_to_rerank;
	}

	float get_score_den_power_norm() const
	{
		return score_den_power_norm;
	}

	void set_score_den_power_norm(float score_den_power_norm = 0.5)
	{
		this->score_den_power_norm = score_den_power_norm;
	}

	float get_score_den_power_norm_rerank() const
	{
		return score_den_power_norm_rerank;
	}

	void set_score_den_power_norm_rerank(
	    float score_den_power_norm_rerank = 0.5)
	{
		this->score_den_power_norm_rerank = score_den_power_norm_rerank;
	}

	int get_shot_mode() const
	{
		return shot_mode;
	}

	void set_shot_mode(int shot_mode = -1)
	{
		this->shot_mode = shot_mode;
	}

	int get_verbose_level() const
	{
		return verbose_level;
	}

	int get_word_selection_mode() const
	{
		return word_selection_mode;
	}

	void set_word_selection_mode(int word_selection_mode = WORD_SOFT_ASSGN)
	{
		this->word_selection_mode = word_selection_mode;
	}

	float get_word_selection_thresh() const
	{
		return word_selection_thresh;
	}

	void set_word_selection_thresh(float word_selection_thresh = 7)
	{
		this->word_selection_thresh = word_selection_thresh;
	}

	float get_word_selection_thresh_rerank() const
	{
		return word_selection_thresh_rerank;
	}

	void set_word_selection_thresh_rerank(
	    float word_selection_thresh_rerank = 6)
	{
		this->word_selection_thresh_rerank = word_selection_thresh_rerank;
	}

	/**********getter&setter**************/

	const std::string& get_gdindex_path() const
	{
		return gdindex_path;
	}

	void set_gdindex_path(const std::string& gdindex_path)
	{
		this->gdindex_path = gdindex_path;
	}

	const std::string& get_query_index_path() const
	{
		return query_index_path;
	}

	void set_query_index_path(const std::string& query_index_path)
	{
		this->query_index_path = query_index_path;
	}
};

}
} /* namespace vrs */
