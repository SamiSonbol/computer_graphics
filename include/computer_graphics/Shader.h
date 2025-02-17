#pragma once
#include <iostream>
#include <vector>

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
	GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, PROJECT_NAME, NULL, NULL);
	if (window == NULL) {

		std::cout << "failed";
		glfwTerminate();
		exit(EXIT_FAILURE);

	};
	glfwMakeContextCurrent(window);
	gladLoadGL();
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	glViewport(0, 0, screen_size.x, screen_size.y);
	glEnable(GL_DEPTH_TEST);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);

	return window;

};

class Shader {

public:

	//this is a struct that holds the basic vectors needed to update and initialize our image, used this instead of an std::vector<vec3> inorder to attain readability, clarity, and avoid the issue of guessing which index is which vec3.
	struct graphics_vectors_container {

		vec3 right_vector;
		vec3 up_vector;
		vec3 direction_vector;
		vec3 camera_position;
		vec3 camera_rotation_vector;
		vec3 translation_vector;
		vec3 scaling_vector;
		vec3 rotation_vector;
		vec3 light_position;
		vec3 light_color;
		vec4 material_properties;

	};

	//same functionality as *graphics_vectors_container* but for bools
	struct graphics_booleans_container {

		bool orthogonal_projection;
		bool gamma_correction;
		bool texturing;
		bool normal_mapping;
		bool displacement_mapping;
		bool height_coloring;

	};

	//same functionality as *graphics_vectors_container* but for bools
	struct graphics_floats_container {

		float orthogonal_size;
		float FOV;
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

	//sets the transformation matrices and light vectors
	void init_matrices(const vec2& screen_size, const float& orthogonal_size, const float& FOV, const bool& orthogonal_projection, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& camera_rotation_vector, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);
	void init_light(const vec3& light_position, const vec3& light_color, const vec4& material_properties);
	void init_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& displacement_mapping, const bool& height_coloring);
	void init_floats(const float& tesselation_multiplier, const float& displacement_scale);
	void initialize(const vec2& screen_size, const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container);

	void update(const vec2& screen_size, const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container);

	void bind_mesh_buffers(Mesh& mesh, const unsigned int& GL_DRAW_TYPE, const bool& gamma_correction);
	void bind_and_draw_mesh_elements(Mesh& mesh, const unsigned int& GL_PRIMITIVE_TYPE = GL_TRIANGLES, const unsigned int& GL_DRAW_MODE = GL_STATIC_DRAW, const bool& gamma_correction = true);

	void delete_buffers();
	void delete_program();
	void delete_all();

	unsigned int compile_shader(const unsigned int& type, const std::string& source);
	Shader(std::vector<unsigned int>& compiled_shaders_ids);
	Shader(const std::string& shader_directory);

};