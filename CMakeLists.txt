cmake_minimum_required (VERSION 2.6) 

SET(CMAKE_BUILD_TYPE "Release")

SET( PD_INC C:/Users/GVITC/Documents/_Matthias/pd-bin/src ) 
SET( PD_LIB C:/Users/GVITC/Documents/_Matthias/pd-bin/bin ) 

SET( GEM_INC C:/Users/GVITC/Documents/_Matthias/Gem-0.93.3-bin/Gem/dev ) 
SET( GEM_LIB C:/Users/GVITC/Documents/_Matthias/Gem-0.93.3-bin/Gem/dev ) 

INCLUDE_DIRECTORIES( . .. ${PD_INC} ${GEM_INC} )
LINK_DIRECTORIES( ${PD_LIB} ${GEM_LIB} )

add_library (pix_threshold_depth SHARED pix_threshold_depth.cpp) 

target_link_libraries (pix_threshold_depth Gem pd)