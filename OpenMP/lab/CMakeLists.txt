find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

set(target app0)
add_executable(${target} var0/app0.cpp)

set(target app0_mpi)
add_executable(${target} var0/app0_mpi.cpp)
target_link_libraries(${target} PRIVATE ${MPI_CXX_LIBRARIES})
target_include_directories(${target} PRIVATE ${MPI_INCLUDE_PATH})

set(target app0_omp)
add_executable(${target} var0/app0_omp.cpp)
target_compile_options(${target} PRIVATE -Wall ${OpenMP_CXX_FLAGS})
target_link_libraries(${target} PRIVATE ${OpenMP_CXX_FLAGS})

set(target app1)
add_executable(${target} var1a/app1.cpp)

set(target app1_mpi)
add_executable(${target} var1a/app1_mpi.cpp)
target_link_libraries(${target} PRIVATE ${MPI_CXX_LIBRARIES})
target_include_directories(${target} PRIVATE ${MPI_INCLUDE_PATH})

set(target app3)
add_executable(${target} var3e/app3.cpp)

set(target app3_omp)
add_executable(${target} var3e/app3_omp.cpp)
target_compile_options(${target} PRIVATE -Wall ${OpenMP_CXX_FLAGS})
target_link_libraries(${target} PRIVATE ${OpenMP_CXX_FLAGS})