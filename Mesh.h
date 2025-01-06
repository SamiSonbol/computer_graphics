#pragma once
#include <iostream>
#include <vector>
#include "Math.h"
#include <stb/stb_image.h>

class Texture {

 public:

	int width, height, n_color_channels;

	unsigned int GL_TEXTUREindex;

	int index;

	const char* uniform_name;

	unsigned char* bytes;

	unsigned int txt;

	Texture(const std::string& file_path, const char* uniform_name, const unsigned int& GL_TEXTUREindex, const int& index);

	std::vector<float> generate_normal_map();

	~Texture();

};

class Mesh {

 public:

	struct Vertex {

	 public:

		vec3 position;
		unsigned short index;
		bool duplicated = false;

		Vertex(const vec3& position, const int& index);

		Vertex(const float& x, const float& y, const float& z, const int& index);

	};

    struct Triangle {

	 public:

		Vertex a;
		Vertex b;
		Vertex c;

		Triangle(const Vertex& a, const Vertex& b, const Vertex& c);

	};

	struct Square {

	 public:

		Triangle A;
		Triangle B;

		Square(const Triangle& A, const Triangle& B);

	};

	Texture* texture;
	std::vector<std::vector<Square>> Grid;

	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<unsigned short> indices;
	std::vector<float> texture_coordinates;
	std::vector<float> colors;

	//checks if the vertex we have is a duplicate of another vertex on the we are comparing it with triangle, if it is a duplicate then it takes the lower index number of both vertices
	void update_vertex_indices(Vertex& vertex, Triangle& triangle);
	void update_vertex_indices(Triangle& A, Triangle& B);
	void update_vertex_indices(Square& A, Square& B);

	void check_for_duplicates_and_insert(Triangle& triangle);
	void check_for_duplicates_and_insert(Square& square);

	void get_positions();
	void compute_normals();
	void get_texture_coordinates();

	//uses the function "update_vertex_indices" to update the indices of all vertices and avoid adding duplicate vertices to the "indices" vector, which will later be used as index buffer
	void sort_indices();

	void fill_data();

	Mesh(Texture* t);

};