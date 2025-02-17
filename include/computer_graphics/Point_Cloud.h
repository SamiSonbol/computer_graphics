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

public:

#pragma pack(push, 1) 
	struct Public_Header_Block_Version_1_0 {

		char file_signature[4]; // "LASF"
		uint32_t reserved; // Reserved field
		uint32_t GUID_data1;
		uint16_t GUID_data2;
		uint16_t GUID_data3;
		uint8_t GUID_data4[8];
		uint8_t version_major; // Major version number
		uint8_t version_minor; // Minor version number
		char system_identifier[32]; // System identifier
		char generating_software[32]; // Generating software
		uint16_t flight_date_julian; // Julian day of the flight date
		uint16_t year; // Year of the flight
		uint16_t header_size; // Size of the header block
		uint32_t offset_to_point_data; // Offset to point data
		uint32_t number_of_variable_length_records; // Number of variable-length records
		uint8_t point_data_record_format; // Point data record format (0 in LAS 1.0)
		uint16_t point_data_record_length; // Length of the point data record
		uint32_t number_of_point_records; // Number of point records in the file
		uint32_t number_of_points_by_return[5]; // Number of points by return (for 5 returns)
		double X_scale_factor, Y_scale_factor, Z_scale_factor; // Scale factors
		double X_offset, Y_offset, Z_offset; // Offsets
		double max_X, min_X; // Maximum and minimum X coordinates
		double max_Y, min_Y; // Maximum and minimum Y coordinates
		double max_Z, min_Z; // Maximum and minimum Z coordinates

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

	template<typename Public_Header_Block_Version_X_X, typename Point_Data_Record_Format_X>
	vec3 compute_point_coordinates(const Public_Header_Block_Version_X_X& header, const Point_Data_Record_Format_X& point_data_record) {

		float x = (point_data_record.X * header.X_scale_factor) + header.X_offset;
		float y = (point_data_record.Y * header.Y_scale_factor) + header.Y_offset;
		float z = (point_data_record.Z * header.Z_scale_factor) + header.Z_offset;

		return vec3(x, y, z);

	};
	void from_global_geospatial_coordinates_to_openGL(std::vector<vec3>& positions);
	std::vector<vec3> extract_points_coordinates(const std::string& path_to_LASer_file);

};
	

