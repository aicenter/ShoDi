//
// Created by horakj31 on 2024-03-17.
//
#include "gtest/gtest.h"
#include "common.h"

TEST(tnraf_test, from_xengraph1) {
    run_preprocessor("tnraf xengraph slow 3 functest/01_xengraph.xeng from_xengraph1");
    compare_tgaf_files("from_xengraph1.tgaf", "functest/01_3.tgaf");
}

TEST(tnraf_test, from_xengraph2) {
    run_preprocessor("tnraf xengraph dm 2 functest/01_xengraph.xeng from_xengraph2");
    compare_tgaf_files("from_xengraph2.tgaf", "functest/01_2.tgaf");
}

TEST(tnraf_test, from_xengraph3) {
    run_preprocessor("tnraf xengraph dm 1 functest/02_xengraph.xeng from_xengraph3 100");
    compare_tgaf_files("from_xengraph3.tgaf", "functest/02_1_div100.tgaf");
}



TEST(tnraf_test, from_dimacs1) {
    run_preprocessor("tnraf dimacs slow 2 functest/01_dimacs.gr from_dimacs1");
    compare_tgaf_files("from_dimacs1.tgaf", "functest/01_2.tgaf");
}

TEST(tnraf_test, from_dimacs2) {
    run_preprocessor("tnraf dimacs dm 1 functest/01_dimacs.gr from_dimacs2");
    compare_tgaf_files("from_dimacs2.tgaf", "functest/01_1.tgaf");
}

TEST(tnraf_test, from_dimacs3) {
    run_preprocessor("tnraf dimacs slow 3 functest/02_dimacs.gr from_dimacs3 100");
    compare_tgaf_files("from_dimacs3.tgaf", "functest/02_3_div100.tgaf");
}



TEST(tnraf_test, from_csv1) {
    run_preprocessor("tnraf csv slow 1 functest/01_adj.csv from_csv1");
    compare_tgaf_files("from_csv1.tgaf", "functest/01_1.tgaf");
}

TEST(tnraf_test, from_csv2) {
    run_preprocessor("tnraf csv dm 3 functest/01_adj.csv from_csv2");
    compare_tgaf_files("from_csv2.tgaf", "functest/01_3.tgaf");
}

TEST(tnraf_test, from_csv3) {
    run_preprocessor("tnraf csv dm 2 functest/02_adj.csv from_csv3 100");
    compare_tgaf_files("from_csv3.tgaf", "functest/02_2_div100.tgaf");
}
