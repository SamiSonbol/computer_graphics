#include "computer_graphics/Point_Cloud.h"

void Point_Cloud::from_global_geospatial_coordinates_to_openGL(std::vector<vec3>& positions) {

	std::pair<vec3, vec3> bounds = get_min_max(positions);

	//compute center and scale factor
	vec3 center = (bounds.first + bounds.second) * 0.5f;
	float length = std::abs((bounds.first - bounds.second).magnitude());

	float scale_factor = 100.0f / length;

	for (vec3& point : positions) {

		point = (point - center);//move to origin and scale down
		point *= scale_factor;
		point = vec3(point.x, point.z, -point.y);//since the LAS geospace take Z as up whilst openGL takes Y(and Y inverted at that), we need to switch them

	};

};

std::vector<vec3> Point_Cloud::extract_points_coordinates(const std::string& path_to_LASer_file) {

	exit_if_file_doesnt_exist(path_to_LASer_file);
	if (std::filesystem::is_directory(std::filesystem::path(path_to_LASer_file))) { std::cerr << "ERROR: path is a directory and not a LAS file!\n"; exit(EXIT_FAILURE); };

	std::ifstream data(path_to_LASer_file, std::ios::binary);
	if (!data.is_open()) { std::cerr << "ERROR: failed to open LAS file!\n"; exit(EXIT_FAILURE); };

	char signature[4];
	data.read(signature, 4);
	if (std::string(signature, 4) != "LASF") { std::cerr << "ERROR: not a valid LAS file!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

	uint8_t version_major, version_minor;
	data.seekg(24, std::ios::beg);
	data.read(reinterpret_cast<char*>(&version_major), 1);
	data.read(reinterpret_cast<char*>(&version_minor), 1);
	if (version_major != 1) { std::cerr << "ERROR: major version must be 1! read version is " << static_cast<int>(version_major) << "!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
	std::cout << "minor: " << static_cast<int>(version_minor) << "\n";
	data.seekg(0, std::ios::beg);
	std::vector<vec3> points_coordinates;
	switch (version_minor) {

		case 0: {

			Public_Header_Block_Version_1_0 header;
			data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_0));
			if (!data) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			points_coordinates.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				switch (header.point_data_record_format) {

					case 0: { Point_Data_Record_Format_0 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_0)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 1: { Point_Data_Record_Format_1 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_1)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 2: { Point_Data_Record_Format_2 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_2)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 3: { Point_Data_Record_Format_3 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_3)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 4: { Point_Data_Record_Format_4 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_4)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 5: { Point_Data_Record_Format_5 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_5)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 6: { Point_Data_Record_Format_6 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_6)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 7: { Point_Data_Record_Format_7 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_7)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 8: { Point_Data_Record_Format_8 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_8)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 9: { Point_Data_Record_Format_9 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_9)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 10: { Point_Data_Record_Format_10 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_10)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					default: std::cerr << "ERROR: unsupported Data Record Format " << static_cast<int>(header.point_data_record_format) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE);

				};

			};

			break;

		};

		case 1: {

			Public_Header_Block_Version_1_1 header;
			data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_1));
			if (!data) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			points_coordinates.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				switch (header.point_data_record_format) {

					case 0: { Point_Data_Record_Format_0 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_0)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 1: { Point_Data_Record_Format_1 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_1)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 2: { Point_Data_Record_Format_2 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_2)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 3: { Point_Data_Record_Format_3 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_3)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 4: { Point_Data_Record_Format_4 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_4)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 5: { Point_Data_Record_Format_5 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_5)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 6: { Point_Data_Record_Format_6 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_6)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 7: { Point_Data_Record_Format_7 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_7)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 8: { Point_Data_Record_Format_8 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_8)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 9: { Point_Data_Record_Format_9 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_9)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 10: { Point_Data_Record_Format_10 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_10)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					default: std::cerr << "ERROR: unsupported Data Record Format " << static_cast<int>(header.point_data_record_format) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE);

				};

			};

			break;

		};

		case 2: {

			Public_Header_Block_Version_1_2 header;
			data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_2));
			if (!data) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			points_coordinates.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				switch (header.point_data_record_format) {

					case 0: { Point_Data_Record_Format_0 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_0)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 1: { Point_Data_Record_Format_1 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_1)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 2: { Point_Data_Record_Format_2 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_2)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 3: { Point_Data_Record_Format_3 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_3)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 4: { Point_Data_Record_Format_4 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_4)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 5: { Point_Data_Record_Format_5 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_5)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 6: { Point_Data_Record_Format_6 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_6)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 7: { Point_Data_Record_Format_7 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_7)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 8: { Point_Data_Record_Format_8 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_8)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 9: { Point_Data_Record_Format_9 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_9)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 10: { Point_Data_Record_Format_10 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_10)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					default: std::cerr << "ERROR: unsupported Data Record Format " << static_cast<int>(header.point_data_record_format) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE);

				};

			};

			break;

		};

		case 3: {

			Public_Header_Block_Version_1_3 header;
			data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_3));
			if (!data) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			points_coordinates.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				switch (header.point_data_record_format) {

					case 0: { Point_Data_Record_Format_0 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_0)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 1: { Point_Data_Record_Format_1 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_1)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 2: { Point_Data_Record_Format_2 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_2)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 3: { Point_Data_Record_Format_3 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_3)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 4: { Point_Data_Record_Format_4 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_4)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 5: { Point_Data_Record_Format_5 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_5)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 6: { Point_Data_Record_Format_6 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_6)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 7: { Point_Data_Record_Format_7 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_7)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 8: { Point_Data_Record_Format_8 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_8)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 9: { Point_Data_Record_Format_9 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_9)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 10: { Point_Data_Record_Format_10 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_10)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					default: std::cerr << "ERROR: unsupported Data Record Format " << static_cast<int>(header.point_data_record_format) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE);

				};

			};

			break;

		};

		case 4: {

			Public_Header_Block_Version_1_4 header;
			data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block_Version_1_4));
			if (!data) { std::cerr << "ERROR: failed to read LAS header!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
			if (header.header_size != sizeof(header)) { std::cerr << "ERROR: size of header was incorrect! Current size: " << static_cast<int>(header.header_size) << ", should be: " << sizeof(header) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };

			points_coordinates.resize(header.number_of_point_records);
			data.seekg(header.offset_to_point_data, std::ios::beg);
			for (int i = 0; i < header.number_of_point_records; i++) {

				switch (header.point_data_record_format) {

					case 0: { Point_Data_Record_Format_0 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_0)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 1: { Point_Data_Record_Format_1 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_1)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 2: { Point_Data_Record_Format_2 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_2)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 3: { Point_Data_Record_Format_3 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_3)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 4: { Point_Data_Record_Format_4 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_4)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 5: { Point_Data_Record_Format_5 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_5)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 6: { Point_Data_Record_Format_6 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_6)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 7: { Point_Data_Record_Format_7 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_7)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 8: { Point_Data_Record_Format_8 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_8)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 9: { Point_Data_Record_Format_9 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_9)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					case 10: { Point_Data_Record_Format_10 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_10)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
					default: std::cerr << "ERROR: unsupported Data Record Format " << static_cast<int>(header.point_data_record_format) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE);

				};

			};

			break;

		};

	};

	if (!data.good()) { std::cerr << "ERROR: data stream was bad!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
	if (points_coordinates.empty()) { std::cerr << "ERROR: points_coordinates vector was empty!\n"; data.clear(); data.close(); exit(EXIT_FAILURE); };
	data.clear();
	data.close();

	from_global_geospatial_coordinates_to_openGL(points_coordinates);
	return points_coordinates;

};