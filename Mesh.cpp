#include "Mesh.h"

//*Vertex* class constructors
Vertex::Vertex(const vec3& position) : position(position) {};
Vertex::Vertex(const float& x, const float& y, const float& z) : position(x, y, z) {};


//moves the vertices of the Triangle by the inputted offset
void Triangle::offset_positions(const vec3& offset) {

	this->A.position += offset;
	this->B.position += offset;
	this->C.position += offset;

};

//returns the Tangent, Bitangent and Normal of the Triangle face.
std::array<vec3, 3> Triangle::compute_TBN() {

	vec3 AB = this->B.position - this->A.position;
	vec3 AC = this->C.position - this->A.position;

	vec2 uv0 = this->A.uv;
	vec2 uv1 = this->B.uv;
	vec2 uv2 = this->C.uv;

	vec2 delta_uv1 = uv1 - uv0;
	vec2 delta_uv2 = uv2 - uv0;

	float denominator = delta_uv1.x * delta_uv2.y - delta_uv2.x * delta_uv1.y;
	float F = 1.0f / denominator;
	vec3 Tangent = (AB * delta_uv2.y - AC * delta_uv1.y) * F;
	vec3 Bitangent = (AB * -delta_uv2.x + AC * delta_uv1.x) * F;

	vec3 Normal = AB.cross(AC);

	return { Tangent.normalize(), Bitangent.normalize(), Normal.normalize() };

};

//sets the TBN of each vertex to the Triangle face TBN
void Triangle::update_TBNs() {

	std::array<vec3, 3> TBN = this->compute_TBN();
	this->A.tangent = TBN[0];
	this->B.tangent = TBN[0];
	this->C.tangent = TBN[0];

	this->A.bitangent = TBN[1];
	this->B.bitangent = TBN[1];
	this->C.bitangent = TBN[1];

	this->A.normal = TBN[2];
	this->B.normal = TBN[2];
	this->C.normal = TBN[2];

};

//updates the UVs of the vertices in said Triangle, such that the triangle will be merged as a face(part of the whole image) on the mesh. VIP: THIS FUNCTION WORKS ONLY IF THE VERTEX POSITIONS ARE [0, Dimensions]
void Triangle::merge_face(const vec2& mesh_dimensions) {

	this->A.uv = this->A.position.xy() / mesh_dimensions;
	this->B.uv = this->B.position.xy() / mesh_dimensions;
	this->C.uv = this->C.position.xy() / mesh_dimensions;

};

//sets the vertices color of the Triangle
void Triangle::set_color(const vec3& color) {

	this->A.color = color;
	this->B.color = color;
	this->C.color = color;

};

//*Triangle* class constructor
Triangle::Triangle(const Vertex& A, const Vertex& B, const Vertex& C) : A(A), B(B), C(C) {};

std::vector<float> Texture::generate_normal_map() {

 std::vector<float> normals;

 for (int j = 0; j < this->height; ++j) {

	for (int i = 0; i < this->width; ++i) {

		int index = (j * this->width + i) * 3;

		float x = bytes[index] / 255.0f * 2.0f - 1.0f;
		float y = bytes[index + 1] / 255.0f * 2.0f - 1.0f;
		float z = bytes[index + 2] / 255.0f * 2.0f - 1.0f;

		vec3 v = vec3(x, y, z);
		v = v.normalize();

		normals.emplace_back(v.x);
		normals.emplace_back(v.y);
		normals.emplace_back(v.z);

	};

 };

 return normals;

};

//freeing the bytes so we wont have memory leeks
void Texture::free_bytes() {

	stbi_image_free(this->bytes);

};

//*Texture* class constructor
Texture::Texture(const std::string& file_path, const char* uniform_name, const unsigned int& GL_TEXTUREindex, const int& index) :

	bytes(stbi_load(file_path.c_str(), &this->width, &this->height, &this->n_color_channels, 0)),
	uniform_name(uniform_name),
	GL_TEXTUREindex(GL_TEXTUREindex),
	index(index) {

};

//*Texture* class destructor
Texture::~Texture() {

	this->uniform_name = nullptr;
	this->bytes = nullptr;

};

//checks if the inputed vertex exists with the same position and uv coords. If yes then the same index of the original vertex will be emplaced again into the *indices* vector and its TBN will be accumalated.
//If not then it will be inserted into the map and all its data will be emplaced into the respective data vectors
void Mesh::check_accumalate_add(Vertex& vertex, int& index_counter) {

	auto key = std::make_pair(vertex.position, vertex.uv);
	auto iterator = this->vertices_map.find(key);
	if (iterator != this->vertices_map.end()) {

		this->indices.emplace_back(iterator->second);

		this->normals[iterator->second] += vertex.normal;
		this->tangents[iterator->second] += vertex.tangent;
		this->bitangents[iterator->second] += vertex.bitangent;

	}
	else {

		vertex.index = index_counter;
		this->vertices_map[key] = index_counter;
		this->indices.emplace_back(index_counter);	
		
		this->positions.emplace_back(vertex.position);
		this->normals.emplace_back(vertex.normal);
		this->tangents.emplace_back(vertex.tangent);
		this->bitangents.emplace_back(vertex.bitangent);
		this->texture_coordinates.emplace_back(vertex.uv);
		this->colors.emplace_back(vertex.color);

		index_counter++;	

	};
	
};

