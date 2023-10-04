#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../prj.cw/include/algorithm/kmeans.hpp"
#include "../prj.cw/include/output/latex.hpp"

#include <sstream>
#include <vector>


TEST_CASE("[kmeans] - kmeans ctor"){
    CHECK_THROWS(kMeans(11));
    CHECK_THROWS(kMeans(-1));
}

TEST_CASE("[kmeans] - kmeans read file"){
    CHECK_THROWS(kMeans().read_csv(""));
    CHECK_THROWS(kMeans().read_csv("../project/include/kmeans.cpp"));
    CHECK_THROWS(kMeans().read_csv("../project/include/datasets/dataset0001.csv", std::vector<int>{4}));
    CHECK_THROWS(kMeans().read_csv("../project/include/datasets/dataset0001.csv", std::vector<int>{4, 6}));
    CHECK_THROWS(kMeans().read_csv("../project/include/datasets/dataset0001.csv", std::vector<int>()));
}

TEST_CASE("[kmeans] - kmeans output"){
    std::vector<Point> check_vec;
    check_vec.push_back(Point());
    CHECK_THROWS(kMeans().output_file(check_vec, ""));
    CHECK_THROWS(kMeans().output_file(check_vec, "../project/include/kmeans.cpp"));
}

TEST_CASE("[latex] - latex make body") {
    outputLatex* latex = new outputLatex("");
    CHECK_THROWS(latex->make_body());
    latex = new outputLatex("../project/include/kmeans.cpp");
    CHECK_THROWS(latex->make_body());
}

TEST_CASE("[latex] - latex output") {
    outputLatex* latex = new outputLatex("../project/include/datasets/dataset0001.csv", "");
    CHECK_THROWS(latex->writeLatex());
    latex = new outputLatex("../project/include/datasets/dataset0001.csv", "../project/include/kmeans.cpp");
    CHECK_THROWS(latex->writeLatex());
}
