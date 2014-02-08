cmake_minimum_required (VERSION 2.6) 
project (asearch)

add_executable (asearch asearch.cpp)

target_link_libraries (asearch  armadillo sndfile)   
