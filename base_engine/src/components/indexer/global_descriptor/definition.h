#pragma once

#include <vector>
#include <string>
#include <sys/types.h>
#include <cstdint>

extern "C" {
#include <yael/gmm.h>
}

//using namespace std;

namespace vrs {

// Default initializations for private variables
constexpr std::uint32_t LD_LENGTH_DEFAULT = 128;
constexpr std::uint32_t LD_FRAME_LENGTH_DEFAULT = 4;
constexpr const char *LD_EXTENSION_DEFAULT = ".siftb";
constexpr const char *LD_NAME_DEFAULT = "sift";
constexpr float LD_PRE_PCA_POWER_DEFAULT = 0.5;
constexpr std::uint32_t GD_NUMBER_GAUSSIANS_DEFAULT = 512;
constexpr float GD_POWER_DEFAULT = 0.5;
constexpr bool GD_INTRA_NORMALIZATION_DEFAULT = false;
constexpr bool GD_UNBINARIZED_DEFAULT = false;
constexpr std::uint32_t MIN_NUMBER_WORDS_SELECTED_DEFAULT = 20;
constexpr int ASYM_SCORING_MODE_DEFAULT = 1; // default is ASYM_QAGS
constexpr float SCORE_DEN_POWER_NORM_DEFAULT = 0.5;
constexpr int WORD_SELECTION_MODE_DEFAULT = 0;
constexpr float WORD_SELECTION_THRESH_DEFAULT = 7;

constexpr const char *SIFT_EXTENSION = LD_EXTENSION_DEFAULT;
constexpr const char *SIFTGEO_EXTENSION = ".siftgeo";
constexpr const char *SIFT_NAME = LD_NAME_DEFAULT;
constexpr const char *SIFTGEO_NAME = "siftgeo";

// Hamming distance above which score is just set to zero
const std::uint32_t CORR_WEIGHTS_CLIPPING = 16;

// L2 norm sq thresh
const double L2_NORM_SQ_THRESH = 0.00001;

// Modes for shot detection
enum {
	SHOT_MODE_INDEP_KEYF = 0,
	SHOT_MODE_SHOT_AGG = 1,
	SHOT_MODE_GLOBAL_AGG = 2,
	SHOT_MODE_TRACK_AGG = 3
};
// Modes for local descriptor
enum {
	SIFT_LOCAL_DESCRIPTOR = 0, SIFTGEO_LOCAL_DESCRIPTOR = 1
};
// Modes for asymmetric scoring
enum {
	ASYM_OFF = 0, ASYM_QAGS = 1, ASYM_DAGS = 2, ASYM_SGS = 3
};
// SIFT constants
enum {
	SIFT_LENGTH = 128, SIFTGEO_LENGTH = 128
};
enum {
	SIFT_FRAME_LENGTH = 4, SIFTGEO_FRAME_LENGTH = 9
};

enum {
	LD_PCA_DIM = 32
};

// Mode used in word selection for querying
enum {
	WORD_L1_NORM = 0, WORD_SOFT_ASSGN = 1
};

// Variables that will hold the index and number of signatures stored
struct struct_index {
	// The index will contain, at any point, either Binarized FVs or FVs,
	// so only one of the two following variables will be used at a given
	// point, depending on index_parameters_.gd_unbinarized
	// By default, Binarized FVs are used
	std::vector<std::vector<std::uint32_t > > word_descriptor; // Binarized FV
	std::vector<std::vector<float> > fv; // FV

	// Auxiliary word selection information
	std::vector<std::vector<float> > word_l1_norms;
	std::vector<std::vector<float> > word_total_soft_assignment;

	// std::vector that keeps frame numbers that are actually indexed in the db,
	// used only when SHOT_MODE_INDEP_KEYF mode is used
	std::vector<std::uint32_t > frame_numbers_in_db;

	// Number of global descriptors in database;
	// this variable is always updated in function update_index
	std::uint32_t number_global_descriptors;

