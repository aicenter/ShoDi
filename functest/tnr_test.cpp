//
// Created by horakj31 on 2024-03-12.
//
#include "gtest/gtest.h"
#include "common.h"

TEST(tnr_test, from_xengraph1) {
    run_preprocessor("tnr xengraph fast 1 functest/01_xengraph.xeng from_xengraph1");
    compare_tnrg_files("from_xengraph1.tnrg", "functest/01_1.tnrg");
}

TEST(tnr_test, from_xengraph2) {
    run_preprocessor("tnr xengraph slow 2 functest/01_xengraph.xeng from_xengraph2");
    compare_tnrg_files("from_xengraph2.tnrg", "functest/01_2.tnrg");
}

TEST(tnr_test, from_xengraph3) {
    run_preprocessor("tnr xengraph dm 3 functest/01_xengraph.xeng from_xengraph3");
    compare_tnrg_files("from_xengraph3.tnrg", "functest/01_3.tnrg");
}

TEST(tnr_test, from_xengraph4) {
    run_preprocessor("tnr xengraph fast 2 functest/02_xengraph.xeng from_xengraph4 100");
    compare_tnrg_files("from_xengraph4.tnrg", "functest/02_2_div100.tnrg");
}



TEST(tnr_test, from_dimacs1) {
    run_preprocessor("tnr dimacs fast 2 functest/01_dimacs.gr from_dimacs1");
    compare_tnrg_files("from_dimacs1.tnrg", "functest/01_2.tnrg");
}

TEST(tnr_test, from_dimacs2) {
    run_preprocessor("tnr dimacs slow 3 functest/01_dimacs.gr from_dimacs2");
    compare_tnrg_files("from_dimacs2.tnrg", "functest/01_3.tnrg");
}

TEST(tnr_test, from_dimacs3) {
    run_preprocessor("tnr dimacs dm 1 functest/01_dimacs.gr from_dimacs2");
    compare_tnrg_files("from_dimacs2.tnrg", "functest/01_1.tnrg");
}

TEST(tnr_test, from_dimacs4) {
    run_preprocessor("tnr dimacs slow 3 functest/02_dimacs.gr from_dimacs4 100");
    compare_tnrg_files("from_dimacs4.tnrg", "functest/02_3_div100.tnrg");
}



TEST(tnr_test, from_csv1) {
    run_preprocessor("tnr csv fast 3 functest/01_adj.csv from_csv1");
    compare_tnrg_files("from_csv1.tnrg", "functest/01_3.tnrg");
}

TEST(tnr_test, from_csv2) {
    run_preprocessor("tnr csv slow 1 functest/01_adj.csv from_csv2");
    compare_tnrg_files("from_csv2.tnrg", "functest/01_1.tnrg");
}

TEST(tnr_test, from_csv3) {
    run_preprocessor("tnr csv dm 2 functest/01_adj.csv from_csv3");
    compare_tnrg_files("from_csv3.tnrg", "functest/01_2.tnrg");
}

TEST(tnr_test, from_csv4) {
    run_preprocessor("tnr csv dm 1 functest/02_adj.csv from_csv4 100");
    compare_tnrg_files("from_csv4.tnrg", "functest/02_1_div100.tnrg");
}
