cmake_minimum_required(VERSION 2.8)

project (circbuffer)
#add_executable(circbuffer circbuffer.cpp)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

#include_directories(${Boost_INCLUDE_DIRS}) 
add_executable(circbuffer circbuffer.cpp) 
target_link_libraries(circbuffer boost_system boost_thread)                                                                                                                                                                                                                            

