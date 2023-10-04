add_executable(kmeans_tests kmeans_tests.cpp)
target_link_libraries(kmeans_tests kmeansAlgorithm latexOutput doctest::doctest)
add_test(NAME kmeans_tests COMMAND kmeans_tests)