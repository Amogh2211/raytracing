#include "Renderer.h"
#include "Walnut/Random.h"


void Renderer:: OnResize(uint32_t width, uint32_t height) {

	if (m_FinalImage) {
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) // Check if image needs resizing in the first place
			return;

		m_FinalImage->Resize(width, height);
	}
	else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

}

void Renderer::Render() {

	// GO Y FIRST AS CACHEING AT CPU IS EASIER

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };

			coord = coord * 2.0f - 1.0f; // Map the coordinates to (-1,1) instead of (2,0)
			
			m_ImageData[x+ y * m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{

	glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 0.5f;
	rayDirection = glm::normalize(rayDirection);

	// Circle equation with a vector subbed in
	//set a ray origin. ray direction, radius of sphere, hit distance for the ray

	// float a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z;


	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * (glm::dot(rayOrigin, rayDirection));
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	// Plugging it into discriminant

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0.0f)
		return 0xffff00ff;


	return 0xff000000;
}