#include "computer_graphics/Shader.h"

void Shader::create_uniform_bool(const bool& boolean, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1i(location, boolean);

};
void Shader::create_uniform_int(const int& data_variable, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1i(location, data_variable);

};
void Shader::create_uniform_float(const float& data_variable, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1f(location, data_variable);

};
void Shader::create_uniform_vec2(const std::vector<float>& data_vector, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform2fv(location, 1, data_vector.data());

};
void Shader::create_uniform_vec3(const std::vector<float>& data_vector, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform3fv(location, 1, data_vector.data());

};
void Shader::create_uniform_mat4(const std::vector<float>& data_vector, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniformMatrix4fv(location, 1, GL_FALSE, data_vector.data());

};
void Shader::create_uniform_2D_texture(const int& index, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1i(location, index);

};

void Shader::upload_bool_uniforms() {

	if (!this->bool_uniforms_map.empty()) {

		for (auto iterator = this->bool_uniforms_map.begin(); iterator != this->bool_uniforms_map.end(); iterator++) {

			this->create_uniform_bool(iterator->second, iterator->first.c_str());

		};

	};

};
void Shader::upload_int_uniforms() {

	if (!this->int_uniforms_map.empty()) {

		for (auto iterator = this->int_uniforms_map.begin(); iterator != this->int_uniforms_map.end(); iterator++) {

			this->create_uniform_int(iterator->second, iterator->first.c_str());

		};

	};

};
void Shader::upload_float_uniforms() {

	if (!this->float_uniforms_map.empty()) {

		for (auto iterator = this->float_uniforms_map.begin(); iterator != this->float_uniforms_map.end(); iterator++) {

			this->create_uniform_float(iterator->second, iterator->first.c_str());

		};

	};

};
void Shader::upload_vec2_uniforms() {

	if (!this->vec2_uniforms_map.empty()) {

		for (auto iterator = this->vec2_uniforms_map.begin(); iterator != this->vec2_uniforms_map.end(); iterator++) {

			this->create_uniform_vec2(iterator->second.to_GL(), iterator->first.c_str());

		};

	};

};
void Shader::upload_vec3_uniforms() {

	if (!this->vec3_uniforms_map.empty()) {

		for (auto iterator = this->vec3_uniforms_map.begin(); iterator != this->vec3_uniforms_map.end(); iterator++) {

			this->create_uniform_vec3(iterator->second.to_GL(), iterator->first.c_str());

		};

	};

};
void Shader::upload_mat4_uniforms() {

	if (!this->mat4_uniforms_map.empty()) {

		for (auto iterator = this->mat4_uniforms_map.begin(); iterator != this->mat4_uniforms_map.end(); iterator++) {

			this->create_uniform_mat4(iterator->second.to_GL(), iterator->first.c_str());

		};

	};

};

bool& Shader::get_reference_bool_uniform(const std::string& uniform_name) {

	auto iterator = this->bool_uniforms_map.find(uniform_name);
	if (iterator != this->bool_uniforms_map.end()) { return iterator->second; }
	else { std::cerr << "ERROR: couldnt find " << uniform_name << " inside bool uniforms map!\n"; exit(EXIT_FAILURE); };

};
int& Shader::get_reference_int_uniform(const std::string& uniform_name) {

	auto iterator = this->int_uniforms_map.find(uniform_name);
	if (iterator != this->int_uniforms_map.end()) { return iterator->second; }
	else { std::cerr << "ERROR: couldnt find " << uniform_name << " inside int uniforms map !\n"; exit(EXIT_FAILURE); };

};
float& Shader::get_reference_float_uniform(const std::string& uniform_name) {

	auto iterator = this->float_uniforms_map.find(uniform_name);
	if (iterator != this->float_uniforms_map.end()) { return iterator->second; }
	else { std::cerr << "ERROR: couldnt find " << uniform_name << " inside float uniforms map!\n"; exit(EXIT_FAILURE); };

};
vec2& Shader::get_reference_vec2_uniform(const std::string& uniform_name) {

	auto iterator = this->vec2_uniforms_map.find(uniform_name);
	if (iterator != this->vec2_uniforms_map.end()) { return iterator->second; }
	else { std::cerr << "ERROR: couldnt find " << uniform_name << " inside vec2 uniforms map!\n"; exit(EXIT_FAILURE); };

};
vec3& Shader::get_reference_vec3_uniform(const std::string& uniform_name) {

	auto iterator = this->vec3_uniforms_map.find(uniform_name);
	if (iterator != this->vec3_uniforms_map.end()) { return iterator->second; }
	else { std::cerr << "ERROR: couldnt find " << uniform_name << " inside vec3 uniforms map!\n"; exit(EXIT_FAILURE); };

};
mat4& Shader::get_reference_mat4_uniform(const std::string& uniform_name) {

	auto iterator = this->mat4_uniforms_map.find(uniform_name);
	if (iterator != this->mat4_uniforms_map.end()) { return iterator->second; }
	else { std::cerr << "ERROR: couldnt find " << uniform_name << " inside mat4 uniforms map!\n"; exit(EXIT_FAILURE); };

};

