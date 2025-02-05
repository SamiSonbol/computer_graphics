#include "computer_graphics/Shader.h"

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

Shader::graphics_vectors_container Shader::create_standard_shader_vectors() {

	vec3 right_vector(1, 0, 0);
	vec3 up_vector(0, 1, 0);
	vec3 direction_vector(0, 0, 1);
	vec3 camera_position(0, 0, -1);

	vec3 translation_vector(0, 0, 0);
	vec3 scaling_vector(1, 1, 1);
	vec3 rotation_vector(0, 0, 0);

	vec3 light_position(0, 0, -1);
	vec3 light_color(230, 80, 0);
	vec4 material_properties(1.0f, 0.7f, 0.1f, 10);

	return { right_vector, up_vector, direction_vector, camera_position, translation_vector, scaling_vector, rotation_vector, light_position, light_color, material_properties};

};

Shader::graphics_booleans_container Shader::create_standard_shader_booleans() {

	bool gamma_correction = true;
	bool texturing = true;
	bool normal_mapping = true;
	bool height_mapping = true;

	return { gamma_correction, texturing, normal_mapping, height_mapping };

};

Shader::graphics_floats_container Shader::create_standard_shader_floats() {

	float tesselation = 1.0f;

	float displacement_scale = 1.0f;

	return { tesselation, displacement_scale };

};

void Shader::init_matrices(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	create_uniform_vec3(camera_position.to_GL(), "eye_vector");
	create_uniform_mat4(create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector).to_GL(), "model_transformation_matrix");
	create_uniform_mat4(create_view_matrix(right_vector.normalize(), up_vector.normalize(), direction_vector.normalize(), camera_position).to_GL(), "view_matrix");
	create_uniform_mat4(create_frustum_projection_matrix(90.0f, screen_size.x, screen_size.y, 0.1f, 1000.0f).to_GL(), "projection_matrix");

};

void Shader::init_light(const vec3& light_position, const vec3& light_color, const vec4& material_properties) {

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

};

void Shader::init_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping) {

	create_uniform_bool(gamma_correction, "gamma_correction");
	create_uniform_bool(texturing, "texturing");
	create_uniform_bool(normal_mapping, "normal_mapping");
	create_uniform_bool(height_mapping, "height_mapping");

};

void Shader::init_floats(const float& tesselation_multiplier, const float& displacement_scale) {

	create_uniform_float(tesselation_multiplier, "tesselation_multiplier");
	create_uniform_float(displacement_scale, "displacement_scale");

};

//sets the transformation matrices and light vectors
void Shader::initialize(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping, const float& tesselation_multiplier, const float& displacement_scale) {

	init_matrices(screen_size, right_vector, up_vector, direction_vector, camera_position, translation_vector, scaling_vector, rotation_vector);
	init_light(light_position, light_color, material_properties);
	init_booleans(gamma_correction, texturing, normal_mapping, height_mapping);
	init_floats(tesselation_multiplier, displacement_scale);

};

//override
void Shader::initialize(const vec2& screen_size, const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container) {

	init_matrices(screen_size, vectors_container.right_vector, vectors_container.up_vector, vectors_container.direction_vector, vectors_container.camera_position, vectors_container.translation_vector, vectors_container.scaling_vector, vectors_container.rotation_vector);
	init_light(vectors_container.light_position, vectors_container.light_color, vectors_container.material_properties);
	init_booleans(booleans_container.gamma_correction, booleans_container.texturing, booleans_container.normal_mapping, booleans_container.height_mapping);
	init_floats(floats_container.tesselation_multiplier, floats_container.displacement_scale);

};

void Shader::update_matrices(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	create_uniform_vec3(camera_position.to_GL(), "eye_vector");
	create_uniform_mat4(create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector).to_GL(), "model_transformation_matrix");
	create_uniform_mat4(create_view_matrix(right_vector.normalize(), up_vector.normalize(), direction_vector.normalize(), camera_position).to_GL(), "view_matrix");

};

void Shader::update_light(const vec3& light_position, const vec3& light_color, const vec4& material_properties) {

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

};

