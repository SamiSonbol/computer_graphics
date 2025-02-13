#include "computer_graphics/Shader.h"

void Shader::create_uniform_mat4(const std::vector<float>& data_vector, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniformMatrix4fv(location, 1, GL_FALSE, data_vector.data());

};

void Shader::create_uniform_vec3(const std::vector<float>& data_vector, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform3fv(location, 1, data_vector.data());

};

void Shader::create_uniform_float(const float& data_variable, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1f(location, data_variable);

};

void Shader::create_uniform_int(const int& data_variable, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1i(location, data_variable);

};

void Shader::create_uniform_bool(const bool& boolean, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1i(location, boolean);

};

void Shader::create_uniform_2D_texture(const int& index, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1i(location, index);

};

Shader::graphics_vectors_container Shader::create_standard_shader_vectors() {

	vec3 right_vector(1, 0, 0);
	vec3 up_vector(0, 1, 0);
	vec3 direction_vector(0, 0, 1);
	vec3 camera_position(0, 0, -1);

	vec3 translation_vector(0, 0, 0);
	vec3 scaling_vector(1, 1, 1);
	vec3 rotation_vector(0, 0, 0);

	vec3 light_position(0, 0, -1);
	vec3 light_color(255, 0, 0);
	vec4 material_properties(1.0f, 0.7f, 0.1f, 10);

	return { right_vector, up_vector, direction_vector, camera_position, translation_vector, scaling_vector, rotation_vector, light_position, light_color, material_properties};

};

Shader::graphics_booleans_container Shader::create_standard_shader_booleans() {

	bool gamma_correction = true;
	bool texturing = true;
	bool normal_mapping = false;
	bool displacement_mapping = false;

	return { gamma_correction, texturing, normal_mapping, displacement_mapping };

};

Shader::graphics_floats_container Shader::create_standard_shader_floats() {

	float tesselation = 2.0f;

	float displacement_scale = 0.1f;

	return { tesselation, displacement_scale };

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

void Shader::init_matrices(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	this->create_uniform_vec3(camera_position.to_GL(), "eye_vector");
	this->create_uniform_mat4(create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector).to_GL(), "model_transformation_matrix");
	this->create_uniform_mat4(create_view_matrix(right_vector, up_vector, direction_vector, camera_position).to_GL(), "view_matrix");
	this->create_uniform_mat4(create_frustum_projection_matrix(90.0f, screen_size.x, screen_size.y, 0.1f, 1000.0f).to_GL(), "projection_matrix");

};

void Shader::init_light(const vec3& light_position, const vec3& light_color, const vec4& material_properties) {

	float ambient = material_properties.x;
	float diffuse = material_properties.y;
	float specular = material_properties.z;
	float shininess = material_properties.w;

	this->create_uniform_vec3(light_position.to_GL(), "light_position");
	this->create_uniform_vec3(light_color.to_GL(), "light_color");

	this->create_uniform_float(ambient, "ambient");
	this->create_uniform_float(diffuse, "diffuse");
	this->create_uniform_float(specular, "specular");
	this->create_uniform_float(shininess, "shininess");

};

void Shader::init_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& displacement_mapping) {

	this->create_uniform_bool(gamma_correction, "gamma_correction");
	this->create_uniform_bool(texturing, "texturing");
	this->create_uniform_bool(normal_mapping, "normal_mapping");
	this->create_uniform_bool(displacement_mapping, "displacement_mapping");

};

void Shader::init_floats(const float& tesselation_multiplier, const float& displacement_scale) {

	this->create_uniform_float(tesselation_multiplier, "tesselation_multiplier");
	this->create_uniform_float(displacement_scale, "displacement_scale");

};

//override
void Shader::initialize(const vec2& screen_size, const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container) {

	this->init_matrices(screen_size, vectors_container.right_vector, vectors_container.up_vector, vectors_container.direction_vector, vectors_container.camera_position, vectors_container.translation_vector, vectors_container.scaling_vector, vectors_container.rotation_vector);
	this->init_light(vectors_container.light_position, vectors_container.light_color, vectors_container.material_properties);
	this->init_booleans(booleans_container.gamma_correction, booleans_container.texturing, booleans_container.normal_mapping, booleans_container.displacement_mapping);
	this->init_floats(floats_container.tesselation_multiplier, floats_container.displacement_scale);

};

void Shader::update_matrices(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	this->create_uniform_vec3(camera_position.to_GL(), "eye_vector");
	this->create_uniform_mat4(create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector).to_GL(), "model_transformation_matrix");
	this->create_uniform_mat4(create_view_matrix(right_vector, up_vector, direction_vector, camera_position).to_GL(), "view_matrix");

};