void Shader::set_value_bool_uniform(const std::string& uniform_name, const bool& value) {

	get_reference_bool_uniform(uniform_name) = value;

};
void Shader::set_value_int_uniform(const std::string& uniform_name, const int& value) {

	get_reference_int_uniform(uniform_name) = value;

};
void Shader::set_value_float_uniform(const std::string& uniform_name, const float& value) {

	get_reference_float_uniform(uniform_name) = value;

};
void Shader::set_value_vec2_uniform(const std::string& uniform_name, const vec2& value) {

	get_reference_vec2_uniform(uniform_name) = value;

};
void Shader::set_value_vec3_uniform(const std::string& uniform_name, const vec3& value) {

	get_reference_vec3_uniform(uniform_name) = value;

};
void Shader::set_value_mat4_uniform(const std::string& uniform_name, const mat4& value) {

	get_reference_mat4_uniform(uniform_name) = value;

};

void Shader::default_uniforms_maps_initialization(const vec2& screen_size) {

	this->bool_uniforms_map["orthogonal_projection"] = false;
	this->bool_uniforms_map["gamma_correction"] = true;
	this->bool_uniforms_map["texturing"] = true;
	this->bool_uniforms_map["normal_mapping"] = false;
	this->bool_uniforms_map["displacement_mapping"] = false;
	this->bool_uniforms_map["height_coloring"] = false;

	//camera vectors
	this->vec3_uniforms_map["forward_vector"] = vec3(0.0f, 0.0f, 1.0f);
	this->vec3_uniforms_map["up_vector"] = vec3(0.0f, 1.0f, 0.0f);
	this->vec3_uniforms_map["camera_position"] = vec3(0.0f, 0.0f, -1.0f);
	this->vec3_uniforms_map["camera_rotation_vector"] = vec3(0.0f, 0.0f, 0.0f);

	//model trasnformation vectors
	this->vec3_uniforms_map["model_translation_vector"] = vec3(0.0f, 0.0f, 0.0f);
	this->vec3_uniforms_map["model_scaling_vector"] = vec3(1.0f, 1.0f, 1.0f);
	this->vec3_uniforms_map["model_rotation_vector"] = vec3(0.0f, 0.0f, 0.0f);

	//light vectors
	this->vec3_uniforms_map["light_position"] = vec3(0.0f, 0.0f, -1.0f);
	this->vec3_uniforms_map["light_color"] = vec3(255.0f, 0.0f, 0.0f);

	this->float_uniforms_map["orthogonal_size"] = 10.0f;
	this->float_uniforms_map["FOV"] = 90.0f;
	this->float_uniforms_map["tesselation_multiplier"] = 2.0f;
	this->float_uniforms_map["displacement_scale"] = 0.1f;
	this->float_uniforms_map["point_size"] = 1.0f;

	this->float_uniforms_map["ambient"] = 1.0f;
	this->float_uniforms_map["diffuse"] = 0.7f;
	this->float_uniforms_map["specular"] = 0.1f;
	this->float_uniforms_map["shininess"] = 10.0f;

	this->vec2_uniforms_map["screen_size"] = screen_size;
	this->vec3_uniforms_map["mouse_ray_vector"] = vec3(-0.0001, -0.0001, -0.0001);

};

void Shader::update_uniforms() {

	upload_bool_uniforms();
	upload_int_uniforms();
	upload_float_uniforms();
	upload_vec2_uniforms();
	upload_vec3_uniforms();
	upload_mat4_uniforms();

};

