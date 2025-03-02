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
#include <functional>

#include "computer_graphics/File.h"
#include "computer_graphics/Math.h"

class Point_Cloud {

private:

	std::string Public_Header_Block_Version;
	std::string Point_Data_Record_Format;

	vec3 openGL_min_position;
	vec3 openGL_max_position;
	vec3 openGL_center;
	float length;
	float scale_factor;

public:

#pragma pack(push, 1) 
	struct Public_Header_Block_Version_1_0 {

		char file_signature[4];
		uint32_t reserved;
		uint32_t GUID_data1;
		uint16_t GUID_data2;
		uint16_t GUID_data3;
		uint8_t GUID_data4[8];
		uint8_t version_major;
		uint8_t version_minor;
		char system_identifier[32];
		char generating_software[32];
		uint16_t flight_date_julian;
		uint16_t year;
		uint16_t header_size;
		uint32_t offset_to_point_data;
		uint32_t number_of_variable_length_records;
		uint8_t point_data_record_format;
		uint16_t point_data_record_length;
		uint32_t number_of_point_records;
		uint32_t number_of_points_by_return[5];
		double X_scale_factor, Y_scale_factor, Z_scale_factor;
		double X_offset, Y_offset, Z_offset;
		double max_X, min_X;
		double max_Y, min_Y;
		double max_Z, min_Z;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Public_Header_Block_Version_1_1 {

		char file_signature[4];
		uint16_t file_source_ID;
		uint16_t reserved;
		uint32_t GUID_data1;
		uint16_t GUID_data2;
		uint16_t GUID_data3;
		uint8_t GUID_data4[8];
		uint8_t version_major;
		uint8_t version_minor;
		char system_identifier[32];
		char generating_software[32];
		uint16_t file_creation_day_of_year;
		uint16_t file_creation_year;
		uint16_t header_size;
		uint32_t offset_to_point_data;
		uint32_t number_of_variable_length_records;
		uint8_t point_data_record_format;
		uint16_t point_data_record_length;
		uint32_t number_of_point_records;
		uint32_t number_of_points_by_return[5];
		double X_scale_factor, Y_scale_factor, Z_scale_factor;
		double X_offset, Y_offset, Z_offset;
		double max_X, min_X;
		double max_Y, min_Y;
		double max_Z, min_Z;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Public_Header_Block_Version_1_2 {

		char file_signature[4];
		uint16_t file_source_ID;
		uint16_t global_encoding;
		uint32_t GUID_data1;
		uint16_t GUID_data2;
		uint16_t GUID_data3;
		uint8_t GUID_data4[8];
		uint8_t version_major;
		uint8_t version_minor;
		char system_identifier[32];
		char generating_software[32];
		uint16_t file_creation_day_of_year;
		uint16_t file_creation_year;
		uint16_t header_size;
		uint32_t offset_to_point_data;
		uint32_t number_of_variable_length_records;
		uint8_t point_data_record_format;
		uint16_t point_data_record_length;
		uint32_t number_of_point_records;
		uint32_t number_of_points_by_return[5];
		double X_scale_factor, Y_scale_factor, Z_scale_factor;
		double X_offset, Y_offset, Z_offset;
		double max_X, min_X;
		double max_Y, min_Y;
		double max_Z, min_Z;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Public_Header_Block_Version_1_3 {

		char file_signature[4];
		uint16_t file_source_ID;
		uint16_t global_encoding;
		uint32_t GUID_data1;
		uint16_t GUID_data2;
		uint16_t GUID_data3;
		uint8_t GUID_data4[8];
		uint8_t version_major;
		uint8_t version_minor;
		char system_identifier[32];
		char generating_software[32];
		uint16_t file_creation_day_of_year;
		uint16_t file_creation_year;
		uint16_t header_size;
		uint32_t offset_to_point_data;
		uint32_t number_of_variable_length_records;
		uint8_t point_data_record_format;
		uint16_t point_data_record_length;
		uint32_t number_of_point_records;
		uint32_t number_of_points_by_return[7];
		double X_scale_factor, Y_scale_factor, Z_scale_factor;
		double X_offset, Y_offset, Z_offset;
		double max_X, min_X;
		double max_Y, min_Y;
		double max_Z, min_Z;
		uint64_t start_of_waveform_data_packet_record;

	};
#pragma pack(pop)

//inorder to remove the padding of bits that the compiler adds to fill up variables and to keep our structs memory size very small and to stay on par with the requirements of a .las file, we add these #pragma commands
#pragma pack(push, 1) 
	struct Public_Header_Block_Version_1_4 {

		char file_signature[4];
		uint16_t file_source_ID;
		uint16_t global_encoding;
		uint32_t GUID_data1;
		uint16_t GUID_data2;
		uint16_t GUID_data3;
		uint8_t GUID_data4[8];
		uint8_t version_major;
		uint8_t version_minor;
		char system_identifier[32];
		char generating_software[32];
		uint16_t file_creation_day_of_year;
		uint16_t file_creation_year;
		uint16_t header_size;
		uint32_t offset_to_point_data;
		uint32_t number_of_variable_length_records;
		uint8_t point_data_record_format;
		uint16_t point_data_record_length;
		uint32_t legacy_number_of_point_records;
		uint32_t legacy_number_of_point_by_return[5];
		double X_scale_factor, Y_scale_factor, Z_scale_factor;
		double X_offset, Y_offset, Z_offset;
		double max_X, max_Y, max_Z;
		double min_X, min_Y, min_Z;
		uint64_t start_of_waveform_data_packet_record;
		uint64_t start_of_first_extended_variable_length_record;
		uint32_t number_of_extended_variable_length_records;
		uint64_t number_of_point_records;
		uint64_t number_of_points_by_return[15];

	};
#pragma pack(pop)

/////////DATA FORMAT

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_0 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 3;
		uint8_t number_of_returns : 3;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t scan_angle_rank;
		uint8_t user_data;
		uint16_t point_source_ID;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_1 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 3;
		uint8_t number_of_returns : 3;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t scan_angle_rank;
		uint8_t user_data;
		uint16_t point_source_ID;
		double GPS_time;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_2 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 3;
		uint8_t number_of_returns : 3;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t scan_angle_rank;
		uint8_t user_data;
		uint16_t point_source_ID;
		uint16_t red;
		uint16_t green;
		uint16_t blue;


	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_3 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 3;
		uint8_t number_of_returns : 3;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t scan_angle_rank;
		uint8_t user_data;
		uint16_t point_source_ID;
		double GPS_time;
		uint16_t red;
		uint16_t green;
		uint16_t blue;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_4 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 3;
		uint8_t number_of_returns : 3;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t scan_angle_rank;
		uint8_t user_data;
		uint16_t point_source_ID;
		double GPS_time;
		uint8_t wave_packet_descriptor_index;
		uint64_t byte_offset_to_waveform_data;
		uint32_t waveform_packet_size_in_bytes;
		float return_point_waveform_location;
		float Dx, Dy, Dz;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_5 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 3;
		uint8_t number_of_returns : 3;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t scan_angle_rank;
		uint8_t user_data;
		uint16_t point_source_ID;
		double GPS_time;
		uint16_t red;
		uint16_t green;
		uint16_t blue;
		uint8_t wave_packet_descriptor_index;
		uint64_t byte_offset_to_waveform_data;
		uint32_t waveform_packet_size_in_bytes;
		float return_point_waveform_location;
		float  Dx, Dy, Dz;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_6 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 4;
		uint8_t number_of_returns : 4;
		uint8_t classification_flags : 4;
		uint8_t scanner_channel : 2;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t user_data;
		int16_t scan_angle;
		uint16_t point_source_ID;
		double GPS_time;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_7 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 4;
		uint8_t number_of_returns : 4;
		uint8_t classification_flags : 4;
		uint8_t scanner_channel : 2;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t user_data;
		int16_t scan_angle;
		uint16_t point_source_ID;
		double GPS_time;
		uint16_t red;
		uint16_t green;
		uint16_t blue;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_8 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 4;
		uint8_t number_of_returns : 4;
		uint8_t classification_flags : 4;
		uint8_t scanner_channel : 2;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t user_data;
		int16_t scan_angle;
		uint16_t point_source_ID;
		double GPS_time;
		uint16_t red;
		uint16_t green;
		uint16_t blue;
		uint16_t near_infrared;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_9 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 4;
		uint8_t number_of_returns : 4;
		uint8_t classification_flags : 4;
		uint8_t scanner_channel : 2;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t user_data;
		int16_t scan_angle;
		uint16_t point_source_ID;
		double GPS_time;
		uint8_t wave_packet_descriptor_index;
		uint64_t byte_offset_to_waveform_data;
		uint32_t waveform_packet_size_in_bytes;
		float return_point_waveform_location;
		float  Dx, Dy, Dz;

	};
#pragma pack(pop)

#pragma pack(push, 1) 
	struct Point_Data_Record_Format_10 {

		int32_t X;
		int32_t Y;
		int32_t Z;
		uint16_t intensity;
		uint8_t return_number : 4;
		uint8_t number_of_returns : 4;
		uint8_t classification_flags : 4;
		uint8_t scanner_channel : 2;
		uint8_t scan_direction_flag : 1;
		uint8_t edge_of_flight_line : 1;
		uint8_t classification;
		uint8_t user_data;
		int16_t scan_angle;
		uint16_t point_source_ID;
		double GPS_time;
		uint16_t red;
		uint16_t green;
		uint16_t blue;
		uint16_t near_infrared;
		uint8_t wave_packet_descriptor_index;
		uint64_t byte_offset_to_waveform_data;
		uint32_t waveform_packet_size_in_bytes;
		float return_point_waveform_location;
		float  Dx, Dy, Dz;

	};
#pragma pack(pop)

	template<typename Public_Header_Block_Version_X_X>
	void extract_members_data(const Public_Header_Block_Version_X_X& header) {

		this->Public_Header_Block_Version = std::to_string(header.version_major) + "." + std::to_string(header.version_minor);
		this->Point_Data_Record_Format = std::to_string(header.point_data_record_format);

		this->openGL_min_position = vec3(header.min_X, header.min_Y, header.min_Z);
		this->openGL_min_position = vec3(this->openGL_min_position.x, this->openGL_min_position.z, -this->openGL_min_position.y);

		this->openGL_max_position = vec3(header.max_X, header.max_Y, header.max_Z);
		this->openGL_max_position = vec3(this->openGL_max_position.x, this->openGL_max_position.z, -this->openGL_max_position.y);

		this->openGL_center = (this->openGL_min_position + this->openGL_max_position) * 0.5f;
		this->length = (this->openGL_min_position - this->openGL_max_position).magnitude();
		this->scale_factor = 100.0f / this->length;

	};

	template<typename Public_Header_Block_Version_X_X, typename Point_Data_Record_Format_X>
	vec3 compute_geospatial_point_coordinates(const Public_Header_Block_Version_X_X& header, const Point_Data_Record_Format_X& point_data_record) {

		float x = (point_data_record.X * header.X_scale_factor) + header.X_offset;
		float y = (point_data_record.Y * header.Y_scale_factor) + header.Y_offset;
		float z = (point_data_record.Z * header.Z_scale_factor) + header.Z_offset;

		return vec3(x, y, z);

	};

	template<typename Public_Header_Block_Version_X_X, typename Point_Data_Record_Format_X>
	vec3 compute_openGL_point_coordinates(const Public_Header_Block_Version_X_X& header, const Point_Data_Record_Format_X& point_data_record) {

		vec3 geospatial_coordinates = this->compute_geospatial_point_coordinates(header, point_data_record);

		//since the LAS geospace takes Z as up whilst openGL takes Y(and Y inverted at that), we need to switch them.
		return (vec3(geospatial_coordinates.x, geospatial_coordinates.z, -geospatial_coordinates.y) - this->openGL_center) * this->scale_factor;//move to origin and scale down

	};

	template<typename Point_Data_Record_Format_X>
	vec3 compute_openGL_point_colors(const Point_Data_Record_Format_X& point_data_record) {

		return vec3(point_data_record.red, point_data_record.green, point_data_record.blue) / 255.0f;

	};

	template<typename Public_Header_Block_Version_X_X>
	void read_point_and_extract_openGL_attributes(const Public_Header_Block_Version_X_X& header, std::ifstream& data, vec3& position, vec3& color) {

		switch (header.point_data_record_format) {

		    case 0: { Point_Data_Record_Format_0 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_0)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(vec3(1.0f, 1.0f, 1.0f)); break; };
			case 1: { Point_Data_Record_Format_1 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_1)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(vec3(1.0f, 1.0f, 1.0f)); break; };
			case 2: { Point_Data_Record_Format_2 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_2)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(compute_openGL_point_colors(point_data_record)); break; };
			case 3: { Point_Data_Record_Format_3 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_3)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(compute_openGL_point_colors(point_data_record)); break; };
			case 4: { Point_Data_Record_Format_4 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_4)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(vec3(1.0f, 1.0f, 1.0f)); break; };
			case 5: { Point_Data_Record_Format_5 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_5)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(compute_openGL_point_colors(point_data_record)); break; };
			case 6: { Point_Data_Record_Format_6 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_6)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(vec3(1.0f, 1.0f, 1.0f)); break; };
			case 7: { Point_Data_Record_Format_7 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_7)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(compute_openGL_point_colors(point_data_record)); break; };
			case 8: { Point_Data_Record_Format_8 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_8)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(compute_openGL_point_colors(point_data_record)); break; };
			case 9: { Point_Data_Record_Format_9 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_9)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(vec3(1.0f, 1.0f, 1.0f)); break; };
			case 10: { Point_Data_Record_Format_10 point_data_record; data.read(reinterpret_cast<char*>(&point_data_record), sizeof(Point_Data_Record_Format_10)); position = std::move(compute_openGL_point_coordinates(header, point_data_record)); color = std::move(compute_openGL_point_colors(point_data_record)); break; };
			default: std::cerr << "ERROR: unsupported Data Record Format " << static_cast<int>(header.point_data_record_format) << "\n"; data.clear(); data.close(); exit(EXIT_FAILURE);

		};

	};

	void extract_openGL_points_attributes(const std::filesystem::path& path_to_LASer_file, std::vector<vec3>& points_coordinates, std::vector<vec3>& points_colors);

};
	

