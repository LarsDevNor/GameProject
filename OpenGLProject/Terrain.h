#pragma once 

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
	void initFBO();
	void initTextures();
	void initSamplers();

	void render();
	void renderTerrain();
	void renderFullscreenQuad();

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
	GLuint fbo; 
	GLuint colorTex;
	GLuint depthTex;
	GLuint defaultSampler; // linear, clamp 
	std::vector<glm::vec3> normals;
	std::vector<float> heights;
	std::vector<PNTVertex> vertices;
	std::vector<unsigned int> indices;
	Shader* shaderDefault;
	Shader* shaderPostproc;
	
	GLuint quadBuffer;
	GLuint quadVAO;
	std::vector<glm::vec2> quadVertices;

private:
	GameManager* gm;
	PerlinNoise* pn;
};