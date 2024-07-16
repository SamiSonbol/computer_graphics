#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Vector.h"
#include <stb/stb_image.h>

static const std::string vertex_shader = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexture_coordinates;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

uniform mat4 normal_matrix;
uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vColor;
out vec2 vTexture_coordinates;
out vec3 vTangent;
out vec3 vBitangent;
out mat3 TBN;

void main() {

    vPosition = (model_view_matrix * vec4(aPosition, 1)).xyz;
    vNormal = normalize((normal_matrix * vec4(aNormal, 0)).xyz);
    vColor = aColor;

    vTexture_coordinates = aTexture_coordinates;
    vTangent = normalize((normal_matrix * vec4(aTangent, 0)).xyz);
    vBitangent = normalize((normal_matrix * vec4(aBitangent, 0)).xyz);
    TBN = mat3(vTangent, vBitangent, vNormal);

    gl_Position = projection_matrix * vec4(vPosition, 1);

}
)";

static const std::string geometry_shader = R"(
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec3 vColor[];
in vec3 vPosition[];
in vec2 vTexture_coordinates[];
in vec3 vTangent[];
in vec3 vBitangent[];

out vec3 gNormal;
out vec3 gColor;
out vec3 gPosition;
out vec2 gTexture_coordinates;
out vec3 gTangent;
out vec3 gBitangent;

void main() {
    for (int i = 0; i < 3; ++i) {
        gNormal = vNormal[i];
        gColor = vColor[i];
        gPosition = vPosition[i];
        gTexture_coordinates = vTexture_coordinates[i];
        gTangent = vTangent[i];
        gBitangent = vBitangent[i];

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
)";

static const std::string fragment_shader = R"(
#version 330 core

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 eye_vector;

uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform float shininess;
uniform sampler2D uTexture;
uniform sampler2D uNormal_map;

in vec3 gColor;
in vec3 gPosition;
in vec3 gNormal;
in vec2 gTexture_coordinates;
in vec3 gTangent;
in vec3 gBitangent;
in mat3 TBN;

void main() {
    
    vec3 view_vector = normalize(-gPosition);
    vec3 light_vector = normalize(light_position - gPosition);
    vec3 half_vector = normalize(view_vector + light_vector);
    vec3 n = gNormal;

    vec3 texture_color = texture(uTexture, gTexture_coordinates).rgb;

/////////////////////////////////////////////////////////////////////////////////
	vec3 sampled_normal = texture(uNormal_map, gTexture_coordinates).rgb;
    sampled_normal = sampled_normal * 2.0 - 1.0;

    vec3 transformed_normal = normalize(TBN * sampled_normal);
///////////////////////////////////////////////////////////////////////////////////

    float cos_alpha = max(dot(transformed_normal, light_vector), 0.0);
    float cos_theta_prime = max(dot(transformed_normal, half_vector), 0.0);
    
    vec3 Ca = ambient * texture_color;
    vec3 Cd = diffuse * cos_alpha * texture_color;   
    vec3 Cs = specular * pow(cos_theta_prime, shininess) * vec3(1.0, 1.0, 1.0);

    vec3 phong = (Ca + Cd + Cs) * light_color;

    gl_FragColor = vec4(phong, 1.0);

}
)";

static unsigned int compile_shader(unsigned int type, const std::string& source) {

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

}

static unsigned int create_shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader) {

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

}

//generates a buffer from the inputted paramter and binds it OR binds the inputted buffer
template<typename T>
static void bind_buffer(unsigned int GL_ARRAY_TYPE, unsigned int* buffer, const std::vector<T>& buffer_data, unsigned int GL_DRAW_TYPE, int attribute_position, int attribute_size, bool generate_buffer = false) {

	if (generate_buffer) {

		glGenBuffers(1, buffer);

	}

	glBindBuffer(GL_ARRAY_TYPE, *buffer);
	glBufferData(GL_ARRAY_TYPE, buffer_data.size() * sizeof(T), buffer_data.data(), GL_DRAW_TYPE);

	if (attribute_position > -1) {

		glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, sizeof(T) * attribute_size, (void*)0);
		
		glEnableVertexAttribArray(attribute_position);

	}

};

