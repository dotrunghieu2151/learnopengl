add_library(Vendors STATIC)

target_sources(Vendors 
  PRIVATE
    stbImage/stb_image.cpp
)
target_include_directories(Vendors PUBLIC ${CMAKE_CURRENT_LIST_DIR})