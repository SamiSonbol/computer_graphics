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

};

bool Mouse::check_for_mouse_click(GLFWwindow* window) {

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {

		return true;

	};

	return false;

};

vec2 Mouse::from_screen_to_NDC(const vec2& screen_position, const vec2& screen_size) {

	return vec2((screen_position.x / screen_size.x) * 2.0f - 1.0f, 1.0f - (screen_position.y / screen_size.y) * 2.0f);

};

vec3 Mouse::from_screen_to_model(const vec2& screen_position, const vec2& screen_size, const mat4& projection_matrix, const mat4& view_matrix, const mat4& model_matrix) {

	vec2 pos_ndc = this->from_screen_to_NDC(screen_position, screen_size);
	vec4 pos_world = (projection_matrix * view_matrix).inverse() * vec4(pos_ndc, -1, 1);
	pos_world /= pos_world.w;
	pos_world.w = 1;

	return (model_matrix.inverse() * pos_world).xyz();

};

vec2 Mouse::from_model_to_screen(const vec3& model_position, const vec2& screen_size, const mat4& projection_matrix, const mat4& view_matrix, const mat4& model_matrix) {

	vec4 pos_clip = projection_matrix * view_matrix * model_matrix * vec4(model_position, 1);
	pos_clip /= pos_clip.w;
	return vec2((pos_clip.x + 1.0) * 0.5 * screen_size.x, (1.0 - pos_clip.y) * 0.5 * screen_size.y);

};

