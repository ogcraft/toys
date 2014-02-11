#ifndef __utils_h__
#define __utils_h__

////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include <sndfile.h>
#include <armadillo>

/* Open stereo file and place channels in rows of a matrix */
sf_count_t load_stereo_file_into_matrix(SNDFILE *sndfile, sf_count_t nframes, arma::mat& data );
void spectrogram(const arma::mat& S, int window_sz, int hop_sz, arma::mat& result);


#endif // __utils_h__