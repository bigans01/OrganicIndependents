#pragma once

#ifndef DEBUGOPTION_H
#define DEBUGOPTION_H

// These options allow developers/library users to tailor the debug output to their liking. 
// Some keywords below cover debug output of multiple classes:

// |||||||||||||||||||||||||||| Library: OrganicGLWinLib
// FACTORY
//	--CleaveSequenceFactory
//  --QMVoidFindCyclingDirection
// FACTORY_MERGER
//  --CategorizedLineMerger
//  --CategorizedLineInterceptsPointPreciseMerger
//  --CategorizedLinePartialBoundMerger
//  --CategorizedLineASliceMerger
// FRACTURER




enum class DebugOption
{
	// ||||||||||||||||||||||||||||||||||||||||||||||||| Options used by class SPolySet (OrganicGLWinLib)
	// generic options, for no specific SPoly (set via SPolySet::setDOGeneric)
	SPOLYSET_TYPICAL,
	SPOLYSET_BOUNDARIES,
	SPOLYSET_ALL_SPOLY_MAIN,
	SPOLYSET_ALL_SPOLY_FACTORY,
	SPOLYSET_ALL_SPOLY_FACTORY_MERGER,
	SPOLYSET_ALL_FRACTURER,
	SPOLYSET_ALL_COPLANAR_RELATIONSHIPS_BASIC,		// will output metadata about specific STriangles that are part of a tracked SPoly, and halt (wait for input) between 
													// cutting operations that are performed on the STriangles; see CoplanarRelationships::performCuttingSequenceTest()

	// specific poly options,
	SPECIFIC_SPOLY_MAIN,
	SPECIFIC_SPOLY_FACTORY,
	SPECIFIC_SPOLY_FACTORY_MERGER,
	SPECIFIC_FRACTURER_EXECUTION,
	SPECIFIC_SPOLY_CATEGORIZED_LINES,
	SPECIFIC_SPOLY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// ||||||||||||||||||||||||||||||||||||||||||||||||| Options used by MassZonePointClipper (OrganicGLWinLib)
	SPECIFIC_SPOLY_CLIPPING_MASS_TRIAL,				// outputs data related to the PointToMassTrial, of a specific SPoly.
	SPECIFIC_SPOLY_CLIPPING_SHELL_SPOLYS,			// will print out all points of all shell SPolys in a MassZone, before a specific SPoly's PointToMassTrial. (see MassZonePointClipper::compareMeshMatterMetaAgainstClippingShells())

	// ||||||||||||||||||||||||||||||||||||||||||||||||| Options used by class MassZoneMaster (OrganicGLWinLib)
	// Options for OLD ZONE (See MassZoneMaster::oldZone, in OrganicGLWinLib)
	// debug options for Old zone (generic)
	MASSZONEMASTER_OLDZONE_PRINT_BOUNDARY_CATEGORIZED_LINES,				// prints the categorized lines in each boundary SPoly
	MASSZONEMASTER_OLDZONE_HALT_BETWEEN_BOUNDARY_SPOLY_CONSTRUCTION,		// waits for input between SPoly construction in the boundaries of a MassZoneBox.
	MASSZONEMASTER_OLDZONE_POINT_CLIPPING,

	// debug options for Old Zone, NEG_Z (North)
	MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_FRACTURER,
	MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for Old zone, POS_X, (East)
	MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_FRACTURER,
	MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for Old zone, POS_Z, (South)
	MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_FRACTURER,
	MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for Old zone, NEG_X, (South)
	MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_FRACTURER,
	MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for Old zone, POS_Y (Above)
	MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_FRACTURER,
	MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for Old zone, NEG_Y (Below)
	MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_FRACTURER,
	MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// Options for NEW ZONE (See MassZoneMaster::oldZone, in OrganicGLWinLib)
	MASSZONEMASTER_NEWZONE_PRINT_BOUNDARY_CATEGORIZED_LINES,				// prints the categorized lines in each boundary SPoly
	MASSZONEMASTER_NEWZONE_HALT_BETWEEN_BOUNDARY_SPOLY_CONSTRUCTION,		// waits for input between SPoly construction in the boundaries of a MassZoneBox.
	MASSZONEMASTER_NEWZONE_POINT_CLIPPING,

	// debug options for New zone, NEG_Z (North)
	MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_FRACTURER,
	MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for New zone, POS_X, (East)
	MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_FRACTURER,
	MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for New zone, POS_Z, (South)
	MASSZONEMASTER_NEWZONE_POS_Z_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_NEWZONE_POS_Z_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_NEWZONE_POS_Z_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_NEWZONE_POS_Z_BOUNDARY_FRACTURER,
	MASSZONEMASTER_NEWZONE_POS_Z_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_NEWZONE_POS_Z_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for New zone, NEG_X, (South)
	MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_FRACTURER,
	MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for New zone, POS_Y (Above)
	MASSZONEMASTER_NEWZONE_POS_Y_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_NEWZONE_POS_Y_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_NEWZONE_POS_Y_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_NEWZONE_POS_Y_BOUNDARY_FRACTURER,
	MASSZONEMASTER_NEWZONE_POS_Y_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_NEWZONE_POS_Y_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// debug options for New zone, NEG_Y (Below)
	MASSZONEMASTER_NEWZONE_NEG_Y_BOUNDARY_SPOLY_MAIN,
	MASSZONEMASTER_NEWZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY,
	MASSZONEMASTER_NEWZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY_MERGER,
	MASSZONEMASTER_NEWZONE_NEG_Y_BOUNDARY_FRACTURER,
	MASSZONEMASTER_NEWZONE_NEG_Y_BOUNDARY_CATEGORIZED_LINES,
	MASSZONEMASTER_NEWZONE_NEG_Y_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS,

