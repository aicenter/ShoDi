//
// Created by horakj31 on 2024-02-09.
//
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"

TEST(dm_test, xengraph_to_dm_csv_slow1) {
    // slow without precision loss
    run_preprocessor("dm xengraph csv slow functest/01_xengraph.xeng xengraph_to_dm_slow1");
    compare_files("functest/01_dm.csv", "xengraph_to_dm_slow1.csv");
}

TEST(dm_test, xengraph_to_dm_csv_slow2) {
    // slow with precision loss
    run_preprocessor("dm xengraph csv slow functest/02_xengraph.xeng xengraph_to_dm_slow2 2");
    compare_files("functest/02_dm_div2.csv", "xengraph_to_dm_slow2.csv");
}

TEST(dm_test, xengraph_to_dm_csv_fast1) {
    // fast without precision loss
    run_preprocessor("dm xengraph csv fast functest/01_xengraph.xeng xengraph_to_dm_fast1");
    compare_files("functest/01_dm.csv", "xengraph_to_dm_fast1.csv");
}

TEST(dm_test, xengraph_to_dm_csv_fast2) {
    // fast with precision loss
    run_preprocessor("dm xengraph csv fast functest/02_xengraph.xeng xengraph_to_dm_fast2 2");
    compare_files("functest/02_dm_div2.csv", "xengraph_to_dm_fast2.csv");
}

TEST(dm_test, dimacs_to_dm_csv_slow1) {
    // slow without precision loss
    run_preprocessor("dm dimacs csv slow functest/01_dimacs.gr dimacs_to_dm_slow1");
    compare_files("functest/01_dm.csv", "dimacs_to_dm_slow1.csv");
}

TEST(dm_test, dimacs_to_dm_csv_slow2) {
    // slow with precision loss
    run_preprocessor("dm dimacs csv slow functest/02_dimacs.gr dimacs_to_dm_slow2 2");
    compare_files("functest/02_dm_div2.csv", "dimacs_to_dm_slow2.csv");
}

TEST(dm_test, dimacs_to_dm_csv_fast1) {
    // fast without precision loss
    run_preprocessor("dm dimacs csv fast functest/01_dimacs.gr dimacs_to_dm_fast1");
    compare_files("functest/01_dm.csv", "dimacs_to_dm_fast1.csv");
}

TEST(dm_test, dimacs_to_dm_csv_fast2) {
    // fast with precision loss
    run_preprocessor("dm dimacs csv fast functest/02_dimacs.gr dimacs_to_dm_fast2 2");
    compare_files("functest/02_dm_div2.csv", "dimacs_to_dm_fast2.csv");
}

TEST(dm_test, csv_to_dm_csv_slow1) {
    // slow without precision loss
    run_preprocessor("dm csv csv slow functest/01_adj.csv csv_to_dm_slow1");
    compare_files("functest/01_dm.csv", "csv_to_dm_slow1.csv");
}

TEST(dm_test, csv_to_dm_csv_slow2) {
    // slow with precision loss
    run_preprocessor("dm csv csv slow functest/02_adj.csv csv_to_dm_slow2 2");
    compare_files("functest/02_dm_div2.csv", "csv_to_dm_slow2.csv");
}

TEST(dm_test, csv_to_dm_csv_fast1) {
    // fast without precision loss
    run_preprocessor("dm csv csv fast functest/02_adj.csv csv_to_dm_fast1");
    compare_files("functest/02_dm.csv", "csv_to_dm_fast1.csv");
}

TEST(dm_test, csv_to_dm_csv_fast2) {
    // fast with precision loss
    run_preprocessor("dm csv csv fast functest/02_adj.csv csv_to_dm_fast2 2");
    compare_files("functest/02_dm_div2.csv", "csv_to_dm_fast2.csv");
}
