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
vec3 Mesh::Triangle::get_triangle_normal() {

	vec3 AB = this->B.position - this->A.position;
	vec3 AC = this->C.position - this->A.position;

	return AB.cross(AC).normalize();

};
Mesh::Triangle::Triangle(const Vertex& A, const Vertex& B, const Vertex& C) : A(A), B(B), C(C) {};
Mesh::Square::Square(const Triangle& A, const Triangle& B) : A(A), B(B) {};

//checks if the vertex we have is a duplicate of another vertex on the we are comparing it with triangle, if it is a duplicate then it takes the lower index number of both vertices
void Mesh::update_vertex_indices(Vertex& vertex, Triangle& triangle) {
 
	if (vertex.position == triangle.A.position) {
		vertex.index = std::min(triangle.A.index, vertex.index);
		triangle.A.index = vertex.index;
		triangle.A.duplicated = true;
	}
	else if (vertex.position == triangle.B.position) {
		vertex.index = std::min(triangle.B.index, vertex.index);
		triangle.B.index = vertex.index;
		triangle.B.duplicated = true;
	}
	else if (vertex.position == triangle.C.position) {
		vertex.index = std::min(triangle.C.index, vertex.index);
		triangle.C.index = vertex.index;
		triangle.C.duplicated = true;
	};

};

void Mesh::update_vertex_indices(Triangle& A, Triangle& B) {

	update_vertex_indices(A.A, B);
	update_vertex_indices(A.B, B);
	update_vertex_indices(A.C, B);

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

	this->positions.emplace_back(triangle.A.position.x);
	this->positions.emplace_back(triangle.A.position.y);
	this->positions.emplace_back(triangle.A.position.z);
	if (!triangle.A.duplicated) {

		this->indices.emplace_back(triangle.A.index);

	};

	this->positions.emplace_back(triangle.B.position.x);
	this->positions.emplace_back(triangle.B.position.y);
	this->positions.emplace_back(triangle.B.position.z);
	if (!triangle.B.duplicated) {	

		this->indices.emplace_back(triangle.B.index);

	};

	this->positions.emplace_back(triangle.C.position.x);
	this->positions.emplace_back(triangle.C.position.y);
	this->positions.emplace_back(triangle.C.position.z);
	if (!triangle.C.duplicated) {

		this->indices.emplace_back(triangle.C.index);

	};

};

void Mesh::check_for_duplicates_and_insert(Square& square) {

	check_for_duplicates_and_insert(square.A);
	check_for_duplicates_and_insert(square.B);

};

void Mesh::init_grid() {

    int n_columns = this->texture.width - 1;
	int n_rows = this->texture.height - 1;

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

void Mesh::get_normals() {

	int n_columns = this->texture.width - 1;
	int n_rows = this->texture.height - 1;
	for (int y = 0; y < n_rows; ++y) {

		for (int x = 0; x < n_columns; ++x) {

			vec3 normal_A = Grid[y][x].A.get_triangle_normal();
			this->normals.emplace_back(normal_A.x);
			this->normals.emplace_back(normal_A.y);
			this->normals.emplace_back(normal_A.z);

			vec3 normal_B = Grid[y][x].B.get_triangle_normal();
			this->normals.emplace_back(normal_B.x);
			this->normals.emplace_back(normal_B.y);
			this->normals.emplace_back(normal_B.z);

		};

	};

};

//uses the function "update_vertex_indices" to update the indices of all vertices and avoid adding duplicate vertices to the "indices" vector, which will later be used as index buffer
void Mesh::get_indices() {
	
	int n_columns = this->texture.width - 1;
	int n_rows = this->texture.height - 1;
	for (int y = 0; y < n_rows; ++y) {

		for (int x = 0; x < n_columns; ++x) {

			int next_y = (y + 1) % n_rows;
			int next_x = (x + 1) % n_columns;

			update_vertex_indices(this->Grid[y][x], this->Grid[y][next_x]);
			update_vertex_indices(this->Grid[y][x], this->Grid[next_y][x]);
			update_vertex_indices(this->Grid[y][x], this->Grid[next_y][next_x]);

		};

	};

};

void Mesh::get_positions() {

	int n_columns = this->texture.width - 1;
	int n_rows = this->texture.height - 1;
	for (int y = 0; y < n_rows; ++y) {

		for (int x = 0; x < n_columns; ++x) {

			check_for_duplicates_and_insert(this->Grid[y][x]);

		};

	};

};

void Mesh::get_texture_coordinates() {

	int n_columns = this->texture.width - 1;
	int n_rows = this->texture.height - 1;
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

void Mesh::get_colors() {

	this->colors.reserve(this->positions.size() / 3);
	for (int i = 0; i < this->positions.size() / 3; ++i) {

		this->colors.emplace_back(0);
		this->colors.emplace_back(0);
		this->colors.emplace_back(1);

	};

};

void Mesh::fill_data() {

	init_grid();
	get_normals();
	get_indices();	
	get_positions();
	get_texture_coordinates();
	get_colors();

	std::cout << "width: " << this->texture.width << " height: " << this->texture.height << std::endl;
	std::cout << "n_vertices: " << positions.size() / 3 << " Grid size: " << this->Grid.size() << " x " << this->Grid[0].size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;

};//"fill data" bracket

Mesh::Mesh(Texture& texture) : texture(texture) {

	fill_data();
	std::cout << "filled data\n";

	this->Grid.clear();
	std::cout << "cleared Grid\n";

};
