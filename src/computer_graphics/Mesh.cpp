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

	exit_if_file_doesnt_exist(file_path);
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

//adds the vertex without checking if it has a duplicate in the buffer
void Mesh::add_without_check(Vertex& vertex, int& index_counter) {

	vertex.index = index_counter;
	this->indices.emplace_back(index_counter);

	this->positions.emplace_back(vertex.position);
	this->normals.emplace_back(vertex.normal);
	this->tangents.emplace_back(vertex.tangent);
	this->bitangents.emplace_back(vertex.bitangent);
	this->texture_coordinates.emplace_back(vertex.uv);
	this->colors.emplace_back(vertex.color);

	index_counter++;

};
//override
void Mesh::add_without_check(Triangle& triangle, int& index_counter) {

	add_without_check(triangle.A, index_counter);
	add_without_check(triangle.B, index_counter);
	add_without_check(triangle.C, index_counter);

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

void Mesh::generate_terrain(const uint8_t& ADD_VERTICES) {

	/*the grid takes the width and height of our texture and fills it up as cells and not as points, hence the n_rows and n_columns is subtracted by 1 inorder to not go out of bounds when reaching the last point on a row or column.
	a  ------  c
	|          |
	|          |
	b  ------  d */
	//this->mesh_dimensions = vec2(600, 600);
	vec3 half_size(this->mesh_dimensions / 2.0f, 0.0f);
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

			switch (ADD_VERTICES) {

				case ADD_ONLY_UNIQUE_VERTICES: {

					//checking for duplicates, accumalating TBNs, adding to our data vectors(buffers)
					check_accumalate_add(A, index_counter);
					check_accumalate_add(B, index_counter);
					break;

				};

				case ADD_ALL_VERTICES: {

					add_without_check(A, index_counter);
					add_without_check(B, index_counter);
					break;

				};

				default: {

					std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
					exit(EXIT_FAILURE);

				};

			};

		};

	};

	//normalizing the TBN of each vertex after it was accumalated
	if (ADD_VERTICES == ADD_ONLY_UNIQUE_VERTICES) {

		for (int i = 0; i < this->positions.size(); i++) {

			this->normals[i].normalize();
			this->tangents[i].normalize();
			this->bitangents[i].normalize();

		};

	};

};

