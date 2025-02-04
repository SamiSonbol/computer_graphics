#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <stb_image/stb_image.h>
#include "computer_graphics/Math.h"
#include "computer_graphics/Mesh.h"

static const std::string vertex_shader = R"(
#version 440 core

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
uniform bool height_mapping;
uniform float displacement_scale;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vColor;
out vec2 vTexture_coordinates;
out vec3 vTangent;
out vec3 vBitangent;

vec3 displace(vec3 position, vec3 normal, float displacement_offset, float displacement_scale) {

    return position + (normal * displacement_offset * displacement_scale); 

}; 

void main() {

    mat3 model_matrix = transpose(inverse(mat3(model_transformation_matrix)));
    if (height_mapping) {

		float displacement_offset = texture(uDisplacement_map, aTexture_coordinates).r;
		vPosition = displace(aPosition, aNormal, displacement_offset, displacement_scale); 
        vPosition = (model_transformation_matrix * vec4(vPosition, 1)).xyz;
         
    } else {

        vPosition = (model_transformation_matrix * vec4(aPosition, 1)).xyz;

    };
    
    vTexture_coordinates = aTexture_coordinates;
    vTangent = normalize(model_matrix * aTangent);
    vBitangent = normalize(model_matrix * aBitangent);
    vNormal = normalize(model_matrix * aNormal);
 
    vColor = aColor;

    gl_Position = projection_matrix * view_matrix * vec4(vPosition, 1);

};
)";

static const std::string tesselation_control_shader = R"(
#version 440 core

layout (vertices = 3) out;

in vec3 vNormal[];
in vec3 vColor[];
in vec3 vPosition[];
in vec2 vTexture_coordinates[];
in vec3 vTangent[];
in vec3 vBitangent[];

uniform float tesselation_multiplier;

out vec3 cNormal[];
out vec3 cColor[];
out vec3 cPosition[];
out vec2 cTexture_coordinates[];
out vec3 cTangent[];
out vec3 cBitangent[];

void main() {

  gl_TessLevelOuter[0] = tesselation_multiplier;
  gl_TessLevelOuter[1] = tesselation_multiplier;
  gl_TessLevelOuter[2] = tesselation_multiplier;

  gl_TessLevelInner[0] = tesselation_multiplier;

  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  
  cPosition[gl_InvocationID] = vPosition[gl_InvocationID];
  cNormal[gl_InvocationID] = vNormal[gl_InvocationID];
  cTangent[gl_InvocationID] = vTangent[gl_InvocationID];
  cBitangent[gl_InvocationID] = vBitangent[gl_InvocationID];
  cTexture_coordinates[gl_InvocationID] = vTexture_coordinates[gl_InvocationID];
  cColor[gl_InvocationID] = vColor[gl_InvocationID];

};
)";

static const std::string tesselation_evaluation_shader = R"(
#version 440 core

layout (triangles, equal_spacing, ccw) in;

in vec3 cNormal[];
in vec3 cColor[];
in vec3 cPosition[];
in vec2 cTexture_coordinates[];
in vec3 cTangent[];
in vec3 cBitangent[];

out vec3 gNormal;
out vec3 gColor;
out vec3 gPosition;
out vec2 gTexture_coordinates;
out vec3 gTangent;
out vec3 gBitangent;

