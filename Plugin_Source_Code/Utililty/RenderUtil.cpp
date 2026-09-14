
#include "../Header.h"
#include "RenderUtil.h"

Mesh::Mesh()
{
    vtx = new vector<Vertex>();
    idx = new vector<WORD>();
}

Mesh::~Mesh()
{
    if (vtx != nullptr) delete vtx; vtx = nullptr;
    if (idx != nullptr) delete idx; idx = nullptr;
    
}