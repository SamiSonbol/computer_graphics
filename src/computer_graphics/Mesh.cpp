#include "computer_graphics/Mesh.h"

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

	return { Tangent, Bitangent, Normal };

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
Triangle::Triangle(const vec3& A, const vec3& B, const vec3& C) : A(A), B(B), C(C) {};

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

vec4 Texture::get_pixel_color(const size_t& x, const size_t& y) {

	if (x >= this->width || y >= this->height) {

		std::cerr << "Error: Pixel coordinates are out of bounds!";
		exit(EXIT_FAILURE);

	};

	vec4 color;
	if (this->n_color_channels == 3) {
	
		color.x = this->bytes[3 * (y * this->width + x) + 0];
		color.y = this->bytes[3 * (y * this->width + x) + 1];
		color.z = this->bytes[3 * (y * this->width + x) + 2];
		color.w = 255;
		return color;

	}
	else if (this->n_color_channels == 4) {

		color.x = this->bytes[4 * (y * this->width + x) + 0];
		color.y = this->bytes[4 * (y * this->width + x) + 1];
		color.z = this->bytes[4 * (y * this->width + x) + 2];
		color.w = this->bytes[4 * (y * this->width + x) + 3];
		return color;

	};

};

void Texture::set_pixel_color(const size_t& x, const size_t& y, const vec4& color) {

	if (x >= this->width || y >= this->height) {

		std::cerr << "Error: Pixel coordinates are out of bounds!";
		//exit(EXIT_FAILURE);

	};

	if (this->n_color_channels == 3) {

		this->bytes[3 * (y * this->width + x) + 0] = (unsigned char)color.x;
		this->bytes[3 * (y * this->width + x) + 1] = (unsigned char)color.y;
		this->bytes[3 * (y * this->width + x) + 2] = (unsigned char)color.z;

	}
	else if (this->n_color_channels == 4) {

		this->bytes[4 * (y * this->width + x) + 0] = (unsigned char)color.x;
		this->bytes[4 * (y * this->width + x) + 1] = (unsigned char)color.y;
		this->bytes[4 * (y * this->width + x) + 2] = (unsigned char)color.z;
		this->bytes[4 * (y * this->width + x) + 3] = (unsigned char)color.w;

	};

};

//*Texture* class constructor
Texture::Texture(const std::string& file_path, const char* uniform_name, const unsigned int& GL_TEXTUREindex, const int& index) : 
	
	uniform_name(uniform_name), 
	GL_TEXTUREindex(GL_TEXTUREindex), 
	index(index) {

	if (!std::filesystem::exists(file_path)) {

		std::cerr << "ERROR: texture file doesnt exist!\n";
		exit(EXIT_FAILURE);

	};

	bytes = stbi_load(file_path.c_str(), &this->width, &this->height, &this->n_color_channels, 0);

};

Texture::Texture(Texture&& other) noexcept : width(other.width), height(other.height), n_color_channels(other.n_color_channels), index(other.index), GL_TEXTUREindex(other.GL_TEXTUREindex), texture_ID(other.texture_ID), uniform_name(other.uniform_name), bytes(other.bytes) {

	//nullify the moved-from object (but DO NOT free it)
	other.bytes = nullptr;
	other.uniform_name = nullptr;
	other.width = 0;
	other.height = 0;
	other.n_color_channels = 0;

};

Texture& Texture::operator=(Texture&& other) noexcept {

	if (this != &other) {

		//free existing resources (only if they exist)
		if (bytes) {

			stbi_image_free(bytes);

		};

		//move resources
		width = other.width;
		height = other.height;
		n_color_channels = other.n_color_channels;
		index = other.index;
		GL_TEXTUREindex = other.GL_TEXTUREindex;
		texture_ID = other.texture_ID;
		uniform_name = other.uniform_name;
		bytes = other.bytes;

		//nullify the moved-from object (DO NOT free it)
		other.bytes = nullptr;
		other.uniform_name = nullptr;
		other.width = 0;
		other.height = 0;
		other.n_color_channels = 0;

	};

	return *this;

};