//VIPNOTE: since alot of .obj files use quads instead of triangles whilst am using triangles, the number of vertices in the *positions* buffer will be more than the number of extracted vertices due to the transition from 4 vertices to 6 vertices.
void Mesh::extract_from_OBJ_file(const std::string& file_path, const uint8_t& ADD_VERTICES) {

	std::vector<vec3> temp_positions;
	std::vector<vec3> temp_normals;
	std::vector<vec2> temp_uv;
	int index_counter = 0;
	vec3 vec(0, 0, 0);
	
	std::vector<Vertex> vertices;
	Vertex vertex(0, 0, 0);

	std::vector<std::string> lines = read_file_by_line(file_path);
	std::vector<std::string> line_tokens;
	std::string attribute_type;

	int v_index;
	int vn_index;
	int vt_index;

	int n_faces = 0;

	for (auto& line : lines) {

		attribute_type = line.substr(0, 2);

		if (attribute_type == "vn") {
	
			safe_sscanf(line.c_str() + 3, "%f %f %f", 3, &vec.x, &vec.y, &vec.z);
			temp_normals.emplace_back(vec.x, vec.y, vec.z);		

		}
		else if (attribute_type == "vt") {

			safe_sscanf(line.c_str() + 3, "%f %f", 2, &vec.x, &vec.y);
			temp_uv.emplace_back(vec.x, vec.y);

		}
		else if (attribute_type == "v ") {

			safe_sscanf(line.c_str() + 2, "%f %f %f", 3, &vec.x, &vec.y, &vec.z);
			temp_positions.emplace_back(vec.x, vec.y, vec.z);

		}
		else if (attribute_type == "f ") {
			
			vertices.clear();
			line_tokens.clear();
			line_tokens = tokenise_data(line.substr(2), ' ');
			for (auto& line_token : line_tokens) {

				//searches first for *//* to check if we are working with the format: v//vn; otherwise the face command will have the format v/vt/vn
				if (line_token.find("//") != std::string::npos) {

					safe_sscanf(line_token.c_str(), "%d//%d", 2, &v_index, &vn_index);
					vertex.uv = vec2(0.0f, 0.0f);

				}
				else {//format: v/vt/vn

					safe_sscanf(line_token.c_str(), "%d/%d/%d", 3, &v_index, &vt_index, &vn_index);
					vertex.uv = temp_uv[vt_index - 1];

				};
	
				vertex.position = temp_positions[v_index - 1];
				vertex.normal = temp_normals[vn_index - 1];
				vertex.tangent = vec3(1, 0, 0);
				vertex.bitangent = vec3(0, 1, 0);
				vertex.color = vec3(255, 0, 0);				

				vertices.emplace_back(vertex);

			};

			//creating triangles based off of the number of vertices we extracted from the face command as a triangle fan. Example: a,b,c,d = abc and acd. Example: a,b,c,d,e = abc and acd and ade.
			int A = 0;
			int B = 0;
			int C = 0;
			switch (ADD_VERTICES) {

				case ADD_ONLY_UNIQUE_VERTICES: {

					for (int i = 1; i < vertices.size() - 1; i++) {
	
						B = i;
						C = i + 1;

						//checking for duplicates, accumalating TBNs, adding to our data vectors(buffers)
						check_accumalate_add(vertices[A], index_counter);
						check_accumalate_add(vertices[B], index_counter);
						check_accumalate_add(vertices[C], index_counter);

						n_faces += 1;

					};

					break;

				};

				case ADD_ALL_VERTICES: {

					for (int i = 1; i < vertices.size() - 1; i++) {
			
						B = i;
						C = i + 1;

						add_without_check(vertices[A], index_counter);
						add_without_check(vertices[B], index_counter);
						add_without_check(vertices[C], index_counter);

						n_faces += 1;

					};

					break;

				};

				default: {

					std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
					exit(EXIT_FAILURE);

				};

			};

		};

	};

	//normalizing the TBN of each vertex after it was accumalated
	if (ADD_VERTICES == ADD_ONLY_UNIQUE_VERTICES) {

		for (int i = 0; i < this->positions.size(); i++) {

			this->normals[i].normalize();
			this->tangents[i].normalize();
			this->bitangents[i].normalize();

		};

	};

	std::cout << "n_extracted vertices = " << temp_positions.size() << "\n";
	std::cout << "n_extracted faces = " << n_faces << "\n";

};

void Mesh::extract_from_LAS_file(const std::string& file_path, const uint8_t& ADD_VERTICES) {

	Point_Cloud cloud;
	std::vector<vec3> temp_positions = cloud.extract_points_coordinates(file_path);
	vec3 color(0, 255, 0);

	//we traverse the grid by going to every row where we work on every column on said row.
	int index_counter = 0;
	Vertex vertex(0, 0, 0);
	for (int i = 0; i < temp_positions.size(); ++i) {

		vertex.position = temp_positions[i];
		vertex.normal = (0, 0, 1);
		vertex.bitangent = vec3(0, 1, 0);
		vertex.tangent = vec3(1, 0, 0);
		vertex.uv = 0.0;
		vertex.color = color;

		switch (ADD_VERTICES) {

			case ADD_ONLY_UNIQUE_VERTICES: {

				//checking for duplicates, accumalating TBNs, adding to our data vectors(buffers)
				check_accumalate_add(vertex, index_counter);
				break;

			};

			case ADD_ALL_VERTICES: {

				add_without_check(vertex, index_counter);
				break;

			};

			default: {

				std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
				exit(EXIT_FAILURE);

			};

		};

	};

	//normalizing the TBN of each vertex after it was accumalated
	if (ADD_VERTICES == ADD_ONLY_UNIQUE_VERTICES) {

		for (int i = 0; i < this->positions.size(); i++) {

			this->normals[i].normalize();
			this->tangents[i].normalize();
			this->bitangents[i].normalize();

		};

	};

};

