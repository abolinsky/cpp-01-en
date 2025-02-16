#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

std::string strip_leading_whitespace(const std::string& str) {
    auto first_non_space = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    return std::string(first_non_space, str.end());
}

void extract_variables(std::string& content) {
    std::istringstream stream(content);
    std::ostringstream includes;
    std::ostringstream globals;
    std::ostringstream main_content;
    std::string line;
    bool in_main_function = false;

    std::vector<std::string> data_types = {
        "int", "float", "double", "char", "long", "short", "bool", "std::string",
        "int*", "float*", "double*", "char*", "long*", "short*", "bool*",
        "std::vector<int>", "std::vector<float>", "std::vector<double>", "std::vector<char>",
        "std::vector<long>", "std::vector<short>", "std::vector<bool>", "std::vector<std::string>",
        "std::array<int>", "std::array<float>", "std::array<double>", "std::array<char>",
        "std::array<long>", "std::array<short>", "std::array<bool>", "std::array<std::string>"
    };

    while (std::getline(stream, line)) {
        if (line.find("int dummy_main()") != std::string::npos) {
            in_main_function = true;
            main_content << line << '\n';
            continue;
        }

        if (in_main_function) {
            std::istringstream line_stream(line);
            std::string first_word;
            line_stream >> first_word;

            if (std::find(data_types.begin(), data_types.end(), first_word) != data_types.end()) {
                globals << strip_leading_whitespace(line) << '\n';
                continue;
            }
        } else {
            includes << line << '\n';
            continue;
        }

        main_content << line << '\n';
    }

    content = includes.str() + globals.str() + "\n" + main_content.str();
}

void add_return(std::string& content) {
    auto it = std::find_if(content.rbegin(), content.rend(), [](char ch) {
        return !std::isspace(ch);
    });
    content.erase(it.base(), content.end());
    
    if (!content.empty() && content.back() == '}') {
        content.pop_back();
        content += "\n    return 0;\n}";
    }
}

void rename_main_function(const std::string& input_filepath, const std::string& output_filepath, bool empty = false) {
    std::ofstream output_file(output_filepath);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return;
    }

    std::ifstream input_file(input_filepath);
    if (!input_file.is_open() || empty) {
        output_file << "int dummy_main() { return 1; }";
        output_file.close();
        return;
    }

    std::string content;
    std::string line;
    while (std::getline(input_file, line)) {
        content += line + '\n';
    }
    input_file.close();

    size_t pos = content.find("int main(");

    if (pos != std::string::npos) {
        content.replace(pos, 9, "int dummy_main(");
        add_return(content);
        extract_variables(content);
        output_file << content;
    } else {
        output_file << "int dummy_main() { return 1; }";
    }

    output_file.close();
}

int main(int argc, char** argv) {
    bool empty = argc > 3;
    rename_main_function(argv[1], argv[2], empty);
}