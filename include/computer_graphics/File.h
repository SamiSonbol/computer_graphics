#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <istream>
#include <regex>
#include <cstdarg>
#include <cstdio>

static void exit_if_file_doesnt_exist(const std::string& file_path) {

	if (!std::filesystem::exists(file_path)) {

		std::cerr << "ERROR: file " << file_path << " doesnt exist!\n";
		exit(EXIT_FAILURE);

	};

};

static bool check_if_directory(const std::string& file_path) {

	if (std::filesystem::is_directory(std::filesystem::path(file_path))) {

		return true;

	};

	return false;

};

static std::string read_file(const std::string& file_path) {

	exit_if_file_doesnt_exist(file_path);

	std::ifstream file(file_path);
	std::stringstream data;
	if (!file.is_open()) {

		std::cerr << "ERROR: failed to read from file  " << file_path << " !\n";
		exit(EXIT_FAILURE);

	};

	data << file.rdbuf();
	file.close();
	return data.str();

};

static std::vector<std::string> read_file_by_line(const std::string& file_path) {

	exit_if_file_doesnt_exist(file_path);

	std::ifstream file(file_path);
	if (!file.is_open()) {

		std::cerr << "ERROR: failed to read from file  " << file_path << " !\n";
		exit(EXIT_FAILURE);

	};
	
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line)) {

		lines.emplace_back(line);

	};

	file.close();
	return lines;

};

static std::string read_line_in_file(const std::string& file_path, const unsigned int& line_number) {

	exit_if_file_doesnt_exist(file_path);

	std::ifstream file(file_path);
	if (!file.is_open()) {

		std::cerr << "ERROR: failed to read from file  " << file_path << " !\n";
		exit(EXIT_FAILURE);

	};

	int i = 0;
	std::string line;
	while (std::getline(file, line)) {

		if (line_number == i) {

			file.close();
			return line;

		};

		i++;

	};

	file.close();
	std::cerr << "ERROR: line doesnt exist\n";
	exit(EXIT_FAILURE);

};

//returns all tokens as a vector
static std::vector<std::string> tokenise_data(const std::string& data, const char& delimeter) {

	if (data.empty()) {

		std::cerr << "ERROR: string data " << data << " is empty!\n";
		exit(EXIT_FAILURE);

	};
	
	std::istringstream string_data(data);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(string_data, token, delimeter)) {

		tokens.emplace_back(token);

	};

	return tokens;

};

//uses regex to extract matches. If *extract_all_matches* is false, then the function skips the first match, which is a good thing if we want to use the match as a dellimeter
static std::vector<std::string> extract_data(const std::string& data, const std::regex& regex, bool extract_all_matches = true, bool print_warning = true) {

	std::vector<std::string> tokens;

	auto begin = std::sregex_iterator(data.begin(), data.end(), regex);
	auto end = std::sregex_iterator();
	for (std::sregex_iterator i = begin; i != end; ++i) {

		size_t j = 0;
		std::smatch match = *i;
		if (!extract_all_matches) { j = 1; };
		for (j; j < match.size(); ++j) {

			tokens.emplace_back(match[j].str());

		};

	};

	if (tokens.empty() && print_warning) {

		std::cerr << "WARNING: no matches from regex were found in data " << data << ", empty vector returned!\n";

	};

	return tokens;

};

//a wrapper function for sscanf that checks if the parsing was correct.
static void safe_sscanf(const char* buffer, const char* format, const int n_of_variables_to_parse, ...) {//VIMP NOTE: va_start() cannot take refrenced variables.

	//initializing variadic args with the size of *n_of_variables_to_parse* to hold our *...* parameters
	va_list args;
	va_start(args, n_of_variables_to_parse);

	//vsscanf to parse variadic arguments
	int parsed = vsscanf(buffer, format, args);
	va_end(args);

	//checking if we parsed the same number of variables we inputed
	if (parsed != n_of_variables_to_parse) {

		std::cerr << "ERROR: safe_sscanf couldn't parse data, expected " << n_of_variables_to_parse << ", but parsed " << parsed << "!\n";
		exit(EXIT_FAILURE);

	};

};