Texture::~Texture() {

	if (bytes) {//only free if not null

		stbi_image_free(bytes);
		bytes = nullptr;

	};

};

//sets the 4 inputed vertices as 1 single face on the mesh that represents a whole image, used in tiled meshes. VIP: HAS TO BE USED BEFORE THE INPUTED VERTICES ARE USED TO CONSTRUCT TRIANGLES.
void Mesh::set_as_single_face(Vertex& top_left, Vertex& bottom_left, Vertex& top_right, Vertex& bottom_right) {

	top_left.uv = vec2(0.0, 1.0);//top left
	bottom_left.uv = vec2(0.0, 0.0);//bottom left
	top_right.uv = vec2(1.0, 1.0);//top right
	bottom_right.uv = vec2(1.0, 0.0);//bottom right

};

//checks if the inputed vertex exists with the same position and uv coords. If yes then the same index of the original vertex will be emplaced again into the *indices* vector and its TBN will be accumalated.
//If not then it will be inserted into the map and all its data will be emplaced into the respective data vectors
void Mesh::check_accumalate_add(Vertex& vertex, int& index_counter) {

	auto key = std::make_tuple(vertex.position, vertex.normal, vertex.uv);
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

void Mesh::generate_terrain() {

	/*the grid takes the width and height of our texture and fills it up as cells and not as points, hence the n_rows and n_columns is subtracted by 1 inorder to not go out of bounds when reaching the last point on a row or column.
	a  ------  c
	|          |
	|          |
	b  ------  d */
	//this->mesh_dimensions = vec2(600, 600);
	vec3 half_size(this->mesh_dimensions/2, 0);
	vec3 color(0, 255, 0);
	
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

			//creating our 2 triangle that make up the cell. VIP: NEVER CHANGE THE VERTEX ORDER/WINDUP  abc  bdc
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

void Mesh::extract_from_obj_file(const std::string& file_path, const bool& uv_mapped) {

	std::vector<vec3> temp_positions;
	std::vector<vec3> temp_normals;
	std::vector<vec2> temp_uv;
	int index_counter = 0;
	vec3 vec(0, 0, 0);
	Vertex a(0, 0, 0), b(0, 0, 0), c(0, 0, 0), d(0, 0, 0); std::array<Vertex, 4> vertices = { a, b, c, d };
	Vertex vertex(0, 0, 0);

	std::vector<std::string> lines = read_file_by_line(file_path);
	std::vector<std::string> line_tokens;
	std::vector<std::string> face_tokens;
	std::string attribute_type1;
	std::string attribute_type2;
	
	if (!uv_mapped) {

		std::regex expression(R"((\d+)//(\d+))");
		for (auto& line : lines) {

			attribute_type1 = line.substr(0, 1);
			attribute_type2 = line.substr(0, 2);

			if (attribute_type2 == "vn") {

				line_tokens.clear();
				line_tokens = tokenise_data(line.substr(3), ' ');
				vec.x = std::stof(line_tokens[0].c_str());
				vec.y = std::stof(line_tokens[1].c_str());
				vec.z = std::stof(line_tokens[2].c_str());
				temp_normals.emplace_back(vec);

			}
			else if (attribute_type1 == "v") {

				line_tokens.clear();
				line_tokens = tokenise_data(line.substr(2), ' ');
				vec.x = std::stof(line_tokens[0].c_str());
				vec.y = std::stof(line_tokens[1].c_str());
				vec.z = std::stof(line_tokens[2].c_str());
				temp_positions.emplace_back(vec);

				vec.x = 0.0;
				vec.y = 0.0;
				temp_uv.emplace_back(vec.x, vec.y);

			}
			else if (attribute_type1 == "f") {

				line_tokens.clear();
				line_tokens = tokenise_data(line.substr(2), ' ');
				for (int i = 0; i < line_tokens.size(); i++) {

					face_tokens.clear();
					face_tokens = tokenise_data(line_tokens[i], expression);
					int v_index = std::stoi(face_tokens[0].c_str()) - 1;
					int vn_index = std::stoi(face_tokens[1].c_str()) - 1;

					vertices[i].position = temp_positions[v_index];
					vertices[i].normal = temp_normals[vn_index];
					vertices[i].tangent = vec3(1, 0, 0);
					vertices[i].bitangent = vec3(0, 1, 0);
					vertices[i].color = vec3(255, 0, 0);				

				};

				set_as_single_face(vertices[0], vertices[1], vertices[2], vertices[3]);
				Triangle A(vertices[0], vertices[1], vertices[2]);
				Triangle B(vertices[0], vertices[2], vertices[3]);
				check_accumalate_add(A, index_counter);
				check_accumalate_add(B, index_counter);

			};

		};

	}
	else {

		std::regex expression(R"((\d+)/(\d*)/(\d+))");
		for (auto& line : lines) {

			attribute_type1 = line.substr(0, 1);
			attribute_type2 = line.substr(0, 2);

			if (attribute_type2 == "vn") {

				line_tokens.clear();
				line_tokens = tokenise_data(line.substr(3), ' ');
				vec.x = std::stof(line_tokens[0].c_str());
				vec.y = std::stof(line_tokens[1].c_str());
				vec.z = std::stof(line_tokens[2].c_str());
				temp_normals.emplace_back(vec.x, vec.y, vec.z);

			}
			else if (attribute_type2 == "vt") {

				line_tokens.clear();
				line_tokens = tokenise_data(line.substr(3), ' ');
				vec.x = std::stof(line_tokens[0].c_str());
				vec.y = std::stof(line_tokens[1].c_str());
				temp_uv.emplace_back(vec.x, vec.y);

			}
			else if (attribute_type1 == "v") {

				line_tokens.clear();
				line_tokens = tokenise_data(line.substr(2), ' ');
				vec.x = std::stof(line_tokens[0].c_str());
				vec.y = std::stof(line_tokens[1].c_str());
				vec.z = std::stof(line_tokens[2].c_str());
				temp_positions.emplace_back(vec.x, vec.y, vec.z);

			}
			else if (attribute_type1 == "f") {

				line_tokens.clear();
				line_tokens = tokenise_data(line.substr(2), ' ');
				for (int i = 0; i < line_tokens.size(); i++) {

					face_tokens.clear();
					face_tokens = tokenise_data(line_tokens[i], expression);
					int v_index = std::stoi(face_tokens[0].c_str()) - 1;
					int vt_index = std::stoi(face_tokens[1].c_str()) - 1;
					int vn_index = std::stoi(face_tokens[2].c_str()) - 1;

					vertices[i].position = temp_positions[v_index];
					vertices[i].normal = temp_normals[vn_index];
					vertices[i].tangent = vec3(1, 0, 0);
					vertices[i].bitangent = vec3(0, 1, 0);
					vertices[i].uv = temp_uv[vt_index];
					vertices[i].color = vec3(255, 0, 0);

				};

				Triangle A(vertices[0], vertices[1], vertices[2]);
				Triangle B(vertices[0], vertices[2], vertices[3]);
				check_accumalate_add(A, index_counter);
				check_accumalate_add(B, index_counter);

			};

		};

	};

	//normalizing the TBN of each vertex after it was accumalated
	for (int i = 0; i < this->positions.size(); i++) {

		this->normals[i].normalize();
		this->tangents[i].normalize();
		this->bitangents[i].normalize();

	};

};

Mesh::Mesh(const vec2& mesh_dimensions, Texture diffuse_map, Texture normal_map, Texture displacement_map) :

	mesh_dimensions(mesh_dimensions),
	diffuse_map(std::move(diffuse_map)),
	normal_map(std::move(normal_map)),
	displacement_map(std::move(displacement_map)) {

	generate_terrain();
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;
	
	this->vertices_map.clear();
	std::cout << "cleared map\n";

};

Mesh::Mesh(const std::string& obj_file_path, const bool& uv_mapped, Texture diffuse_map, Texture normal_map, Texture displacement_map) :

	diffuse_map(std::move(diffuse_map)),
	normal_map(std::move(normal_map)),
	displacement_map(std::move(displacement_map)) {

	extract_from_obj_file(obj_file_path, uv_mapped);
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;
	
	this->vertices_map.clear();
	std::cout << "cleared map\n";

};

