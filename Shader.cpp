#include "Shader.h"

unsigned int Shader::compile_shader(unsigned int type, const std::string & source) {

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
			<< (type == GL_VERTEX_SHADER ? "vertex"
				: type == GL_FRAGMENT_SHADER ? "fragment"
				: "geometry")
			<< " shader." << std::endl;

		std::cout << message << std::endl;

		glDeleteShader(id);

		return -1;

	};

	return id;

};

unsigned int Shader::create_shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader) {

	unsigned int program = glCreateProgram();

	unsigned int v_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader);

	unsigned int g_shader = compile_shader(GL_GEOMETRY_SHADER, geometry_shader);

	unsigned int f_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, v_shader);
	glAttachShader(program, g_shader);
	glAttachShader(program, f_shader);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(v_shader);
	glDeleteShader(g_shader);
	glDeleteShader(f_shader);

	return program;

};

Shader::Shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader) :

	program(create_shader(vertex_shader, geometry_shader, fragment_shader)) {

};

void Shader::create_uniform_mat4(const std::vector<float>& data_vector, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);

	glUniformMatrix4fv(location, 1, GL_FALSE, data_vector.data());

};

void Shader::create_uniform_vec3(const std::vector<float>& data_vector, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);

	glUniform3fv(location, 1, data_vector.data());

};

void Shader::create_uniform_float(const float data_variable, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);

	glUniform1f(location, data_variable);

};

void Shader::create_uniform_2D_texture(const int index, const char* uniform_name) {

	unsigned int location = glGetUniformLocation(this->program, uniform_name);

	//glUseProgram(this->program);

	glUniform1i(location, index);

};

std::vector<vec3> Shader::create_standard_matrix_vectors() {

	vec3 right = vec3(1, 0, 0);
	vec3 up = vec3(0, 1, 0);
	vec3 direction = vec3(0, 0, 1);
	vec3 camera = vec3(0, 0, -3);

	vec3 translation_vector = vec3(0, 0, 0);
	vec3 scaling_vector = vec3(1, 1, 1);
	vec3 rotation_vector = vec3(0, 0, 0);

	return { right, up, direction, camera, translation_vector, scaling_vector, rotation_vector };

};

std::vector<vec3> Shader::create_standard_light_vectors() {

	vec3 light_position = vec3(0, 0, -3);
	vec3 light_color = vec3(1, 1, 1);

	return { light_position, light_color };

};

void Shader::setup_matrices(const vec2& screen_size, const vec3& right, const vec3& up, const vec3& direction, const vec3& camera, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	mat4 model_transformation_matrix = create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector);
	mat4 look_at_matrix = create_view_matrix(right, up, direction, camera);
	mat4 model_view_mat = create_model_view_matrix(model_transformation_matrix, look_at_matrix);

	std::vector<float> normal_matrix = model_view_mat.inverse().transpose().to_GL();
	std::vector<float> model_view_matrix = model_view_mat.to_GL();
	std::vector<float> projection_matrix = create_frustum_projection_matrix(60.0f, screen_size.x, screen_size.y, 0.1f, 100.0f).to_GL();
	std::vector<float> eye_vector = camera.to_GL();

	create_uniform_vec3(eye_vector, "eye_vector");
	create_uniform_mat4(normal_matrix, "normal_matrix");
	create_uniform_mat4(model_view_matrix, "model_view_matrix");
	create_uniform_mat4(projection_matrix, "projection_matrix");

};

void Shader::setup_light(const vec3& light_pos, const vec3& color, const vec4& material_properties) {

	std::vector<float> light_position = light_pos.to_GL();
	std::vector<float> light_color = color.to_GL();

	float ambient = material_properties.x;
	float diffuse = material_properties.y;
	float specular = material_properties.z;
	float shininess = material_properties.w;

	create_uniform_vec3(light_position, "light_position");
	create_uniform_vec3(light_color, "light_color");

	create_uniform_float(ambient, "ambient");
	create_uniform_float(diffuse, "diffuse");
	create_uniform_float(specular, "specular");
	create_uniform_float(shininess, "shininess");

};

void Shader::add_texture(Texture& texture) {

	std::vector<int> texture_details = { texture.width, texture.height, texture.n_color_channels };

	bind_texture(&texture.txt, texture.GL_TEXTUREindex, texture.bytes, texture_details);

	create_uniform_2D_texture(texture.index, texture.uniform_name);

};

void Shader::setup_textures(std::vector<Texture>& textures) {


	for (int i = 0; i < textures.size(); ++i) {

		add_texture(textures[i]);

		//this->textures.emplace_back(texture);

	};

};

//sets the transformation matrices and light vectors
void Shader::setup(const vec2& screen_size, const std::vector<vec3>& matrices_vectors, const std::vector<vec3>& light_vectors, const vec4& material_properties, std::vector<Texture>& textures) {

	setup_matrices(screen_size, matrices_vectors[0], matrices_vectors[1], matrices_vectors[2], matrices_vectors[3], matrices_vectors[4], matrices_vectors[5], matrices_vectors[6]);
	setup_light(light_vectors[0], light_vectors[1], material_properties);
	setup_textures(textures);

};

void Shader::update_matrices(const vec3& right, const vec3& up, const vec3& direction, const vec3& camera, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector) {

	mat4 model_transformation_matrix = create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector);
	mat4 model_view_mat = create_view_matrix(right, up, direction, camera) * model_transformation_matrix;
	std::vector<float> normal_matrix = model_view_mat.inverse().transpose().to_GL();
	std::vector<float> model_view_matrix = model_view_mat.to_GL();
	std::vector<float> eye_vector = camera.to_GL();

	create_uniform_vec3(eye_vector, "eye_vector");
	create_uniform_mat4(normal_matrix, "normal_matrix");
	create_uniform_mat4(model_view_matrix, "model_view_matrix");

};

void Shader::update_light(const vec3& light_pos, const vec4& material_properties) {

	std::vector<float> light_position = light_pos.to_GL();
	create_uniform_vec3(light_position, "light_position");

	float ambient = material_properties.x;
	float diffuse = material_properties.y;
	float specular = material_properties.z;
	float shininess = material_properties.w;

	create_uniform_float(ambient, "ambient");
	create_uniform_float(diffuse, "diffuse");
	create_uniform_float(specular, "specular");
	create_uniform_float(shininess, "shininess");

};

void Shader::update(const std::vector<vec3>& matrices_vectors, const std::vector<vec3>& light_vectors, const vec4& material_properties) {

	update_matrices(matrices_vectors[0], matrices_vectors[1], matrices_vectors[2], matrices_vectors[3], matrices_vectors[4], matrices_vectors[5], matrices_vectors[6]);
	update_light(light_vectors[0], material_properties);

};

void Shader::delete_textures() {

	int length = this->textures.size();
	for (int i = 0; i < length; ++i) {

		glDeleteTextures(1, &textures[i]);

	};

};