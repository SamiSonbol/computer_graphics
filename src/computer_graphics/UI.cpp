#include "computer_graphics/UI.h"

void Mouse::load_cursor_texture(GLFWwindow* window, const std::string& file_path) {

	if (!std::filesystem::exists(file_path)) {

		std::cerr << "ERROR: cursor texture file doesnt exist!\n";
		exit(EXIT_FAILURE);

	} else if (std::filesystem::path(file_path).extension().string() != ".ani" && std::filesystem::path(file_path).extension().string() != ".cur" && std::filesystem::path(file_path).extension().string() != ".png") {

		std::cerr << "ERROR: no correct file extension such as *.ani* or *.cur* or *.png* was detected, file isnt a cursor texture!\n";
		exit(EXIT_FAILURE);

	};

	int n_color_channels;
	GLFWimage cursor_texture;
	cursor_texture.pixels = stbi_load(file_path.c_str(), &cursor_texture.width, &cursor_texture.height, &n_color_channels, 0);
	if (!cursor_texture.pixels) {

		std::cerr << "ERROR: failed to load cursor texture!\n";
		exit(EXIT_FAILURE);

	};

	this->cursor = glfwCreateCursor(&cursor_texture, 0, 0);
	if (!this->cursor) {

		std::cerr << "ERROR: failed to create cursor!\n";
		stbi_image_free(cursor_texture.pixels);
		exit(EXIT_FAILURE);

	};

	stbi_image_free(cursor_texture.pixels);

};

void Mouse::update_position(GLFWwindow* window) {

	glfwGetCursorPos(window, &this->position_x, &this->position_y);
	this->position = vec2(this->position_x, this->position_y);

};

bool Mouse::check_for_mouse_click(GLFWwindow* window) {

	static bool pressed = false;
	static bool clicked = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {

		if (!pressed) { clicked = true; }; 
		pressed = true;

	}
	else { pressed = false; };

	bool was_clicked = clicked;
	clicked = false;

	return was_clicked;

};

vec2 Mouse::from_screen_to_NDC(const vec2& screen_position, const vec2& screen_size) {

	float x = (2.0f * screen_position.x) / screen_size.x - 1.0f;
	float y = 1.0f - (2.0f * screen_position.y) / screen_size.y;//invert y since GLFW 0,0 is top left whilst openGL is bottom left

	return vec2(x, y);

};

vec3 Mouse::from_screen_to_world(const vec2& screen_position, const vec2& screen_size, const mat4& projection_matrix, const mat4& view_matrix) {

	vec2 pos_ndc = this->from_screen_to_NDC(screen_position, screen_size);
	vec4 pos_clip = vec4(pos_ndc, -1.0f, 1.0f);
	vec4 pos_camera_space = projection_matrix.inverse() * pos_clip;
	pos_camera_space = vec4(pos_camera_space.x, pos_camera_space.y, -1.0f, 0.0f);
	vec4 pos_world = view_matrix.inverse() * pos_camera_space;

	return pos_world.xyz();

};

vec2 Mouse::from_model_to_screen(const vec3& model_position, const vec2& screen_size, const mat4& projection_matrix, const mat4& view_matrix, const mat4& model_matrix) {

	vec4 pos_clip = projection_matrix * view_matrix * model_matrix * vec4(model_position, 1);
	pos_clip /= pos_clip.w;
	return vec2((pos_clip.x + 1.0) * 0.5 * screen_size.x, (1.0 - pos_clip.y) * 0.5 * screen_size.y);

};

void Mouse::plot_point(const bool& plot, GLFWwindow* window, Shader& shader) {

	if (plot && check_for_mouse_click(window)) {

		mat4 model_transformation_matrix = create_model_transformation_matrix(shader.get_reference_vec3_uniform("model_translation_vector"), shader.get_reference_vec3_uniform("model_scaling_vector"), shader.get_reference_vec3_uniform("model_rotation_vector"));

		mat4 camera_rotation_matrix = create_rotation_matrix(shader.get_reference_vec3_uniform("camera_rotation_vector"));
		vec3 forward = (camera_rotation_matrix * vec4(shader.get_reference_vec3_uniform("forward_vector"), 0.0f)).xyz();
		vec3 camera_target_position = shader.get_reference_vec3_uniform("camera_position") + forward;
		mat4 view_matrix = create_view_matrix(shader.get_reference_vec3_uniform("camera_position"), camera_target_position, shader.get_reference_vec3_uniform("up_vector"));

		mat4 projection_matrix = create_frustum_projection_matrix(shader.get_reference_float_uniform("FOV"), shader.get_reference_vec2_uniform("screen_size"), 0.1f, 50000.0f);

		vec3 mouse_ray = from_screen_to_world(this->position, vec2(1920, 1080), projection_matrix, view_matrix);
		std::cout << "ray position: "; print_vec(mouse_ray); std::cout << "ray_direction: "; print_vec(mouse_ray.normalize());
		shader.set_value_vec3_uniform("mouse_ray_vector", mouse_ray.normalize());

	};

};

