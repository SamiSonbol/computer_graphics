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

class UI {

 public:

	class Window {

	 public:

		std::string label;
		std::vector<std::function<void()>> functions;

		void add_function(std::function<void()>&& lambda) {
			
			functions.emplace_back(std::move(lambda));

		};
		void delete_function(const int& index);

		void update();

		Window() : label("default construction") {};//default constructor so the compiler doesnt throw *no default constructor available* when initializing an std::unordered_map that holds *Window*
		Window(const std::string& label);

	};
	
	std::unordered_map<std::string, Window, label_hasher> windows;
	ImGuiIO* IO;

	void add_window(const std::string& label);
	void delete_window(const std::string& label);

	void new_frame();

	void vec2_float_sliders(const std::string& label, vec2& vec, const vec2& min_max);
	void vec2_float_sliders(const std::string& label_x, const std::string& label_y, vec2& vec, const vec2& min_max);
	void vec2_vertical_float_sliders(const std::string& label, vec2& vec, const vec2& min_max, const vec2& size);
	void vec2_vertical_float_sliders(const std::string& label_x, const std::string& label_y, vec2& vec, const vec2& min_max, const vec2& size);

	void vec3_float_sliders(const std::string& label, vec3& vec, const vec2& min_max);
	void vec3_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, vec3& vec, const vec2& min_max);
	void vec3_vertical_float_sliders(const std::string& label, vec3& vec, const vec2& min_max, const vec2& size);
	void vec3_vertical_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, vec3& vec, const vec2& min_max, const vec2& size);
	void vec3_color_picker(const std::string& label, vec3& RGB);

	void vec4_float_sliders(const std::string& label, vec4& vec, const vec2& min_max);
	void vec4_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, const std::string& label_w, vec4& vec, const vec2& min_max);
	void vec4_vertical_float_sliders(const std::string& label, vec4& vec, const vec2& min_max, const vec2& size);
	void vec4_vertical_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, const std::string& label_w, vec4& vec, const vec2& min_max, const vec2& size);
	void vec4_color_picker(const std::string& label, vec4& RGBA);
		
	void shader_debug_mode(Shader::graphics_vectors_container& vectors_container, Shader::graphics_booleans_container& booleans_container, Shader::graphics_floats_container& floats_container);

	void update();
	void render();

	void destroy();

	UI(GLFWwindow* window, const std::string& GLSL_VERSION);

};