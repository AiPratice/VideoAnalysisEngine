#pragma once

#include <string>
#include <vector>
#include <sys/types.h>
#include "../../../common/feature_set/feature_set.h"
#include "definition.h"

extern "C" {
#include <yael/gmm.h>
}

#ifndef PUSH_BIT
#define PUSH_BIT(packed, bit) \
	packed = packed << 1; \
	packed += bit;
#endif

#ifndef POWER_LAW
#define POWER_LAW(v, a, w)                        \
	w = (v >= 0) ? pow(v,a) : -1 * pow(-v,a);
#endif

#ifndef POWER_LAW_SAME
#define POWER_LAW_SAME(v, a)                        \
	v = (v >= 0) ? pow(v,a) : -1 * pow(-v,a);
#endif

namespace vrs
{
namespace components
{

class gdindex
{
public:
	gdindex()
	{

	}

	~gdindex()
	{

	}

	// Index I/O
	// -- write function that is used after indexing, writing
	//    descriptors, l1 norms and total soft assignment information
	//    per Gaussian
	void write(const std::string index_path);
	// -- read function that will load index into index_ variables;
	//    if the index already contains items, it will append items to it.
	//    If both_sel_modes is false, this will load either L1 norms OR
	//    Total Soft Assignment information, depending on
	//    query_parameters_.word_selection_mode
	//    If both_sel_modes is true, this will load BOTH types of
	//    information.
	void read(const std::string index_path, const bool both_sel_modes = false);
	// -- This function writes a file useful in the case of using shots
	//    with independent keyframes indexing
	void write_frame_list(const std::string file_path);

	// Clean variables in index_
	void clean_index();

	// Get number of stored global descriptors in index
	uint get_number_global_descriptors();

	// Generate index: these will populate index_
	// -- frame-based signatures
	void generate_index(const std::vector<std::string>& feature_files,
	                    const int verbose_level = 1);
	// -- shot/scene-based signatures
	void generate_index_shot_based(const std::vector<std::string>& feature_files,
	                               const std::vector<uint>& shot_beg_frames, const int shot_mode,
	                               const int shot_keyf, const int verbose_level = 1);
	// -- generate one global descriptor from features
	void generate_global_descriptor(const common::feature_set* feature_set,
	                                std::vector<uint>& gd_word_descriptor, std::vector<float>& gd_fv,
	                                std::vector<float>& gd_word_l1_norm,
	                                std::vector<float>& gd_word_total_soft_assignment);

	// Functions to generate point-indexed FV, index will be returned
	// by value
	void generate_point_index(const std::vector<std::string>& feature_files,
	                          const int verbose_level, std::vector<std::vector<uint> >& vec_feat_assgns,
	                          std::vector<std::vector<float> >& vec_feat_assgn_weights,
	                          std::vector<std::vector<std::vector<float> > >& vec_feat_residuals);
	// -- generate one point-indexed descriptor from features
	void generate_point_indexed_descriptor(const common::feature_set* feature_set,
	                                       const int verbose_level, std::vector<uint>& feat_assgns,
	                                       std::vector<float>& feat_assgn_weights,
	                                       std::vector<std::vector<float> >& feat_residuals);

	// Query index from query's local descriptor path or pre-computed query global
	// descriptor (if using "query_index_ptr")
	void perform_query(const std::string local_descriptors_path,
	                   const gdindex* query_index_ptr, const uint query_number,
	                   const std::vector<uint>& indices, std::vector< std::pair<float, uint> >& results,
	                   const uint number_2nd_stage_rerank = 0,
	                   gdindex* gdindex_ptr_rerank = NULL,
	                   const std::vector<std::vector<uint> >& group_lists_rerank = std::vector<
	                           std::vector<uint> >(), const int verbose_level = 1);

	// Function to set index_parameters_
	void set_index_parameters(const uint ld_length,
	                          const uint ld_frame_length,
	                          const std::string ld_extension,
	                          const std::string ld_name,
	                          const uint ld_pca_dim,
	                          const float ld_pre_pca_power,
	                          const uint gd_number_gaussians,
	                          const float gd_power,
	                          const bool gd_intra_normalization,
	                          const bool gd_unbinarized,
	                          const std::string trained_parameters_path,
	                          const int verbose_level = 1);

	// Function to set query_parameters_
	// -- Note that the correlation weights loaded here require that
	//    some of the index_parameters_ variables be set. So, ALWAYS
	//    load index_parameters_ BEFORE loading query_parameters_
	void set_query_parameters(const uint min_number_words_selected,
	                          const int asym_scoring_mode, const int word_selection_mode,
	                          const float word_selection_thresh, const float score_den_power_norm,
	                          const std::string trained_parameters_path, const int verbose_level = 1);

private:
	/************ Variable  *************/
	struct_index index_;

	struct_index_parameters index_parameters_;

	struct_query_parameters query_parameters_;

	/************ End of Variables  *************/

	/************ Functions *************/
	// Update index after additions or deletions to it
	void update_index();

	// Sign binarize residuals
	void sign_binarize(const std::vector<float>& gd_word_residuals,
	                   std::vector<uint>& gd_word_descriptor);

	// PCA projection for local descriptors
	void project_local_descriptor_pca(const float* desc, float* pca_desc);

	// This function samples the number_frames_out from the shot that begins at
	// frame first_frame and contains number_frames_this_shot, and returns the frame
	// indices in a sorted std::vector.
	// If one frame is requested (number_frames_out = 1), it will return the center one;
	// otherwise, it will try to take equally spaced frames; if this results in frames
	// too concentrated at the beginning or the end, it will take only the center ones.
	void sample_frames_from_shot(const uint number_frames_out,
	                             const uint first_frame, const uint number_frames_this_shot,
	                             std::vector<uint>& out_frames);

	// Obtain score for database item, given a query descriptor
	void score_database_item(const std::vector<uint>& query_word_descriptor,
	                         const std::vector<float>& query_fv,
	                         const std::vector<float>& query_word_l1_norm,
	                         const std::vector<float>& query_word_total_soft_assignment,
	                         const std::vector<float>& query_word_l2_norm, const uint db_ind,
	                         float& score);

	// Query index with query global descriptor
	void query(const std::vector<uint>& query_word_descriptor,
	           const std::vector<float>& query_fv,
	           const std::vector<float>& query_word_l1_norm,
	           const std::vector<float>& query_word_total_soft_assignment,
	           const std::vector<uint>& database_indices,
	           std::vector< std::pair<float, uint> >& database_scores_indices);

	// Query index with global descriptor in a second stage
	void query_2nd_stage(const std::vector<uint>& query_word_descriptor,
	                     const std::vector<float>& query_fv,
	                     const std::vector<float>& query_word_l1_norm,
	                     const std::vector<float>& query_word_total_soft_assignment,
	                     const uint number_2nd_stage_rerank,
	                     const std::vector<std::vector<uint> >& group_lists_rerank,
	                     const std::vector< std::pair<float, uint> >& first_stage_scores_indices,
	                     std::vector< std::pair<float, uint> >& database_scores_indices);

	// Functions to load trained parameters from index_parameters_
	void load_ld_mean_vector(std::string path);
	void load_ld_pca_eigenvectors(std::string path);
	void load_gd_gmm(std::string path);

	// Functions to load trained parameters from query_parameters_
	void load_corr_weights(std::string path);

	// Helper function to compare pairs
	static bool cmp_float_uint_ascend(const  std::pair<float, uint> pair1,
	                                  const  std::pair<float, uint> pair2)
	{
		return pair1.first < pair2.first;
	}
	/************ End of Functions *************/

};

}
}
