
#include "utils.h"

using namespace arma;

/* Open stereo file and place channels in rows of a matrix */
sf_count_t load_stereo_file_into_matrix(SNDFILE *sndfile, sf_count_t nframes, mat& data )
{
  int count = 0;
  /* A frame contains samples for all channels at time t */
  double frame[2];
  /* de-interleave data and place into 2 row matrix */
  for(count = 0; count < nframes; count++)
  {
      /* read interleaved frames one-by-one and de-interleave */
      sf_readf_double( sndfile, frame, (sf_count_t)1);
      data(0,count) = frame[0];
      data(1,count) = frame[1];
  }
  return count;
}