vec4 interpolate(vec4 v1, vec4 v2, vec4 v3, vec4 v4) {

  vec4 a = mix(v1, v2, gl_TessCoord.x);
  vec4 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec4 interpolate(vec4 v1, vec4 v2, vec4 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec3 interpolate(vec3 v1, vec3 v2, vec3 v3, vec3 v4) {

  vec3 a = mix(v1, v2, gl_TessCoord.x);
  vec3 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec3 interpolate(vec3 v1, vec3 v2, vec3 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec2 interpolate(vec2 v1, vec2 v2, vec2 v3, vec2 v4) {

  vec2 a = mix(v1, v2, gl_TessCoord.x);
  vec2 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec2 interpolate(vec2 v1, vec2 v2, vec2 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

void main() {

  gl_Position = interpolate(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);

  gPosition = interpolate(cPosition[0], cPosition[1], cPosition[2]);
  gNormal = interpolate(cNormal[0], cNormal[1], cNormal[2]);
  gTangent = interpolate(cTangent[0], cTangent[1], cTangent[2]);
  gBitangent = interpolate(cBitangent[0], cBitangent[1], cBitangent[2]);
  gTexture_coordinates = interpolate(cTexture_coordinates[0], cTexture_coordinates[1], cTexture_coordinates[2]);
  gColor = interpolate(cColor[0], cColor[1], cColor[2]);

};
)";

static const std::string geometry_shader = R"(
#version 440 core

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

};
)";

static const std::string fragment_shader = R"(
#version 440 core

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 eye_vector;

uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform float shininess;
uniform sampler2D uTexture;
uniform sampler2D uNormal_map;
uniform bool gamma_correction;
uniform bool texturing;
uniform bool normal_mapping;

in vec3 gColor;
in vec3 gPosition;
in vec3 gNormal;
in vec2 gTexture_coordinates;
in vec3 gTangent;
in vec3 gBitangent;

out vec4 FragColor;

float calculate_light_intensity(float distance_from_light, float initial_light_intensity, float constant_attenuation_component, float linear_attenuation_coefficient, float quadratic_attenuation_coefficient) {

    //constant_attenuation_component is overall light intensity, works like a multiplier to ambient light
    //linear_attenuation_coefficient is diminishing light intensity over distance
    //quadratic_attenuation_coefficient is further decreasing intensity with the square of the distance

    float attenuation = constant_attenuation_component + linear_attenuation_coefficient * distance_from_light + quadratic_attenuation_coefficient * pow(distance_from_light, 2);
    return initial_light_intensity / attenuation;

}; 

vec3 calculate_phong_lighting(vec3 frag_position, vec3 normal, vec3 eye_vector, vec3 light_position, vec3 light_color, float light_intensity, vec3 frag_color, float ambient, float diffuse, float specular, float shininess) {

    vec3 view_vector = normalize(eye_vector - frag_position);
    vec3 light_vector = normalize(light_position - frag_position);
    vec3 half_vector = normalize(view_vector + light_vector);

    float cos_alpha = max(dot(normal, light_vector), 0.0);
    float cos_theta_prime = max(dot(normal, half_vector), 0.0);
    
    vec3 Ca = ambient * frag_color;
    vec3 Cd = diffuse * cos_alpha * frag_color * light_intensity;   
    vec3 Cs = specular * pow(cos_theta_prime, shininess) * light_color * light_intensity;

    return Ca + Cd + Cs;

};

void main() {
   
    float distance_from_light = length(light_position - gPosition);
    float light_intensity = calculate_light_intensity(distance_from_light, 100.0, 0.0, 0.1, 0.1);

    vec3 Normal = gNormal;
    if (normal_mapping) {

		vec3 sampled_normal = texture(uNormal_map, gTexture_coordinates).rgb * 2.0 - 1.0;//getting the normal from the normal map and making it in the range of [-1, 1]
		mat3 TBN = mat3(gTangent, gBitangent, gNormal);
		Normal = normalize(TBN * sampled_normal);

    };

    vec3 Color = gColor;
    if (texturing) {

       Color = texture(uTexture, gTexture_coordinates).rgb;

    };
    vec3 phong = calculate_phong_lighting(gPosition, Normal, eye_vector, light_position, light_color, light_intensity, Color, ambient, diffuse, specular, shininess);
    if (gamma_correction) {

        vec3 gamma = vec3(1.0 / 2.2);
        phong = pow(phong, gamma);   

     };

    FragColor = vec4(phong, 1.0);

};
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
				: type == GL_TESS_CONTROL_SHADER ? "tesselation control"
				: "tesselation evaluation")
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

};


class Shader {

public:

	//this is a struct that holds the basic vectors needed to update and initialize our image, used this instead of an std::vector<vec3> inorder to attain readability, clarity, and avoid the issue of guessing which index is which vec3.
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
		vec4 material_properties;

	};

	//same functionality as *graphics_vectors_container* but for bools
	struct graphics_booleans_container {

		bool gamma_correction;
		bool texturing;
		bool normal_mapping;
		bool height_mapping;

	};

	//same functionality as *graphics_vectors_container* but for bools
	struct graphics_floats_container {

		float tesselation_multiplier;
		float displacement_scale;

	};

	unsigned int program;
	std::vector<unsigned int> textures;//not used rn
	unsigned int positions_buffer, normals_buffer, colors_buffer, indices_buffer, texture_coordinates_buffer, tangents_buffer, bitangents_buffer;

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

	graphics_vectors_container create_standard_shader_vectors();
	graphics_booleans_container create_standard_shader_booleans();
	graphics_floats_container create_standard_shader_floats();

	void init_matrices(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);
	void init_light(const vec3& light_position, const vec3& color, const vec4& material_properties);
	void init_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping);
	void init_floats(const float& tesselation_multiplier, const float& displacement_scale);

	void add_texture(Texture& texture, const bool& gamma_correction);
	void add_textures(std::vector<Texture>& textures, const bool& gamma_correction);

	//sets the transformation matrices and light vectors
	void initialize(const vec2& screen_size, const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping, const float& tesselation_multiplier, const float& displacement_scale, std::vector<Texture>& textures);
	void initialize(const vec2& screen_size, const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container, std::vector<Texture>& textures);

	void update_matrices(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector);
	void update_light(const vec3& light_position, const vec3& light_color, const vec4& material_properties);
	void update_booleans(const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping);
	void update_floats(const float& tesselation_multiplier, const float& displacement_scale);

	void update(const vec3& right_vector, const vec3& up_vector, const vec3& direction_vector, const vec3& camera_position, const vec3& translation_vector, const vec3& scaling_vector, const vec3& rotation_vector, const vec3& light_position, const vec3& light_color, const vec4& material_properties, const bool& gamma_correction, const bool& texturing, const bool& normal_mapping, const bool& height_mapping, const float& tesselation_multiplier, const float& displacement_scale);
	void update(const graphics_vectors_container& vectors_container, const graphics_booleans_container& booleans_container, const graphics_floats_container& floats_container);

	void delete_buffers();
	void delete_textures();
	void delete_program();
	void delete_all();

};