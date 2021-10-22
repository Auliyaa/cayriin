# library: rawvideo
add_library(rawvideo STATIC
  src/imgbuf.cpp
  src/v210.cpp
)
target_include_directories(rawvideo PUBLIC
  include
)

# samples
if(CAYRIIN_SAMPLES)
  add_subdirectory(samples)
endif(CAYRIIN_SAMPLES)

# TODO: tests
