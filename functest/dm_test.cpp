//
// Created by horakj31 on 2024-02-09.
//
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"

TEST(dm_test, from_xengraph_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format xengraph --output-format csv --preprocessing-mode slow -i functest/01_xengraph.xeng -o from_xengraph_slow1");
    compare_txt_files("functest/01_dm.csv", "from_xengraph_slow1.csv");
}

TEST(dm_test, from_xengraph_slow2) {
    // slow with precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode slow --input-path functest/02_xengraph.xeng --output-path from_xengraph_slow2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_xengraph_slow2.csv");
}

TEST(dm_test, from_xengraph_fast1) {
    // fast without precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode fast --input-path functest/01_xengraph.xeng -o from_xengraph_fast1");
    compare_txt_files("functest/01_dm.csv", "from_xengraph_fast1.csv");
}

TEST(dm_test, from_xengraph_fast2) {
    // fast with precision loss
    run_preprocessor("--method dm -f xengraph --output-format csv --preprocessing-mode fast -i functest/02_xengraph.xeng --output-path from_xengraph_fast2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_xengraph_fast2.csv");
}

TEST(dm_test, from_xengraph_badgraph) {
    // non single component graph
    const std::string out = run_preprocessor("--method dm -f xengraph --output-format csv --preprocessing-mode fast -i functest/03_xengraph.xeng --output-path from_xengraph_badgraph");
    ASSERT_EQ(out.find("could not find a path beetween nodes") == std::string::npos, false);
}



TEST(dm_test, from_dimacs_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format csv --preprocessing-mode slow --input-path functest/01_dimacs.gr --output-path from_dimacs_slow1");
    compare_txt_files("functest/01_dm.csv", "from_dimacs_slow1.csv");
}

TEST(dm_test, from_dimacs_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --output-format csv --preprocessing-mode slow --input-path functest/02_dimacs.gr --output-path from_dimacs_slow2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_dimacs_slow2.csv");
}

TEST(dm_test, from_dimacs_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format csv --preprocessing-mode fast -i functest/01_dimacs.gr --output-path from_dimacs_fast1");
    compare_txt_files("functest/01_dm.csv", "from_dimacs_fast1.csv");
}

TEST(dm_test, from_dimacs_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --input-format dimacs --output-format csv --preprocessing-mode fast --input-path functest/02_dimacs.gr -o from_dimacs_fast2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_dimacs_fast2.csv");
}

TEST(dm_test, from_dimacs_badgraph) {
    // non single component graph
    const std::string out = run_preprocessor("-m dm --output-format hdf --preprocessing-mode fast -i functest/03_dimacs.gr --output-path from_dimacs_badgraph");
    ASSERT_EQ(out.find("could not find a path beetween nodes") == std::string::npos, false);
}



TEST(dm_test, from_adj_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format adj --output-format csv --preprocessing-mode slow -i functest/01_adj.csv --output-path from_adj_slow1");
    compare_txt_files("functest/01_dm.csv", "from_adj_slow1.csv");
}

TEST(dm_test, from_adj_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --output-format csv --preprocessing-mode slow --input-path functest/02_adj.csv --output-path from_adj_slow2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_adj_slow2.csv");
}

TEST(dm_test, from_adj_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm -f adj --output-format csv --preprocessing-mode fast -i functest/02_adj.csv --output-path from_adj_fast1");
    compare_txt_files("functest/02_dm.csv", "from_adj_fast1.csv");
}

TEST(dm_test, from_adj_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --input-format adj --output-format csv --preprocessing-mode fast --input-path functest/02_adj.csv -o from_adj_fast2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_adj_fast2.csv");
}

TEST(dm_test, from_adj_badgraph) {
    // non single component graph
    const std::string out = run_preprocessor("--method dm -f adj --output-format xdm --preprocessing-mode slow -i functest/03_adj.csv --output-path from_adj_badgraph");
    ASSERT_EQ(out.find("could not find a path beetween nodes") == std::string::npos, false);
}



TEST(dm_test, from_csv_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --output-format csv --preprocessing-mode slow --input-path functest/01_csv --output-path from_csv_slow1");
    compare_txt_files("functest/01_dm.csv", "from_csv_slow1.csv");
}

TEST(dm_test, from_csv_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --input-format csv --output-format csv --preprocessing-mode slow --input-path functest/02_csv --output-path from_csv_slow2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_csv_slow2.csv");
}

TEST(dm_test, from_csv_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm -f csv --output-format csv --preprocessing-mode fast -i functest/02_csv --output-path from_csv_fast1");
    compare_txt_files("functest/02_dm.csv", "from_csv_fast1.csv");
}

TEST(dm_test, from_csv_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode fast --input-path functest/02_csv -o from_csv_fast2 --precision-loss 100");
    compare_txt_files("functest/02_dm_div100.csv", "from_csv_fast2.csv");
}

TEST(dm_test, from_csv_badgraph) {
    // non single component graph
    const std::string out = run_preprocessor("--method dm -f csv --output-format csv --preprocessing-mode fast -i functest/03_csv --output-path from_csv_badgraph");
    ASSERT_EQ(out.find("could not find a path beetween nodes") == std::string::npos, false);
}
