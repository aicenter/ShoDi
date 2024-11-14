//
// Created by horakj31 on 2024-02-09.
//
#include <fstream>
#include <sstream>
#include "gtest/gtest.h"
#include "common.h"
#include "H5Cpp.h"
#include "DistanceMatrix/CSV_reader.h"

void check_hdf(const std::string& computed, const std::string& expected) {
	// Read computed values from HDF5 file
	H5::H5File* file;
    file = new H5::H5File(computed, H5F_ACC_RDONLY);

    auto dataset = file->openDataSet("dm");
    auto dataspace = dataset.getSpace();

    hsize_t dims[2];
    dataspace.getSimpleExtentDims(dims);

    size_t size = dims[0] * dims[1];
    auto* values_computed = new dist_t[size];

    dataset.read(values_computed, H5::PredType::NATIVE_UINT);

    dataset.close();
    dataspace.close();
    file->close();

	// Read expected values from CSV file
	CSV_reader csv_reader;
	const auto& [dm_expected, size_expected] = csv_reader.read_matrix(expected);

	ASSERT_EQ(dims[0], size_expected);

	for(size_t i = 0; i < size; i++) {
		ASSERT_EQ(dm_expected[i], values_computed[i]);
	}
}

TEST(dm_hdf_test, from_xengraph_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --output-format hdf --preprocessing-mode slow --input-path functest/01_xengraph.xeng --output-path from_xengraph_slow1");
    check_hdf("from_xengraph_slow1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_xengraph_slow2) {
    // slow with precision loss
    run_preprocessor("-m dm --input-format xengraph --output-format hdf --preprocessing-mode slow -i functest/02_xengraph.xeng --output-path from_xengraph_slow2 --precision-loss 100");
    check_hdf("from_xengraph_slow2.hdf5", "functest/02_dm_div100.csv");
}

TEST(dm_hdf_test, from_xengraph_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm --input-format xengraph --output-format hdf --preprocessing-mode fast --input-path functest/01_xengraph.xeng -o from_xengraph_fast1");
    check_hdf("from_xengraph_fast1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_xengraph_fast2) {
    // fast with precision loss
    run_preprocessor("--method dm --input-format xengraph --output-format hdf --preprocessing-mode fast --input-path functest/02_xengraph.xeng -o from_xengraph_fast2 --precision-loss 100");
    check_hdf("from_xengraph_fast2.hdf5", "functest/02_dm_div100.csv");
}



TEST(dm_hdf_test, from_dimacs_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format hdf --preprocessing-mode slow --input-path functest/01_dimacs.gr --output-path from_dimacs_slow1");
    check_hdf("from_dimacs_slow1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_dimacs_slow2) {
    // slow with precision loss
    run_preprocessor("-m dm --output-format hdf --preprocessing-mode slow --input-path functest/02_dimacs.gr --output-path from_dimacs_slow2 --precision-loss 100");
    check_hdf("from_dimacs_slow2.hdf5", "functest/02_dm_div100.csv");
}

TEST(dm_hdf_test, from_dimacs_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format hdf --preprocessing-mode fast -i functest/01_dimacs.gr -o from_dimacs_fast1");
    check_hdf("from_dimacs_fast1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_dimacs_fast2) {
    // fast with precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format hdf --preprocessing-mode fast --input-path functest/02_dimacs.gr --output-path from_dimacs_fast2 --precision-loss 100");
    check_hdf("from_dimacs_fast2.hdf5", "functest/02_dm_div100.csv");
}



TEST(dm_hdf_test, from_adj_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format adj --output-format hdf --preprocessing-mode slow --input-path functest/01_adj.csv --output-path from_adj_slow1");
    check_hdf("from_adj_slow1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_adj_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --input-format adj --output-format hdf --preprocessing-mode slow --input-path functest/02_adj.csv --output-path from_adj_slow2 --precision-loss 100");
    check_hdf("from_adj_slow2.hdf5", "functest/02_dm_div100.csv");
}

TEST(dm_hdf_test, from_adj_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm --input-format adj --output-format hdf --preprocessing-mode fast --input-path functest/02_adj.csv --output-path from_adj_fast1");
    check_hdf("from_adj_fast1.hdf5", "functest/02_dm.csv");
}

TEST(dm_hdf_test, from_adj_fast2) {
    // fast with precision loss
    run_preprocessor("--method dm --input-format adj --output-format hdf --preprocessing-mode fast -i functest/02_adj.csv --output-path from_adj_fast2 --precision-loss 100");
    check_hdf("from_adj_fast2.hdf5", "functest/02_dm_div100.csv");
}



TEST(dm_hdf_test, from_csv_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format csv --output-format hdf --preprocessing-mode slow --input-path functest/01_csv --output-path from_csv_slow1");
    check_hdf("from_csv_slow1.hdf5", "functest/01_dm.csv");
}

TEST(dm_hdf_test, from_csv_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm -f csv --output-format hdf --preprocessing-mode slow --input-path functest/02_csv --output-path from_csv_slow2 --precision-loss 100");
    check_hdf("from_csv_slow2.hdf5", "functest/02_dm_div100.csv");
}

TEST(dm_hdf_test, from_csv_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm --output-format hdf --preprocessing-mode fast --input-path functest/02_csv --output-path from_csv_fast1");
    check_hdf("from_csv_fast1.hdf5", "functest/02_dm.csv");
}

TEST(dm_hdf_test, from_csv_fast2) {
    // fast with precision loss
    run_preprocessor("--method dm --input-format csv --output-format hdf --preprocessing-mode fast --input-path functest/02_csv -o from_csv_fast2 --precision-loss 100");
    check_hdf("from_csv_fast2.hdf5", "functest/02_dm_div100.csv");
}
