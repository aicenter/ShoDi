//
// Created by horakj31 on 2024-02-09.
//
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"

TEST(dm_test, from_xengraph_slow1) {
    // slow without precision loss
    run_preprocessor("dm xengraph csv slow functest/01_xengraph.xeng from_xengraph_slow1");
    compare_files("functest/01_dm.csv", "from_xengraph_slow1.csv");
}

TEST(dm_test, from_xengraph_slow2) {
    // slow with precision loss
    run_preprocessor("dm xengraph csv slow functest/02_xengraph.xeng from_xengraph_slow2 100");
    compare_files("functest/02_dm_div100.csv", "from_xengraph_slow2.csv");
}

TEST(dm_test, from_xengraph_fast1) {
    // fast without precision loss
    run_preprocessor("dm xengraph csv fast functest/01_xengraph.xeng from_xengraph_fast1");
    compare_files("functest/01_dm.csv", "from_xengraph_fast1.csv");
}

TEST(dm_test, from_xengraph_fast2) {
    // fast with precision loss
    run_preprocessor("dm xengraph csv fast functest/02_xengraph.xeng from_xengraph_fast2 100");
    compare_files("functest/02_dm_div100.csv", "from_xengraph_fast2.csv");
}



TEST(dm_test, from_dimacs_slow1) {
    // slow without precision loss
    run_preprocessor("dm dimacs csv slow functest/01_dimacs.gr from_dimacs_slow1");
    compare_files("functest/01_dm.csv", "from_dimacs_slow1.csv");
}

TEST(dm_test, from_dimacs_slow2) {
    // slow with precision loss
    run_preprocessor("dm dimacs csv slow functest/02_dimacs.gr from_dimacs_slow2 100");
    compare_files("functest/02_dm_div100.csv", "from_dimacs_slow2.csv");
}

TEST(dm_test, from_dimacs_fast1) {
    // fast without precision loss
    run_preprocessor("dm dimacs csv fast functest/01_dimacs.gr from_dimacs_fast1");
    compare_files("functest/01_dm.csv", "from_dimacs_fast1.csv");
}

TEST(dm_test, from_dimacs_fast2) {
    // fast with precision loss
    run_preprocessor("dm dimacs csv fast functest/02_dimacs.gr from_dimacs_fast2 100");
    compare_files("functest/02_dm_div100.csv", "from_dimacs_fast2.csv");
}



TEST(dm_test, from_csv_slow1) {
    // slow without precision loss
    run_preprocessor("dm csv csv slow functest/01_adj.csv from_csv_slow1");
    compare_files("functest/01_dm.csv", "from_csv_slow1.csv");
}

TEST(dm_test, from_csv_slow2) {
    // slow with precision loss
    run_preprocessor("dm csv csv slow functest/02_adj.csv from_csv_slow2 100");
    compare_files("functest/02_dm_div100.csv", "from_csv_slow2.csv");
}

TEST(dm_test, from_csv_fast1) {
    // fast without precision loss
    run_preprocessor("dm csv csv fast functest/02_adj.csv from_csv_fast1");
    compare_files("functest/02_dm.csv", "from_csv_fast1.csv");
}

TEST(dm_test, from_csv_fast2) {
    // fast with precision loss
    run_preprocessor("dm csv csv fast functest/02_adj.csv from_csv_fast2 100");
    compare_files("functest/02_dm_div100.csv", "from_csv_fast2.csv");
}
