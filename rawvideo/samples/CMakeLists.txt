function(add_sample name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} rawvideo)
endfunction(add_sample)

add_sample(convert)
