#version 440 core

layout (triangles, equal_spacing, ccw) in;

in vec3 cNormal[];
in vec3 cColor[];
in vec3 cPosition[];
in vec2 cTexture_coordinates[];
in vec3 cTangent[];
in vec3 cBitangent[];

uniform bool orthogonal_projection;
uniform bool displacement_mapping;

uniform float orthogonal_size;
uniform	float FOV;
uniform float displacement_scale;

uniform vec3 forward_vector;
uniform vec3 up_vector;
uniform vec3 camera_position;
uniform vec3 camera_rotation_vector;

uniform vec3 model_translation_vector;
uniform vec3 model_scaling_vector;
uniform vec3 model_rotation_vector;

uniform vec2 screen_size;

uniform sampler2D uDisplacement_map;

out vec3 tNormal;
out vec3 tColor;
out vec3 tPosition;
out vec2 tTexture_coordinates;
out vec3 tTangent;
out vec3 tBitangent;

float to_radians(float degree) {

	return degree * 3.14159265359 / 180.0;

};

mat4 create_translation_matrix(vec3 translation_vector) {

	mat4 matrix = mat4(

		1.0, 0.0, 0.0, translation_vector.x,
		0.0, 1.0, 0.0, translation_vector.y,
		0.0, 0.0, 1.0, translation_vector.z,
		0.0, 0.0, 0.0, 1.0

	);

	return transpose(matrix);

};

mat4 create_scale_matrix(vec3 scale_vector) {

	mat4 matrix = mat4(

		scale_vector.x, 0.0, 0.0, 0.0,
		0.0, scale_vector.y, 0.0, 0.0,
		0.0, 0.0, scale_vector.z, 0.0,
		0.0, 0.0, 0.0, 1.0

	);

	return transpose(matrix);

};

mat4 create_X_rotation_matrix(float angle) {

	float alpha = to_radians(angle);

	mat4 matrix = mat4(

		1.0, 0.0, 0.0, 0.0,
		0.0, cos(alpha), sin(alpha), 0.0,
		0.0, -sin(alpha), cos(alpha), 0.0,
		0.0, 0.0, 0.0, 1.0

	);

	return transpose(matrix);

};

mat4 create_Y_rotation_matrix(float angle) {

	float alpha = to_radians(angle);

	mat4 matrix = mat4(

		cos(alpha), 0.0, -sin(alpha), 0.0,
		0.0, 1.0, 0.0, 0.0,
		sin(alpha), 0.0, cos(alpha), 0.0,
		0.0, 0.0, 0.0, 1.0

	);

	return transpose(matrix);

};

mat4 create_Z_rotation_matrix(float angle) {

	float alpha = to_radians(angle);

	mat4 matrix = mat4(

		cos(alpha), sin(alpha), 0.0, 0.0,
		-sin(alpha),cos(alpha), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0

	);

	return transpose(matrix);

};

mat4 create_rotation_matrix(vec3 rotation_vector) {

	return create_X_rotation_matrix(rotation_vector.x) * create_Y_rotation_matrix(rotation_vector.y) * create_Z_rotation_matrix(rotation_vector.z);

};

mat4 create_model_transformation_matrix(vec3 T, vec3 S, vec3 R) {

	return create_translation_matrix(T) * create_rotation_matrix(R) * create_scale_matrix(S) * create_translation_matrix(vec3(0.0, 0.0, 0.0));

};

mat4 create_view_matrix(vec3 camera_position, vec3 target_position, vec3 up_vector) {//also known as lookAtMatrix

	vec3 forward = normalize(camera_position - target_position);
	vec3 right = normalize(cross(up_vector, forward));
	vec3 up = normalize(cross(forward, right));

	mat4 matrix = mat4(

		right.x, right.y, right.z, -dot(right, camera_position),
		up.x, up.y, up.z, -dot(up, camera_position),
		-forward.x, -forward.y, -forward.z, dot(forward, camera_position),
		0.0, 0.0, 0.0, 1.0

	);

	return transpose(matrix);

};