Mesh::Mesh(const vec2& mesh_dimensions, const uint8_t& ADD_VERTICES, Texture&& diffuse_map, Texture&& normal_map, Texture&& displacement_map) :

	generate_buffers_and_textures(true),
	mesh_dimensions(mesh_dimensions),
	diffuse_map(std::move(diffuse_map)),
	normal_map(std::move(normal_map)),
	displacement_map(std::move(displacement_map)) {

	switch (ADD_VERTICES) {

		case ADD_ONLY_UNIQUE_VERTICES: {

			draw_as_elements = true;
			break;

		};

		case ADD_ALL_VERTICES: {

			draw_as_elements = false;
			break;

		};

		default: {

			std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
			exit(EXIT_FAILURE);

		};

	};

	generate_terrain(ADD_VERTICES);
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;
	
	this->vertices_map.clear();
	std::cout << "cleared map\n";

};
Mesh::Mesh(const std::string& file_path, const uint8_t& ADD_VERTICES, Texture&& diffuse_map, Texture&& normal_map, Texture&& displacement_map, const uint8_t& FILE_TYPE) :

	generate_buffers_and_textures(true),
	mesh_dimensions(100, 100),
	diffuse_map(std::move(diffuse_map)),
	normal_map(std::move(normal_map)),
	displacement_map(std::move(displacement_map)) {

	switch (ADD_VERTICES) {

		case ADD_ONLY_UNIQUE_VERTICES: {

			draw_as_elements = true;
			break;

		};

		case ADD_ALL_VERTICES: {

			draw_as_elements = false;
			break;

		};

		default: {

			std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
			exit(EXIT_FAILURE);

		};

	};

	if (FILE_TYPE == OBJ_FILE) { extract_from_OBJ_file(file_path, ADD_VERTICES); }
	else if (FILE_TYPE == LAS_FILE) { extract_from_LAS_file(file_path, ADD_VERTICES); };
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;
	
	this->vertices_map.clear();
	std::cout << "cleared map\n";

};

Mesh::Mesh(const vec2& mesh_dimensions, const std::string& path_diffuse_map_file, const uint8_t& ADD_VERTICES, const std::string& path_normal_map_file, const std::string& path_displacement_map_file) :

	generate_buffers_and_textures(true),
	mesh_dimensions(mesh_dimensions),
	diffuse_map(path_diffuse_map_file, "uTexture", GL_TEXTURE0, 0),
	normal_map(path_normal_map_file, "uNormal_map", GL_TEXTURE1, 1),
	displacement_map(path_displacement_map_file, "uDisplacement_map", GL_TEXTURE2, 2) {

	switch (ADD_VERTICES) {

		case ADD_ONLY_UNIQUE_VERTICES: {

			draw_as_elements = true;
			break;

		};

		case ADD_ALL_VERTICES: {

			draw_as_elements = false;
			break;

		};

		default: {

			std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
			exit(EXIT_FAILURE);

		};

	};

	generate_terrain(ADD_VERTICES);
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;

	this->vertices_map.clear();
	std::cout << "cleared map\n";

};
Mesh::Mesh(const std::string& file_path, const std::string& path_diffuse_map_file, const uint8_t& ADD_VERTICES, const std::string& path_normal_map_file, const std::string& path_displacement_map_file, const uint8_t& FILE_TYPE) :

	generate_buffers_and_textures(true),
	mesh_dimensions(100, 100),
	diffuse_map(path_diffuse_map_file, "uTexture", GL_TEXTURE0, 0),
	normal_map(path_normal_map_file, "uNormal_map", GL_TEXTURE1, 1),
	displacement_map(path_displacement_map_file, "uDisplacement_map", GL_TEXTURE2, 2) {

	switch (ADD_VERTICES) {

		case ADD_ONLY_UNIQUE_VERTICES: {

			draw_as_elements = true;
			break;

		};

		case ADD_ALL_VERTICES: {

			draw_as_elements = false;
			break;

		};

		default: {

			std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
			exit(EXIT_FAILURE);

		};

	};

	if (FILE_TYPE == OBJ_FILE) { extract_from_OBJ_file(file_path, ADD_VERTICES); }
	else if (FILE_TYPE == LAS_FILE) { extract_from_LAS_file(file_path, ADD_VERTICES); };
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;

	this->vertices_map.clear();
	std::cout << "cleared map\n";

};

