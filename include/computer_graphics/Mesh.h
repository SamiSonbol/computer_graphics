#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include "computer_graphics/Math.h"
#include "computer_graphics/File.h"

//struct used to hash the unordered_map we are using in our *Mesh* class
struct vec3_vec3_vec2_hasher {

	std::size_t operator()(const std::tuple<vec3, vec3, vec2>& tuple) const {

		std::size_t h1 = vec3_hasher()(std::get<0>(tuple));
		std::size_t h2 = vec3_hasher()(std::get<1>(tuple));
		std::size_t h3 = vec2_hasher()(std::get<2>(tuple));

		std::size_t seed = h1;
		seed = hash_combine(seed, h2);
		seed = hash_combine(seed, h3);
		return seed;

	};

};

class Vertex {

public:

	unsigned short index;

	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec2 uv;
	vec3 color;

	Vertex(const vec3& position);
	Vertex(const float& x, const float& y, const float& z);

};

class Triangle {

public:

	Vertex A;
	Vertex B;
	Vertex C;

	void offset_positions(const vec3& offset);

	std::array<vec3, 3> compute_TBN();
	void update_TBNs();

	void merge_face(const vec2& mesh_dimensions);

	void set_color(const vec3& color);

	Triangle(const Vertex& A, const Vertex& B, const Vertex& C);
	Triangle(const vec3& A, const vec3& B, const vec3& C);

};

class Texture {

 public:

	int width, height, n_color_channels, index;
	unsigned int GL_TEXTUREindex;

	unsigned int texture_ID;//vipNote: dont ever make this a pointer
	const char* uniform_name;
	unsigned char* bytes;

	std::vector<float> generate_normal_map();

	vec4 get_pixel_color(const size_t& x, const size_t& y);
	void set_pixel_color(const size_t& x, const size_t& y, const vec4& color);

	//since *Texture.bytes* will be used inside openGL and will be moved/copied inside an instance of *Mesh* we need to make sure that there is no 2 copies of *Texture* that hold the same pointer, hence we need to nullify the original instance if it was moved
	Texture(const std::string& file_path, const char* uniform_name, const unsigned int& GL_TEXTUREindex, const int& index);

	//copy operator
	Texture(Texture&& other) noexcept;

	//move assignment operator
	Texture& operator=(Texture&& other) noexcept;

	//destructor
	~Texture();

};

class Mesh {

 public:

	Texture diffuse_map;
	Texture normal_map;
	Texture displacement_map;

	vec2 mesh_dimensions;
	std::unordered_map<std::tuple<vec3, vec3, vec2>, unsigned short, vec3_vec3_vec2_hasher> vertices_map;

	std::vector<unsigned short> indices;
	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec3> tangents;
	std::vector<vec3> bitangents;
	std::vector<vec2> texture_coordinates;
	std::vector<vec3> colors;

	//used as a signal to the shader to decide if buffers and textures of this mesh have been already generated or not
	bool generate_buffers_and_textures;

	void check_accumalate_add(Vertex& vertex, int& index_counter);
	void check_accumalate_add(Triangle& triangle, int& index_counter);

	void set_as_single_face(Vertex& top_left, Vertex& bottom_left, Vertex& top_right, Vertex& bottom_right);
	
	void generate_terrain();
	void extract_from_obj_file(const std::string& file_path, const bool& uv_mapped);
	
	Mesh(const vec2& mesh_dimensions, 
		 Texture diffuse_map = Texture(RESOURCES_DIR"/diffuse_maps/default_diffuse.png", "uTexture", GL_TEXTURE0, 0), 
		 Texture normal_map = Texture(RESOURCES_DIR"/normal_maps/default_normal.png", "uNormal_map", GL_TEXTURE1, 1), 
		 Texture displacement_map = Texture(RESOURCES_DIR"/displacement_maps/default_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2)
	);

	
	Mesh(const std::string& obj_file_path, const bool& uv_mapped,
		 Texture diffuse_map = Texture(RESOURCES_DIR"/diffuse_maps/default_diffuse.png", "uTexture", GL_TEXTURE0, 0),
		 Texture normal_map = Texture(RESOURCES_DIR"/normal_maps/default_normal.png", "uNormal_map", GL_TEXTURE1, 1),
		 Texture displacement_map = Texture(RESOURCES_DIR"/displacement_maps/default_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2)
	);

};