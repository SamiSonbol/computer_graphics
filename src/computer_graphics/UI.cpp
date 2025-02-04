#include "computer_graphics/UI.h"

void UI::Window::delete_function(const int& index) {

	if (index < 0 || index > this->functions.size()) {

		std::cerr << "ERROR: index out of bounds";
		exit(EXIT_FAILURE);

	};

	int i = 0;
	for (auto iterator = this->functions.begin(); iterator != this->functions.end(); ++iterator) {

		if (i == index) {

			iterator = this->functions.erase(iterator);
			return;

		};

		i++;

	};

};

void UI::Window::update() {

	ImGui::Begin(this->label.c_str());

	for (auto& function : this->functions) {

		function();

	};

	ImGui::End();

};

UI::Window::Window(const std::string& label) : label(label) {};

void UI::add_window(const std::string& label) {

	auto iterator = this->windows.find(label);
	if (iterator == this->windows.end()) {//if it doesnt exist than its added

		this->windows[label] = UI::Window(label);

	}
	else {

		std::cerr << "WARNING: window already exists, nothing new was added\n";

	};
	
};

void UI::delete_window(const std::string& label) {

	auto iterator = this->windows.find(label);
	if (iterator != this->windows.end()) {

		iterator = this->windows.erase(iterator);
		return;

	}
	else {

		std::cerr << "ERROR: could not find window to delete";
		exit(EXIT_FAILURE);

	};

};

void UI::new_frame() {

	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

};

