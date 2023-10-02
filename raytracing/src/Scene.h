#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Material {

	glm::vec3 Albedo{1.0f}; // Non lit diffuse component of a material
	float Roughness = 1.0f;
	float Metallic = 0.0f;

};

struct Sphere {
	glm::vec3 Position{0.0f};
	float Radius = 0.5f;

	int MaterialIndex = 0;
};

struct Scene {
	std::vector<Material> Materials;
	std::vector<Sphere> Spheres;
};