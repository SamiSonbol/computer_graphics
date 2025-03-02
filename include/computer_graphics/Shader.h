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
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

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
	unsigned int positions_buffer, normals_buffer, colors_buffer, indices_buffer, texture_coordinates_buffer, tangents_buffer, bitangents_buffer, frame_buffer;
	unsigned int frame_buffer_colors_texture_ID, frame_buffer_positions_texture_ID, frame_buffer_depth_texture_ID;
	
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
	static void bind_array_buffer(const bool& generate_array_buffer, unsigned int* array_buffer, const std::vector<T>& array_buffer_data, const unsigned int& GL_DRAW_TYPE, const int& attribute_position, const int& attribute_size) {

		if (generate_array_buffer) { glGenBuffers(1, array_buffer); };
		glBindBuffer(GL_ARRAY_BUFFER, *array_buffer);
		if (*array_buffer == 0) {

			std::cerr << "ERORR: failed to generate buffer!\n";
			exit(EXIT_FAILURE);

		};

		glBufferData(GL_ARRAY_BUFFER, array_buffer_data.size() * sizeof(T), array_buffer_data.data(), GL_DRAW_TYPE);

		if (generate_array_buffer) {

			glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);
			glEnableVertexAttribArray(attribute_position);

		};

	};

	template<typename T>
	static void bind_index_buffer(const bool& generate_index_buffer, unsigned int* index_buffer, const std::vector<T>& index_buffer_data, const unsigned int& GL_DRAW_TYPE) {

		if (generate_index_buffer) { glGenBuffers(1, index_buffer); };
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer);
		if (*index_buffer == 0) {

			std::cerr << "ERORR: failed to generate buffer!\n";
			exit(EXIT_FAILURE);

		};

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(T), index_buffer_data.data(), GL_DRAW_TYPE);

	};

	void generate_frame_buffer(unsigned int* frame_buffer, unsigned int* colors_texture_ID, unsigned int* positions_texture_ID, const vec2& screen_size, unsigned int* depth_texture_ID) {

		glGenFramebuffers(1, frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, *frame_buffer);
		
		glGenTextures(1, colors_texture_ID);
		glBindTexture(GL_TEXTURE_2D, *colors_texture_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screen_size.x, screen_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *colors_texture_ID, 0);

		glGenTextures(1, positions_texture_ID);
		glBindTexture(GL_TEXTURE_2D, *positions_texture_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screen_size.x, screen_size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, *positions_texture_ID, 0);

		GLenum draw_buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, draw_buffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { 
			
			std::cerr << "ERROR: framebuffer is not complete! Frame buffer status code: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
			exit(EXIT_FAILURE); 
		
		};
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	};
	void bind_frame_buffer(const unsigned int& GL_XX_FRAMEBUFFER) { glBindFramebuffer(GL_XX_FRAMEBUFFER, this->frame_buffer); };
	std::array<float, 4> read_frame_buffer(const unsigned int& GL_COLOR_ATTACHMENT_X, const vec2& pixel_position, const unsigned int& data_format, const unsigned int& type) {

		bind_frame_buffer(GL_READ_FRAMEBUFFER);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) { std::cerr << "ERROR: framebuffer incomplete: " << status << "\n"; exit(EXIT_FAILURE); };

		std::array<float, 4> pixel;
		glReadBuffer(GL_COLOR_ATTACHMENT_X);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {

			std::cerr << "OpenGL Error: " << error << std::endl;
			exit(EXIT_FAILURE);

		};

		glReadPixels(pixel_position.x, pixel_position.y, 1, 1, data_format, type, pixel.data());
		error = glGetError();
		if (error != GL_NO_ERROR) {

			std::cerr << "OpenGL Error: " << error << std::endl;
			exit(EXIT_FAILURE);

		};

		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		return pixel;

	};

	void bind_texture(const bool& generate_texture, unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels, const bool& gamma_correction);
	void update_texture(unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels);

	static constexpr unsigned int DRAW_TO_FRAME_BUFFER = 1;
	void bind_mesh_buffers_and_textures(Mesh& mesh, const vec2& screen_size, const unsigned int& GL_DRAW_TYPE, const bool& gamma_correction);
	void draw_mesh_elements(Mesh& mesh, const unsigned int& GL_PRIMITIVE_TYPE = GL_TRIANGLES);
	void bind_and_draw_mesh_elements(Mesh& mesh, const vec2& screen_size, const unsigned int& GL_PRIMITIVE_TYPE = GL_TRIANGLES, const unsigned int& GL_DRAW_MODE = GL_STATIC_DRAW, const bool& gamma_correction = true);

	void delete_buffers();
	void delete_program();
	void delete_all();

	unsigned int compile_shader(const unsigned int& type, const std::filesystem::path& source);
	Shader(std::vector<unsigned int>& compiled_shaders_ids);
	Shader(const std::filesystem::path& shader_directory);

	void rebuild(const std::filesystem::path& shader_directory, unsigned int& vertex_array);
	void rebuild(std::vector<unsigned int>& compiled_shaders_ids, unsigned int& vertex_array);

};