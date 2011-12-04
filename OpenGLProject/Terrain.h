#pragma once 

// TODO: extract to common header 
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
struct PTVertex
{
	PTVertex( glm::vec3 const & position, glm::vec2 const & texcoord ) :
		position(position), texcoord(texcoord) {}

	glm::vec3 position;
	glm::vec2 texcoord;
};
namespace semantic 
{
	namespace attr
	{
		enum type
		{
			POSITION = 0,
			COLOR	 = 3,
			TEXCOORD = 4
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
	float width;
	float height;
	size_t nVertsWidth;
	size_t nVertsHeight;
	GLuint vbo;
	GLuint vao;
	GLuint ibo;
	std::vector<PTVertex> vertices;
	std::vector<unsigned int> indices;
	Shader* shaderDefault;

private:
	GameManager* gm;

	PerlinNoise* pn;
};