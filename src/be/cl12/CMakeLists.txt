set(BENCHMARK_NAME be_cl12)
set(SOURCE_FILES be_cl12_benchmark.cc main.cc)
set(HEADER_FILES be_cl12_benchmark.h)
set(EXTRA_FILES kernels.cl)

if (OpenCV_FOUND)

  if(OPENCL_INCLUDE_DIRS AND OPENCL_LIBRARIES )
    add_executable(${BENCHMARK_NAME} ${SOURCE_FILES} ${INCLUDE_FILES} ${EXTRA_FILES})

    target_link_libraries(${BENCHMARK_NAME}
      be cl_util time_measurement command_line_option benchmark
      ${OPENCL_LIBRARIES}
      ${OpenCV_LIBS})
    
    foreach(extra_file ${EXTRA_FILES})
      add_custom_command(
	TARGET ${BENCHMARK_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_CURRENT_SOURCE_DIR}/${extra_file}  ${CMAKE_CURRENT_BINARY_DIR}
	)
    endforeach(extra_file)
  endif()
endif()
