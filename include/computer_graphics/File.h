#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <istream>
#include <regex>

static std::string read_file(const std::string& file_path) {

	if (!std::filesystem::exists(file_path)) {

		std::cerr << "ERROR: file " << file_path <<" doesnt exist!\n";
		exit(EXIT_FAILURE);

	};

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

	if (!std::filesystem::exists(file_path)) {

		std::cerr << "ERROR: file " << file_path << " doesnt exist!\n";
		exit(EXIT_FAILURE);

	};

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

	if (!std::filesystem::exists(file_path)) {

		std::cerr << "ERROR: file " << file_path << " doesnt exist!\n";
		exit(EXIT_FAILURE);

	};

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

//uses regex to tokenise instead of a char
static std::vector<std::string> tokenise_data(const std::string& data, const std::regex& regex) {

	std::vector<std::string> tokens;

	auto begin = std::sregex_iterator(data.begin(), data.end(), regex);
	auto end = std::sregex_iterator();
	for (std::sregex_iterator i = begin; i != end; ++i) {

		std::smatch match = *i;
		for (size_t j = 1; j < match.size(); ++j) {//only subgroups since we are tokenising and not matching

			tokens.emplace_back(match[j].str());

		};

	};

	if (tokens.empty()) {

		std::cerr << "WARNING: no matches from regex were found in data " << data << ", empty vector returned!\n";

	};

	return tokens;

};

//returns the first token found
static std::string tokenise_once(const std::string& data, const char& delimeter) {

	if (data.empty()) {

		std::cerr << "ERROR: string data " << data << " is empty!\n";
		exit(EXIT_FAILURE);

	};

	std::istringstream string_data(data);
	std::string token;
	if (std::getline(string_data, token, delimeter)) {

		return token;

	}
	else {

		std::cerr << "WARNING: no token found! returning " << data << " as is\n";
		return data;

	};

	return token;

};

//returns the first token using regex
static std::string tokenise_once(const std::string& data, const std::regex& regex) {

	std::smatch matches;
	std::regex_search(data, matches, regex);
	if (!matches.empty() && matches.size() > 1) {

		return matches[1].str();

	}
	else {

		std::cerr << "WARNING: no matches from regex were found in data " << data << ", empty string returned!\n";
		return "";

	};

};

//uses regex to return all regex matches, difference between this function and *tokenise_data(string, regex)* is that this function returns also matches[0],... whilst *tokenise_data(string, regex)* returns matches[1],...
static std::vector<std::string> capture_all_regex_matches(const std::string& data, const std::regex& regex) {

	std::vector<std::string> tokens;

	auto begin = std::sregex_iterator(data.begin(), data.end(), regex);
	auto end = std::sregex_iterator();
	for (std::sregex_iterator i = begin; i != end; ++i) {

		std::smatch match = *i;
		for (size_t j = 0; j < match.size(); ++j) {

			tokens.emplace_back(match[j].str());

		};

	};

	if (tokens.empty()) {

		std::cerr << "WARNING: no matches from regex were found in data " << data << ", empty vector returned!\n";

	};

	return tokens;

};

