//
// Created by horakj31 on 2024-02-09.
//
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"

TEST(dm_test, from_xengraph_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format xengraph --output-format csv --preprocessing-mode slow --input-file functest/01_xengraph.xeng --output-file from_xengraph_slow1");
    compare_txt_files("functest/01_dm.csv", "from_xengraph_slow1.csv");
}

TEST(dm_test, from_xengraph_slow2) {
    // slow with precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode slow --input-file functest/02_xengraph.xeng --output-file from_xengraph_slow2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_xengraph_slow2.csv");
}

TEST(dm_test, from_xengraph_fast1) {
    // fast without precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode fast --input-file functest/01_xengraph.xeng --output-file from_xengraph_fast1");
    compare_txt_files("functest/01_dm.csv", "from_xengraph_fast1.csv");
}

TEST(dm_test, from_xengraph_fast2) {
    // fast with precision loss
    run_preprocessor("--method dm -f xengraph --output-format csv --preprocessing-mode fast --input-file functest/02_xengraph.xeng --output-file from_xengraph_fast2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_xengraph_fast2.csv");
}



TEST(dm_test, from_dimacs_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format csv --preprocessing-mode slow --input-file functest/01_dimacs.gr --output-file from_dimacs_slow1");
    compare_txt_files("functest/01_dm.csv", "from_dimacs_slow1.csv");
}

TEST(dm_test, from_dimacs_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --output-format csv --preprocessing-mode slow --input-file functest/02_dimacs.gr --output-file from_dimacs_slow2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_dimacs_slow2.csv");
}

TEST(dm_test, from_dimacs_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format csv --preprocessing-mode fast --input-file functest/01_dimacs.gr --output-file from_dimacs_fast1");
    compare_txt_files("functest/01_dm.csv", "from_dimacs_fast1.csv");
}

TEST(dm_test, from_dimacs_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --input-format dimacs --output-format csv --preprocessing-mode fast --input-file functest/02_dimacs.gr --output-file from_dimacs_fast2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_dimacs_fast2.csv");
}



TEST(dm_test, from_adj_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format adj --output-format csv --preprocessing-mode slow --input-file functest/01_adj.csv --output-file from_adj_slow1");
    compare_txt_files("functest/01_dm.csv", "from_adj_slow1.csv");
}

TEST(dm_test, from_adj_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --input-format adj --output-format csv --preprocessing-mode slow --input-file functest/02_adj.csv --output-file from_adj_slow2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_adj_slow2.csv");
}

TEST(dm_test, from_adj_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm -f adj --output-format csv --preprocessing-mode fast --input-file functest/02_adj.csv --output-file from_adj_fast1");
    compare_txt_files("functest/02_dm.csv", "from_adj_fast1.csv");
}

TEST(dm_test, from_adj_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --input-format adj --output-format csv --preprocessing-mode fast --input-file functest/02_adj.csv --output-file from_adj_fast2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_adj_fast2.csv");
}
