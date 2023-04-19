set(INLINE_VECTOR_WRAPPER_SRC_FILES src/invec_wrapper.cpp)

add_executable(inline_vector_wrapper ${INLINE_VECTOR_WRAPPER_SRC_FILES})

set_target_properties(inline_vector_wrapper PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)