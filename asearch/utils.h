////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include <sndfile.h>
#include <armadillo>

/* Open stereo file and place channels in rows of a matrix */
sf_count_t load_stereo_file_into_matrix(SNDFILE *sndfile, sf_count_t nframes, arma::mat& data );