	// ||||||||||||||||||||||||||||||||||||||||||||||||| Options used by class CoplanarRelationships (OrganicGLWinLib)
	COPLANARRELATIONSHIP_SHOW_TRACKED_SPOLY_BORDER_LINES,

	// ||||||||||||||||||||||||||||||||||||||||||||||||| Options used by class CuttableTriangle (OrganicGLWinLib)
	REFERENCED_CUTTINGTRIANGLE_COMPARISON,					// for CuttableTriangle::compareAgainstCuttingTriangle; prints out generic data in this function.
	REFERENCED_CUTTINGTRIANGLE_NONCOLINEAR_INTERSECT,		// for CuttableTriangle::compareAgainstCuttingTriangle; prints out data for noncolinear intersects.
	REFERENCED_CUTTINGTRIANGLE_TJUNCTION_INTERSECT,			// for CuttableTriangle::compareAgainstCuttingTriangle; prints out data for t-junction intersects.
	REFERENCED_CUTTINGTRIANGLE_COLINEAR_INTERSECT,			
	REFERENCED_CUTTINGTRIANGLE_QUAT_COLINEAR,				// for CuttableTriangle::compareAgainstCuttingTriangle's instance of TwoDLineSegmentIntersectAnalyzerV2 
															// flags debugging in colinear quat machine.
	REFERENCED_CUTTINGTRIANGLE_QUAT_INTERSECTING,			// for CuttableTriangle::compareAgainstCuttingTriangle's instance of TwoDLineSegmentIntersectAnalyzerV2 
															// flags debugging in intersecting quat machine.
	REFERENCED_CUTTINGTRIANGLE_PBZ_TEST,					// CuttableTriangle::testIfCuttingTriangleConsumesThisTriangle; shows output for the PBZ test.
	REFERENCED_CUTTINGTRIANGLE_CYCLING_DIRECTION,			// used during call to QuatUtils::findPointForDeterminingCyclingDirection
	REFERENCED_CUTTINGTRIANGLE_INTERSECT_ANALYZER_RESULT,   // used mainly during TwoDLineSegmentIntersectAnalyzerV2::performAnalysis(); outputs generic intersection data.
	REFERENCED_CUTTINGTRIANGLE_SLICE_ATTEMPTS,				// used by CuttableTriangle::buildLinesFromSliceAttempt; shows output related to building a TwoDCrawlingAttempt of type SLICE.
	REFERENCED_CUTTINGTRIANGLE_TYPICAL_ATTEMPTS,			// used by CuttableTriangle::buildLinesFromTypicalAttempt; shows output reloated to building a TwoDCrawlingAttempt of type TYPICAL.
	REFERENCED_CUTTINGTRIANGLE_CUTLINEWELDER,				// used by CutLineWelder::handleTypicalRun(); shows output of the CutLineWelder as it produces lines.
	REFERENCED_CUTTINGTRIANGLE_CUTTRIANGLEGROUPBUILDER,		// used by CutTriangleGroupBuilder::runCutTraceObserver(), and the classe's instance of CutLineTraceObserver.
	REFERENCED_CUTTINGTRIANGLE_CUT_TRIANGLE_PRODUCTION,		// for CuttableTriangle::produceCutTriangles (OrganicGLWinLib)

	// ||||||||||||||||||||||||||||||||||||||||||||||||| Options used by class STriangleCutter (OrganicGLWinLib)
	STRIANGLECUTTER_BASIC,									// for showing output of a particular STriangleCutter run; set by SPolySet::setDOTrackedSPolySTriangle (OrganicGLWinLib)
															// also shows output of remaining/spawned SCuttableTriangles after a comparison to a specific SCuttingTriangle.

	// RPoly
	RPOLY_RTRIANGLE_TRACING,										// shows how an RTriangle is traced through a Rasterized3DMassGrid
	RMORPHABLEAREASCANNER_PRINT_RMORPHABLEMESHGROUP_MESH_KEYS,		// prints the EnclaveKey value of each RMorphableMesh in each RMorphableMeshGroup, when calling RMorphableAreaScanner::scanGridMass() in OrganicGLWinLib.
	RMORPHABLEAREASCANNER_PRINT_MESHES_CONTAINING_MASS,				// prints any RMorphableMesh that contains mass, when calling the function RMorphableAreaScanner::scanGridMass() in OrganicGLWinLib.
	RMORPHABLEAREASCANNER_MESH_GROUP_CONSTRUCTION,					// shows output data in RMorphableAreaScanner, for when it attempts to build RMorphableMeshGroups.
	RMORPHABLEMESHGROUP_SPECIFIC_MESH_FACE_GENERATION				// shows output data for a call to generateRProductFaces() in a specific RMorphableMesh, in the function RMorphableMeshGroup::generateRProductFacesInRemainingMeshes().
};

#endif