cmake_minimum_required (VERSION 2.6) 
project (asearch)

add_executable (asearch asearch.cpp utils.cpp)

target_link_libraries (asearch /home/olegg/src/Fingerprinter/lib/libfplib.a fftw3f samplerate armadillo sndfile boost_iostreams boost_system )   

