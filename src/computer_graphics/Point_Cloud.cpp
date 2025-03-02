#include "computer_graphics/Point_Cloud.h"

void Point_Cloud::extract_openGL_points_attributes(const std::filesystem::path& path_to_LASer_file, std::vector<vec3>& points_coordinates, std::vector<vec3>& points_colors) {

	exit_if_file_doesnt_exist(path_to_LASer_file);
	if (std::filesystem::is_directory(std::filesystem::path(path_to_LASer_file))) { std::cerr << "ERROR: path is a directory and not a LAS file!\n"; exit(EXIT_FAILURE); };

	std::ifstream data(path_to_LASer_file, std::ios::binary);
	if (!data.is_open()) { std::cerr << "ERROR: failed to open LAS file!\n"; exit(EXIT_FAILURE); };

	char signature[4];
	if (!data.read(signature, 4)) { std::cerr << "ERROR: couldnt read file!\n"; data.close(); exit(EXIT_FAILURE); };
	if (std::string(signature, 4) != "LASF") { std::cerr << "ERROR: not a valid LAS file!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

	data.seekg(24, std::ios::beg);
	uint8_t version_major, version_minor;
	data.read(reinterpret_cast<char*>(&version_major), 1); data.read(reinterpret_cast<char*>(&version_minor), 1);
	if (version_major != 1) { std::cerr << "ERROR: major version must be 1! read version is " << static_cast<int>(version_major) << "!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

	data.seekg(0, std::ios::beg);
	switch (version_minor) {

		case 0: {

			Public_Header_Block_Version_1_0 header;
			if (!data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_0))) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			this->extract_members_data(header);

			points_coordinates.resize(header.number_of_point_records);
			points_colors.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				read_point_and_extract_openGL_attributes(header, data, points_coordinates[i], points_colors[i]);

			};

			break;

		};

		case 1: {

			Public_Header_Block_Version_1_1 header;
			if (!data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_1))) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			this->extract_members_data(header);

			points_coordinates.resize(header.number_of_point_records);
			points_colors.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				read_point_and_extract_openGL_attributes(header, data, points_coordinates[i], points_colors[i]);

			};

			break;

		};

		case 2: {

			Public_Header_Block_Version_1_2 header;
			if (!data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_2))) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			this->extract_members_data(header);

			points_coordinates.resize(header.number_of_point_records);
			points_colors.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				read_point_and_extract_openGL_attributes(header, data, points_coordinates[i], points_colors[i]);

			};

			break;

		};

		case 3: {

			Public_Header_Block_Version_1_3 header;
			if (!data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_3))) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			this->extract_members_data(header);

			points_coordinates.resize(header.number_of_point_records);
			points_colors.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				read_point_and_extract_openGL_attributes(header, data, points_coordinates[i], points_colors[i]);

			};

			break;

		};

		case 4: {

			Public_Header_Block_Version_1_4 header;
			if (!data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_4))) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			this->extract_members_data(header);

			points_coordinates.resize(header.number_of_point_records);
			points_colors.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				read_point_and_extract_openGL_attributes(header, data, points_coordinates[i], points_colors[i]);

			};

			break;

		};

	};

	if (!data.good()) { std::cerr << "ERROR: data stream was bad!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
	if (points_coordinates.empty()) { std::cerr << "ERROR: points_coordinates vector was empty!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
	data.clear();
	data.close();

	std::cout << "Header Version: " << this->Public_Header_Block_Version << " Data Format: " << this->Point_Data_Record_Format << "\n";

};