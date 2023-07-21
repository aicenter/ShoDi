#pragma once

#include "Distance_matrix_reader.h"

class CSV_reader: public Distance_matrix_reader {
public:
	std::pair<std::unique_ptr<dist_t[]>, unsigned> read_matrix(const std::string& file_path) override;
};