void Shader::update(const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container) {

	this->update_matrices(vectors_container.right_vector, vectors_container.up_vector, vectors_container.direction_vector, vectors_container.camera_position, vectors_container.translation_vector, vectors_container.scaling_vector, vectors_container.rotation_vector);
	this->init_light(vectors_container.light_position, vectors_container.light_color, vectors_container.material_properties);
	this->init_booleans(booleans_container.gamma_correction, booleans_container.texturing, booleans_container.normal_mapping, booleans_container.displacement_mapping);
	this->init_floats(floats_container.tesselation_multiplier, floats_container.displacement_scale);

};

void Shader::bind_mesh_buffers(Mesh& mesh, const bool& gamma_correction) {

	this->bind_buffer(mesh.generate_buffers_and_textures, GL_ARRAY_BUFFER, &this->positions_buffer, mesh.positions, GL_STATIC_DRAW, 0, 3);
	this->bind_buffer(mesh.generate_buffers_and_textures, GL_ARRAY_BUFFER, &this->normals_buffer, mesh.normals, GL_STATIC_DRAW, 1, 3);
	this->bind_buffer(mesh.generate_buffers_and_textures, GL_ARRAY_BUFFER, &this->colors_buffer, mesh.colors, GL_STATIC_DRAW, 2, 3);
	this->bind_buffer(mesh.generate_buffers_and_textures, GL_ELEMENT_ARRAY_BUFFER, &this->indices_buffer, mesh.indices, GL_STATIC_DRAW, -1, 1);
	this->bind_buffer(mesh.generate_buffers_and_textures, GL_ARRAY_BUFFER, &this->texture_coordinates_buffer, mesh.texture_coordinates, GL_STATIC_DRAW, 3, 2);
	this->bind_buffer(mesh.generate_buffers_and_textures, GL_ARRAY_BUFFER, &this->tangents_buffer, mesh.tangents, GL_STATIC_DRAW, 4, 3);
	this->bind_buffer(mesh.generate_buffers_and_textures, GL_ARRAY_BUFFER, &this->bitangents_buffer, mesh.bitangents, GL_STATIC_DRAW, 5, 3);

	this->bind_texture(mesh.generate_buffers_and_textures, &mesh.diffuse_map.texture_ID, mesh.diffuse_map.GL_TEXTUREindex, mesh.diffuse_map.bytes, mesh.diffuse_map.width, mesh.diffuse_map.height, mesh.diffuse_map.n_color_channels, gamma_correction);
	this->bind_texture(mesh.generate_buffers_and_textures, &mesh.normal_map.texture_ID, mesh.normal_map.GL_TEXTUREindex, mesh.normal_map.bytes, mesh.normal_map.width, mesh.normal_map.height, mesh.normal_map.n_color_channels, gamma_correction);
	this->bind_texture(mesh.generate_buffers_and_textures, &mesh.displacement_map.texture_ID, mesh.displacement_map.GL_TEXTUREindex, mesh.displacement_map.bytes, mesh.displacement_map.width, mesh.displacement_map.height, mesh.displacement_map.n_color_channels, gamma_correction);

	if (mesh.generate_buffers_and_textures) {

		this->create_uniform_2D_texture(mesh.diffuse_map.index, mesh.diffuse_map.uniform_name);
		this->create_uniform_2D_texture(mesh.normal_map.index, mesh.normal_map.uniform_name);
		this->create_uniform_2D_texture(mesh.displacement_map.index, mesh.displacement_map.uniform_name);

	};
	mesh.generate_buffers_and_textures = false;

};

void Shader::bind_and_draw_mesh_elements(Mesh& mesh, const bool& gamma_correction, const unsigned int& GL_PRIMITIVE_TYPE) {

	this->bind_mesh_buffers(mesh, gamma_correction);
	glDrawElements(GL_PRIMITIVE_TYPE, mesh.indices.size(), GL_UNSIGNED_SHORT, 0);

};

void Shader::delete_buffers() {

	glDeleteBuffers(1, &this->positions_buffer);
	glDeleteBuffers(1, &this->normals_buffer);
	glDeleteBuffers(1, &this->indices_buffer);
	glDeleteBuffers(1, &this->texture_coordinates_buffer);
	glDeleteBuffers(1, &this->colors_buffer);
	glDeleteBuffers(1, &this->tangents_buffer);
	glDeleteBuffers(1, &this->bitangents_buffer);

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