#include "CsvParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace csv {
    std::vector<std::vector<std::string> > CsvParser::parse() const {
        std::vector<std::vector<std::string> > data;
        try {
            std::ifstream file(_filename);

            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file: " + _filename);
            }

            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string field;
                std::vector<std::string> row;

                while (std::getline(ss, field, ',')) {
                    row.push_back(field);
                }

                data.push_back(row);
            }

            file.close();
        } catch (std::exception const &e) {
            std::cerr << e.what() << std::endl;
        }
        return data;
    }
} // csv