static void bind_texture(unsigned int* texture, unsigned int GL_TEXTUREindex, unsigned char* bytes, std::vector<int>& image_details_vector) {

	glGenTextures(1, texture);/////////////////////////////////////
	glActiveTexture(GL_TEXTUREindex);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_details_vector[0], image_details_vector[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

};

static void get_indices(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texture_coordinates, std::vector<unsigned short>& indices) {
	size_t vertex_count = positions.size() / 3;
	
	for (int i = 0; i < vertex_count; ++i) {

		indices.emplace_back(i);

	}

	for (size_t i = 0; i < vertex_count; ++i) {

		vec3 vertex = vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]);
		vec3 normal = vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		vec2 texture_coords = vec2(texture_coordinates[i * 2], texture_coordinates[i * 2 + 1]);

		//indices.emplace_back(i);	
		indices[i] = i;

		for (size_t j = i + 1; j < vertex_count; ++j) {

			vec3 other_vertex = vec3(positions[j * 3], positions[j * 3 + 1], positions[j * 3 + 2]);
			vec3 other_normal = vec3(normals[j * 3], normals[j * 3 + 1], normals[j * 3 + 2]);
			vec2 other_texture_coords = vec2(texture_coordinates[j * 2], texture_coordinates[j * 2 + 1]);

			if (vertex.x == other_vertex.x && vertex.y == other_vertex.y && vertex.z == other_vertex.z) {

				if (normal.x == other_normal.x && normal.y == other_normal.y && normal.z == other_normal.z) {

					if (texture_coords.x == other_texture_coords.x && texture_coords.y == other_texture_coords.y) {
					
						//indices.emplace_back(j);
						indices[j] = i;

					}	

				}

			}
					
		}
	}

}

static void compute_normals(const std::vector<float>& positions, const std::vector<unsigned short>& indices, std::vector<float>& normals) {

	normals.clear();
	//normals.resize(positions.size(), 0.0f);
	
	for (size_t i = 0; i < indices.size(); i += 3) {
		int p1 = indices[i];
		int p2 = indices[i + 1];
		int p3 = indices[i + 2];

		vec3 a = vec3(positions[p1 * 3], positions[p1 * 3 + 1], positions[p1 * 3 + 2]);
		vec3 b = vec3(positions[p2 * 3], positions[p2 * 3 + 1], positions[p2 * 3 + 2]);
		vec3 c = vec3(positions[p3 * 3], positions[p3 * 3 + 1], positions[p3 * 3 + 2]);

		vec3 deltaPos1 = b - a;
		vec3 deltaPos2 = c - a;

		vec3 normal = deltaPos1.cross(deltaPos2);
		normal.normalize();

		/*normals.emplace_back(normal.x);
		normals.emplace_back(normal.y);
		normals.emplace_back(normal.z);

		normals.emplace_back(normal.x);
		normals.emplace_back(normal.y);
		normals.emplace_back(normal.z);

		normals.emplace_back(normal.x);
		normals.emplace_back(normal.y);
		normals.emplace_back(normal.z);*/

	}

}

