#include "stdafx.h"
#include "RenderableTriangleContainerManager.h"

//void RenderableTriangleContainerManager::insertRenderableTriangleIntoContainer(int in_uniqueID, std::shared_ptr<RenderableTriangleBase> in_renderableTrianglePtr)


void RenderableTriangleContainerManager::insertRenderableTriangleIntoContainer(int in_uniqueID, std::unique_ptr<RenderableTriangleBase>* in_renderableTrianglePtr)
{
	mappedContainers[in_uniqueID].insertTriangle(in_renderableTrianglePtr);
}