void Mouse::update(Shader& shader, GLFWwindow* window, const vec2& screen_size, const bool& plot) {

	update_position(window);
	//pick_object(shader, window, this->position, screen_size, plot);//
	plot_point(plot, window, shader);
	//glfwSetCursor(window, this->cursor);

};

Mouse::Mouse(GLFWwindow* window, const std::string& texture_file_path) {

	load_cursor_texture(window, texture_file_path);

};

void UI::Window::add_function(std::function<void()>&& lambda) {

	this->functions.emplace_back(std::move(lambda));

};

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

void UI::shader_debug_mode(Shader& shader, bool& plot) {

	this->windows.clear();
	this->add_window("Transformations");

	//Model collapse header
	this->windows["Transformations"].add_function([&]() {

		if (ImGui::CollapsingHeader("Model")) {

			ImGui::SeparatorText("Translation");
			this->vec3_float_sliders("Model Translation", shader.get_reference_vec3_uniform("model_translation_vector"), vec2(-10000, 10000));
			ImGui::SeparatorText("Rotation");
			this->vec3_float_sliders("Model Rotation", shader.get_reference_vec3_uniform("model_rotation_vector"), vec2(-180, 180));
			ImGui::SeparatorText("Scale");
			this->vec3_float_sliders("Model Scale", shader.get_reference_vec3_uniform("model_scaling_vector"), vec2(-20, 20));

			ImGui::SeparatorText("Texturing & Coloring");
			ImGui::Checkbox("Texturing", &shader.get_reference_bool_uniform("texturing"));
			ImGui::SameLine();
			ImGui::Checkbox("Height Coloring", &shader.get_reference_bool_uniform("height_coloring"));

			ImGui::SeparatorText("Tesselation & Displacement");
			ImGui::Checkbox("Displacement Mapping", &shader.get_reference_bool_uniform("displacement_mapping"));
			ImGui::SliderFloat("Tesselation Multiplier", &shader.get_reference_float_uniform("tesselation_multiplier"), 0.0f, 500.0f);
			ImGui::SliderFloat("Displacement Scale", &shader.get_reference_float_uniform("displacement_scale"), 0.0f, 500.0f);
			ImGui::SliderFloat("Point Size", &shader.get_reference_float_uniform("point_size"), 1.0f, 200.0f);

		};

	});

	//Light collapse header
	this->windows["Transformations"].add_function([&]() {

		if (ImGui::CollapsingHeader("Light")) {

			ImGui::Checkbox("Gamma Correction", &shader.get_reference_bool_uniform("gamma_correction"));
			ImGui::SameLine();
			ImGui::Checkbox("Normal Mapping", &shader.get_reference_bool_uniform("normal_mapping"));

			this->vec3_color_picker("Light Color", shader.get_reference_vec3_uniform("light_color"));

			ImGui::SeparatorText("Translation");
			this->vec3_float_sliders("Light Translation", shader.get_reference_vec3_uniform("light_position"), vec2(-10000, 10000));

			ImGui::SeparatorText("Material Properties");
			//this->vec4_vertical_float_sliders("Ambient", "Diffuse", "Specular", "Shininess", vectors_container.material_properties, vec2(0, 20), vec2(60, 100));

		};

		});

	//Camera collapse header
	this->windows["Transformations"].add_function([&]() {

		if (ImGui::CollapsingHeader("Camera")) {

			ImGui::SeparatorText("Orthographic Camera");
			ImGui::Checkbox("Orthographic Projection", &shader.get_reference_bool_uniform("orthogonal_projection"));
			ImGui::SameLine();
			ImGui::SliderFloat("Orthogonal Size", &shader.get_reference_float_uniform("orthogonal_size"), 0.0f, 1000.0f);

			ImGui::SeparatorText("Prespective Camera");
			ImGui::SliderFloat("FOV", &shader.get_reference_float_uniform("FOV"), 1.0f, 190.0f);
			this->vec3_float_sliders("Forward Vector", shader.get_reference_vec3_uniform("forward_vector"), vec2(-1000, 1000));
			this->vec3_float_sliders("Up Vector", shader.get_reference_vec3_uniform("up_vector"), vec2(-1000, 1000));

			ImGui::SeparatorText("Translation");
			this->vec3_float_sliders("Camera Translation", shader.get_reference_vec3_uniform("camera_position"), vec2(-10000, 10000));
			ImGui::SeparatorText("Rotation");
			this->vec3_float_sliders("Camera Rotation", shader.get_reference_vec3_uniform("camera_rotation_vector"), vec2(-180, 180));

		};

		});

	this->windows["Transformations"].add_function([&]() {

		if (ImGui::CollapsingHeader("Mouse")) {

			ImGui::SeparatorText("Plotting");
			ImGui::Checkbox("Plot", &plot);

		};

		});

};

void UI::render() {

	for (auto iterator = this->windows.begin(); iterator != this->windows.end(); ++iterator) {

		iterator->second.update();

	};

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
	ImGui_ImplOpenGL3_Init("#version 440 core");
	ImGui::StyleColorsDark();

};