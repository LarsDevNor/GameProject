#pragma once 

class Shader;
class GameManager;
class PerlinNoise;
class PostProcessEdit;
class TerrainEditor;
class ParticleSystem;

class Terrain
{
public:
    enum EDIT_TYPE { edit_up, edit_down};
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
	/*void edit(const glm::ivec2& screenCoord);*/
    glm::vec2 pick(const glm::ivec2& screenCoord);
	//void renderFullscreenQuad();

    float getWidth() const { return width; }
    float getHeight() const { return height; }
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
	std::vector<Texture> heightTextures;
	void swapTex() { inputTex = (inputTex==0) ? 1 : 0; }
	const Texture& getInputTex() { return heightTextures[inputTex]; }
	const Texture& getOutputTex() { return (inputTex==0) ? heightTextures[1] : heightTextures[0] ; }

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
private:
    friend TerrainEditor;
    friend ParticleSystem;
};