static void compute_tangents(const std::vector<float>& positions, const std::vector<unsigned short>& indices, std::vector<float>& normals, std::vector<float>& tangents, std::vector<float>& bitangents, const std::vector<float>& texture_coordinates) {

	tangents.resize(normals.size());
	bitangents.resize(normals.size());

	for (size_t i = 0; i < indices.size(); i += 3) {
		int p1 = indices[i];
		int p2 = indices[i + 1];
		int p3 = indices[i + 2];

		vec3 a = vec3(positions[p1 * 3], positions[p1 * 3 + 1], positions[p1 * 3 + 2]);
		vec3 b = vec3(positions[p2 * 3], positions[p2 * 3 + 1], positions[p2 * 3 + 2]);
		vec3 c = vec3(positions[p3 * 3], positions[p3 * 3 + 1], positions[p3 * 3 + 2]);

		vec3 edge1 = b - a;
		vec3 edge2 = c - a;

		vec2 uv0 = vec2(texture_coordinates[p1 * 2], texture_coordinates[p1 * 2 + 1]);
		vec2 uv1 = vec2(texture_coordinates[p2 * 2], texture_coordinates[p2 * 2 + 1]);
		vec2 uv2 = vec2(texture_coordinates[p3 * 2], texture_coordinates[p3 * 2 + 1]);

		vec2 deltaUV1 = uv1 - uv0;
		vec2 deltaUV2 = uv2 - uv0;

		//E1 = d1x*T + d1y*B
		//E2 = d2x*T + d2y*B

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec3 tangent, bitangent;

		tangent.x = r * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = r * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = r * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitangent.x = r * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = r * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = r * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		tangents[p1 * 3] = tangent.x, tangents[p1 * 3 + 1] = tangent.y, tangents[p1 * 3 + 2] = tangent.z;
		tangents[p2 * 3] = tangent.x, tangents[p2 * 3 + 1] = tangent.y, tangents[p2 * 3 + 2] = tangent.z;
		tangents[p3 * 3] = tangent.x, tangents[p3 * 3 + 1] = tangent.y, tangents[p3 * 3 + 2] = tangent.z;

		bitangents[p1 * 3] = bitangent.x, bitangents[p1 * 3 + 1] = bitangent.y, bitangents[p1 * 3 + 2] = bitangent.z;
		bitangents[p2 * 3] = bitangent.x, bitangents[p2 * 3 + 1] = bitangent.y, bitangents[p2 * 3 + 2] = bitangent.z;
		bitangents[p3 * 3] = bitangent.x, bitangents[p3 * 3 + 1] = bitangent.y, bitangents[p3 * 3 + 2] = bitangent.z;

		/*tangents.emplace_back(tangent.x);
		tangents.emplace_back(tangent.y);
		tangents.emplace_back(tangent.z);*/
		
		/*bitangents.emplace_back(bitangent.x);
		bitangents.emplace_back(bitangent.y);
		bitangents.emplace_back(bitangent.z);*/

	};

	std::cout << normals.size() << std::endl;
	std::cout << bitangents.size() << std::endl;

};

class Texture {

public:

	int width, height, n_color_channels;

	unsigned int GL_TEXTUREindex;

	int index;

	const char* uniform_name;	

	unsigned char* bytes;

	unsigned int txt;

	Texture(const std::string& file_path, const char* uniform_name, unsigned int GL_TEXTUREindex, int index) {

		this->bytes = stbi_load(file_path.c_str(), &this->width, &this->height, &this->n_color_channels, 0);

		this->uniform_name = uniform_name;

		this->GL_TEXTUREindex = GL_TEXTUREindex;

		this->index = index;

	}

	std::vector<float> generate_normal_map() {

		std::vector<float> normals;

		for (int j = 0; j < height; ++j) {

			for (int i = 0; i < width; ++i) {

				int index = (j * width + i) * 3;

				float x = bytes[index] / 255.0f * 2.0f - 1.0f;
				float y = bytes[index + 1] / 255.0f * 2.0f - 1.0f;
				float z = bytes[index + 2] / 255.0f * 2.0f - 1.0f;

				vec3 v = vec3(x, y, z);
				v.normalize();

				normals.emplace_back(v.x);
				normals.emplace_back(v.y);
				normals.emplace_back(v.z);

			}

		}

		return normals;

	}

	~Texture() {

		this->uniform_name = nullptr;
		this->bytes = nullptr;

	};

};

class Shader {

public:

	unsigned int program;

	std::vector<unsigned int> textures;

	Shader(std::string vertex_shader, std::string geometry_shader, std::string fragment_shader) {

		this->program = create_shader(vertex_shader, geometry_shader, fragment_shader);

	};
	
	void create_uniform_mat4(std::vector<float>& data_vector, const char* uniform_name) {

		unsigned int location = glGetUniformLocation(this->program, uniform_name);

		glUniformMatrix4fv(location, 1, GL_FALSE, data_vector.data());

	}

	void create_uniform_vec3(std::vector<float>& data_vector, const char* uniform_name) {

		unsigned int location = glGetUniformLocation(this->program, uniform_name);

		glUniform3fv(location, 1, data_vector.data());

	}

	void create_uniform_float(float data_variable, const char* uniform_name) {

		unsigned int location = glGetUniformLocation(this->program, uniform_name);

		glUniform1f(location, data_variable);

	}

	void create_uniform_2D_texture(int index, const char* uniform_name) {

		unsigned int location = glGetUniformLocation(this->program, uniform_name);

		//glUseProgram(this->program);

		glUniform1i(location, index);

	}

	std::vector<vec3> create_standard_matrix_vectors() {

		vec3 right = vec3(1, 0, 0);
		vec3 up = vec3(0, 1, 0);
		vec3 direction = vec3(0, 0, 1);
		vec3 camera = vec3(0, 0, -3);

		vec3 translation_vector = vec3(0, 0, 0);
		vec3 scaling_vector = vec3(1, 1, 1);
		vec3 rotation_vector = vec3(0, 0, 0);

		std::vector<vec3> vectors = { right, up, direction, camera, translation_vector, scaling_vector, rotation_vector };
		return vectors;

	}

