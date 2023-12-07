#pragma once

#ifndef RENDERABLETRIANGLECONTAINERMANAGER_H
#define RENDERABLETRIANGLECONTAINERMANAGER_H

#include "RenderableTriangleContainer.h"
#include "RenderableTriangleBase.h"

/*

Description: This class is meant to associated supergroup/ECBPoly IDs with a respective RenderableTriangleContainer.
It is also responsible for passing down already-instantiated and prepared RenderableTriangleBase-derived objects. 

Usage of this class is practically exclusive to the RenderableTriangleHandler class.

*/

class RenderableTriangleContainerManager
{
	public:
		// Below: insert a new entry; the referenced unique_ptr should already have a value instantiated to it and set up, before
		// the function is called.
		void insertRenderableTriangleIntoContainer(int in_uniqueID,														
												   std::unique_ptr<RenderableTriangleBase>* in_renderableTrianglePtr);	 

		void eraseContainer(int in_supergroupUniqueID);
			
	private:
		friend class RenderableTriangleHandler;
		std::map<int, RenderableTriangleContainer> mappedContainers;	// the key value in this map shoudl represent the unique ID of the ECBPoly used in the blueprint.
};

#endif
