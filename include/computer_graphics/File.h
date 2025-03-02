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

static void exit_if_file_doesnt_exist(const std::filesystem::path& file_path) {

	if (!std::filesystem::exists(file_path)) {

		std::cerr << "ERROR: file " << file_path << " doesnt exist!\n";
		exit(EXIT_FAILURE);

	};

};

static bool check_if_directory(const std::filesystem::path& file_path) {

	if (std::filesystem::is_directory(file_path)) {

		return true;

	};

	return false;

};

static std::string read_file(const std::filesystem::path& file_path) {

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

static std::vector<std::string> read_file_by_line(const std::filesystem::path& file_path) {

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

static std::string read_line_in_file(const std::filesystem::path& file_path, const unsigned int& line_number) {

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

static std::vector<std::filesystem::path> get_files_and_folders_as_paths(const std::filesystem::path& directory_path) {

	exit_if_file_doesnt_exist(directory_path);
	if (!check_if_directory(directory_path)) { std::cerr << "ERROR: inputed directory path " << directory_path << " isnt actually a directory!\n"; };
	std::vector <std::filesystem::path> paths;
	for (auto const& iterator : std::filesystem::directory_iterator(directory_path)) {

		paths.emplace_back(iterator.path());

	};

	return paths;

};

static std::vector<std::filesystem::path> get_files_as_paths(const std::filesystem::path& directory_path) {

	exit_if_file_doesnt_exist(directory_path);
	if (!check_if_directory(directory_path)) { std::cerr << "ERROR: inputed directory path " << directory_path << " isnt actually a directory!\n"; };
	std::vector <std::filesystem::path> files;
	for (auto const& iterator : std::filesystem::directory_iterator(directory_path)) {

		if (!iterator.is_directory()) { files.emplace_back(iterator.path()); };

	};

	return files;

};

static std::vector<std::filesystem::path> get_files_as_paths_recursively(const std::filesystem::path& directory_path) {

	exit_if_file_doesnt_exist(directory_path);
	if (!check_if_directory(directory_path)) { std::cerr << "ERROR: inputed directory path " << directory_path << " isnt actually a directory!\n"; };
	std::vector <std::filesystem::path> files;
	for (auto const& iterator : std::filesystem::recursive_directory_iterator(directory_path)) {

		if (!iterator.is_directory()) { files.emplace_back(iterator.path()); };

	};

	return files;

};

static std::vector<std::filesystem::path> get_sub_directories_as_paths(const std::filesystem::path& directory_path) {

	exit_if_file_doesnt_exist(directory_path);
	if (!check_if_directory(directory_path)) { std::cerr << "ERROR: inputed directory path " << directory_path << " isnt actually a directory!\n"; };
	std::vector <std::filesystem::path> sub_directories;
	for (auto const& iterator : std::filesystem::directory_iterator(directory_path)) {

		if (iterator.is_directory()) { sub_directories.emplace_back(iterator.path()); };

	};

	return sub_directories;

};

static std::vector<std::filesystem::path> get_sub_directories_as_paths_recursively(const std::filesystem::path& directory_path) {

	exit_if_file_doesnt_exist(directory_path);
	if (!check_if_directory(directory_path)) { std::cerr << "ERROR: inputed directory path " << directory_path << " isnt actually a directory!\n"; };
	std::vector <std::filesystem::path> sub_directories;
	for (auto const& iterator : std::filesystem::recursive_directory_iterator(directory_path)) {

		if (iterator.is_directory()) { sub_directories.emplace_back(iterator.path()); };

	};

	return sub_directories;

};

//renames all actual files on the disk with the new name and a number example: myFile =>   myFile_0 myFile_1
static void rename_all_files_in_directory_ON_DISK(const std::filesystem::path& directory_path, const std::string& new_file_name) {

	std::vector<std::filesystem::path> paths = get_files_as_paths(directory_path);
	int i = 0;
	for (auto& path : paths) {

		std::filesystem::path original_absolute_path(path);
		std::string directory = path.parent_path().string();
		std::string new_absolute_path = directory + "/" + new_file_name + "_" + std::to_string(i) + path.extension().string();

		try {

			std::filesystem::rename(original_absolute_path, std::filesystem::path(new_absolute_path));

		}
		catch (const std::filesystem::filesystem_error& e) {

			std::cerr << "Error renaming file: " << e.what() << "\n";

		}

		++i;

	};

};

static void move_files_with_extension(const std::filesystem::path& source_directory, const std::filesystem::path& destination_directory, const std::string& extension) {

	if (!std::filesystem::exists(source_directory) || !std::filesystem::is_directory(source_directory)) {

		std::cerr << "ERROR: source directory does not exist or is not a directory!\n";
		exit(EXIT_FAILURE);

	};

	if (!std::filesystem::exists(destination_directory) || !std::filesystem::is_directory(destination_directory)) {

		std::cerr << "ERROR: destination directory does not exist or is not a directory!\n";
		exit(EXIT_FAILURE);

	};

	for (const auto& entry : std::filesystem::directory_iterator(source_directory)) {

		if (entry.is_regular_file() && entry.path().extension() == extension) {

			std::filesystem::path destination_path = destination_directory / entry.path().filename();

			try {

				std::filesystem::rename(entry.path(), destination_path);
				std::cout << "Moved: " << entry.path() << " to " << destination_path << std::endl;

			}
			catch (const std::filesystem::filesystem_error& error) {

				std::cerr << "Error moving file: " << error.what() << std::endl;

			};

		};

	};

};
