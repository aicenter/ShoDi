/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */
#include <fstream>
#include "gtest/gtest.h"
#include "common.h"
#include "DistanceMatrix/CSV_reader.h"

void compare_dm_files(const std::string& p1, const std::string& p2) {
	CSV_reader csv_reader;
	const auto& [dm_1, size_1] = csv_reader.read_matrix(p1);
	const auto& [dm_2, size_2] = csv_reader.read_matrix(p2);

	ASSERT_EQ(size_1, size_2);

	auto dist_count = size_1 * size_1;

	for(size_t i = 0; i < dist_count; i++) {
		ASSERT_EQ(dm_1[i], dm_2[i]);
	}
}

TEST(dm_test, from_xengraph_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format xengraph --output-format csv --preprocessing-mode slow -i functest/01_xengraph.xeng -o from_xengraph_slow1");
	compare_dm_files("functest/01_dm.csv", "from_xengraph_slow1.csv");
}

TEST(dm_test, from_xengraph_slow2) {
    // slow with precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode slow --input-path functest/02_xengraph.xeng --output-path from_xengraph_slow2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_xengraph_slow2.csv");
}

TEST(dm_test, from_xengraph_fast1) {
    // fast without precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode fast --input-path functest/01_xengraph.xeng -o from_xengraph_fast1");
	compare_dm_files("functest/01_dm.csv", "from_xengraph_fast1.csv");
}

TEST(dm_test, from_xengraph_fast2) {
    // fast with precision loss
    run_preprocessor("--method dm -f xengraph --output-format csv --preprocessing-mode fast -i functest/02_xengraph.xeng --output-path from_xengraph_fast2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_xengraph_fast2.csv");
}



TEST(dm_test, from_dimacs_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format csv --preprocessing-mode slow --input-path functest/01_dimacs.gr --output-path from_dimacs_slow1");
	compare_dm_files("functest/01_dm.csv", "from_dimacs_slow1.csv");
}

TEST(dm_test, from_dimacs_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --output-format csv --preprocessing-mode slow --input-path functest/02_dimacs.gr --output-path from_dimacs_slow2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_dimacs_slow2.csv");
}

TEST(dm_test, from_dimacs_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm --input-format dimacs --output-format csv --preprocessing-mode fast -i functest/01_dimacs.gr --output-path from_dimacs_fast1");
	compare_dm_files("functest/01_dm.csv", "from_dimacs_fast1.csv");
}

TEST(dm_test, from_dimacs_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --input-format dimacs --output-format csv --preprocessing-mode fast --input-path functest/02_dimacs.gr -o from_dimacs_fast2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_dimacs_fast2.csv");
}



TEST(dm_test, from_adj_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --input-format adj --output-format csv --preprocessing-mode slow -i functest/01_adj.csv --output-path from_adj_slow1");
	compare_dm_files("functest/01_dm.csv", "from_adj_slow1.csv");
}

TEST(dm_test, from_adj_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --output-format csv --preprocessing-mode slow --input-path functest/02_adj.csv --output-path from_adj_slow2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_adj_slow2.csv");
}

TEST(dm_test, from_adj_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm -f adj --output-format csv --preprocessing-mode fast -i functest/02_adj.csv --output-path from_adj_fast1");
	compare_dm_files("functest/02_dm.csv", "from_adj_fast1.csv");
}

TEST(dm_test, from_adj_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --input-format adj --output-format csv --preprocessing-mode fast --input-path functest/02_adj.csv -o from_adj_fast2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_adj_fast2.csv");
}



TEST(dm_test, from_csv_slow1) {
    // slow without precision loss
    run_preprocessor("--method dm --output-format csv --preprocessing-mode slow --input-path functest/01_csv --output-path from_csv_slow1");
	compare_dm_files("functest/01_dm.csv", "from_csv_slow1.csv");
}

TEST(dm_test, from_csv_slow2) {
    // slow with precision loss
    run_preprocessor("--method dm --input-format csv --output-format csv --preprocessing-mode slow --input-path functest/02_csv --output-path from_csv_slow2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_csv_slow2.csv");
}

TEST(dm_test, from_csv_fast1) {
    // fast without precision loss
    run_preprocessor("--method dm -f csv --output-format csv --preprocessing-mode fast -i functest/02_csv --output-path from_csv_fast1");
	compare_dm_files("functest/02_dm.csv", "from_csv_fast1.csv");
}

TEST(dm_test, from_csv_fast2) {
    // fast with precision loss
    run_preprocessor("-m dm --output-format csv --preprocessing-mode fast --input-path functest/02_csv -o from_csv_fast2 --precision-loss 100");
	compare_dm_files("functest/02_dm_div100.csv", "from_csv_fast2.csv");
}
