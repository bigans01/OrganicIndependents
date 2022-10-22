#include "stdafx.h"
#include "FRayIntersectionMachine.h"

#define FRAY_SMALL_NUM 0.00000001 // anything that avoids division overflow

FRayIntersectionMachine::FRayIntersectionResult FRayIntersectionMachine::runIntersectionTest()
{
	// Logic for interception is below
	// Assumptions on FRay, based on how they are fixed to a grid:
	//
	//	-An FRay should never hit any of 3 points of a triangle, as all FRays exist at the intersection of exactly two planes of the grid
	//	 AND because FRays should never be at the same X,Y, or Z coordinate of a point of the triangle. (need a diagram to explain this)
	// 
	//	-If an FRay is parallel to the triangle, the intersection test should signify that it is parallel, and no point should be returned.
	//	 In reality, there should ba test done on the triangle to determine which FRays (ZFray, XFRay, YFRay etc) should actually be used, before 
	//	 we decide which FRayCasterQuadBase-derived classes to use. For example, if two points have the same X and Y values BUT different Z values,
	//	 then this means that all ZFRays casted against this triangle will be parallel to it.
	//
	//	The logic for this code is modeled off of a combination of the code from .cpp #6 from the Practical_Geomtery_Algorithms_Code_2021-05-30.2.zip
	//	file, and from other tested code in the FusionCandidateProducer class in OrganicGLWinLib. For more in-depth analysis of the math, 
	//  use the book Practical Geometry Algorithms: with C++ Code, by Dr. Daniel Sunday.

	auto trianglePointA = interceptableTrianglePoints[0];
	auto trianglePointB = interceptableTrianglePoints[1];
	auto trianglePointC = interceptableTrianglePoints[2];

	glm::vec3 u = trianglePointB - trianglePointA;
	glm::vec3 v = trianglePointC - trianglePointA;

	glm::vec3 N = cross(u, v);	// the normal of the triangle

	glm::vec3 dir = rayToCast.vecB - rayToCast.vecA;
	glm::vec3 w0 = rayToCast.vecA - trianglePointA;

	float a = -dot(N, w0);
	float b = dot(N, dir);

	// Check if Ray is parallel.
	if (fabs(b) < FRAY_SMALL_NUM)
	{
		// Check if ray lies in traingle plane; if it's true, return.
		if
		(
			(a < FRAY_SMALL_NUM)
			&&
			(a > (FRAY_SMALL_NUM* -1.0f))
		)
		{
			return FRayIntersectionResult(rayToCast.vecA,
										glm::vec3(0, 0, 0),
										intersectionRoundingMode,
										FRayIntersectionType::WITHIN_PLANE);
		}

		// Parallel, but not in the triangle plane
		else
		{
			return FRayIntersectionResult(rayToCast.vecA,
										glm::vec3(0, 0, 0),
										intersectionRoundingMode,
										FRayIntersectionType::PARALLEL);
		}
	}

	// If the ray is not parallel to the triangle, check for the normal intersection.
	// For this to work, the values of S, and S + T, must be within 0.0f and 1.0f (with some margin for error allowed)
	float r = a / b;
	if (r < 0.0f)
	{
		// would not work, ray goes away from triangle.
		return FRayIntersectionResult(rayToCast.vecA,
									glm::vec3(0,0,0),
									intersectionRoundingMode,
									FRayIntersectionType::DOES_NOT_INTERSECT);
	}

	auto intersectionCandidatePoint = rayToCast.vecA + (r * dir);

	glm::vec3 w;
	float    uu, uv, vv, wu, wv, D;
	uu = dot(u, u);
	uv = dot(u, v);
	vv = dot(v, v);
	w = intersectionCandidatePoint - trianglePointA;
	wu = dot(w, u);
	wv = dot(w, v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	bool withinS = true;
	bool withinSandT = true;

	s = (uv * wv - vv * wu) / D;
	if (s < -0.0f || s > 1.00f)         // I is outside S
	{
		withinS = false;
		return FRayIntersectionResult(rayToCast.vecA,
									intersectionCandidatePoint,
									intersectionRoundingMode,
									FRayIntersectionType::DOES_NOT_INTERSECT);
	}

	t = (uv * wu - uu * wv) / D;
	if (t < -0.0f || (s + t) > 1.00f)  // I is outside T
	{
		withinSandT = false;
		return FRayIntersectionResult(rayToCast.vecA,
									intersectionCandidatePoint,
									intersectionRoundingMode,
									FRayIntersectionType::DOES_NOT_INTERSECT);
	}

	// If we got to this point, and both withinS AND withinSAndT are true, the point is valid.
	if (withinS && withinSandT)
	{
		std::cout << "(FRayIntersectionMachine): intersecting point found, for the ray with points ";
		std::cout << "A: (" << rayToCast.vecA.x << ", " << rayToCast.vecA.y << ", " << rayToCast.vecA.z << ") ";
		std::cout << "B: (" << rayToCast.vecB.x << ", " << rayToCast.vecB.y << ", " << rayToCast.vecB.z << "); point of intersection was: "; 
		std::cout << intersectionCandidatePoint.x << ", " << intersectionCandidatePoint.y << ", " << intersectionCandidatePoint.z << std::endl;

		return FRayIntersectionResult(rayToCast.vecA, 
									intersectionCandidatePoint, 
									intersectionRoundingMode,
									FRayIntersectionType::INTERSECTS);
	}
}