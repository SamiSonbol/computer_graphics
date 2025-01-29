#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <filesystem>

#include <stb_image/stb_image.h>
#include "computer_graphics/Math.h"

//struct used to hash the unordered_map we are using in our *Mesh* class
struct vec3_and_vec2_hasher {

	std::size_t operator()(const std::pair<vec3, vec2>& pair) const {

		std::size_t h1 = vec3_hasher()(pair.first);
		std::size_t h2 = vec2_hasher()(pair.second);

		std::size_t seed = h1;
		seed = hash_combine(seed, h2);
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

};

class Texture {

 public:

	int width, height, n_color_channels, index;
	unsigned int GL_TEXTUREindex;

	unsigned int texture_ID;//vipNote: dont ever make this a pointer
	const char* uniform_name;
	unsigned char* bytes;

	void free_bytes();
	bool check_if_file_exists(const std::string& file_path);
	Texture(const std::string& file_path, const char* uniform_name, const unsigned int& GL_TEXTUREindex, const int& index);

	std::vector<float> generate_normal_map();

	~Texture();

};

class Mesh {

 public:

	Texture texture;
	vec2 mesh_dimensions;
	std::unordered_map<std::pair<vec3, vec2>, unsigned short, vec3_and_vec2_hasher> vertices_map;

	std::vector<unsigned short> indices;
	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec3> tangents;
	std::vector<vec3> bitangents;
	std::vector<vec2> texture_coordinates;
	std::vector<vec3> colors;

	void check_accumalate_add(Vertex& vertex, int& index_counter);
	void check_accumalate_add(Triangle& triangle, int& index_counter);

	void set_as_single_face(Vertex& top_left, Vertex& bottom_left, Vertex& top_right, Vertex& bottom_right);
	
	void init_grid();
	void fill_data();

	Mesh(Texture& texture, const vec2& mesh_dimensions);

};