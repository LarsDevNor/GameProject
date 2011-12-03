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
			COLOR	 = 5,
			TEXCOORD = 6
		};
	};
};

class Shader;

class Terrain
{
public:
	Terrain();

public:
	void initGeometry();
	void initShader();
	void render();

private:
	size_t width;
	size_t height;
	size_t nVertsWidth;
	size_t nVertsHeight;
	GLuint vbo;
	GLuint vao;
	std::vector<PTVertex> vertices;
	Shader* shaderDefault;
};