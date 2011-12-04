#pragma once 

// TODO: extract to common header 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
struct PNTVertex
{
	PNTVertex( glm::vec3 const & position, glm::vec3 normal, glm::vec2 const & texcoord ) :
		position(position), normal(normal), texcoord(texcoord) {}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};
namespace semantic 
{
	namespace attr
	{
		enum type
		{
			POSITION = 0,
			NORMAL = 3,
			COLOR	 = 6,
			TEXCOORD = 7
		};
	};
};

class Shader;
class GameManager;
class PerlinNoise;

class Terrain
{
public:
	Terrain();

public:
	void initGeometry();
	void initShader();
	void render();

private:
	void calcNormals();

private:
	float width;
	float height;
	size_t nVertsWidth;
	size_t nVertsHeight;
	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	std::vector<glm::vec3> normals;
	std::vector<float> heights;
	std::vector<PNTVertex> vertices;
	std::vector<unsigned int> indices;
	Shader* shaderDefault;

private:
	GameManager* gm;

	PerlinNoise* pn;
};