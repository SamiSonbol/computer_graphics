#include "Mesh.h"

Texture::Texture(const std::string& file_path, const char* uniform_name, const unsigned int& GL_TEXTUREindex, const int& index) :

	bytes(stbi_load(file_path.c_str(), &this->width, &this->height, &this->n_color_channels, 0)),
	uniform_name(uniform_name),
	GL_TEXTUREindex(GL_TEXTUREindex),
	index(index) {

};

std::vector<float> Texture::generate_normal_map() {

 std::vector<float> normals;

 for (int j = 0; j < this->height; ++j) {

	for (int i = 0; i < this->width; ++i) {

		int index = (j * this->width + i) * 3;

		float x = bytes[index] / 255.0f * 2.0f - 1.0f;
		float y = bytes[index + 1] / 255.0f * 2.0f - 1.0f;
		float z = bytes[index + 2] / 255.0f * 2.0f - 1.0f;

		vec3 v = vec3(x, y, z);
		v.normalize();

		normals.emplace_back(v.x);
		normals.emplace_back(v.y);
		normals.emplace_back(v.z);

	};

 };

 return normals;

};

Texture::~Texture() {

	this->uniform_name = nullptr;
	this->bytes = nullptr;

};


Mesh::Vertex::Vertex(const vec3& position, const int& index) : position(position), index(index) {};

Mesh::Vertex::Vertex(const float& x, const float& y, const float& z, const int& index) : position(x, y, z), index(index) {};

Mesh::Triangle::Triangle(const Vertex& a, const Vertex& b, const Vertex& c) : a(a), b(b), c(c) {};

Mesh::Square::Square(const Triangle& A, const Triangle& B) : A(A), B(B) {};

//checks if the vertex we have is a duplicate of another vertex on the we are comparing it with triangle, if it is a duplicate then it takes the lower index number of both vertices
void Mesh::update_vertex_indices(Vertex& vertex, Triangle& triangle) {
 
	if (vertex.position == triangle.a.position) {
		vertex.index = std::min(triangle.a.index, vertex.index);
		triangle.a.index = vertex.index;
		triangle.a.duplicated = true;
	}
	else if (vertex.position == triangle.b.position) {
		vertex.index = std::min(triangle.b.index, vertex.index);
		triangle.b.index = vertex.index;
		triangle.b.duplicated = true;
	}
	else if (vertex.position == triangle.c.position) {
		vertex.index = std::min(triangle.c.index, vertex.index);
		triangle.c.index = vertex.index;
		triangle.c.duplicated = true;
	};

};

void Mesh::update_vertex_indices(Triangle& A, Triangle& B) {

	update_vertex_indices(A.a, B);
	update_vertex_indices(A.b, B);
	update_vertex_indices(A.c, B);

};

void Mesh::update_vertex_indices(Square& A, Square& B) {

	update_vertex_indices(A.A, A.B);
	update_vertex_indices(B.A, B.B);

	update_vertex_indices(A.A, B.A);
	update_vertex_indices(A.B, B.B);

	update_vertex_indices(A.A, B.B);
	update_vertex_indices(A.B, B.A);

};

void Mesh::check_for_duplicates_and_insert(Triangle& triangle) {

	this->positions.emplace_back(triangle.a.position.x);
	this->positions.emplace_back(triangle.a.position.y);
	this->positions.emplace_back(triangle.a.position.z);
	if (!triangle.a.duplicated) {

		this->indices.emplace_back(triangle.a.index);

	};

	this->positions.emplace_back(triangle.b.position.x);
	this->positions.emplace_back(triangle.b.position.y);
	this->positions.emplace_back(triangle.b.position.z);
	if (!triangle.b.duplicated) {	

		this->indices.emplace_back(triangle.b.index);

	};

	this->positions.emplace_back(triangle.c.position.x);
	this->positions.emplace_back(triangle.c.position.y);
	this->positions.emplace_back(triangle.c.position.z);
	if (!triangle.c.duplicated) {

		this->indices.emplace_back(triangle.c.index);

	};

};

