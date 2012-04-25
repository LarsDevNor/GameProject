#pragma once


class Terrain;
class PostProcessEdit;

class TerrainEditor
{
public:
    enum EditType { RAISE, LOWER };

public:
    TerrainEditor(Terrain* terrain);
    virtual ~TerrainEditor();

    bool configure(EditType _editType);
    void nextConfig();
    void run(glm::ivec2 pos);

private:
    EditType editType;
    float radius;
    Terrain* terrain;
    PostProcessEdit* processor;
};