void Shader::bind_texture(const bool& generate_texture, unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels, const bool& gamma_correction) {

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

void Shader::update_texture(unsigned int* texture_ID, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels) {

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

void Shader::bind_mesh_buffers_and_textures(Mesh& mesh, const vec2& screen_size, const unsigned int& GL_DRAW_TYPE, const bool& gamma_correction) {

	this->bind_array_buffer(mesh.generate_buffers_and_textures, &this->positions_buffer, mesh.positions, GL_DRAW_TYPE, 0, 3);
	if (!mesh.indices.empty()) { this->bind_index_buffer(mesh.generate_buffers_and_textures, &this->indices_buffer, mesh.indices, GL_DRAW_TYPE); };
	if (!mesh.normals.empty()) { this->bind_array_buffer(mesh.generate_buffers_and_textures, &this->normals_buffer, mesh.normals, GL_DRAW_TYPE, 1, 3); };
	if (!mesh.tangents.empty()) { this->bind_array_buffer(mesh.generate_buffers_and_textures, &this->tangents_buffer, mesh.tangents, GL_DRAW_TYPE, 2, 3); };
	if (!mesh.bitangents.empty()) { this->bind_array_buffer(mesh.generate_buffers_and_textures, &this->bitangents_buffer, mesh.bitangents, GL_DRAW_TYPE, 3, 3); };
	if (!mesh.texture_coordinates.empty()) { this->bind_array_buffer(mesh.generate_buffers_and_textures, &this->texture_coordinates_buffer, mesh.texture_coordinates, GL_DRAW_TYPE, 4, 2); };
	if (!mesh.colors.empty()) { this->bind_array_buffer(mesh.generate_buffers_and_textures, &this->colors_buffer, mesh.colors, GL_DRAW_TYPE, 5, 3); };

	if (mesh.diffuse_map.bytes != NULL) {

		this->bind_texture(mesh.generate_buffers_and_textures, &mesh.diffuse_map.texture_ID, mesh.diffuse_map.GL_TEXTUREindex, mesh.diffuse_map.bytes, mesh.diffuse_map.width, mesh.diffuse_map.height, mesh.diffuse_map.n_color_channels, gamma_correction);
		if (mesh.generate_buffers_and_textures) { this->create_uniform_2D_texture(mesh.diffuse_map.index, mesh.diffuse_map.uniform_name); };

	};
	if (mesh.normal_map.bytes != NULL) { 

		this->bind_texture(mesh.generate_buffers_and_textures, &mesh.normal_map.texture_ID, mesh.normal_map.GL_TEXTUREindex, mesh.normal_map.bytes, mesh.normal_map.width, mesh.normal_map.height, mesh.normal_map.n_color_channels, gamma_correction); 
		if (mesh.generate_buffers_and_textures) { this->create_uniform_2D_texture(mesh.normal_map.index, mesh.normal_map.uniform_name); };

	};
	if (mesh.displacement_map.bytes != NULL) { 

		this->bind_texture(mesh.generate_buffers_and_textures, &mesh.displacement_map.texture_ID, mesh.displacement_map.GL_TEXTUREindex, mesh.displacement_map.bytes, mesh.displacement_map.width, mesh.displacement_map.height, mesh.displacement_map.n_color_channels, gamma_correction);
		if (mesh.generate_buffers_and_textures) { this->create_uniform_2D_texture(mesh.displacement_map.index, mesh.displacement_map.uniform_name); mesh.generate_buffers_and_textures = false; };

	};

};

void Shader::draw_mesh_elements(Mesh& mesh, const unsigned int& GL_PRIMITIVE_TYPE) {

	if (mesh.draw_as_elements) {

		glDrawElements(GL_PRIMITIVE_TYPE, mesh.indices.size(), GL_UNSIGNED_INT, 0);

	}
	else {

		glDrawArrays(GL_PRIMITIVE_TYPE, 0, mesh.positions.size());

	};

};

void Shader::bind_and_draw_mesh_elements(Mesh& mesh, const vec2& screen_size, const unsigned int& GL_PRIMITIVE_TYPE, const unsigned int& GL_DRAW_MODE, const bool& gamma_correction) {

	this->bind_mesh_buffers_and_textures(mesh, screen_size, GL_DRAW_MODE, gamma_correction);
	this->draw_mesh_elements(mesh, GL_PRIMITIVE_TYPE);

};

void Shader::delete_buffers() {

	glDeleteBuffers(1, &this->positions_buffer);
	glDeleteBuffers(1, &this->normals_buffer);
	glDeleteBuffers(1, &this->indices_buffer);
	glDeleteBuffers(1, &this->texture_coordinates_buffer);
	glDeleteBuffers(1, &this->colors_buffer);
	glDeleteBuffers(1, &this->tangents_buffer);
	glDeleteBuffers(1, &this->bitangents_buffer);

	glDeleteFramebuffers(1, &this->frame_buffer);
	glDeleteTextures(1, &this->frame_buffer_colors_texture_ID);
	glDeleteTextures(1, &this->frame_buffer_positions_texture_ID);
	glDeleteTextures(1, &this->frame_buffer_depth_texture_ID);

};

void Shader::delete_program() {

	glDeleteProgram(this->program);

};

void Shader::delete_all() {

	this->delete_buffers();
	this->delete_program();

};

unsigned int Shader::compile_shader(const unsigned int& type, const std::string& source) {

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

Shader::Shader(std::vector<unsigned int>& compiled_shaders_ids) {

	this->program = glCreateProgram();

	for (auto& compiled_shader_id : compiled_shaders_ids) {

		glAttachShader(this->program, compiled_shader_id);

	};

	glLinkProgram(this->program);
	glValidateProgram(this->program);

	for (auto& compiled_shader_id : compiled_shaders_ids) {

		glDeleteShader(compiled_shader_id);

	};

	compiled_shaders_ids.clear();

};

Shader::Shader(const std::string& shader_directory) {

	//checking if the directory is correct
	if (!std::filesystem::exists(shader_directory)) {

		std::cerr << "ERROR: shader directory doesnt exist!\n";
		exit(EXIT_FAILURE);

	};
	if (!std::filesystem::is_directory(shader_directory)) {

		std::cerr << "ERROR: path to shader location isn't a directory!\n";
		exit(EXIT_FAILURE);

	};

	//extracting the data from the shader files and compiling them then storing them in a vector next to their own shader type. Will be used to deduct correct shader order
	std::vector<std::pair<unsigned int, unsigned int>> shaders;//holds the shader data and its type
	std::filesystem::path directory(shader_directory);
	for (const auto& file : std::filesystem::directory_iterator(directory)) {

		if (file.is_regular_file()) {

			std::string shader_data = read_file(file.path().string());
			std::string shader_type = file.path().extension().string();
			if (shader_type == ".vert") {

				shaders.emplace_back(this->compile_shader(GL_VERTEX_SHADER, shader_data), GL_VERTEX_SHADER);

			}
			else if (shader_type == ".tesc") {

				shaders.emplace_back(this->compile_shader(GL_TESS_CONTROL_SHADER, shader_data), GL_TESS_CONTROL_SHADER);

			}
			else if (shader_type == ".tese") {

				shaders.emplace_back(this->compile_shader(GL_TESS_EVALUATION_SHADER, shader_data), GL_TESS_EVALUATION_SHADER);

			}
			else if (shader_type == ".geom") {

				shaders.emplace_back(this->compile_shader(GL_GEOMETRY_SHADER, shader_data), GL_GEOMETRY_SHADER);

			}
			else if (shader_type == ".comp") {

				shaders.emplace_back(this->compile_shader(GL_COMPUTE_SHADER, shader_data), GL_COMPUTE_SHADER);

			}
			else if (shader_type == ".frag") {

				shaders.emplace_back(this->compile_shader(GL_FRAGMENT_SHADER, shader_data), GL_FRAGMENT_SHADER);

			}
			else {

				std::cerr << "ERROR: unknown shader type: " << shader_type << ". Supported types are: .vert, .tesc, .tese, .geom, .comp and .frag\n";
				exit(EXIT_FAILURE);

			};

		};

	};

	//initializing the order sequence of our shaders based off of the number of shaders that exist
	int n_shaders = shaders.size();
	std::vector<unsigned int> ordered_shaders;
	std::vector<unsigned int> order;
	if (n_shaders == 2) {

		order = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };

	}
	else if (n_shaders == 3) {

		order = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };

	}
	else if (n_shaders == 4) {

		order = { GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER };

	}
	else if (n_shaders == 5) {

		order = { GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };

	};//compute shader still not implemented

	//cross comparing the current shader type from our *shaders* vector with the current shader type found in our order sequence. If we find a hit then the shader is added to our *ordered_shaders* vector
	for (auto& shader : shaders) {

		for (auto& shader_type : order) {

			if (shader.second == shader_type) {

				ordered_shaders.emplace_back(shader.first);
				break;

			};

		};

	};
	
	//using the *ordered_shaders* vector to create our *Shader* object using the same logic found in the first *Shader* constructor
	this->program = glCreateProgram();
	for (auto& compiled_shader_id : ordered_shaders) {

		glAttachShader(this->program, compiled_shader_id);

	};

	glLinkProgram(this->program);
	glValidateProgram(this->program);

	for (auto& compiled_shader_id : ordered_shaders) {

		glDeleteShader(compiled_shader_id);

	};
	ordered_shaders.clear();

};