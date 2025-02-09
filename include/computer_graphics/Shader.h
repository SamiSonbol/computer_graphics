#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>

#include <stb_image/stb_image.h>
#include "computer_graphics/Math.h"
#include "computer_graphics/Mesh.h"

static std::string read_shader(const std::string& file_path) {

	std::ifstream shader_file(file_path);
	std::stringstream shader_data;
	if (!shader_file.is_open()) {

		std::cerr << "ERROR: failed to read shader from file!\n";
		exit(EXIT_FAILURE);

	};

    std::string extension = file_path.substr(file_path.size() - 5);
	if (extension != ".vert" && extension != ".tesc" && extension != ".tese" && extension != ".geom" && extension != ".comp" && extension != ".frag") {

		std::cerr << "ERROR: no shader extension such as *.vert* was detected, file is not a shader!\n";
		exit(EXIT_FAILURE);

	};

	shader_data << shader_file.rdbuf();
	shader_file.close();
	return shader_data.str();

};

static unsigned int compile_shader(const unsigned int& type, const std::string& source) {

	unsigned int id = glCreateShader(type);

	const char* src = source.c_str();//SOURCE HAS TO EXIST TO USE THIS, MAKE SURE IT IS ALIVE AT THE TIME OF THIS COMPILATION

	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {

		int length;

		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "Vertex"
				: type == GL_FRAGMENT_SHADER ? "Fragment"
				: type == GL_TESS_CONTROL_SHADER ? "Tesselation Control"
				: type == GL_TESS_EVALUATION_SHADER ? "Tesselation Evaluation"
				: "Compute")
			<< " Shader." << std::endl;

		std::cout << message << std::endl;

		glDeleteShader(id);

		return -1;

	};

	return id;

};

//generates a buffer from the inputted paramter and binds it OR binds the inputted buffer
template<typename T>
static void bind_buffer(const bool& generate_buffer, const unsigned int& GL_ARRAY_TYPE, unsigned int* buffer, const std::vector<T>& buffer_data, const unsigned int& GL_DRAW_TYPE, const int& attribute_position, const int& attribute_size) {

	if (generate_buffer) { glGenBuffers(1, buffer); };
	glBindBuffer(GL_ARRAY_TYPE, *buffer);
	if (*buffer == 0) {

		std::cerr << "ERORR: failed to generate buffer!\n";
		exit(EXIT_FAILURE);

	};

	if (generate_buffer) {

		glBufferData(GL_ARRAY_TYPE, buffer_data.size() * sizeof(T), buffer_data.data(), GL_DRAW_TYPE);

		if (attribute_position > -1) {

			glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);

			glEnableVertexAttribArray(attribute_position);

		};

	};

};

static void bind_texture(const bool& generate_texture, unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels, const bool& gamma_correction) {

	glActiveTexture(GL_TEXTUREindex);
	if (generate_texture) { glGenTextures(1, texture_ID); };
	glBindTexture(GL_TEXTURE_2D, *texture_ID);
	if (*texture_ID == 0) {

		std::cerr << "ERORR: failed to generate texture!\n";
		exit(EXIT_FAILURE);

	};

	if (generate_texture) {

		GLenum internal_format;
		GLenum data_format;
		if (n_color_channels == 4) {//checks if n_color_channels is 3 or 4; RGB or RGBA

			internal_format = gamma_correction ? GL_SRGB_ALPHA : GL_RGBA;
			data_format = GL_RGBA;

		}
		else if (n_color_channels == 3) {

			internal_format = gamma_correction ? GL_SRGB : GL_RGB;
			data_format = GL_RGB;

		}
		else if (n_color_channels == 1) {

			internal_format = GL_RED;
			data_format = GL_RED;

		};

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, texture_width, texture_height, 0, data_format, GL_UNSIGNED_BYTE, bytes);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);

	};

};

static void update_texture(unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels) {

	glActiveTexture(GL_TEXTUREindex);
	glBindTexture(GL_TEXTURE_2D, *texture_ID);
	if (*texture_ID == 0) {

		std::cerr << "ERORR: failed to update texture!\n";
		exit(EXIT_FAILURE);

	};

	GLenum data_format;
	if (n_color_channels == 4) {//checks if n_color_channels is 3 or 4; RGB or RGBA

		data_format = GL_RGBA;

	}
	else if (n_color_channels == 3) {

		data_format = GL_RGB;

	}
	else if (n_color_channels == 1) {

		data_format = GL_RED;

	};

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture_width, texture_height, data_format, GL_UNSIGNED_BYTE, bytes);

};

class Shader {

public:

	//this is a struct that holds the basic vectors needed to update and initialize our image, used this instead of an std::vector<vec3> inorder to attain readability, clarity, and avoid the issue of guessing which index is which vec3.
	struct graphics_vectors_container {

		vec3 right_vector;
		vec3 up_vector;
		vec3 direction_vector;
		vec3 camera_position;
		vec3 translation_vector;
		vec3 scaling_vector;
		vec3 rotation_vector;
		vec3 light_position;
		vec3 light_color;
		vec4 material_properties;

	};

	//same functionality as *graphics_vectors_container* but for bools
	struct graphics_booleans_container {

		bool gamma_correction;
		bool texturing;
		bool normal_mapping;
		bool displacement_mapping;

	};

	//same functionality as *graphics_vectors_container* but for bools
	struct graphics_floats_container {

		float tesselation_multiplier;
		float displacement_scale;

	};

	unsigned int program;
	unsigned int positions_buffer, normals_buffer, colors_buffer, indices_buffer, texture_coordinates_buffer, tangents_buffer, bitangents_buffer;
	
	void create_uniform_mat4(const std::vector<float>& data_vector, const char* uniform_name);
	void create_uniform_vec3(const std::vector<float>& data_vector, const char* uniform_name);
	void create_uniform_float(const float& data_variable, const char* uniform_name);
	void create_uniform_int(const int& data_variable, const char* uniform_name);
	void create_uniform_bool(const bool& boolean, const char* uniform_name);
	void create_uniform_2D_texture(const int& index, const char* uniform_name);

	graphics_vectors_container create_standard_shader_vectors();
	graphics_booleans_container create_standard_shader_booleans();
	graphics_floats_container create_standard_shader_floats();

	void init_matrices(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);
	void init_light(const vec3& light_position, const vec3& color, const vec4& material_properties);
	void init_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& displacement_mapping);
	void init_floats(const float& tesselation_multiplier, const float& displacement_scale);

	//sets the transformation matrices and light vectors
	void initialize(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& displacement_mapping, const float& tesselation_multiplier, const float& displacement_scale);
	void initialize(const vec2& screen_size, const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container);

	void update_matrices(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);
	void update_light(const vec3& light_position, const vec3& light_color, const vec4& material_properties);
	void update_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& displacement_mapping);
	void update_floats(const float& tesselation_multiplier, const float& displacement_scale);

	void update(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& displacement_mapping, const float& tesselation_multiplier, const float& displacement_scale);
	void update(const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container);

	void bind_mesh_buffers(Mesh& mesh, const bool& gamma_correction);
	void bind_and_draw_mesh_elements(Mesh& mesh, const bool& gamma_correction, const unsigned int& GL_PRIMITIVE_TYPE);


	void delete_buffers();
	void delete_program();
	void delete_all();

	Shader(std::vector<unsigned int>& compiled_shaders_ids);
	Shader(const std::string& shader_directory);

};