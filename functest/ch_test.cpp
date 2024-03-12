//
// Created by horakj31 on 2024-03-01.
//
#include "gtest/gtest.h"
#include "common.h"

TEST(ch_test, from_xengraph1) {
    run_preprocessor("ch xengraph functest/01_xengraph.xeng from_xengraph1");
    compare_ch_files("from_xengraph1.ch", "functest/01.ch");
}

TEST(ch_test, from_xengraph2) {
    run_preprocessor("ch xengraph functest/02_xengraph.xeng from_xengraph2 3");
    compare_ch_files("from_xengraph2.ch", "functest/02_div3.ch");
}

TEST(ch_test, from_csv1) {
    run_preprocessor("ch csv functest/02_adj.csv from_csv1");
    compare_ch_files("from_csv1.ch", "functest/02.ch");
}

TEST(ch_test, from_csv2) {
    run_preprocessor("ch csv functest/01_adj.csv from_csv2 2");
    compare_ch_files("from_csv2.ch", "functest/01_div2.ch");
}

TEST(ch_test, from_dimacs1) {
    run_preprocessor("ch dimacs functest/02_dimacs.gr from_dimacs1");
    compare_ch_files("from_dimacs1.ch", "functest/02.ch");
}

TEST(ch_test, from_dimacs2) {
    run_preprocessor("ch dimacs functest/01_dimacs.gr from_dimacs2 2");
    compare_ch_files("from_dimacs2.ch", "functest/01_div2.ch");
}