Mesh::Mesh(const vec2& mesh_dimensions, const std::string& path_maps_folder, const uint8_t& ADD_VERTICES) :

	generate_buffers_and_textures(true),
	mesh_dimensions(mesh_dimensions),
	diffuse_map(path_maps_folder + "/" + path_maps_folder.substr(path_maps_folder.find_last_of('/') + 1) + "_diffuse.png", "uTexture", GL_TEXTURE0, 0),
	normal_map(path_maps_folder + "/" + path_maps_folder.substr(path_maps_folder.find_last_of('/') + 1) + "_normal.png", "uNormal_map", GL_TEXTURE1, 1),
	displacement_map(path_maps_folder + "/" + path_maps_folder.substr(path_maps_folder.find_last_of('/') + 1) + "_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2) {

	switch (ADD_VERTICES) {

		case ADD_ONLY_UNIQUE_VERTICES: {

			draw_as_elements = true;
			break;

		};

		case ADD_ALL_VERTICES: {

			draw_as_elements = false;
			break;

		};

		default: {

			std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
			exit(EXIT_FAILURE);

		};

	};

	generate_terrain(ADD_VERTICES);
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;

	this->vertices_map.clear();
	std::cout << "cleared map\n";

};
Mesh::Mesh(const std::string& file_path, const std::string& path_maps_folder, const uint8_t& ADD_VERTICES, const uint8_t& FILE_TYPE) :

	generate_buffers_and_textures(true),
	mesh_dimensions(100, 100),
	diffuse_map(path_maps_folder + "/" + path_maps_folder.substr(path_maps_folder.find_last_of('/') + 1) + "_diffuse.png", "uTexture", GL_TEXTURE0, 0),
	normal_map(path_maps_folder + "/" + path_maps_folder.substr(path_maps_folder.find_last_of('/') + 1) + "_normal.png", "uNormal_map", GL_TEXTURE1, 1),
	displacement_map(path_maps_folder + "/" + path_maps_folder.substr(path_maps_folder.find_last_of('/') + 1) + "_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2) {

	switch (ADD_VERTICES) {

		case ADD_ONLY_UNIQUE_VERTICES: {

			draw_as_elements = true;
			break;

		};

		case ADD_ALL_VERTICES: {

			draw_as_elements = false;
			break;

		};

		default: {

			std::cerr << "ERROR: invalid ADD_VERTICES type!\n";
			exit(EXIT_FAILURE);

		};

	};

	if (FILE_TYPE == OBJ_FILE) { extract_from_OBJ_file(file_path, ADD_VERTICES); }
	else if (FILE_TYPE == LAS_FILE) { extract_from_LAS_file(file_path, ADD_VERTICES); };
	std::cout << "actual texture width: " << this->diffuse_map.width << " actual texture height: " << this->diffuse_map.height << " model dimensions: "; print_vec(this->mesh_dimensions);
	std::cout << "n_vertices: " << positions.size() << std::endl;
	std::cout << "n_indices: " << indices.size() << std::endl;
	std::cout << "n_uv_coords: " << this->texture_coordinates.size() << std::endl;
	std::cout << "n_TBNs: " << this->normals.size() << std::endl;

	this->vertices_map.clear();
	std::cout << "cleared map\n";

};

Mesh Mesh::from_procedural_Texture(const vec2& mesh_dimensions, const uint8_t& ADD_VERTICES, Texture&& diffuse_map, Texture&& normal_map, Texture&& displacement_map) {

	return { mesh_dimensions, ADD_VERTICES, std::forward<Texture>(diffuse_map), std::forward<Texture>(normal_map), std::forward<Texture>(displacement_map) };

};
Mesh Mesh::from_OBJ_Texture(const std::string& file_path, const uint8_t& ADD_VERTICES, Texture&& diffuse_map, Texture&& normal_map, Texture&& displacement_map, const uint8_t& FILE_TYPE) {

	return { file_path, ADD_VERTICES, std::forward<Texture>(diffuse_map), std::forward<Texture>(normal_map), std::forward<Texture>(displacement_map), FILE_TYPE };

};
Mesh Mesh::from_LAS_Texture(const std::string& file_path, const uint8_t& ADD_VERTICES, Texture&& diffuse_map, Texture&& normal_map, Texture&& displacement_map, const uint8_t& FILE_TYPE) {

	return { file_path, ADD_VERTICES, std::forward<Texture>(diffuse_map), std::forward<Texture>(normal_map), std::forward<Texture>(displacement_map), FILE_TYPE };

};

