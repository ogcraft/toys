/***************************************************************************
* This file is part of last.fm fingerprint app                             *
*  Last.fm Ltd <mir@last.fm>                                               *
*                                                                          *
* The last.fm fingerprint app is free software: you can redistribute it    *
* and/or modify it under the terms of the GNU General Public License as    *
* published by the Free Software Foundation, either version 3 of the       *
* License, or (at your option) any later version.                          *
*                                                                          *
* The last.fm fingerprint app and library are distributed in the hope that *
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied    *
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    *
* the GNU General Public License for more details.                         *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with The last.fm fingerprint.                                      *
* If not, see <http://www.gnu.org/licenses/>.                              *
*                                                                          *
* Part of this code is based on the work of Y. Ke, D. Hoiem, and           *
* R. Sukthankar - "Computer Vision for Music Identification",              *
* in Proceedings of Computer Vision and Pattern Recognition, 2005.         *
* See also http://www.cs.cmu.edu/~yke/musicretrieval/                      *
***************************************************************************/

#ifndef LASTFM_LIBFPLIB_OPT_FFT_H__
#define LASTFM_LIBFPLIB_OPT_FFT_H__

#include <fftw3.h>
#include <vector>

namespace fingerprint
{

class OptFFT
{
public:

   OptFFT(const size_t maxDataSize);
   ~OptFFT();

   int 
   process(float* pInData, const size_t dataSize);
   
   float** 
   getFrames()    { return m_pFrames; }

private:

   void applyHann(float* pInData, const size_t dataSize);

	fftwf_plan        m_p;
	fftwf_complex *   m_pOut;
	float*            m_pIn;

   //float   m_base;

   int     m_numSamples;
   int     m_numOutSamples;

   float** m_pFrames;
   int     m_maxFrames;

   std::vector<int> m_powTable;

};

} // end of namespace

#endif // OPT_FFT