void UI::vec2_float_sliders(const std::string& label, vec2& vec, const vec2& min_max) {

	ImGui::SliderFloat((label + " X").c_str(), &vec.x, min_max.x, min_max.y);
	ImGui::SliderFloat((label + " Y").c_str(), &vec.y, min_max.x, min_max.y);

};
void UI::vec2_float_sliders(const std::string& label_x, const std::string& label_y, vec2& vec, const vec2& min_max) {

	ImGui::SliderFloat(label_x.c_str(), &vec.x, min_max.x, min_max.y);
	ImGui::SliderFloat(label_y.c_str(), &vec.y, min_max.x, min_max.y);

};
void UI::vec2_vertical_float_sliders(const std::string& label, vec2& vec, const vec2& min_max, const vec2& size) {

	ImGui::VSliderFloat((label + " X").c_str(), ImVec2(size.x, size.y), &vec.x, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat((label + " Y").c_str(), ImVec2(size.x, size.y), &vec.y, min_max.x, min_max.y);

};
void UI::vec2_vertical_float_sliders(const std::string& label_x, const std::string& label_y, vec2& vec, const vec2& min_max, const vec2& size) {

	ImGui::VSliderFloat(label_x.c_str(), ImVec2(size.x, size.y), &vec.x, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat(label_y.c_str(), ImVec2(size.x, size.y), &vec.y, min_max.x, min_max.y);

};

void UI::vec3_float_sliders(const std::string& label, vec3& vec, const vec2& min_max) {

	ImGui::SliderFloat((label + " X").c_str(), &vec.x, min_max.x, min_max.y);
	ImGui::SliderFloat((label + " Y").c_str(), &vec.y, min_max.x, min_max.y);
	ImGui::SliderFloat((label + " Z").c_str(), &vec.z, min_max.x, min_max.y);

};
void UI::vec3_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, vec3& vec, const vec2& min_max) {

	ImGui::SliderFloat(label_x.c_str(), &vec.x, min_max.x, min_max.y);
	ImGui::SliderFloat(label_y.c_str(), &vec.y, min_max.x, min_max.y);
	ImGui::SliderFloat(label_z.c_str(), &vec.z, min_max.x, min_max.y);

};
void UI::vec3_vertical_float_sliders(const std::string& label, vec3& vec, const vec2& min_max, const vec2& size) {

	ImGui::VSliderFloat((label + " X").c_str(), ImVec2(size.x, size.y), &vec.x, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat((label + " Y").c_str(), ImVec2(size.x, size.y), &vec.y, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat((label + " Z").c_str(), ImVec2(size.x, size.y), &vec.z, min_max.x, min_max.y);

};
void UI::vec3_vertical_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, vec3& vec, const vec2& min_max, const vec2& size) {

	ImGui::VSliderFloat(label_x.c_str(), ImVec2(size.x, size.y), &vec.x, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat(label_y.c_str(), ImVec2(size.x, size.y), &vec.y, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat(label_z.c_str(), ImVec2(size.x, size.y), &vec.z, min_max.x, min_max.y);

};
void UI::vec3_color_picker(const std::string& label, vec3& RGB) {

	float color_array[3] = { RGB.x, RGB.y, RGB.z };
	ImGui::ColorEdit3(label.c_str(), color_array);
	RGB = color_array;

};

void UI::vec4_float_sliders(const std::string& label, vec4& vec, const vec2& min_max) {

	ImGui::SliderFloat((label + " X").c_str(), &vec.x, min_max.x, min_max.y);
	ImGui::SliderFloat((label + " Y").c_str(), &vec.y, min_max.x, min_max.y);
	ImGui::SliderFloat((label + " Z").c_str(), &vec.z, min_max.x, min_max.y);
	ImGui::SliderFloat((label + " W").c_str(), &vec.w, min_max.x, min_max.y);

};
void UI::vec4_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, const std::string& label_w, vec4& vec, const vec2& min_max) {

	ImGui::SliderFloat(label_x.c_str(), &vec.x, min_max.x, min_max.y);
	ImGui::SliderFloat(label_y.c_str(), &vec.y, min_max.x, min_max.y);
	ImGui::SliderFloat(label_z.c_str(), &vec.z, min_max.x, min_max.y);
	ImGui::SliderFloat(label_w.c_str(), &vec.w, min_max.x, min_max.y);

};
void UI::vec4_vertical_float_sliders(const std::string& label, vec4& vec, const vec2& min_max, const vec2& size) {

	ImGui::VSliderFloat((label + " X").c_str(), ImVec2(size.x, size.y), &vec.x, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat((label + " Y").c_str(), ImVec2(size.x, size.y), &vec.y, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat((label + " Z").c_str(), ImVec2(size.x, size.y), &vec.z, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat((label + " W").c_str(), ImVec2(size.x, size.y), &vec.w, min_max.x, min_max.y);

};
void UI::vec4_vertical_float_sliders(const std::string& label_x, const std::string& label_y, const std::string& label_z, const std::string& label_w, vec4& vec, const vec2& min_max, const vec2& size) {

	ImGui::VSliderFloat(label_x.c_str(), ImVec2(size.x, size.y), &vec.x, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat(label_y.c_str(), ImVec2(size.x, size.y), &vec.y, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat(label_z.c_str(), ImVec2(size.x, size.y), &vec.z, min_max.x, min_max.y);
	ImGui::SameLine();
	ImGui::VSliderFloat(label_w.c_str(), ImVec2(size.x, size.y), &vec.w, min_max.x, min_max.y);

};
void UI::vec4_color_picker(const std::string& label, vec4& RGBA) {

	float color_array[4] = { RGBA.x, RGBA.y, RGBA.z, RGBA.w };
	ImGui::ColorEdit3(label.c_str(), color_array);
	RGBA = color_array;

};

void UI::shader_debug_mode(Shader::graphics_vectors_container& vectors_container, Shader::graphics_booleans_container& booleans_container, Shader::graphics_floats_container& floats_container) {

	this->windows.clear();
	this->add_window("Transformations");

	//Model collapse header
	this->windows["Transformations"].add_function([&]() {

		if (ImGui::CollapsingHeader("Model")) {

			ImGui::SeparatorText("Translation");
			this->vec3_float_sliders("Model Translation", vectors_container.translation_vector, vec2(-500, 500));
			ImGui::SeparatorText("Rotation");
			this->vec3_float_sliders("Model Rotation", vectors_container.rotation_vector, vec2(0, 360));
			ImGui::SeparatorText("Scale");
			this->vec3_vertical_float_sliders("Model Scale", vectors_container.scaling_vector, vec2(-20, 20), vec2(40, 100));
			ImGui::SameLine();
			ImGui::VSliderFloat("Tesselation Multiplier", ImVec2(40, 100), &floats_container.tesselation_multiplier, 0.0f, 1000.0f);
			ImGui::SameLine();
			ImGui::VSliderFloat("Displacement Scale", ImVec2(40, 100), &floats_container.displacement_scale, 0.0f, 1000.0f);

		};

	});

	//Light collapse header
	this->windows["Transformations"].add_function([&]() {

		if (ImGui::CollapsingHeader("Light")) {

			ImGui::Checkbox("Gamma Correction", &booleans_container.gamma_correction);
			ImGui::SameLine();
			ImGui::Checkbox("Texturing", &booleans_container.texturing);
			ImGui::SameLine();
			ImGui::Checkbox("Normal Mapping", &booleans_container.normal_mapping);
			ImGui::SameLine();
			ImGui::Checkbox("Height Mapping", &booleans_container.height_mapping);

			this->vec3_color_picker("Light Color", vectors_container.light_color);
			ImGui::SeparatorText("Translation");
			this->vec3_float_sliders("Light Translation", vectors_container.light_position, vec2(-500, 500));
			ImGui::SeparatorText("Material Properties");
			this->vec4_vertical_float_sliders("Ambient", "Diffuse", "Specular", "Shininess", vectors_container.material_properties, vec2(0, 20), vec2(60, 100));

		};

	});

	//Camera collapse header
	this->windows["Transformations"].add_function([&]() {

		if (ImGui::CollapsingHeader("Camera")) {

			ImGui::SeparatorText("Translation");
			this->vec3_float_sliders("Camera Translation", vectors_container.camera_position, vec2(-500, 500));
			ImGui::SeparatorText("Up");
			this->vec3_float_sliders("Up Direction", vectors_container.up_vector, vec2(-500, 500));
			ImGui::SeparatorText("Right");
			this->vec3_float_sliders("Right Direction", vectors_container.right_vector, vec2(-500, 500));
			ImGui::SeparatorText("Forward");
			this->vec3_float_sliders("Forward Direction", vectors_container.direction_vector, vec2(-500, 500));

		};

	});

};

void UI::update() {

	for (auto iterator = this->windows.begin(); iterator != this->windows.end(); ++iterator) {

		iterator->second.update();

	};

};

void UI::render() {

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

};

void UI::destroy() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

};

UI::UI(GLFWwindow* window, const std::string& GLSL_VERSION) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	this->IO = &ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsDark();

};