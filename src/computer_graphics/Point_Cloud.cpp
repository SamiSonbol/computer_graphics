#include "computer_graphics/Point_Cloud.h"

void Point_Cloud::from_global_geospatial_coordinates_to_openGL(std::vector<vec3>& positions) {

	std::pair<vec3, vec3> bounds = get_min_max(positions);

	// Compute center and scale factor
	vec3 center = (bounds.first + bounds.second) * 0.5f;
	float scale_factor = 1.0f / 100.0f;

	// Transform the point cloud
	for (vec3& point : positions) {

		point = (point - center); // Move to origin and scale down
		//point *= scale_factor;
		point = vec3(point.x, point.z, point.y);

	};

};

std::vector<vec3> Point_Cloud::extract_points_coordinates(const std::string& path_to_LASer_file) {

	exit_if_file_doesnt_exist(path_to_LASer_file);
	if (std::filesystem::is_directory(std::filesystem::path(path_to_LASer_file))) {

		std::cerr << "ERROR: path is a directory and not a LAS file!\n";
		exit(EXIT_FAILURE);

	};

	std::ifstream data(path_to_LASer_file, std::ios::binary);
	if (!data.is_open()) {

		std::cerr << "ERROR: failed to open LAS file!\n";
		exit(EXIT_FAILURE);

	};

	Public_Header_Block header;
	data.read(reinterpret_cast<char*>(&header), sizeof(Public_Header_Block));
	if (!data) {

		std::cerr << "ERROR: failed to read LAS header!\n";
		data.clear();
		data.close();
		exit(EXIT_FAILURE);

	};

	std::cout << "file signature: " << header.file_signature << "\n";
	std::cout << "major: " << static_cast<int>(header.version_major) << " minor: " << static_cast<int>(header.version_minor) << "\n";
	if (header.version_minor != 4 || header.version_major != 1) {

		std::cerr << "ERROR: incorrect LAS file version!\n";
		data.clear();
		data.close();
		exit(EXIT_FAILURE);

	};

	std::cout << "size of header: " << header.header_size << "\n";
	if (header.header_size != sizeof(header)) {

		std::cerr << "ERROR: size of header was incorrect!\n";
		data.clear();
		data.close();
		exit(EXIT_FAILURE);

	};

	std::cout << "point data record format: " << static_cast<int>(header.point_data_record_format) << "\n";
	std::vector<vec3> points_coordinates;
	points_coordinates.resize(header.number_of_point_records);
	data.seekg(header.offset_to_point_data, std::ios::beg);
	for (int i = 0; i < header.number_of_point_records; i++) {

		switch (header.point_data_record_format) {

			case 1: { Point_Data_Record_Format_1 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_1)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
			case 6: { Point_Data_Record_Format_6 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_6)); points_coordinates[i] = compute_point_coordinates(header, point_data_record); break; };
			default: std::cerr << "ERROR: unsupported Data Record Format!\n"; data.clear(); data.close(); exit(EXIT_FAILURE);

		};

	};

	if (!data.good()) {

		std::cerr << "ERROR: data stream was bad!\n";
		data.clear();
		data.close();
		exit(EXIT_FAILURE);

	};

	if (points_coordinates.empty()) {

		std::cerr << "ERROR: points_coordinates vector was empty!\n";
		data.clear();
		data.close();
		exit(EXIT_FAILURE);

	};

	from_global_geospatial_coordinates_to_openGL(points_coordinates);

	data.clear();
	data.close();
	return points_coordinates;

};

//std::vector<Point_Cloud::Point_Data_Record> Point_Cloud::extract_points(const std::string& path_to_LASer_file) {
//
//	exit_if_file_doesnt_exist(path_to_LASer_file);
//	std::ifstream data(path_to_LASer_file, std::ios::binary);
//	if (!data.is_open()) {
//
//		std::cerr << "ERROR: failed to open las file!\n";
//		exit(EXIT_FAILURE);
//
//	};
//
//	Public_Header_Block header;
//	data.read((char*)&header, sizeof(Public_Header_Block));
//	if (!data) {
//
//		std::cerr << "ERROR: failed to read LAS header!\n";
//		data.clear();
//		data.close();
//		exit(EXIT_FAILURE);
//
//	};
//
//	std::cout << "file signature: " << header.file_signature << "\n";
//	std::cout << "minor: " << header.version_minor << " major: " << header.version_major << "\n";
//	if (header.version_major != 1 || header.version_minor != 2) {
//
//		std::cerr << "ERROR: incorrect las versions!\n";
//		data.clear();
//		data.close();
//		exit(EXIT_FAILURE);
//
//	};
//
//	std::cout << "size of header: " << header.header_size << "\n";
//	if (header.header_size != sizeof(header)) {
//
//		std::cerr << "ERROR: size of header was incorrect!\n";
//		data.clear();
//		data.close();
//		exit(EXIT_FAILURE);
//
//	};
//
//	std::cout << "point data record format: " << header.point_data_record_format << "\n";
//	if (header.point_data_record_format != 1) {
//
//		std::cerr << "ERROR: incorrect las format!\n";
//		data.clear();
//		data.close();
//		exit(EXIT_FAILURE);
//
//	};
//
//	std::vector<Point_Data_Record> point_data_records;
//	point_data_records.resize(header.number_of_point_records);
//	data.seekg(header.offset_to_point_data, std::ios::beg);
//	for (int i = 0; i < header.number_of_point_records; i++) {
//
//		data.read((char*)&point_data_records[i], sizeof(Point_Data_Record));
//
//	};
//
//	if (!data.good()) {
//
//		std::cerr << "ERROR: data stream was bad!\n";
//		data.clear();
//		data.close();
//		exit(EXIT_FAILURE);
//
//	};
//
//	if (point_data_records.empty()) {
//
//		std::cerr << "ERROR: Point_Data_Record vector was empty!\n";
//		data.clear();
//		data.close();
//		exit(EXIT_FAILURE);
//
//	};
//
//	data.clear();
//	data.close();
//	return point_data_records;
//
//};

//Point_Cloud::Point_Cloud() {
//
//	auto format_1 = [](){}
//
//};