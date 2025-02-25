#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "computer_graphics/File.h"
#include "computer_graphics/Math.h"
#include "computer_graphics/Mesh.h"

static GLFWwindow* INIT_GLAD_GLFW_WINDOW(vec2& screen_size, const vec3& clear_color) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);

	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor) {

		std::cerr << "Failed to get primary monitor\n";
		glfwTerminate();
		exit(EXIT_FAILURE);

	};

	const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {

		std::cerr << "Failed to get video mode\n";
		glfwTerminate();
		exit(EXIT_FAILURE);

	};
	screen_size = vec2(video_mode->width, video_mode->height);

	GLFWwindow* window = glfwCreateWindow(video_mode->width, video_mode->height, PROJECT_NAME, NULL, NULL);
	if (window == NULL) {

		std::cout << "failed";
		glfwTerminate();
		exit(EXIT_FAILURE);

	};
	glfwMakeContextCurrent(window);
	gladLoadGL();
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	glViewport(0, 0, video_mode->width, video_mode->height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);

	return window;

};

class Shader {

public:

	unsigned int program;
	unsigned int positions_buffer, normals_buffer, colors_buffer, indices_buffer, texture_coordinates_buffer, tangents_buffer, bitangents_buffer;
	
	void create_uniform_bool(const bool& boolean, const char* uniform_name);
	void create_uniform_int(const int& data_variable, const char* uniform_name);
	void create_uniform_float(const float& data_variable, const char* uniform_name);
	void create_uniform_vec2(const std::vector<float>& data_vector, const char* uniform_name);
	void create_uniform_vec3(const std::vector<float>& data_vector, const char* uniform_name);
	void create_uniform_mat4(const std::vector<float>& data_vector, const char* uniform_name);
	void create_uniform_2D_texture(const int& index, const char* uniform_name);

private:

	std::unordered_map<std::string, bool> bool_uniforms_map;
	std::unordered_map<std::string, int> int_uniforms_map;
	std::unordered_map<std::string, float> float_uniforms_map;
	std::unordered_map<std::string, vec2> vec2_uniforms_map;
	std::unordered_map<std::string, vec3> vec3_uniforms_map;
	std::unordered_map<std::string, mat4> mat4_uniforms_map;

	void upload_bool_uniforms();
	void upload_int_uniforms();
	void upload_float_uniforms();
	void upload_vec2_uniforms();
	void upload_vec3_uniforms();
	void upload_mat4_uniforms();

public:

	//opted to use a different function for each type instead of a templated function which checks for size and gets the correct map type, since i need speed. Having to check everytime for all possible types before getting a hit will take along time if we have alot of uniforms.
	bool& get_reference_bool_uniform(const std::string& uniform_name);
	int& get_reference_int_uniform(const std::string& uniform_name);
	float& get_reference_float_uniform(const std::string& uniform_name);
	vec2& get_reference_vec2_uniform(const std::string& uniform_name);
	vec3& get_reference_vec3_uniform(const std::string& uniform_name);
	mat4& get_reference_mat4_uniform(const std::string& uniform_name);

	void set_value_bool_uniform(const std::string& uniform_name, const bool& value);
	void set_value_int_uniform(const std::string& uniform_name, const int& value);
	void set_value_float_uniform(const std::string& uniform_name, const float& value);
	void set_value_vec2_uniform(const std::string& uniform_name, const vec2& value);
	void set_value_vec3_uniform(const std::string& uniform_name, const vec3& value);
	void set_value_mat4_uniform(const std::string& uniform_name, const mat4& value);

	void default_uniforms_maps_initialization(const vec2& screen_size);
	void update_uniforms();

	//generates a buffer from the inputted paramter and binds it OR binds the inputted buffer
	template<typename T>
	static void bind_buffer(const bool& generate_buffer, const unsigned int& GL_ARRAY_TYPE, unsigned int* buffer, const std::vector<T>& buffer_data, const unsigned int& GL_DRAW_TYPE, const int& attribute_position, const int& attribute_size) {

		if (generate_buffer) { glGenBuffers(1, buffer); };
		glBindBuffer(GL_ARRAY_TYPE, *buffer);
		if (*buffer == 0) {

			std::cerr << "ERORR: failed to generate buffer!\n";
			exit(EXIT_FAILURE);

		};

		glBufferData(GL_ARRAY_TYPE, buffer_data.size() * sizeof(T), buffer_data.data(), GL_DRAW_TYPE);

		if (generate_buffer) {

			if (GL_ARRAY_TYPE == GL_ARRAY_BUFFER) {

				glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);

				glEnableVertexAttribArray(attribute_position);

			};

		};

	};
	void bind_texture(const bool& generate_texture, unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels, const bool& gamma_correction);
	void update_texture(unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels);

	void bind_mesh_buffers_and_textures(Mesh& mesh, const unsigned int& GL_DRAW_TYPE, const bool& gamma_correction);
	void draw_mesh_elements(Mesh& mesh, const unsigned int& GL_PRIMITIVE_TYPE = GL_TRIANGLES);
	void bind_and_draw_mesh_elements(Mesh& mesh, const unsigned int& GL_PRIMITIVE_TYPE = GL_TRIANGLES, const unsigned int& GL_DRAW_MODE = GL_STATIC_DRAW, const bool& gamma_correction = true);

	void delete_buffers();
	void delete_program();
	void delete_all();

	unsigned int compile_shader(const unsigned int& type, const std::string& source);
	Shader(std::vector<unsigned int>& compiled_shaders_ids);
	Shader(const std::string& shader_directory);

};