void Mesh::check_for_duplicates_and_insert(Square& square) {

	check_for_duplicates_and_insert(square.A);
	check_for_duplicates_and_insert(square.B);

};

void Mesh::get_positions() {

    int n_columns = this->texture->width - 1;
	int n_rows = this->texture->height - 1;

	//since i have no default constructor for Square i have to init the vector in this long way
	Vertex a(vec3(0, 0, 0), 0);
	Triangle A(a, a, a);
	Square s(A, A);
	this->Grid.resize(n_rows, std::vector<Square>(n_columns, s));

	int index = 0;
	for (int y = 0; y < n_rows; ++y) {

		for (int x = 0; x < n_columns; ++x) {

			int next_y = (y + 1) % n_rows;
			int next_x = (x + 1) % n_columns;

			Vertex a(vec3(x, y, 0), index);
			Vertex b(vec3(x, next_y, 0), index + 1);
			Vertex c(vec3(next_x, y, 0), index + 2);
			Vertex d(vec3(next_x, next_y, 0), index + 3);

			a.position.normalize();
			b.position.normalize();
			c.position.normalize();
			d.position.normalize();

			Triangle A(a, b, c);
			Triangle B(b, c, d);

			this->Grid[y][x] = Square(A, B);

			++index;

		};

	};

};

void Mesh::compute_normals() {

	this->normals.reserve(this->positions.size() / 3);
	this->colors.reserve(this->positions.size() / 3);
	for (int i = 0; i < this->positions.size() / 3; ++i) {

		vec3 n = vec3(this->positions[3 * i], this->positions[3 * i + 1], this->positions[3 * i + 2]);

		this->normals.emplace_back(n.x);
		this->normals.emplace_back(n.y);
		this->normals.emplace_back(n.z);

		this->colors.emplace_back(0);
		this->colors.emplace_back(0);
		this->colors.emplace_back(1);

	};

};

//uses the function "update_vertex_indices" to update the indices of all vertices and avoid adding duplicate vertices to the "indices" vector, which will later be used as index buffer
void Mesh::sort_indices() {

	int n_columns = this->texture->width - 1;
	int n_rows = this->texture->height - 1;

	for (int y = 0; y < n_rows; ++y) {

		for (int x = 0; x < n_columns; ++x) {

			int next_y = (y + 1) % n_rows;
			int next_x = (x + 1) % n_columns;

			update_vertex_indices(this->Grid[y][x], this->Grid[y][next_x]);
			update_vertex_indices(this->Grid[y][x], this->Grid[next_y][x]);
			update_vertex_indices(this->Grid[y][x], this->Grid[next_y][next_x]);

		};

	};

	for (int y = 0; y < n_rows; ++y) {

		for (int x = 0; x < n_columns; ++x) {

			check_for_duplicates_and_insert(this->Grid[y][x]);

		};

	};

};

void Mesh::get_texture_coordinates() {

	int n_columns = this->texture->width - 1;
	int n_rows = this->texture->height - 1;
	int n = n_rows * n_columns;
	for (int i = 0; i < n; ++i) {

		this->texture_coordinates.emplace_back(0.0);
		this->texture_coordinates.emplace_back(0.0);

		this->texture_coordinates.emplace_back(1.0);
		this->texture_coordinates.emplace_back(0.0);

		this->texture_coordinates.emplace_back(1.0);
		this->texture_coordinates.emplace_back(1.0);

		this->texture_coordinates.emplace_back(0.0);
		this->texture_coordinates.emplace_back(1.0);

	};

};

void Mesh::fill_data() {

	get_positions();
	compute_normals();
	sort_indices();
	get_texture_coordinates();

	std::cout << "n_vertices: " << positions.size() / 3 << " Grid size: " << this->Grid.size() << "x" << this->Grid[0].size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "ended";

};//"fill data" bracket

Mesh::Mesh(Texture* t) : texture(t) {

	std::cout << "width: " << this->texture->width << "height: " << this->texture->height << std::endl;

	fill_data();

	this->Grid.clear();

};
