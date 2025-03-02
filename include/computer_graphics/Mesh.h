#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include "computer_graphics/Math.h"
#include "computer_graphics/File.h"
#include "computer_graphics/Point_Cloud.h"

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
	Texture(const std::filesystem::path& file_path = "EMPTY TEXTURE", const char* uniform_name = "EMPTY TEXTURE", const unsigned int& GL_TEXTUREindex = -1, const int& index = -1);

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

	std::vector<unsigned int> indices;
	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec3> tangents;
	std::vector<vec3> bitangents;
	std::vector<vec2> texture_coordinates;
	std::vector<vec3> colors;

	//used as a signal to the shader to decide if buffers and textures of this mesh have been already generated or not
	bool generate_buffers_and_textures;
	bool draw_as_elements;

	vec3 minimum_bounds;
	vec3 maximum_bounds;

	void add_without_check(Vertex& vertex, int& index_counter);
	void add_without_check(Triangle& triangle, int& index_counter);

	void check_accumalate_add(Vertex& vertex, int& index_counter);
	void check_accumalate_add(Triangle& triangle, int& index_counter);
	
	void set_as_single_face(Vertex& top_left, Vertex& bottom_left, Vertex& top_right, Vertex& bottom_right);
	
	void generate_terrain(const uint8_t& ADD_VERTICES);
	void extract_from_OBJ_file(const std::filesystem::path& file_path, const uint8_t& ADD_VERTICES);
	void extract_from_LAS_file(const std::filesystem::path& file_path);

 private:

	Mesh(const std::vector<vec3>& positions);

	Mesh(const vec2& mesh_dimensions, const uint8_t& ADD_VERTICES, Texture&& diffuse_map, Texture&& normal_map, Texture&& displacement_map);
	Mesh(const std::filesystem::path& obj_file_path, const uint8_t& ADD_VERTICES, Texture&& diffuse_map, Texture&& normal_map, Texture&& displacement_map);

	Mesh(const vec2& mesh_dimensions, const std::filesystem::path& path_diffuse_map_file, const uint8_t& ADD_VERTICES, const std::filesystem::path& path_normal_map_file, const std::filesystem::path& path_displacement_map_file);
	Mesh(const std::filesystem::path& obj_file_path, const std::filesystem::path& path_diffuse_map_file, const uint8_t& ADD_VERTICES, const std::filesystem::path& path_normal_map_file, const std::filesystem::path& path_displacement_map_file);

	Mesh(const vec2& mesh_dimensions, const std::filesystem::path& path_maps_folder, const uint8_t& ADD_VERTICES);
	Mesh(const std::filesystem::path& obj_file_path, const std::filesystem::path& path_maps_folder, const uint8_t& ADD_VERTICES);

	Mesh(const std::filesystem::path& las_file_path);

 public:

	static constexpr uint8_t ADD_ONLY_UNIQUE_VERTICES = 0;
	static constexpr uint8_t ADD_ALL_VERTICES = 1;

	static Mesh empty();
	static Mesh empty_quad();

	static Mesh from_procedural_Texture(const vec2& mesh_dimensions,
		const uint8_t& ADD_VERTICES = ADD_ONLY_UNIQUE_VERTICES,
		Texture&& diffuse_map = Texture(RESOURCES_DIR"/texture_maps/default/default_diffuse.png", "uTexture", GL_TEXTURE0, 0),
		Texture&& normal_map = Texture(RESOURCES_DIR"/texture_maps/default/default_normal.png", "uNormal_map", GL_TEXTURE1, 1),
		Texture&& displacement_map = Texture(RESOURCES_DIR"/texture_maps/default/default_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2)
	);
	static Mesh from_OBJ_Texture(const std::filesystem::path& obj_file_path,
		const uint8_t& ADD_VERTICES = ADD_ALL_VERTICES,
		Texture&& diffuse_map = Texture(RESOURCES_DIR"/texture_maps/default/default_diffuse.png", "uTexture", GL_TEXTURE0, 0),
		Texture&& normal_map = Texture(RESOURCES_DIR"/texture_maps/default/default_normal.png", "uNormal_map", GL_TEXTURE1, 1),
		Texture&& displacement_map = Texture(RESOURCES_DIR"/texture_maps/default/default_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2)
	);

	static Mesh from_procedural_files(const vec2& mesh_dimensions, 
		const std::filesystem::path& path_diffuse_map_file, 
		const uint8_t& ADD_VERTICES = ADD_ALL_VERTICES,
		const std::filesystem::path& path_normal_map_file = RESOURCES_DIR"/texture_maps/default/default_normal.png", 
		const std::filesystem::path& path_displacement_map_file = RESOURCES_DIR"/texture_maps/default/default_displacement.png"
	);
	static Mesh from_OBJ_files(const std::filesystem::path& obj_file_path, 
		const std::filesystem::path& path_diffuse_map_file, 
		const uint8_t& ADD_VERTICES = ADD_ALL_VERTICES,
		const std::filesystem::path& path_normal_map_file = RESOURCES_DIR"/texture_maps/default/default_normal.png", 
		const std::filesystem::path& path_displacement_map_file = RESOURCES_DIR"/texture_maps/default/default_displacement.png"
	);

	static Mesh from_procedural_folder(const vec2& mesh_dimensions, const std::filesystem::path& path_maps_folder, const uint8_t& ADD_VERTICES = ADD_ALL_VERTICES);
	static Mesh from_OBJ_folder(const std::filesystem::path& obj_file_path, const std::filesystem::path& path_maps_folder, const uint8_t& ADD_VERTICES = ADD_ALL_VERTICES);
	static Mesh from_LAS(const std::filesystem::path& las_file_path);

};