//void Mouse::plot_point(const bool& plot, GLFWwindow* window, const Shader::graphics_vectors_container& vectors_container, const vec2& screen_size, Mesh& mesh, std::vector<vec3>& colors, std::vector<vec3>& positions) {
//
//	if (plot && check_for_mouse_click(window)) {
//
//		mat4 model_transformation_matrix = create_model_transformation_matrix(vectors_container.translation_vector, vectors_container.scaling_vector, vectors_container.rotation_vector);
//		mat4 view_matrix = create_view_matrix(vectors_container.camera_position, vec3(0,0,0), vectors_container.up_vector);
//		mat4 projection_matrix = create_frustum_projection_matrix(90.0f, screen_size, 0.1f, 1000.0f);
//		mat4 viewport_matrix = create_static_view_port_matrix(screen_size.x, screen_size.y);
//
//		float model_min_x = -mesh.mesh_dimensions.x / 2;
//		float model_max_x = mesh.mesh_dimensions.x / 2;
//		float model_min_y = -mesh.mesh_dimensions.y / 2;
//		float model_max_y = mesh.mesh_dimensions.y / 2;
//
//		for (const vec3& vertex : positions) {
//			vec3 transformed = (model_transformation_matrix * vec4(vertex, 1.0f)).xyz();
//
//			model_min_x = std::min(model_min_x, transformed.x);
//			model_max_x = std::max(model_max_x, transformed.x);
//			model_min_y = std::min(model_min_y, transformed.y);
//			model_max_y = std::max(model_max_y, transformed.y);
//		};
//		vec2 viewport_size = vec2(fabs(model_max_x - model_min_x),fabs(model_max_y - model_min_y));
//
//		vec2 pos(this->position_x, this->position_y);
//		vec3 pos_model = this->from_screen_to_model(pos, screen_size, projection_matrix, view_matrix, model_transformation_matrix);
//		//pos_model *= mesh.mesh_dimensions;
//		std::cout << "mouse position in Screen space: "; print_vec(pos);
//		std::cout << "mouse position in Model space: "; print_vec(pos_model);
//
//		vec3 top_left_pos(positions[positions.size() - 1]);
//		vec3 top_left_pos_screen = this->from_model_to_screen(top_left_pos, screen_size, projection_matrix, view_matrix, model_transformation_matrix);
//		std::cout << "top_left_pos position in Model space: "; print_vec(top_left_pos);
//		std::cout << "top_left_pos position in Screen space: "; print_vec(top_left_pos_screen);
//
//		vec2 uv = (pos_model.xy() - vec2(model_min_x, model_min_y)) / viewport_size;
//		//vec2 uv;
//		//uv.x = (pos_model.x + mesh.mesh_dimensions.x / 2) / mesh.mesh_dimensions.x;
//		//uv.y = (pos_model.y + mesh.mesh_dimensions.y / 2) / mesh.mesh_dimensions.y;
//		uv *= vec2(mesh.diffuse_map.width, mesh.diffuse_map.height);
//		std::cout << "UV: "; print_vec(uv);
//
//		//vec4 pos(this->position_x, this->position_y, 0);
//		///*pos = vec4((pos.x / screen_size.x) * 2.0f - 1.0f, 1.0f - (pos.y / screen_size.y) * 2.0f, 0, 1);
//		//pos = (projection_matrix * view_matrix).inverse() * pos;
//		//pos /= pos.w;
//	 //   pos = model_transformation_matrix.inverse() * pos;
//		//vec4 pos_model = model_transformation_matrix * pos;
//		//vec4 pos_world = projection_matrix * view_matrix * pos_model;
//		//pos_world /= pos_world.w;
//		//vec4 pos_ndc = viewport_matrix * pos_world;*/
//		///*std::cout << "mouse position: "; print_vec(vec2(this->position_x, this->position_y));
//		//std::cout << "position: "; print_vec(pos);
//		//std::cout << "position at Model space: "; print_vec(pos_model.xyz());
//		//std::cout << "position at World space: "; print_vec(pos_world.xyz());
//		//std::cout << "position at Screen: "; print_vec(pos_ndc.xyz());	*/
//		//vec2 tetxure_dimensions(mesh.diffuse_map.width, mesh.diffuse_map.height);
//		//mat3 screen_system(vec3(screen_size.x, 0, 0), vec3(0, screen_size.y, 0), vec3(0, 0, 1));
//		//mat3 model_system(vec3(mesh.mesh_dimensions.x, 0, 0), vec3(0, mesh.mesh_dimensions.y, 0), vec3(0, 0, 1));
//		//mat3 texture_system(vec3(tetxure_dimensions.x, 0, 0), vec3(0, tetxure_dimensions.y, 0), vec3(0, 0, 1));
//		////pos *= tetxure_dimensions;
//		//mat3 StM = model_system.inverse() * screen_system;
//		//mat3 MtT = texture_system.inverse() * StM;
//		//vec3 uv = MtT * pos.xyz();
//		//std::cout << "UV: "; print_vec(uv);
//		////uv /= tetxure_dimensions;
//		mesh.diffuse_map.set_pixel_color(uv.x, uv.y, vec4(255, 0, 0, 255));
//		mesh.diffuse_map.set_pixel_color(uv.x + 1, uv.y + 1, vec4(255, 0, 0, 255));
//		mesh.diffuse_map.set_pixel_color(uv.x - 1, uv.y - 1, vec4(255, 0, 0, 255));
//		mesh.diffuse_map.set_pixel_color(uv.x + 1, uv.y, vec4(255, 0, 0, 255));
//		mesh.diffuse_map.set_pixel_color(uv.x, uv.y + 1, vec4(255, 0, 0, 255));
//		mesh.diffuse_map.set_pixel_color(uv.x - 1, uv.y, vec4(255, 0, 0, 255));
//		mesh.diffuse_map.set_pixel_color(uv.x, uv.y - 1, vec4(255, 0, 0, 255));
//		//update_texture(&mesh.diffuse_map.texture_ID, mesh.diffuse_map.GL_TEXTUREindex, mesh.diffuse_map.bytes, mesh.diffuse_map.width, mesh.diffuse_map.height, mesh.diffuse_map.n_color_channels);
//
//
//		/*for (auto& p : positions) {
//
//			if (p.xy() == pos.xy()) {
//
//				std::cout << "FOUND: "; print_vec(pos); std::cout << "pos of mouse: "; print_vec(vec2(this->position_x, this->position_y));
//
//			};
//
//		};*/
//		
//		/*/auto key = std::make_pair(pos_model.xyz(), pos_model.xy() / mesh.mesh_dimensions);
//		auto iterator = mesh.vertices_map.find(key);
//		if (iterator != mesh.vertices_map.end()) {
//
//			std::cout << "Found\n";
//			colors[iterator->second] = vec3(255, 0, 0);
//			std::cout << "pos in container: "; print_vec(pos_model.xyz());
//
//		};*/
//
//	};
//
//};

void Mouse::update(GLFWwindow* window) {

	update_position(window);
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
			ImGui::Checkbox("Height Mapping", &shader.get_reference_bool_uniform("displacement_mapping"));
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
	ImGui_ImplOpenGL3_Init("#version 440 core");
	ImGui::StyleColorsDark();

};