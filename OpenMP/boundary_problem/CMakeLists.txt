find_package(OpenMP REQUIRED)

set(target boundary)
add_executable(${target} boundary_problem.cpp)
target_compile_options(${target} PUBLIC ${OpenMP_CXX_FLAGS})
target_link_libraries(${target} PUBLIC ${OpenMP_CXX_FLAGS})