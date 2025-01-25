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

uniform mat4 model_transformation_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform sampler2D uDisplacement_map;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vColor;
out vec2 vTexture_coordinates;
out vec3 vTangent;
out vec3 vBitangent;

void main() {

    float displacement_scale = 1.0;
    float displacement_offset = texture(uDisplacement_map, aTexture_coordinates).r;
    vPosition = aPosition + (aNormal * displacement_offset * displacement_scale); 

    mat3 model_matrix = transpose(inverse(mat3(model_transformation_matrix)));
    vPosition = (model_transformation_matrix * vec4(vPosition, 1)).xyz;

    vTexture_coordinates = aTexture_coordinates;
    vTangent = normalize(model_matrix * aTangent);
    vBitangent = normalize(model_matrix * aBitangent);
    vNormal = normalize(model_matrix * aNormal);
 
    vColor = aColor;

    gl_Position = projection_matrix * view_matrix * vec4(vPosition, 1);

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
uniform bool gamma_correction;
uniform sampler2D uTexture;
uniform sampler2D uNormal_map;

in vec3 gColor;
in vec3 gPosition;
in vec3 gNormal;
in vec2 gTexture_coordinates;
in vec3 gTangent;
in vec3 gBitangent;

void main() {
    
    vec3 texture_color = texture(uTexture, gTexture_coordinates).rgb;
    vec3 view_vector = normalize(eye_vector - gPosition);
    vec3 light_vector = normalize(light_position - gPosition);
    vec3 half_vector = normalize(view_vector + light_vector);
    vec3 n = gNormal;

    /////////////////////////////////////////////////////////////////////////////

    float distance_from_light = length(light_position - gPosition);
    float initial_light_intensity = 50.0;
    float constant_attenuation_component = 0.0;//overall light intensity, works like a multiplier to ambient light
    float linear_attenuation_coefficient = 0.1;//diminishing light intensity over distance
    float quadratic_attenuation_coefficient = 0.1;//further decreasing intensity with the square of the distance
    float attenuation = constant_attenuation_component + linear_attenuation_coefficient * distance_from_light + quadratic_attenuation_coefficient * pow(distance_from_light, 2);
    float light_intensity = initial_light_intensity / attenuation;

    vec3 sampled_normal = texture(uNormal_map, gTexture_coordinates).rgb * 2.0 - 1.0;//getting the normal from the normal map and making it in the range of [-1, 1]
    mat3 TBN = mat3(gTangent, gBitangent, gNormal);
    vec3 map_gNormal = normalize(TBN * sampled_normal);
    n = map_gNormal;

    float cos_alpha = max(dot(n, light_vector), 0.0);
    float cos_theta_prime = max(dot(n, half_vector), 0.0);
    
    vec3 Ca = ambient * texture_color;
    vec3 Cd = diffuse * cos_alpha * texture_color * light_intensity;   
    vec3 Cs = specular * pow(cos_theta_prime, shininess) * light_color * light_intensity;

    vec3 phong = (Ca + Cd + Cs);
    if (gamma_correction) {

        vec3 gamma = vec3(1.0 / 2.2);
        phong = pow(phong, gamma);   

     };

    gl_FragColor = vec4(phong, 1.0);

}
)";