	std::vector<vec3> create_standard_light_vectors() {

		vec3 light_position = vec3(0, 0, -3);
		vec3 light_color = vec3(1, 1, 1);

		std::vector<vec3> vectors = { light_position, light_color };
		return vectors;

	}

	void setup_matrices(vec2& screen_size, vec3& right, vec3& up, vec3& direction, vec3& camera, vec3& translation_vector, vec3& scaling_vector, vec3& rotation_vector) {

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

	}

	void setup_light(vec3& light_pos, vec3& color, vec4& material_properties) {

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

	}

	void add_texture(Texture& texture) {

		std::vector<int> texture_details = { texture.width, texture.height, texture.n_color_channels };

		bind_texture(&texture.txt, texture.GL_TEXTUREindex, texture.bytes, texture_details);

		create_uniform_2D_texture(texture.index, texture.uniform_name);
		
	}

	void setup_textures(std::vector<Texture>& textures) {


		for (int i = 0; i < textures.size(); ++i) {

			add_texture(textures[i]);

			//this->textures.emplace_back(texture);

		}	
		
	}

	//sets the transformation matrices and light vectors
	void setup(vec2& screen_size, std::vector<vec3>& matrices_vectors, std::vector<vec3>& light_vectors, vec4& material_properties, std::vector<Texture>& textures) {

		setup_matrices(screen_size, matrices_vectors[0], matrices_vectors[1], matrices_vectors[2], matrices_vectors[3], matrices_vectors[4], matrices_vectors[5], matrices_vectors[6]);
		setup_light(light_vectors[0], light_vectors[1], material_properties);
		setup_textures(textures);

	}

	void update_matrices(vec3& right, vec3& up, vec3& direction, vec3& camera, vec3& translation_vector, vec3& scaling_vector, vec3& rotation_vector) {

		mat4 model_transformation_matrix = create_model_transformation_matrix(translation_vector, scaling_vector, rotation_vector);
		mat4 model_view_mat = create_view_matrix(right, up, direction, camera).multiply_matrix(model_transformation_matrix);
		std::vector<float> normal_matrix = model_view_mat.inverse().transpose().to_GL();
		std::vector<float> model_view_matrix = model_view_mat.to_GL();
		std::vector<float> eye_vector = camera.to_GL();

		create_uniform_vec3(eye_vector, "eye_vector");
		create_uniform_mat4(normal_matrix, "normal_matrix");
		create_uniform_mat4(model_view_matrix, "model_view_matrix");

	}

	void update_light(vec3& light_pos, vec4& material_properties) {

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

	}

	void update(std::vector<vec3>& matrices_vectors, std::vector<vec3>& light_vectors, vec4& material_properties) {
		
		update_matrices(matrices_vectors[0], matrices_vectors[1], matrices_vectors[2], matrices_vectors[3], matrices_vectors[4], matrices_vectors[5], matrices_vectors[6]);
		update_light(light_vectors[0], material_properties);

	}

	void delete_textures() {

		int length = this->textures.size();
		for (int i = 0; i < length; ++i) {

			glDeleteTextures(1, &textures[i]);

		}

	}

};

class Mouse {

private:

	double prev_x = 0.0;
	double prev_y = 0.0;

	GLFWwindow* window;

	float find_angle (vec3 A, vec3 B) {

		A.normalize();
		B.normalize();

		float cos_alpha = A.dot(B);

		float alpha = std::acos(cos_alpha);

		return to_radians(alpha);

	}

public:

	double mouse_x, mouse_y;

	Mouse(GLFWwindow* window) {

		this->window = window;

		glfwGetCursorPos(this->window, &this->mouse_x, &this->mouse_y);

		this->prev_x = this->mouse_x;

		this->prev_y = this->mouse_y;

	};
	
	void get_mouse_pos() {

		glfwGetCursorPos(this->window, &this->mouse_x, &this->mouse_y);	

	};