void Shader::update_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping) {

	create_uniform_bool(gamma_correction, "gamma_correction");
	create_uniform_bool(texturing, "texturing");
	create_uniform_bool(normal_mapping, "normal_mapping");
	create_uniform_bool(height_mapping, "height_mapping");

};

void Shader::update_floats(const float& tesselation_multiplier, const float& displacement_scale) {

	create_uniform_float(tesselation_multiplier, "tesselation_multiplier");
	create_uniform_float(displacement_scale, "displacement_scale");

};

void Shader::update(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping, const float& tesselation_multiplier, const float& displacement_scale) {

	update_matrices(right_vector, up_vector, direction_vector, camera_position, translation_vector, scaling_vector, rotation_vector);
	update_light(light_position, light_color, material_properties);
	update_booleans(gamma_correction, texturing, normal_mapping, height_mapping);
	update_floats(tesselation_multiplier, displacement_scale);

};

//override
void Shader::update(const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container) {

	update_matrices(vectors_container.right_vector, vectors_container.up_vector, vectors_container.direction_vector, vectors_container.camera_position, vectors_container.translation_vector, vectors_container.scaling_vector, vectors_container.rotation_vector);
	update_light(vectors_container.light_position, vectors_container.light_color, vectors_container.material_properties);
	update_booleans(booleans_container.gamma_correction, booleans_container.texturing, booleans_container.normal_mapping, booleans_container.height_mapping);
	update_floats(floats_container.tesselation_multiplier, floats_container.displacement_scale);

};

void Shader::bind_mesh_buffers(Mesh& mesh, const bool& gamma_correction) {

	bind_buffer(GL_ARRAY_BUFFER, mesh.generate_buffers_and_textures, &this->positions_buffer, mesh.positions, GL_STATIC_DRAW, 0, 3);
	bind_buffer(GL_ARRAY_BUFFER, mesh.generate_buffers_and_textures, &this->normals_buffer, mesh.normals, GL_STATIC_DRAW, 1, 3);
	bind_buffer(GL_ARRAY_BUFFER, mesh.generate_buffers_and_textures, &this->colors_buffer, mesh.colors, GL_STATIC_DRAW, 2, 3);
	bind_buffer(GL_ELEMENT_ARRAY_BUFFER, mesh.generate_buffers_and_textures, &this->indices_buffer, mesh.indices, GL_STATIC_DRAW, -1, 1);
	bind_buffer(GL_ARRAY_BUFFER, mesh.generate_buffers_and_textures, &this->texture_coordinates_buffer, mesh.texture_coordinates, GL_STATIC_DRAW, 3, 2);
	bind_buffer(GL_ARRAY_BUFFER, mesh.generate_buffers_and_textures, &this->tangents_buffer, mesh.tangents, GL_STATIC_DRAW, 4, 3);
	bind_buffer(GL_ARRAY_BUFFER, mesh.generate_buffers_and_textures, &this->bitangents_buffer, mesh.bitangents, GL_STATIC_DRAW, 5, 3);

	bind_texture(&mesh.diffuse_map.texture_ID, mesh.diffuse_map.GL_TEXTUREindex, mesh.generate_buffers_and_textures, mesh.diffuse_map.bytes, mesh.diffuse_map.width, mesh.diffuse_map.height, mesh.diffuse_map.n_color_channels, gamma_correction);
	bind_texture(&mesh.normal_map.texture_ID, mesh.normal_map.GL_TEXTUREindex, mesh.generate_buffers_and_textures, mesh.normal_map.bytes, mesh.normal_map.width, mesh.normal_map.height, mesh.normal_map.n_color_channels, gamma_correction);
	bind_texture(&mesh.displacement_map.texture_ID, mesh.displacement_map.GL_TEXTUREindex, mesh.generate_buffers_and_textures, mesh.displacement_map.bytes, mesh.displacement_map.width, mesh.displacement_map.height, mesh.displacement_map.n_color_channels, gamma_correction);

	mesh.generate_buffers_and_textures = false;

};

void Shader::draw_mesh_elements(Mesh& mesh, const bool& gamma_correction, const unsigned int& GL_PRIMITIVE_TYPE) {

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

	delete_buffers();
	delete_program();

};