add_library(kmeansAlgorithm kmeans.cpp include/algorithm/kmeans.hpp)
target_include_directories(kmeansAlgorithm PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/algorithm>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS kmeansAlgorithm)

add_library(latexOutput INTERFACE include/output/latex.hpp)
target_include_directories(latexOutput INTERFACE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/output>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS latexOutput)

add_executable(kMeans main.cpp "main.cpp")
target_link_libraries(kMeans kmeansAlgorithm latexOutput)
add_test(NAME kMeans COMMAND kMeans)