Mesh Mesh::from_procedural_files(const vec2& mesh_dimensions, const std::string& path_diffuse_map_file, const uint8_t& ADD_VERTICES, const std::string& path_normal_map_file, const std::string& path_displacement_map_file) {

	exit_if_file_doesnt_exist(path_diffuse_map_file); exit_if_file_doesnt_exist(path_normal_map_file); exit_if_file_doesnt_exist(path_displacement_map_file);
	if (check_if_directory(path_diffuse_map_file) || check_if_directory(path_normal_map_file) || check_if_directory(path_displacement_map_file)) {

		std::cerr << "ERROR: an inputed file path was a directory!\n";
		exit(EXIT_FAILURE);

	};
	return { mesh_dimensions, path_diffuse_map_file, ADD_VERTICES, path_normal_map_file, path_displacement_map_file };

};
Mesh Mesh::from_OBJ_files(const std::string& file_path, const std::string& path_diffuse_map_file, const uint8_t& ADD_VERTICES, const std::string& path_normal_map_file, const std::string& path_displacement_map_file, const uint8_t& FILE_TYPE) {

	exit_if_file_doesnt_exist(path_diffuse_map_file); exit_if_file_doesnt_exist(path_normal_map_file); exit_if_file_doesnt_exist(path_displacement_map_file);
	if (check_if_directory(path_diffuse_map_file) || check_if_directory(path_normal_map_file) || check_if_directory(path_displacement_map_file)) {

		std::cerr << "ERROR: an inputed file path was a directory!\n";
		exit(EXIT_FAILURE);

	};
	return { file_path, path_diffuse_map_file, ADD_VERTICES, path_normal_map_file, path_displacement_map_file, FILE_TYPE };

};
Mesh Mesh::from_LAS_files(const std::string& file_path, const std::string& path_diffuse_map_file, const uint8_t& ADD_VERTICES, const std::string& path_normal_map_file, const std::string& path_displacement_map_file, const uint8_t& FILE_TYPE) {

	exit_if_file_doesnt_exist(path_diffuse_map_file); exit_if_file_doesnt_exist(path_normal_map_file); exit_if_file_doesnt_exist(path_displacement_map_file);
	if (check_if_directory(path_diffuse_map_file) || check_if_directory(path_normal_map_file) || check_if_directory(path_displacement_map_file)) {

		std::cerr << "ERROR: an inputed file path was a directory!\n";
		exit(EXIT_FAILURE);

	};
	return { file_path, path_diffuse_map_file, ADD_VERTICES, path_normal_map_file, path_displacement_map_file, FILE_TYPE };

};

Mesh Mesh::from_procedural_folder(const vec2& mesh_dimensions, const std::string& path_maps_folder, const uint8_t& ADD_VERTICES) {

	exit_if_file_doesnt_exist(path_maps_folder);
	if (!check_if_directory(path_maps_folder)) {

		std::cerr << "ERROR: inputed path " << path_maps_folder << " was a file and not a directory!\n";
		exit(EXIT_FAILURE);

	};
	return { mesh_dimensions, path_maps_folder, ADD_VERTICES };

};
Mesh Mesh::from_OBJ_folder(const std::string& file_path, const std::string& path_maps_folder, const uint8_t& ADD_VERTICES, const uint8_t& FILE_TYPE) {

	exit_if_file_doesnt_exist(path_maps_folder);
	if (!check_if_directory(path_maps_folder)) {

		std::cerr << "ERROR: inputed path " << path_maps_folder << " was a file and not a directory!\n";
		exit(EXIT_FAILURE);

	};
	return { file_path, path_maps_folder, ADD_VERTICES, FILE_TYPE };

};
Mesh Mesh::from_LAS_folder(const std::string& file_path, const std::string& path_maps_folder, const uint8_t& ADD_VERTICES, const uint8_t& FILE_TYPE) {

	exit_if_file_doesnt_exist(path_maps_folder);
	if (!check_if_directory(path_maps_folder)) {

		std::cerr << "ERROR: inputed path " << path_maps_folder << " was a file and not a directory!\n";
		exit(EXIT_FAILURE);

	};
	return { file_path, path_maps_folder, ADD_VERTICES, FILE_TYPE };

};

