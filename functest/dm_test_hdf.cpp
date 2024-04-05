//
// Created by horakj31 on 2024-02-09.
//
#include <fstream>
#include <sstream>
#include "gtest/gtest.h"
#include "common.h"
#include <H5Cpp.h>

void check_hdf(const std::string& computed, const std::string& expected) {
    H5::H5File* file;
    file = new H5::H5File(computed, H5F_ACC_RDONLY);

    auto dataset = file->openDataSet("dm");
    auto dataspace = dataset.getSpace();

    hsize_t dims[2];
    dataspace.getSimpleExtentDims(dims);

    size_t size = dims[0] * dims[1];
    dist_t* values_computed = new dist_t[size];
    auto values_expected = std::vector<std::string>();

    dataset.read(values_computed, H5::PredType::NATIVE_UINT);

    dataset.close();
    dataspace.close();
    file->close();

    size_t i = 0;
    std::ifstream f(expected);
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        std::string num_expected;

        while (getline(iss, num_expected, ',')) {
            values_expected.push_back(num_expected);
            i++;
        }
    }
    f.close();

    EXPECT_EQ(size, values_expected.size());

    for (size_t j = 0; j < size; j++) {
        EXPECT_EQ(std::to_string(values_computed[j]), values_expected[j]);
    }
}

TEST(dm_hdf_test, from_xengraph_slow1) {
    // slow without precision loss
    run_preprocessor("dm xengraph hdf slow functest/01_xengraph.xeng from_xengraph_slow1");
    check_hdf("from_xengraph_slow1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_xengraph_slow2) {
    // slow with precision loss
    run_preprocessor("dm xengraph hdf slow functest/02_xengraph.xeng from_xengraph_slow2 100");
    check_hdf("from_xengraph_slow2.hdf5", "functest/02_dm_div100.csv");
}

TEST(dm_hdf_test, from_xengraph_fast1) {
    // fast without precision loss
    run_preprocessor("dm xengraph hdf fast functest/01_xengraph.xeng from_xengraph_fast1");
    check_hdf("from_xengraph_fast1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_xengraph_fast2) {
    // fast with precision loss
    run_preprocessor("dm xengraph hdf fast functest/02_xengraph.xeng from_xengraph_fast2 100");
    check_hdf("from_xengraph_fast2.hdf5", "functest/02_dm_div100.csv");
}



TEST(dm_hdf_test, from_dimacs_slow1) {
    // slow without precision loss
    run_preprocessor("dm dimacs hdf slow functest/01_dimacs.gr from_dimacs_slow1");
    check_hdf("from_dimacs_slow1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_dimacs_slow2) {
    // slow with precision loss
    run_preprocessor("dm dimacs hdf slow functest/02_dimacs.gr from_dimacs_slow2 100");
    check_hdf("from_dimacs_slow2.hdf5", "functest/02_dm_div100.csv");
}

TEST(dm_hdf_test, from_dimacs_fast1) {
    // fast without precision loss
    run_preprocessor("dm dimacs hdf fast functest/01_dimacs.gr from_dimacs_fast1");
    check_hdf("from_dimacs_fast1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_dimacs_fast2) {
    // fast with precision loss
    run_preprocessor("dm dimacs hdf fast functest/02_dimacs.gr from_dimacs_fast2 100");
    check_hdf("from_dimacs_fast2.hdf5", "functest/02_dm_div100.csv");
}



TEST(dm_hdf_test, from_csv_slow1) {
    // slow without precision loss
    run_preprocessor("dm csv hdf slow functest/01_adj.csv from_csv_slow1");
    check_hdf("from_csv_slow1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_csv_slow2) {
    // slow with precision loss
    run_preprocessor("dm csv hdf slow functest/02_adj.csv from_csv_slow2 100");
    check_hdf("from_csv_slow2.hdf5", "functest/02_dm_div100.csv");
}

TEST(dm_hdf_test, from_csv_fast1) {
    // fast without precision loss
    run_preprocessor("dm csv hdf fast functest/02_adj.csv from_csv_fast1");
    check_hdf("from_csv_fast1.hdf5", "functest/02_dm.csv");
}

TEST(dm_hdf_test, from_csv_fast2) {
    // fast with precision loss
    run_preprocessor("dm csv hdf fast functest/02_adj.csv from_csv_fast2 100");
    check_hdf("from_csv_fast2.hdf5", "functest/02_dm_div100.csv");
}
