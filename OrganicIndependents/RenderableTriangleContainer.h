#pragma once

#ifndef RENDERABLETRIANGLECONTAINER_H
#define RENDERABLETRIANGLECONTAINER_H

#include "RenderableTriangleBase.h"
#include "RenderableTiledTriangle.h"
#include "RDerivedTypeEnum.h"
#include <iostream>
#include <mutex>
#include <vector>

/*

Description: This class is meant to store multiple unique_ptr objects in a vector (see rtVector);
the objects the unique_ptr instances reference should have already been appropriately instantiated
before the call to insertTriangle, or before the overloaded constructors/operators are called.

All code, except for the insertTriangle function, must query the derived class to determnie it's 
associated RDerivedTypeEnum value, in order to determine what to instantiate.

Once populated, the rtVector class can be freely accessed by other classes, as is the case in 
functions such as RenderableTriangleHandler::generateTouchedBlockList() and 
RenderableTriangleHandler::produceTerrainTriangles().

*/

class RenderableTriangleContainer
{
	public:
		
		RenderableTriangleContainer() {};	// default constructor, mostly unused.

		// Below: constructor for same object passed by reference.
		RenderableTriangleContainer(const RenderableTriangleContainer& in_containerA)	
		{
			std::cout << "RenderableTriangleContainer -> calling constructor #1. " << std::endl;
			for (auto& otherContainerEntry : in_containerA.rtVector)
			{
				// Calling the point directly returns a bool-style value that will determine
				// if the associated unique_ptr is actually pointing to data; we don't want to push back empty pointers,
				// as that'd be pointless (haha)
				if (otherContainerEntry)
				{
					// The rendering type must be queried, to determine which derived class to instantiate.
					switch (otherContainerEntry->getRenderingType())
					{
						// For tiling data, we must use RenderableTiledTriangle.
						case RDerivedTypeEnum::R_TILED:
						{
							std::unique_ptr<RenderableTriangleBase> triangleToTransferPtr;
							triangleToTransferPtr.reset(new RenderableTiledTriangle);

							*triangleToTransferPtr = *otherContainerEntry;
							rtVector.push_back(std::move(triangleToTransferPtr));

							break;
						}
					}
				}
			}
		}

		// Below: non-const assignment operator
		RenderableTriangleContainer& operator=(RenderableTriangleContainer&& in_containerA)
		{
			std::cout << "RenderableTriangleContainer -> calling overload operator #1. " << std::endl;
			for (auto& otherContainerEntry : in_containerA.rtVector)
			{
				// Calling the point directly returns a bool-style value that will determine
				// if the associated unique_ptr is actually pointing to data; we don't want to push back empty pointers,
				// as that'd be pointless (haha)
				if (otherContainerEntry)
				{
					// The rendering type must be queried, to determine which derived class to instantiate.
					switch (otherContainerEntry->getRenderingType())
					{
						// For tiling data, we must use RenderableTiledTriangle.
						case RDerivedTypeEnum::R_TILED:	
						{
							std::unique_ptr<RenderableTriangleBase> triangleToTransferPtr;
							triangleToTransferPtr.reset(new RenderableTiledTriangle);

							*triangleToTransferPtr = *otherContainerEntry;
							rtVector.push_back(std::move(triangleToTransferPtr));

							break;
						}
					}
				}
			}
			return *this;
		}

		// Below: const assignment operator
		RenderableTriangleContainer& operator=(const RenderableTriangleContainer& in_containerA)
		{
			std::cout << "RenderableTriangleContainer -> calling overload operator #2. " << std::endl;
			for (auto& otherContainerEntry : in_containerA.rtVector)
			{
				// Calling the point directly returns a bool-style value that will determine
				// if the associated unique_ptr is actually pointing to data; we don't want to push back empty pointers,
				// as that'd be pointless (haha)
				if (otherContainerEntry)
				{
					// The rendering type must be queried, to determine which derived class to instantiate.
					switch (otherContainerEntry->getRenderingType())
					{
						// For tiling data, we must use RenderableTiledTriangle.
						case RDerivedTypeEnum::R_TILED:
						{
							std::unique_ptr<RenderableTriangleBase> triangleToTransferPtr;
							triangleToTransferPtr.reset(new RenderableTiledTriangle);

							*triangleToTransferPtr = *otherContainerEntry;
							rtVector.push_back(std::move(triangleToTransferPtr));

							std::cout << "+++ stats, from vector: " << std::endl;
							for (auto& target : rtVector)
							{
								target->printStats();
							}

							break;
						}
					}
				}
			}
			return *this;
		}

		void insertTriangle(std::unique_ptr<RenderableTriangleBase>* in_rtEntryPtr);	// a function to directly insert an existing object into a unique_ptr,
																						// this function will call std::move on the unique_ptr, which will eliminate
																						// the contents found in the original pointer.

		std::vector<std::unique_ptr<RenderableTriangleBase>> rtVector;	// stores the unique_ptr objects that referece the derived instantiated classes;
																		// will frequently be accessed by other RenderableTriangle-family classes, but should never
																		// realistically be directly accessed by anything outside of this category.
		
	private:
};

#endif