mat4 create_orthographic_projection_matrix(vec2 screen_size, float near, float far, float orthogonal_size) {

	float aspect_ratio = screen_size.x / screen_size.y;
	float left = -orthogonal_size * aspect_ratio;
	float right = orthogonal_size * aspect_ratio;
	float bottom = -orthogonal_size;
	float top = orthogonal_size;

	mat4 matrix = mat4(

		2.0 / (right - left), 0.0, 0.0, -(right + left) / (right - left),
		0.0, 2.0 / (top - bottom), 0.0, -(top + bottom) / (top - bottom),
		0.0, 0.0, -2.0 / (far - near), -(far + near) / (far - near),
		0.0, 0.0, 0.0, 1.0

	);

	return transpose(matrix);

};

mat4 create_frustum_projection_matrix(float FOV, vec2 screen_size, float near, float far) {

	float view_angle = to_radians(FOV);
	float aspect_ratio = screen_size.x / screen_size.y;
	float alpha = tan(view_angle / 2.0);
	float top = near * alpha;
	float bottom = -top;
	float right = top * aspect_ratio;
	float left = -right;

	mat4 matrix = mat4(

		(2.0 * near) / (right - left), 0.0, (right + left) / (right - left), 0.0,
		0.0, (2.0 * near) / (top - bottom), (top + bottom) / (top - bottom), 0.0,
		0.0, 0.0, -(far + near) / (far - near), (-2.0 * far * near) / (far - near),
		0.0, 0.0, -1.0, 0.0

	);

	return transpose(matrix);

};

vec4 interpolate(vec4 v1, vec4 v2, vec4 v3, vec4 v4) {

  vec4 a = mix(v1, v2, gl_TessCoord.x);
  vec4 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec4 interpolate(vec4 v1, vec4 v2, vec4 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec3 interpolate(vec3 v1, vec3 v2, vec3 v3, vec3 v4) {

  vec3 a = mix(v1, v2, gl_TessCoord.x);
  vec3 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec3 interpolate(vec3 v1, vec3 v2, vec3 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec2 interpolate(vec2 v1, vec2 v2, vec2 v3, vec2 v4) {

  vec2 a = mix(v1, v2, gl_TessCoord.x);
  vec2 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec2 interpolate(vec2 v1, vec2 v2, vec2 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec3 displace(vec3 position, vec3 normal, float displacement_offset) {

    return position + normal * displacement_offset; 

}; 

void main() {

	tPosition = interpolate(cPosition[0], cPosition[1], cPosition[2]);
	tNormal = interpolate(cNormal[0], cNormal[1], cNormal[2]);
	tTangent = interpolate(cTangent[0], cTangent[1], cTangent[2]);
	tBitangent = interpolate(cBitangent[0], cBitangent[1], cBitangent[2]);
	tTexture_coordinates = interpolate(cTexture_coordinates[0], cTexture_coordinates[1], cTexture_coordinates[2]);
	tColor = interpolate(cColor[0], cColor[1], cColor[2]);
  
	if (displacement_mapping) {

	  float displacement_offset = texture(uDisplacement_map, tTexture_coordinates).r * displacement_scale;
	  tPosition = displace(tPosition, normalize(tNormal), displacement_offset);   
         
	};

	mat4 model_transformation_matrix = create_model_transformation_matrix(model_translation_vector, model_scaling_vector, model_rotation_vector);

	mat4 camera_rotation_matrix = create_rotation_matrix(camera_rotation_vector);
	vec3 forward = (camera_rotation_matrix * vec4(forward_vector, 0.0)).xyz;
	vec3 camera_target_position = camera_position + forward;
	mat4 view_matrix = create_view_matrix(camera_position, camera_target_position, up_vector);
	
	mat4 projection_matrix;
	if (orthogonal_projection) {

		projection_matrix = create_orthographic_projection_matrix(screen_size, 0.1, 50000.0, orthogonal_size);

	}
	else {

		projection_matrix = create_frustum_projection_matrix(FOV, screen_size, 0.1, 50000.0);

	};
    
	mat3 model_matrix = transpose(inverse(mat3(model_transformation_matrix)));
	tNormal = normalize(model_matrix * tNormal);
	tTangent = normalize(model_matrix * tTangent);
	tBitangent = normalize(model_matrix * tBitangent);

	gl_Position = projection_matrix * view_matrix * model_transformation_matrix * vec4(tPosition, 1);

};
