#include "StdAfx.h"

#include "Terrain.h"
#include "PostProcess.h"
#include "TerrainEditor.h"

TerrainEditor::TerrainEditor(Terrain* terrain) : terrain(terrain), radius(0.1f), editType(RAISE)
{
    processor = new PostProcessEdit();
}

TerrainEditor::~TerrainEditor()
{
}

bool TerrainEditor::configure(EditType _editType)
{
    editType = _editType;
    return true; // return bool so we can return false for illegal configurations 
}

void TerrainEditor::nextConfig()
{
    editType = (editType==RAISE) ? LOWER : RAISE;
}

void TerrainEditor::run(glm::ivec2 pos)
{
    float stren = 2.5f;
    if (editType==LOWER)
        stren *= -1.0f;
    glm::vec2 posOnTerrain = terrain->pick(pos);
    processor->run(posOnTerrain, radius, stren, terrain->getInputTex(), terrain->getOutputTex());
    terrain->swapTex();
}