	// Variables which are used when scoring, holding values
	// for each database item; they are always updated in
	// function update_index()
	std::vector<std::uint32_t > number_words_selected;
	std::vector<std::vector<float> > word_l2_norms_sq;

	struct_index() :
			number_global_descriptors(0) {

	}

	~struct_index(){

	}
};

// Index parameters
struct struct_index_parameters {
	// Local descriptor information
	std::uint32_t ld_length;
	std::uint32_t ld_frame_length;
	std::string ld_extension;
	std::string ld_name;

	// Parameters for PCA-ing local descriptors
	std::uint32_t ld_pca_dim; // this is set to a constant (32), such that the
	// binarized signature conveniently fits in a
	// 4-byte unsigned integer
	float ld_pre_pca_power;
	float* ld_mean_vector;
	std::vector<float*> ld_pca_eigenvectors;

	// Parameters used for global descriptor computation
	gmm_t* gd_gmm;
	std::uint32_t gd_number_gaussians;
	float gd_power; // normalization factor if using SSR normalization
	bool gd_intra_normalization; // flag that sets IN normalization (instead
								 // of SSR) -- in this case, gd_power is
								 // unused
	bool gd_unbinarized; // flag that decides if using FV (true) or
						 // BFV (false). Default is false.
	struct_index_parameters() :
			ld_length(LD_LENGTH_DEFAULT), ld_frame_length(
					LD_FRAME_LENGTH_DEFAULT), ld_extension(
					LD_EXTENSION_DEFAULT), ld_name(LD_NAME_DEFAULT), ld_pca_dim(
					LD_PCA_DIM), ld_pre_pca_power(LD_PRE_PCA_POWER_DEFAULT), ld_mean_vector(
					nullptr), gd_gmm(nullptr), gd_number_gaussians(
					GD_NUMBER_GAUSSIANS_DEFAULT), gd_power(GD_POWER_DEFAULT), gd_intra_normalization(
					GD_INTRA_NORMALIZATION_DEFAULT), gd_unbinarized(
					GD_UNBINARIZED_DEFAULT) {

	}

	~struct_index_parameters(){
		if(ld_mean_vector){
			delete[] ld_mean_vector;
			ld_mean_vector = nullptr;
		}
		for(std::uint32_t i=0;i<ld_pca_eigenvectors.size();i++){
			float *item = ld_pca_eigenvectors[i];
			if(item){
				delete[] item;
				item = nullptr;
			}
		}
		if(gd_gmm){
			gmm_delete(gd_gmm);
			gd_gmm = nullptr;
		}
	}
};

// Variables relevant for query time
struct struct_query_parameters {
	// -- Number of minimum words to require for matching
	std::uint32_t min_number_words_selected;
	// -- Scoring mode (options: ASYM_OFF, ASYM_QAGS, ASYM_DAGS, ASYM_SGS)
	int asym_scoring_mode;
	// -- Type of word selection mode in use
	// WORD_L1_NORM: only globalWordL1Norm is used
	// WORD_SOFT_ASSGN: only globalWordTotalSoftAssignment is used
	int word_selection_mode;
	// -- Threshold to use in visual word selection (used in asymmetric mode)
	float word_selection_thresh;
	// -- Score denominator's power normalization
	float score_den_power_norm;
	// -- Parameters used in scoring
	float* fast_corr_weights;
	int pop_count[65536];

	struct_query_parameters()
		:min_number_words_selected(MIN_NUMBER_WORDS_SELECTED_DEFAULT),
		 asym_scoring_mode(ASYM_SCORING_MODE_DEFAULT),
		 word_selection_mode(WORD_SELECTION_MODE_DEFAULT),
		 word_selection_thresh(WORD_SELECTION_THRESH_DEFAULT),
		 score_den_power_norm(SCORE_DEN_POWER_NORM_DEFAULT),
		 fast_corr_weights(nullptr)
	{

	}

	~struct_query_parameters(){
		if(fast_corr_weights){
			delete[] fast_corr_weights;
			fast_corr_weights = nullptr;
		}
	}
};

}