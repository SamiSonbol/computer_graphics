#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stb/stb_image.h>

#include "Math.h"
#include "Mesh.h"

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

    float cos_alpha = max(dot(n, light_vector), 0.0);
    float cos_theta_prime = max(dot(n, half_vector), 0.0);
    
    vec3 Ca = ambient * texture_color;
    vec3 Cd = diffuse * cos_alpha * texture_color;   
    vec3 Cs = specular * pow(cos_theta_prime, shininess) * vec3(1.0, 1.0, 1.0);

    vec3 phong = (Ca + Cd + Cs) * light_color;

    gl_FragColor = vec4(phong, 1.0);

}
)";



//generates a buffer from the inputted paramter and binds it OR binds the inputted buffer
template<typename T>
static void bind_buffer(const unsigned int& GL_ARRAY_TYPE, unsigned int* buffer, const std::vector<T>& buffer_data, const unsigned int& GL_DRAW_TYPE, const int& attribute_position, const int& attribute_size, const bool generate_buffer = false) {

	if (generate_buffer) {

		glGenBuffers(1, buffer);

	};

	glBindBuffer(GL_ARRAY_TYPE, *buffer);
	glBufferData(GL_ARRAY_TYPE, buffer_data.size() * sizeof(T), buffer_data.data(), GL_DRAW_TYPE);

	if (attribute_position > -1) {

		glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, sizeof(T) * attribute_size, (void*)0);

		glEnableVertexAttribArray(attribute_position);

	};

};

static void bind_texture(unsigned int* texture, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const std::vector<int>& image_details_vector) {

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

static void compute_tangents(const std::vector<float>& positions, const std::vector<unsigned short>& indices, const std::vector<float>& normals, std::vector<float>& tangents, std::vector<float>& bitangents, const std::vector<float>& texture_coordinates) {

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





class Shader {

public:

	unsigned int program;

	std::vector<unsigned int> textures;

	unsigned int compile_shader(unsigned int type, const std::string& source);

	unsigned int create_shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader);

	Shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader);
	
	void create_uniform_mat4(const std::vector<float>& data_vector, const char* uniform_name);

	void create_uniform_vec3(const std::vector<float>& data_vector, const char* uniform_name);

	void create_uniform_float(const float data_variable, const char* uniform_name);

	void create_uniform_2D_texture(const int index, const char* uniform_name);

	std::vector<vec3> create_standard_matrix_vectors();

	std::vector<vec3> create_standard_light_vectors();

	void setup_matrices(const vec2& screen_size, const vec3& right, const vec3& up, const vec3& direction, const vec3& camera, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);

	void setup_light(const vec3& light_pos, const vec3& color, const vec4& material_properties);

	void add_texture(Texture& texture);

	void setup_textures(std::vector<Texture>& textures);

	//sets the transformation matrices and light vectors
	void setup(const vec2& screen_size, const std::vector<vec3>& matrices_vectors, const std::vector<vec3>& light_vectors, const vec4& material_properties, std::vector<Texture>& textures);

	void update_matrices(const vec3& right, const vec3& up, const vec3& direction, const vec3& camera, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);

	void update_light(const vec3& light_pos, const vec4& material_properties);

	void update(const std::vector<vec3>& matrices_vectors, const std::vector<vec3>& light_vectors, const vec4& material_properties);

	void delete_textures();

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