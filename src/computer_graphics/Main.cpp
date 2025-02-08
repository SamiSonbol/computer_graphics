#include <iostream>
#include <vector>

#include "computer_graphics/Math.h"
#include "computer_graphics/Shader.h"
#include "computer_graphics/Mesh.h"
#include "computer_graphics/UI.h"

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor) {

		std::cerr << "Failed to get primary monitor\n";
		glfwTerminate();
		return -1;

	};

	const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {

		std::cerr << "Failed to get video mode\n";
		glfwTerminate();
		return -1;

	};
	 
	vec2 screen_size (video_mode->width, video_mode->height);
	GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, PROJECT_NAME, NULL, NULL);
	if (window == NULL) {

		std::cout << "failed";
		glfwTerminate();
		return -1;

	};
	glfwMakeContextCurrent(window);
	gladLoadGL();
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	glViewport(0, 0, screen_size.x, screen_size.y);
	glEnable(GL_DEPTH_TEST);

	unsigned int g_vertex_shader = compile_shader(GL_VERTEX_SHADER, read_shader(SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_geometry/vertex_shader.vert"));
	unsigned int g_fragment_shader = compile_shader(GL_FRAGMENT_SHADER, read_shader(SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_geometry/fragment_shader.frag"));
	unsigned int geometry_shader = compile_shader(GL_GEOMETRY_SHADER, read_shader(SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_geometry/geometry_shader.geom"));
	std::vector<unsigned int> normal_mapping_and_displacement_mapping_and_geometry = { g_vertex_shader, geometry_shader, g_fragment_shader };

	unsigned int t_vertex_shader = compile_shader(GL_VERTEX_SHADER, read_shader(SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_tesselation/vertex_shader.vert"));
	unsigned int tesselation_control_shader = compile_shader(GL_TESS_CONTROL_SHADER, read_shader(SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_tesselation/tesselation_control_shader.tesc"));
	unsigned int tesselation_evaluation_shader = compile_shader(GL_TESS_EVALUATION_SHADER, read_shader(SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_tesselation/tesselation_evaluation_shader.tese"));
	unsigned int t_fragment_shader = compile_shader(GL_FRAGMENT_SHADER, read_shader(SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_tesselation/fragment_shader.frag"));
	std::vector<unsigned int> normal_mapping_and_displacement_mapping_and_tesselation = { t_vertex_shader, tesselation_control_shader, tesselation_evaluation_shader, t_fragment_shader };
	
	Shader shader(normal_mapping_and_displacement_mapping_and_tesselation);
	glUseProgram(shader.program);
	Shader::graphics_vectors_container vectors_container = shader.create_standard_shader_vectors();
	Shader::graphics_booleans_container booleans_container = shader.create_standard_shader_booleans();
	Shader::graphics_floats_container floats_container = shader.create_standard_shader_floats();
	shader.initialize(screen_size, vectors_container, booleans_container, floats_container);

	UI user_interface(window, (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	Mouse mouse = Mouse();
	bool plot = false;
	user_interface.shader_debug_mode(vectors_container, booleans_container, floats_container, plot);

	/*Texture diffuse_map(RESOURCES_DIR"/diffuse_maps/abra.png", "uTexture", GL_TEXTURE0, 0);
	Texture normal_map(RESOURCES_DIR"/normal_maps/abra_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture displacement_map(RESOURCES_DIR"/displacement_maps/abra_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	/*Texture diffuse_map(RESOURCES_DIR"/diffuse_maps/emma.png", "uTexture", GL_TEXTURE0, 0);
	Texture normal_map(RESOURCES_DIR"/normal_maps/emma_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture displacement_map(RESOURCES_DIR"/displacement_maps/emma_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	/*Texture diffuse_map(RESOURCES_DIR"/diffuse_maps/osna.png", "uTexture", GL_TEXTURE0, 0);
	Texture normal_map(RESOURCES_DIR"/normal_maps/osna_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture displacement_map(RESOURCES_DIR"/displacement_maps/osna_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	/*Texture diffuse_map(RESOURCES_DIR"/diffuse_maps/rocky_rivers.png", "uTexture", GL_TEXTURE0, 0);
	Texture normal_map(RESOURCES_DIR"/normal_maps/osna_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture displacement_map(RESOURCES_DIR"/displacement_maps/rocky_rivers_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	Texture diffuse_map(RESOURCES_DIR"/diffuse_maps/ganges_river.png", "uTexture", GL_TEXTURE0, 0);
	Texture normal_map(RESOURCES_DIR"/normal_maps/ganges_river_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture displacement_map(RESOURCES_DIR"/displacement_maps/ganges_river_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);

	/*Texture diffuse_map(RESOURCES_DIR"/diffuse_maps/mountain_range.png", "uTexture", GL_TEXTURE0, 0);
	Texture normal_map(RESOURCES_DIR"/normal_maps/emma_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture displacement_map(RESOURCES_DIR"/displacement_maps/mountain_range_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	Mesh mesh(std::move(diffuse_map), std::move(normal_map), std::move(displacement_map), vec2(100, 100));

	/*since each object has its own own data buffers and textures inside *Mesh*, we dont need multiple vertex arrays, instead we simply bind said buffers to the shader every time we want to draw said object
	we do that by using the *Shader::draw_mesh_elements* function, where this function binds the *Mesh* data and draws it directly*/
	unsigned int vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader.program);

		shader.update(vectors_container, booleans_container, floats_container);
		user_interface.new_frame();
		mouse.update_position(window);
		//mouse.plot_point(plot, window, vectors_container, screen_size, mesh, mesh.colors, mesh.positions);

		shader.bind_and_draw_mesh_elements(mesh, booleans_container.gamma_correction, GL_PATCHES);

		user_interface.update();
		user_interface.render();

		glfwSwapBuffers(window);
		glfwPollEvents();

	};

	glDeleteVertexArrays(1, &vertex_array);
	shader.delete_all();
	user_interface.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

};