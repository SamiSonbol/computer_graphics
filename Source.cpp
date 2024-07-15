
#include <iostream>
#include <vector>
#include "Vector.h"
#include "shader.h"
#include <stb/stb_image.h>

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor) {

		std::cerr << "Failed to get primary monitor\n";
		glfwTerminate();
		return -1;

	}

	const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {

		std::cerr << "Failed to get video mode\n";
		glfwTerminate();
		return -1;

	}
	 
	vec2 screen_size (video_mode->width, video_mode->height);
	GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, "TEST", NULL, NULL);
	if (window == NULL) {

		std::cout << "failed";
		glfwTerminate();
		return -1;

	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, screen_size.x, screen_size.y);
	glEnable(GL_DEPTH_TEST);

	Mouse mouse = Mouse(window);
	//////

	std::vector<float> positions = {
		// Front face
		-1,  1, -1,  // 0
		-1, -1, -1,  // 1
		 1, -1, -1,  // 2
		 1,  1, -1,  // 3
		 // Back face
		 -1,  1,  1,  // 4
		 -1, -1,  1,  // 5
		  1, -1,  1,  // 6
		  1,  1,  1,  // 7
		  // Left face
		  -1,  1,  1,  // 8 (same as 4)
		  -1, -1,  1,  // 9 (same as 5)
		  -1, -1, -1,  // 10 (same as 1)
		  -1,  1, -1,  // 11 (same as 0)
		  // Right face
		   1,  1, -1,  // 12 (same as 3)
		   1, -1, -1,  // 13 (same as 2)
		   1, -1,  1,  // 14 (same as 6)
		   1,  1,  1,  // 15 (same as 7)
		   // Top face
		   -1,  1,  1,  // 16 (same as 4)
		   -1,  1, -1,  // 17 (same as 0)
			1,  1, -1,  // 18 (same as 3)
			1,  1,  1,  // 19 (same as 7)
			// Bottom face
			-1, -1,  1,  // 20 (same as 5)
			-1, -1, -1,  // 21 (same as 1)
			 1, -1, -1,  // 22 (same as 2)
			 1, -1,  1,  // 23 (same as 6)
	};

	std::vector<float> texture_coordinates = {
		// Front face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Back face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Left face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Right face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Top face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Bottom face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	std::vector<unsigned short> indices = {
		// Front face
		0, 1, 2,  0, 2, 3,
		// Back face
		4, 5, 6,  4, 6, 7,
		// Left face
		8, 9, 10,  8, 10, 11,
		// Right face
		12, 13, 14,  12, 14, 15,
		// Top face
		16, 17, 18,  16, 18, 19,
		// Bottom face
		20, 21, 22,  20, 22, 23,
	};

	std::vector<float> normals;
	std::vector<float> tangents;
	std::vector<float> bitangents;
	std::vector<float> colors;
	
	compute_tangents(positions,  indices, tangents, bitangents, texture_coordinates);
	
	for (int i = 0; i <  positions.size() / 3; ++i) {

		vec3 n = vec3(positions[3 * i], positions[3 * i + 1], positions[3 * i + 2]);

		normals.emplace_back(n.x);
		normals.emplace_back(n.y);
		normals.emplace_back(n.z);

		colors.emplace_back(0);
		colors.emplace_back(0);
		colors.emplace_back(1);
		
	};

	//compute_normals(positions, indices, normals);
	
	//get_indices(positions, normals, texture_coordinates, indices);

	vec4 material_properties = vec4(0.3, 0.7, 0.1, 10);
	Texture river_pebbles = Texture("ganges_river_pebbles_diff_4k.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal_map = Texture("ganges_river_pebbles_nor_gl_4k.png", "uNormal_map", GL_TEXTURE1, 1);

	unsigned int vertex_array, positions_buffer, normals_buffer, colors_buffer, indices_buffer, texture_coordinates_buffer, tangents_buffer, bitangents_buffer;

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	
	Shader shader = Shader(vertex_shader, geometry_shader, fragment_shader);
	glUseProgram(shader.program);

	std::vector<vec3> matrix_vectors = shader.create_standard_matrix_vectors();
	std::vector<vec3> light_vectors = shader.create_standard_light_vectors();
	std::vector<Texture> textures = { river_pebbles, river_pebbles_normal_map };

	bind_buffer(GL_ARRAY_BUFFER, &positions_buffer, positions, GL_STATIC_DRAW, 0, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &normals_buffer, normals, GL_STATIC_DRAW, 1, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &colors_buffer, colors, GL_STATIC_DRAW, 2, 3, true);
	bind_buffer(GL_ELEMENT_ARRAY_BUFFER, &indices_buffer, indices, GL_STATIC_DRAW, -1, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &texture_coordinates_buffer, texture_coordinates, GL_STATIC_DRAW, 3, 2, true);
	bind_buffer(GL_ARRAY_BUFFER, &tangents_buffer, tangents, GL_STATIC_DRAW, 4, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &bitangents_buffer, bitangents, GL_STATIC_DRAW, 5, 3, true);

	shader.setup(screen_size, matrix_vectors, light_vectors, material_properties, textures);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader.program);
		glBindVertexArray(vertex_array);

		mouse.move(matrix_vectors, light_vectors, material_properties);
        shader.update(matrix_vectors, light_vectors, material_properties);
	

		//glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &vertex_array);
	glDeleteBuffers(1, &positions_buffer);
	shader.delete_textures();
	glDeleteProgram(shader.program);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;


}