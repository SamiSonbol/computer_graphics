#include "Shader.h"

void Shader::bind_mesh_buffers(Mesh& mesh) {

	bind_buffer(GL_ARRAY_BUFFER, &this->positions_buffer, mesh.positions, GL_STATIC_DRAW, 0, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &this->normals_buffer, mesh.normals, GL_STATIC_DRAW, 1, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &this->colors_buffer, mesh.colors, GL_STATIC_DRAW, 2, 3, true);
	bind_buffer(GL_ELEMENT_ARRAY_BUFFER, &this->indices_buffer, mesh.indices, GL_STATIC_DRAW, -1, 1, true);
	bind_buffer(GL_ARRAY_BUFFER, &this->texture_coordinates_buffer, mesh.texture_coordinates, GL_STATIC_DRAW, 3, 2, true);
	bind_buffer(GL_ARRAY_BUFFER, &this->tangents_buffer, mesh.tangents, GL_STATIC_DRAW, 4, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &this->bitangents_buffer, mesh.bitangents, GL_STATIC_DRAW, 5, 3, true);

};

Shader::Shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader) {

	this->program = glCreateProgram();

	unsigned int v_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader);

	unsigned int g_shader = compile_shader(GL_GEOMETRY_SHADER, geometry_shader);

	unsigned int f_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(this->program, v_shader);
	glAttachShader(this->program, g_shader);
	glAttachShader(this->program, f_shader);

	glLinkProgram(this->program);
	glValidateProgram(this->program);

	glDeleteShader(v_shader);
	glDeleteShader(g_shader);
	glDeleteShader(f_shader);

};

Shader::Shader(unsigned int& compiled_vertex_shader, unsigned int& compiled_geometry_shader, unsigned int& compiled_fragment_shader) {

	this->program = glCreateProgram();

	glAttachShader(this->program, compiled_vertex_shader);
	glAttachShader(this->program, compiled_geometry_shader);
	glAttachShader(this->program, compiled_fragment_shader);

	glLinkProgram(this->program);
	glValidateProgram(this->program);

	glDeleteShader(compiled_vertex_shader);
	glDeleteShader(compiled_geometry_shader);
	glDeleteShader(compiled_fragment_shader);

};

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

	//VIMP: openGL has to be using glUseProgram(this->program); before this function is called. 

	unsigned int location = glGetUniformLocation(this->program, uniform_name);
	glUniform1i(location, index);

};

Shader::graphics_vectors_container Shader::create_standard_matrix_vectors() {

	vec3 right_vector(1, 0, 0);
	vec3 up_vector(0, 1, 0);
	vec3 direction_vector(0, 0, 1);
	vec3 camera_position(0, 0, -1);

	vec3 translation_vector(0, 0, 0);
	vec3 scaling_vector(1, 1, 1);
	vec3 rotation_vector(0, 0, 0);

	vec3 light_position(0, 0, -1);
	vec3 light_color(230, 80, 0);

	return { right_vector, up_vector, direction_vector, camera_position, translation_vector, scaling_vector, rotation_vector, light_position, light_color.normalize()};

};

void Shader::init_matrices(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	create_uniform_vec3(camera_position.to_GL(), "eye_vector");
	create_uniform_mat4(create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector).to_GL(), "model_transformation_matrix");
	create_uniform_mat4(create_view_matrix(right_vector, up_vector, direction_vector, camera_position).to_GL(), "view_matrix");
	create_uniform_mat4(create_frustum_projection_matrix(90.0f, screen_size.x, screen_size.y, 0.1f, 1000.0f).to_GL(), "projection_matrix");

};

void Shader::init_light(const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction) {

	float ambient = material_properties.x;
	float diffuse = material_properties.y;
	float specular = material_properties.z;
	float shininess = material_properties.w;

	create_uniform_vec3(light_position.to_GL(), "light_position");
	create_uniform_vec3(light_color.to_GL(), "light_color");

	create_uniform_float(ambient, "ambient");
	create_uniform_float(diffuse, "diffuse");
	create_uniform_float(specular, "specular");
	create_uniform_float(shininess, "shininess");

	create_uniform_bool(gamma_correction, "gamma_correction");

};

void Shader::add_texture(Texture& texture, const bool& gamma_correction) {

	bind_texture(&texture.texture_ID, texture.GL_TEXTUREindex, texture.bytes, texture.width, texture.height, texture.n_color_channels, gamma_correction);
	create_uniform_2D_texture(texture.index, texture.uniform_name);

};

void Shader::add_textures(std::vector<Texture>& textures, const bool& gamma_correction) {

	for (int i = 0; i < textures.size(); ++i) {

		add_texture(textures[i], gamma_correction);

	};

};

//sets the transformation matrices and light vectors
void Shader::initialize(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, std::vector<Texture>& textures) {

	init_matrices(screen_size, right_vector, up_vector, direction_vector, camera_position, translation_vector, scaling_vector, rotation_vector);
	init_light(light_position, light_color, material_properties, gamma_correction);
	add_textures(textures, gamma_correction);

};

//override
void Shader::initialize(const vec2& screen_size, const graphics_vectors_container& container, const vec4& material_properties, const bool& gamma_correction, std::vector<Texture>& textures) {

	init_matrices(screen_size, container.right_vector, container.up_vector, container.direction_vector, container.camera_position, container.translation_vector, container.scaling_vector, container.rotation_vector);
	init_light(container.light_position, container.light_color, material_properties, gamma_correction);
	add_textures(textures, gamma_correction);

};

void Shader::update_matrices(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	create_uniform_vec3(camera_position.to_GL(), "eye_vector");
	create_uniform_mat4(create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector).to_GL(), "model_transformation_matrix");
	create_uniform_mat4(create_view_matrix(right_vector, up_vector, direction_vector, camera_position).to_GL(), "view_matrix");

};

void Shader::update_light(const vec3& light_position, const vec4& material_properties) {

	float ambient = material_properties.x;
	float diffuse = material_properties.y;
	float specular = material_properties.z;
	float shininess = material_properties.w;

	create_uniform_vec3(light_position.to_GL(), "light_position");
	create_uniform_float(ambient, "ambient");
	create_uniform_float(diffuse, "diffuse");
	create_uniform_float(specular, "specular");
	create_uniform_float(shininess, "shininess");

};

void Shader::update(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties) {

	update_matrices(right_vector, up_vector, direction_vector, camera_position, translation_vector, scaling_vector, rotation_vector);
	update_light(light_position, material_properties);

};

//override
void Shader::update(const graphics_vectors_container& container, const vec4& material_properties) {

	update_matrices(container.right_vector, container.up_vector, container.direction_vector, container.camera_position, container.translation_vector, container.scaling_vector, container.rotation_vector);
	update_light(container.light_position, material_properties);

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

void Shader::delete_textures() {

	int length = this->textures.size();
	for (int i = 0; i < length; ++i) {

		glDeleteTextures(1, &this->textures[i]);

	};

};

void Shader::delete_program() {

	glDeleteProgram(this->program);

};

void Shader::delete_all() {

	delete_buffers();
	delete_textures();
	delete_program();

};