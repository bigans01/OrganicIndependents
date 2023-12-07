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

All code, except for the insertTriangle function, must query the derived class to determine it's 
associated RDerivedTypeEnum value, in order to determine what to instantiate. This is used to
for the correct class to call static_cast on, so that the new unique_ptr may be appropriately constructed, via
a reference to the other unique_ptr we are reading from.

Once populated, the rtVector class can be freely accessed by other classes, as is the case in 
functions such as RenderableTriangleHandler::generateTouchedBlockList() and 
RenderableTriangleHandler::produceTerrainTriangles().

*/

class RenderableTriangleContainer
{
	public:
		
		RenderableTriangleContainer() {};	// default constructor, mostly unused.

		// For all constructors/overloaded operators below, the process will be this:
		//
		//	1. Cycle through each element in the rtVector of the other container (in_containerA).
		//
		//	2. If the current unique_ptr we are looking at contains data, we will call getRenderingType()
		//	   on the otherContainerEntry, to determine what class it is. Remember, the value of rDerived in these derived classes
		//	   must have already been set before these constructors/ovrloaded operators are called.
		//
		//	3. Once the derived class to instantiate is determined, use static_cast to downcast a pointer to the otherContainerEntry to the corresponding class type,
		//	   and store it in a pointer of that type; then, use .reset to create a new instance of the specified derived type, where we take in 
		//     the casted pointer as an argument. This should call the both the base class constructor, and the derived class constructor, in one line of code.
		//	   This will prevent us from having to make a bunch of custom constructors for each class derived from the RenderableTriangleBase class.

		// Below: constructor for same object passed by reference.
		RenderableTriangleContainer(const RenderableTriangleContainer& in_containerA)	
		{
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
						// For tiling data, we must use RenderableTiledTriangle. In order to use the derived class constructor 
						// correctly, where it is instantiated from another object of the same type, we must downcast the otherContainerEntry
						case RDerivedTypeEnum::R_TILED:
						{
							// Cast the other unique_ptr to RenderableTiledTriangle, and store it; we will dereference this pointer as
							// the argument to the new RenderableTiledTriangle constructor. This will call both the base class constructor (for RenderableTriangleBase)
							// and the constructor RendeableTiledTriangle.
							std::unique_ptr<RenderableTriangleBase> triangleToTransferPtr;
							RenderableTiledTriangle* castedPtr = static_cast<RenderableTiledTriangle*>(otherContainerEntry.get());
							triangleToTransferPtr.reset(new RenderableTiledTriangle(*castedPtr));

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
						// For tiling data, we must use RenderableTiledTriangle. In order to use the derived class constructor 
						// correctly, where it is instantiated from another object of the same type, we must downcast the otherContainerEntry
						case RDerivedTypeEnum::R_TILED:	
						{
							// Cast the other unique_ptr to RenderableTiledTriangle, and store it; we will dereference this pointer as
							// the argument to the new RenderableTiledTriangle constructor. This will call both the base class constructor (for RenderableTriangleBase)
							// and the constructor RendeableTiledTriangle.
							std::unique_ptr<RenderableTriangleBase> triangleToTransferPtr;
							RenderableTiledTriangle* castedPtr = static_cast<RenderableTiledTriangle*>(otherContainerEntry.get());
							triangleToTransferPtr.reset(new RenderableTiledTriangle(*castedPtr));

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
						// For tiling data, we must use RenderableTiledTriangle. In order to use the derived class constructor 
						// correctly, where it is instantiated from another object of the same type, we must downcast the otherContainerEntry
						case RDerivedTypeEnum::R_TILED:
						{
							// Cast the other unique_ptr to RenderableTiledTriangle, and store it; we will dereference this pointer as
							// the argument to the new RenderableTiledTriangle constructor. This will call both the base class constructor (for RenderableTriangleBase)
							// and the constructor RendeableTiledTriangle.
							std::unique_ptr<RenderableTriangleBase> triangleToTransferPtr;
							RenderableTiledTriangle* castedPtr = static_cast<RenderableTiledTriangle*>(otherContainerEntry.get());
							triangleToTransferPtr.reset(new RenderableTiledTriangle(*castedPtr));

							rtVector.push_back(std::move(triangleToTransferPtr));


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
