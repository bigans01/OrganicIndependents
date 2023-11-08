#pragma once

#ifndef FTRIANGLEOUTPUT_H
#define FTRIANGLEOUTPUT_H

#include "FTriangleType.h"

/*

Description:

Meant to store a single triangle that is produced as a result of the fracturing process that occurs from 
an FTriangle. This class can also be used to instantiate an FTriangle, as it's data members are a 1:1 copy.

This class has to be used in order to avoid a circular redundancy problem that was occuring. Before this class,
the FTriangle class contained a map of FTriangleContainers, and FTriangleContainers contained a vector of FTriangles; 
this resulted in very strange compilation artifacts, as a result of a bad decision.

This class addresses that circular dependency.

*/

class FTriangleOutput
{
	public:
		FTriangleOutput() {};
		FTriangleOutput(ECBPolyPoint in_fracturePoint0,
			ECBPolyPoint in_fracturePoint1,
			ECBPolyPoint in_fracturePoint2,
			FTriangleType in_destinedGridType,
			ECBPolyPoint in_fractureEmptyNormal,
			BoundaryOrientation in_requiredOrientation,
			PerfectClampEnum in_perfectClampValue,
			TriangleMaterial in_outputTriangleMaterial)
		{
			// each of the below ECBPolyPoints is converted to DoublePoint, via the special DoublePoint operator.
			fracturePoints[0].point = DoublePoint(in_fracturePoint0);	// each of these points should already be rounded to the nearest hundredth before this.
			fracturePoints[1].point = DoublePoint(in_fracturePoint1);	// "" 
			fracturePoints[2].point = DoublePoint(in_fracturePoint2);	// ""
			destinedGridType = in_destinedGridType;	// the type of the grid that the FTriangle originated in.
			fractureEmptyNormal = in_fractureEmptyNormal;	// the empty normal that was determined for this FTriangle. This should be 
															// constant, and passable to the produced FTriangles in the outputContainers member.
			fractureRequiredOrientation = in_requiredOrientation;
			fractureRequiredClampValue = in_perfectClampValue;

			outputTriangleMaterial = in_outputTriangleMaterial;
		};

		FTriangleOutput(DoublePoint in_fracturePoint0,
			DoublePoint in_fracturePoint1,
			DoublePoint in_fracturePoint2,
			FTriangleType in_destinedGridType,
			ECBPolyPoint in_fractureEmptyNormal,
			BoundaryOrientation in_requiredOrientation,
			PerfectClampEnum in_perfectClampValue,
			TriangleMaterial in_outputTriangleMaterial)
		{
			fracturePoints[0].point = in_fracturePoint0;	// each of these points should already be rounded to the nearest hundredth before this.
			fracturePoints[1].point = in_fracturePoint1;	// "" 
			fracturePoints[2].point = in_fracturePoint2;	// ""
			destinedGridType = in_destinedGridType;	// the type of the grid that the FTriangle originated in.
			fractureEmptyNormal = in_fractureEmptyNormal;	// the empty normal that was determined for this FTriangle. This should be 
															// constant, and passable to the produced FTriangles in the outputContainers member.
			fractureRequiredOrientation = in_requiredOrientation;
			fractureRequiredClampValue = in_perfectClampValue;

			outputTriangleMaterial = in_outputTriangleMaterial;
		}


		//DoublePoint fracturePoints[3];
		FTrianglePoint fracturePoints[3];
		FTriangleType destinedGridType = FTriangleType::NOVAL;	// the value that determines the type of grid this FTriangleOutput is intended to be.
																// Valid values would be:
																//	-BLUEPRINT
		ECBPolyPoint fractureEmptyNormal;
		BoundaryOrientation fractureRequiredOrientation = BoundaryOrientation::NONE;	// must be set by constructor.
		PerfectClampEnum fractureRequiredClampValue = PerfectClampEnum::NONE;	// must be set by constructor

		TriangleMaterial outputTriangleMaterial = TriangleMaterial::NOVAL;	// in reality, this will be something other than NOVAL (GRASS, DIRT, etc)

		// this flag determines if this instance of the FTriangleOutput should be purged, because the intended emptyNormal direction
		// of its BoundaryOrientation doesn't match with it's fractureEmptyNormal. This specific operation will be performed by the function,
		// FTriangleContainer::FTriangleContainerBounds.runBoundaryToNormalAnalysis will run this test, and set to true if the test doesn't pass
		// (that is, the fractureEmptyNormal's value and the BoundaryOrientation-associated normal value don't match -- i.e, POS_X -> 1,0,0)
		bool nonAlignedBoundaryDetected = false;

		// a simple debug function that prints out the points of an output triangle.
		void printOutputTrianglePoints()
		{
			std::cout << "Output triangle points are: ";
			for (int x = 0; x < 3; x++)
			{
				std::cout << " [" << x << "]: ";
				//fracturePoints[x].printPointCoords();
			}
			std::cout << std::endl;
		}

		// normal and point manipulation functions
		glm::vec3 getCurrentCrossProduct()
		{
			DoublePoint doubleU = fracturePoints[1].point - fracturePoints[0].point;
			DoublePoint doubleV = fracturePoints[2].point - fracturePoints[0].point;
			glm::vec3 u(doubleU.x, doubleU.y, doubleU.z);
			glm::vec3 v(doubleV.x, doubleV.y, doubleV.z);
			glm::vec3 calcedNormal = cross(u, v);	// the normal of the triangle
			return calcedNormal;
		}

		glm::vec3 getReversedCrossProduct()	// returns a cross product that is equivalent of the 2nd and 3rd points
											// being swapped (this does NOT swap them)
		{
			DoublePoint doubleU = fracturePoints[2].point - fracturePoints[0].point;
			DoublePoint doubleV = fracturePoints[1].point - fracturePoints[0].point;
			glm::vec3 u(doubleU.x, doubleU.y, doubleU.z);
			glm::vec3 v(doubleV.x, doubleV.y, doubleV.z);
			glm::vec3 calcedNormal = cross(u, v);	// the normal of the triangle
			return calcedNormal;
		}

		void swapPointsForNormalAlignment()	// needed by FTriangleContainer::analyzeForSwapping, in case it ends up 
											// needing to reverse the points for all produced FTriangleOutput instances in 
											// the FTriangleContainer::fracturedTriangles member.
		{
			//DoublePoint point1Copy = fracturePoints[1];
			FTrianglePoint point1Copy = fracturePoints[1];
			fracturePoints[1] = fracturePoints[2];
			fracturePoints[2] = point1Copy;
		}

		void setBuiltInEmptyNormal() // set the value of the fractureEmptyNormal to the unit-vectored cross product of the triangle.
		{
			glm::vec3 unitVectoredNormal = glm::normalize(getCurrentCrossProduct());
			fractureEmptyNormal = ECBPolyPoint(unitVectoredNormal);
		}

};

#endif