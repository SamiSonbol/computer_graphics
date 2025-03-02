#pragma once

#include <iostream>
#include <functional>
#include <unordered_map>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include "computer_graphics/Math.h"
#include "computer_graphics/Shader.h"

struct label_hasher {

	std::size_t operator()(const std::string& label) const {

		return std::hash<std::string>()(label);

	};

};

class Mouse {

 public:

	 GLFWcursor* cursor;
	 vec2 position;

	 void load_cursor_texture(GLFWwindow* window, const std::string& file_path);

	 void update_position(GLFWwindow* window);
	 bool check_for_mouse_click(GLFWwindow* window);
	 vec2 from_screen_to_NDC(const vec2& screen_position, const vec2& screen_size);
	 vec3 from_screen_to_world(const vec2& screen_position, const vec2& screen_size, const mat4& projection_matrix, const mat4& view_matrix);
	 vec2 from_model_to_screen(const vec3& model_position, const vec2& screen_size, const mat4& projection_matrix, const mat4& view_matrix, const mat4& model_matrix);

	 void plot_point(const bool& plot, GLFWwindow* window, Shader& shader);

	 // Convert RGB back to Object ID
	 int colorToID(unsigned char pixel[3]) {

		 return pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);

	 };

	 void pick_object(Shader& shader, GLFWwindow* window, const vec2& mouse_position, const vec2& screen_size, const bool& plot) {

		 if (plot && check_for_mouse_click(window)) {

			 vec2 pixel_position(mouse_position.x, screen_size.y - mouse_position.y - 1);
			 if (pixel_position.x < 0 || pixel_position.x >= screen_size.x ||
				 pixel_position.y < 0 || pixel_position.y >= screen_size.y) {

				 std::cerr << "ERROR: mouse to pixel position out of bounds!\n";
				 return;

			 };

		     std::array<float, 4> pixel = shader.read_frame_buffer(GL_COLOR_ATTACHMENT1, pixel_position, GL_RGBA, GL_FLOAT);
			 std::cout << "position: "; print_vec(vec4(pixel[0], pixel[1], pixel[2], pixel[3]));

		 };

	 };

	 void update(Shader& shader, GLFWwindow* window, const vec2& screen_size, const bool& plot);

	 Mouse(GLFWwindow* window, const std::string& texture_file_path);

};

class UI {

 public:

	class Window {

	 public:

		std::string label;
		std::vector<std::function<void()>> functions;

		void add_function(std::function<void()>&& lambda);
		void delete_function(const int& index);

		void update();

		Window() : label("default construction") {};//default constructor so the compiler doesnt throw *no default constructor available* when initializing an std::unordered_map that holds *Window*
		Window(const std::string& label);

	};
	
	std::unordered_map<std::string, Window, label_hasher> windows;

private:

	vec2 screen_size;

	std::vector<std::filesystem::path> shaders;
	std::vector<std::filesystem::path> texture_maps;
	std::vector<std::filesystem::path> obj_files;
	std::vector<std::filesystem::path> las_files;
	std::filesystem::path shader_folder_path;
	std::filesystem::path obj_file_path;
	std::filesystem::path las_file_path;
	std::filesystem::path texture_map_path;
	unsigned int gl_primitive_type;
	bool from_OBJ_file;
	bool from_LAS_file;
	bool from_Texture_map;

	std::string rendering_information;
	std::string console_message;

public:

	void add_window(const std::string& label);
	void delete_window(const std::string& label);

	void new_frame();

	void vec2_float_sliders(const std::string& label, vec2& vec, const vec2& min_max);
	void vec2_float_sliders(const std::string& label_x, const std::string& label_y, vec2& vec, const vec2& min_max);
	void vec2_float_sliders_drag(const std::string& label, vec2& vec, const float& drag_speed, const vec2& min_max);
	void vec2_vertical_float_sliders(const std::string& label, vec2& vec, const vec2& min_max, const vec2& size);
	void vec2_vertical_float_sliders(const std::string& label_x, const std::string& label_y, vec2& vec, const vec2& min_max, const vec2& size);

	void vec3_float_sliders(const std::string& label, vec3& vec, const vec2& min_max);
	void vec3_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, vec3& vec, const vec2& min_max);
	void vec3_float_sliders_drag(const std::string& label, vec3& vec, const float& drag_speed, const vec2& min_max);
	void vec3_vertical_float_sliders(const std::string& label, vec3& vec, const vec2& min_max, const vec2& size);
	void vec3_vertical_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, vec3& vec, const vec2& min_max, const vec2& size);
	void vec3_color_picker(const std::string& label, vec3& RGB);

	void vec4_float_sliders(const std::string& label, vec4& vec, const vec2& min_max);
	void vec4_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, const std::string& label_w, vec4& vec, const vec2& min_max);
	void vec4_float_sliders_drag(const std::string& label, vec4& vec, const float& drag_speed, const vec2& min_max);
	void vec4_vertical_float_sliders(const std::string& label, vec4& vec, const vec2& min_max, const vec2& size);
	void vec4_vertical_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, const std::string& label_w, vec4& vec, const vec2& min_max, const vec2& size);
	void vec4_color_picker(const std::string& label, vec4& RGBA);

	void shader_rendering_options(Shader& shader, Mesh& mesh, unsigned int& GL_PRIMITIVE_TYPE, unsigned int& vertex_array);
	void shader_scene_and_model_manipulation(Shader& shader, bool& plot);
	void shader_debug_mode(Shader& shader, Mesh& mesh, unsigned int& GL_PRIMITIVE_TYPE, unsigned int& vertex_array, bool& plot);

	void render();

	void destroy();

	UI(GLFWwindow* window, const vec2& screen_size, const std::string& GLSL_VERSION);

};