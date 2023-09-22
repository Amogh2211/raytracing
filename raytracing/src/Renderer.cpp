#include "Renderer.h"
#include "Walnut/Random.h"

// Why keep the color in float? The math stays between 0-1 hence the squares and other stuff also stays in 0-1
// It just keeps things clean by clamping it between a range
namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& color) {
		uint8_t r = (uint8_t)(color.r * 255.0f); // One byte unsigned int, that will be between 0 and 255
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r; // This essentially makes is "aaaa aaaa bbbb bbbb gggg gggg rrrr rrrr" for the 32 bit int
		return result;
	
	}
}

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

void Renderer::Render(const Camera& camera) {

	const glm::vec3& rayOrigin = camera.GetPosition();

	Ray ray;
	ray.Origin = camera.GetPosition();
	// GO Y FIRST AS CACHEING AT CPU IS EASIER

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f)); // KEEP THE COMPONENT OF vec4 TO BE BETWEEN 0.0 and 1.0
			m_ImageData[x+ y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
			
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray& ray)
{

	float radius = 0.5f;

	// Circle equation with a vector subbed in
	//set a ray origin. ray direction, radius of sphere, hit distance for the ray

	// float a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z;


	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * (glm::dot(ray.Origin, ray.Direction));
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

	// Plugging it into discriminant

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return glm::vec4(0,0,0,1);

	// The solutions to the quadratic equation
	float t0 = ((-b + glm::sqrt(discriminant))/ 2.0f * a);
	float closestT = (-b - glm::sqrt(discriminant)) / 2.0f * a; // this is always smalled than t0 hence we can assume this to be the closest hitpoint

	glm::vec3 hitPoint = ray.Origin + ray.Direction * closestT;
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
	float lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f); // cos(angle)

	glm::vec3 sphereColor(1, 0, 1);
	sphereColor *= lightIntensity;

	return glm::vec4(sphereColor, 1.0f); // We add the 1.0f as the sphereCOlor was defined without an alpha
} 