	//changes the property of the scene based off the mouse movements
	void move(std::vector<vec3>& matrix_vectors, std::vector<vec3>& light_vectors, vec4& material_properties) {

		get_mouse_pos();

		bool translate = false;
		bool scale = false;
		bool rotate = false;

		bool translate_camera = false;
		bool zoom = false;

		bool translate_light = false;

		bool ambient = false;
		bool diffuse = false;
		bool specular = false;
		bool shininess = false;

		vec4 transformation_vector;

		const float offset = 0.0299;

		if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {

			zoom = true;

		}

		if (glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {

			transformation_vector = matrix_vectors[3];

			translate_camera = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {

			transformation_vector = light_vectors[0];

			translate_light = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {

			transformation_vector = matrix_vectors[6];

			rotate = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {

			transformation_vector = matrix_vectors[5];

			scale = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {

			transformation_vector = material_properties;

			ambient = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {

			transformation_vector = material_properties;

			shininess = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {

			transformation_vector = material_properties;

			diffuse = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {

			transformation_vector = material_properties;

			specular = true;

		}
		else {

			transformation_vector = matrix_vectors[4];

			translate = true;

		}

		if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) {

			if (ambient) {

				transformation_vector.x += offset;

			}
			else if (diffuse) {

				transformation_vector.y += offset;

			}
			else if (specular) {

				transformation_vector.z += offset;

			}
			else if (shininess) {

				transformation_vector.w += offset;

			}
			else if (zoom) {

				transformation_vector.z += offset;

			}
			else if (rotate) {

				transformation_vector.x -= offset * 20;

			}
			else if (translate_light) {

				transformation_vector.y += offset;

			}
			else {

				transformation_vector.y -= offset;

			}		

		}

		if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) {

			if (ambient) {

				transformation_vector.x -= offset;

			}
			else if (diffuse) {

				transformation_vector.y -= offset;

			}
			else if (specular) {

				transformation_vector.z -= offset;

			}
			else if (shininess) {

				transformation_vector.w -= offset;

			}
			else if (zoom) {

				transformation_vector.z -= offset;

			}
			else if (rotate) {

				transformation_vector.x += offset * 20;

			}
			else if (translate_light) {

				transformation_vector.y -= offset;

			}
			else {

				transformation_vector.y += offset;

			}

		}

		if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

			if (!ambient && !diffuse && !specular && !shininess) {

				if (rotate) {

					transformation_vector.y += offset * 20;

				}
				else if (translate_light) {

					transformation_vector.x += offset;

				}

				else {

					transformation_vector.x -= offset;

				}

			}

		}

		if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) {

			if (!ambient && !diffuse && !specular && !shininess) {

				if (rotate) {

					transformation_vector.y -= offset * 20;

				}
				else if (translate_light) {

					transformation_vector.x -= offset;

				}

				else {

					transformation_vector.x += offset;

				}

			}

		}

		this->prev_x = this->mouse_x;
		this->prev_y = this->mouse_y;

		if (translate) {

			matrix_vectors[4] = vec3(transformation_vector.x, transformation_vector.y, transformation_vector.z);

		}
		else if (scale) {

			matrix_vectors[5] = vec3(transformation_vector.x, transformation_vector.y, transformation_vector.z);

		}
		else if (rotate) {

			matrix_vectors[6] = vec3(transformation_vector.x, transformation_vector.y, transformation_vector.z);

		}
		else if (translate_camera) {

			matrix_vectors[3] = vec3(transformation_vector.x, transformation_vector.y, transformation_vector.z);

		}
		else if (translate_light) {

			light_vectors[0] = vec3(transformation_vector.x, transformation_vector.y, transformation_vector.z);

		}
		else if (ambient || diffuse || specular || shininess) {

			material_properties = transformation_vector;

		}

	}//move bracket

};


static std::vector<float> generateSphereVertices(float radius, int stacks, int slices) {

	std::vector<float> vertices;

	for (int i = 0; i <= stacks; ++i) {
		float stackAngle = 3.14159265359 / 2 - i * 3.14159265359 / stacks; // from pi/2 to -pi/2
		float xy = radius * cosf(stackAngle);           // radius * cos(theta)
		float z = radius * sinf(stackAngle);            // radius * sin(theta)

		for (int j = 0; j <= slices; ++j) {
			float sectorAngle = j * 2 * 3.14159265359 / slices; // from 0 to 2pi

			float x = xy * cosf(sectorAngle);          // radius * cos(theta) * cos(phi)
			float y = xy * sinf(sectorAngle);          // radius * cos(theta) * sin(phi)
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	return vertices;
}