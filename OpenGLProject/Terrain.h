#pragma once 

class Shader;
class GameManager;
class PerlinNoise;
class PostProcessEdit;

class Terrain
{
public:
	Terrain();

public:
	void initGeometry();
	void initShader();
	void initPickingFBO();
	//void initFBO();
	//void initTextures();
	//void initSamplers();

	void render(GLuint sceneFBO);
	void renderTerrain(Shader* renderShader);
	void pick(const glm::ivec2& screenCoord);
	//void renderFullscreenQuad();

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
	 
	//std::vector<GLuint> 
	size_t inputTex; 
	std::vector<GLuint> heightTextures;
	void swapTex() { inputTex = (inputTex==0) ? 1 : 0; }
	GLuint getInputTex() { return heightTextures[inputTex]; }
	GLuint getOutputTex() { return (inputTex==0) ? heightTextures[1] : heightTextures[0] ; }

	GLuint heightTex;
	GLuint colorTex;
	GLuint normalTex;
	GLuint depthTex;
	GLuint defaultSampler; // linear, clamp 
	std::vector<glm::vec3> normals;
	std::vector<float> heights;
	std::vector<PNTVertex> vertices;
	std::vector<unsigned int> indices;
	GLuint quadBuffer;
	GLuint quadVAO;
	std::vector<glm::vec2> quadVertices;

	Shader* defaultShader;
	Shader* pickingShader;
private:
	GLuint pickingFBO;
	GLuint pickingRB;
private:
	GameManager* gm;
	PerlinNoise* pn;
private:
	PostProcessEdit* editPP;
};