//override
void Mesh::check_accumalate_add(Triangle& triangle, int& index_counter) {

	check_accumalate_add(triangle.A, index_counter);
	check_accumalate_add(triangle.B, index_counter);
	check_accumalate_add(triangle.C, index_counter);

};

//sets the 4 inputed vertices as 1 single face on the mesh that represents a whole image, used in tiled meshes. VIP: HAS TO BE USED BEFORE THE INPUTED VERTICES ARE USED TO CONSTRUCT TRIANGLES.
void Mesh::set_as_single_face(Vertex& top_left, Vertex& bottom_left, Vertex& top_right, Vertex& bottom_right) {

	top_left.uv = vec2(0.0, 1.0);//top left
	bottom_left.uv = vec2(0.0, 0.0);//bottom left
	top_right.uv = vec2(1.0, 1.0);//top right
	bottom_right.uv = vec2(1.0, 0.0);//bottom right

};

void Mesh::init_grid() {

	/*the grid takes the width and height of our texture and fills it up as cells and not as points, hence the n_rows and n_columns is subtracted by 1 inorder to not go out of bounds when reaching the last point on a row or column.
	a  ------  c
	|          |
	|          |
	b  ------  d */
	//this->mesh_dimensions = vec2(600, 600);
	vec3 half_size(this->mesh_dimensions/2, 0);
	vec3 color(1, 1, 1);
	
	//we traverse the grid by going to every row where we work on every column on said row.
	int index_counter = 0;
	for (int y = 0; y < this->mesh_dimensions.y; ++y) {

		for (int x = 0; x < this->mesh_dimensions.x; ++x) {

			int next_y = y + 1;
			int next_x = x + 1;

			//creating our cell structure
			Vertex a(x, next_y, 0);//top left
			Vertex b(x, y, 0);//bottom left
			Vertex c(next_x, next_y, 0);//top right
			Vertex d(next_x, y, 0);//bottom right

			//creating our 2 triangle that make up the cell. VIP: NEVER CHANGE THE VERTEX ORDER/WINDUP
			Triangle A(a, b, c); 
			Triangle B(b, d, c);
			
			/*std::cout << "at gridPosition: " << x << ", " << y << std::endl;
			std::cout << "a uv: "; print_vec(A.A.uv);
			std::cout << "b uv: "; print_vec(A.B.uv);
			std::cout << "c uv: "; print_vec(A.C.uv);
			std::cout << "d uv: "; print_vec(B.C.uv);*/

			/*std::cout << "at gridPosition: " << x << ", " << y << std::endl;
			std::cout << "a pos: "; print_vec(A.A.position);
			std::cout << "b pos: "; print_vec(A.B.position);
			std::cout << "c pos: "; print_vec(A.C.position);
			std::cout << "d pos: "; print_vec(B.C.position);*/

			//setting the UVs for the vertices in each Triangle(has to be done before all position changes)
			A.merge_face(this->mesh_dimensions);
			B.merge_face(this->mesh_dimensions);
			
			//shifting the positions to the left(since we start at position 0,0,0) by half the size of the image so the final image would be drawn in the middle of the screen
			A.offset_positions(half_size * -1);
			B.offset_positions(half_size * -1);

			//TBN vectors calculations(has to be done after all position and UV changes)
			A.update_TBNs();
			B.update_TBNs();

			//setting the triangle color (color will not be used whilst using textures)
			A.set_color(color);
			B.set_color(color);

			//checking for duplicates, accumalating TBNs, adding to our data vectors(buffers)
			check_accumalate_add(A, index_counter);
			check_accumalate_add(B, index_counter);

		};

	};

	//normalizing the TBN of each vertex after it was accumalated
	for (int i = 0; i < this->positions.size(); i++) {

		this->normals[i].normalize();
		this->tangents[i].normalize();
		this->bitangents[i].normalize();

	};

};

void Mesh::fill_data() {

	init_grid();

	std::cout << "width: " << this->texture.width << " height: " << this->texture.height << std::endl;
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;

};//"fill data" bracket

Mesh::Mesh(Texture& texture, const vec2& mesh_dimensions) : texture(texture), mesh_dimensions(mesh_dimensions) {

	fill_data();
	std::cout << "filled data\n";

	this->vertices_map.clear();
	std::cout << "cleared Grid\n";

};