static unsigned int compile_shader(const unsigned int& type, const std::string& source) {

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

//generates a buffer from the inputted paramter and binds it OR binds the inputted buffer
template<typename T>
static void bind_buffer(const unsigned int& GL_ARRAY_TYPE, unsigned int* buffer, const std::vector<T>& buffer_data, const unsigned int& GL_DRAW_TYPE, const int& attribute_position, const int& attribute_size, const bool generate_buffer = false) {

	if (generate_buffer) {

		glGenBuffers(1, buffer);

	};

	glBindBuffer(GL_ARRAY_TYPE, *buffer);
	glBufferData(GL_ARRAY_TYPE, buffer_data.size() * sizeof(T), buffer_data.data(), GL_DRAW_TYPE);

	if (attribute_position > -1) {

		glVertexAttribPointer(attribute_position, attribute_size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);

		glEnableVertexAttribArray(attribute_position);

	};

};

static void bind_texture(unsigned int* texture, const unsigned int& GL_TEXTUREindex, unsigned char* bytes, const int& texture_width, const int& texture_height, const int& n_color_channels, const bool& gamma_correction) {

	glGenTextures(1, texture);/////////////////////////////////////
	glActiveTexture(GL_TEXTUREindex);
	glBindTexture(GL_TEXTURE_2D, *texture);
	if (*texture == 0) {
		std::cerr << "Failed to generate texture!" << std::endl;
		return;//handle the error appropriately
	};

	GLenum internal_format;
	GLenum data_format;
	if (n_color_channels == 4) {//checks if n_color_channels is 3 or 4; RGB or RGBA

		internal_format = gamma_correction ? GL_SRGB_ALPHA : GL_RGBA;
		data_format = GL_RGBA;

	}
	else if (n_color_channels == 3) {

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

	stbi_image_free(bytes);

};


class Shader {

public:

	//holds the shader type example: GL_VERTEX_SHADER and holds the shader's source code. Used as a parameter to attach alot of shaders on each other in the *Shader* class constructor 
	struct shader_compile_info {

		unsigned int shader_type;
		std::string source;
		unsigned int id;

	};

	//this is a struct that holds the basic vectors needed to update and initialize our image
	struct graphics_vectors_container {

		vec3 right_vector;
		vec3 up_vector;
		vec3 direction_vector;
		vec3 camera_position;
		vec3 translation_vector;
		vec3 scaling_vector;
		vec3 rotation_vector;
		vec3 light_position;
		vec3 light_color;

	};

	unsigned int program;
	std::vector<unsigned int> textures;//not used rn
	unsigned int positions_buffer, normals_buffer, colors_buffer, indices_buffer, texture_coordinates_buffer, tangents_buffer, bitangents_buffer;

	Shader(std::vector<shader_compile_info>& shaders_to_compile);
	Shader(std::vector<unsigned int>& compiled_shaders_ids);
	Shader(const std::string& vertex_shader, const std::string& geometry_shader, const std::string& fragment_shader);
	
	//binds all data inside a mesh to the all the buffers inside the shader in a standard way. MADE FOR FAST EASE OF USE.
	void bind_mesh_buffers(Mesh& mesh);
	void create_uniform_mat4(const std::vector<float>& data_vector, const char* uniform_name);
	void create_uniform_vec3(const std::vector<float>& data_vector, const char* uniform_name);
	void create_uniform_float(const float& data_variable, const char* uniform_name);
	void create_uniform_int(const int& data_variable, const char* uniform_name);
	void create_uniform_bool(const bool& boolean, const char* uniform_name);
	void create_uniform_2D_texture(const int& index, const char* uniform_name);

	graphics_vectors_container create_standard_matrix_vectors();
	void init_matrices(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);
	void init_light(const vec3& light_position, const vec3& color, const vec4& material_properties, const bool& gamma_correction);
	
	void add_texture(Texture& texture, const bool& gamma_correction);
	void add_textures(std::vector<Texture>& textures, const bool& gamma_correction);

	//sets the transformation matrices and light vectors
	void initialize(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, std::vector<Texture>& textures);
	void initialize(const vec2& screen_size, const graphics_vectors_container& container, const vec4& material_properties, const bool& gamma_correction, std::vector<Texture>& textures);

	void update_matrices(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);
	void update_light(const vec3& light_position, const vec4& material_properties);
	void update(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties);
	void update(const graphics_vectors_container& container, const vec4& material_properties);

	void delete_buffers();
	void delete_textures();
	void delete_program();
	void delete_all();

};

class Mouse {

private:

	double prev_x = 0.0;
	double prev_y = 0.0;

	GLFWwindow* window;

	float find_angle (vec3& A, vec3& B) {
	
		A = A.normalize();
		B = B.normalize();

		float cos_alpha = A.dot(B);

		float alpha = std::acos(cos_alpha);

		return to_radians(alpha);

	}

public:

	vec2 screen_size;
	double mouse_x, mouse_y;
	vec2 scroll_wheel_offset;//x = yaw, y = pitch   z, w previous values

	void get_mouse_pos() {

		glfwGetCursorPos(this->window, &this->mouse_x, &this->mouse_y);

	};

	//static void scroll_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);

	Mouse(GLFWwindow* window, const vec2& screen_size) : window(window), screen_size(screen_size), scroll_wheel_offset(-90.0f, 0.0f) {

		//glfwSetScrollCallback(window, scroll_wheel_callback);

		get_mouse_pos();

		this->prev_x = this->mouse_x;

		this->prev_y = this->mouse_y;		

	};

	//changes the property of the scene based off the mouse movement or key clicks; used for testing purposes
	void move(Shader::graphics_vectors_container& container, vec4& material_properties) {

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

		const float offset = 0.0299;

		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1)) {
			
			get_mouse_pos();

			float ndc_x = (2.0f * this->mouse_x) / this->screen_size.x - 1.0f;  // Maps [0, width] -> [-1, 1]
			float ndc_y = 1.0f - (2.0f * this->mouse_y) / this->screen_size.y; // Maps [0, height] -> [1, -1]

			mat4 inverse_view_projection = create_frustum_projection_matrix(60.0f, screen_size.x, screen_size.y, 0.1f, 100.0f).inverse();
			vec4 ndc_position(ndc_x, ndc_y, -1.0f, 1.0f); // z = 0 for a position on the near plane
			vec3 world_position = (inverse_view_projection * ndc_position).xyz();

			container.light_position = world_position;

		};

		if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {

			zoom = true;

		};

		if (glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {

			translate_camera = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {

			translate_light = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {

			rotate = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {

			scale = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {

			ambient = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {

			shininess = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {

			diffuse = true;

		}
		else if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {

			specular = true;

		}
		else {

			translate = true;

		};

		if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) {

			if (ambient) {

				material_properties.x += offset;

			}
			else if (diffuse) {

				material_properties.y += offset;

			}
			else if (specular) {

				material_properties.z += offset;

			}
			else if (shininess) {

				material_properties.w += offset;

			}
			else if (zoom) {

				container.camera_position.z += offset * 20;

			}
			else if (rotate) {

				container.rotation_vector.x -= offset * 20;

			}
			else if (translate_light) {

				if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
					
					container.light_position.z += offset;

				}
				else {

					container.light_position.y += offset * 50;

				};	

			}
			else {

				container.translation_vector.y -= offset;

			};

		};

		if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) {

			if (ambient) {

				material_properties.x -= offset;

			}
			else if (diffuse) {

				material_properties.y -= offset;

			}
			else if (specular) {

				material_properties.z -= offset;

			}
			else if (shininess) {

				material_properties.w -= offset;

			}
			else if (zoom) {

				container.camera_position.z -= offset * 20;

			}
			else if (rotate) {

				container.rotation_vector.x += offset * 20;

			}
			else if (translate_light) {

				if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {

					container.light_position.z -= offset;

				}
				else {

					container.light_position.y -= offset * 50;

				};

			}
			else {

				container.translation_vector.y += offset;

			};

		};

		if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

			if (!ambient && !diffuse && !specular && !shininess) {

				if (rotate) {

					container.rotation_vector.y += offset * 20;

				}
				else if (translate_light) {

					container.light_position.x += offset * 50;

				}
				else {

					container.translation_vector.x -= offset;

				};

			};

		};

		if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) {

			if (!ambient && !diffuse && !specular && !shininess) {

				if (rotate) {

					container.rotation_vector.y -= offset * 20;

				}
				else if (translate_light) {

					container.light_position.x -= offset * 50;

				}
				else {

					container.translation_vector.x += offset;

				};

			};

		};

		this->prev_x = this->mouse_x;
		this->prev_y = this->mouse_y;

	};//move bracket

};