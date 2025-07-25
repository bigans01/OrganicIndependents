#pragma once

#ifndef QUATROTATIONMANAGER_H
#define QUATROTATIONMANAGER_H

#include "QuatRotationPoints.h"
#include "QuatRotationType.h"
#include "QuatRotationRecord.h"
#include "CyclingDirection.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class QuatRotationManager
{
	public:
		int debugFlag = 0;			// for debugging purposes 
		QuatRotationPoints* rotationpointsRefVector;
		glm::vec3 focusedLine;							// the line that we will be rotating until it's y and z values are equal (calculations are based around this line)
		glm::vec3* triangleNormalRef;					// a reference to the vec3 that is the calculate normal for a triangle
		std::vector<QuatRotationType> rotationOrder;	// stores the types of rotations that need to be executed
		std::stack<QuatRotationRecord> rotationRecords;

		void setDebugLevel(PolyDebugLevel in_polyDebugLevel);

		void initializeAndRunForEmptyNormal(QuatRotationPoints* in_quatpointsRefVector);
		void initializeAndRunForZFracture(QuatRotationPoints* in_quatpointsRefVector);
		glm::vec3 initializeAndRunForPlanarSlide(QuatRotationPoints* in_quatpointsRefVector);
		void initializeAndRunForPlanarAlignmentToZ(QuatRotationPoints* in_quatpointsRefVector);
		void initializeAndRunForCoplanarCategorizedLineEmptyNormal(QuatRotationPoints* in_quatpointsRefVector);

		CyclingDirection initializeAndRunForCyclingDirectionFinder(QuatRotationPoints* in_quatpointsRefVector);
		void initializeAndRunForCyclingDirectionFinderV2(QuatRotationPoints* in_quatpointsRefVector);

		void initializeAndRunforAligningNeighboringCleaveSequencesToPosY(QuatRotationPoints* in_quatpointsRefVector);
		void initializeAndRunForFindingBorderLine(QuatRotationPoints* in_quatpointsRefVector);

		float initializeAndRunForFindingObserverRadians(QuatRotationPoints* in_quatpointsRefVector);
		bool initializeAndRunForCheckingCoplanarity(QuatRotationPoints* in_quatpointsRefVector);

		void calculateEmptyNormal();					// find the empty normal (should only be run when all points of triangle are on y

		void executeRotationsForCyclingDirectionFinderV2();

		void executeRotationsForPlanarSlide();
		void executeRotationsForFindingBorderLine();
		void executeRotationsForFindingCoplanarCategorizedLineEmptyNormal();


		void rotateAroundYAndPushIntoStack();
		void rotateAroundYToPosZForPlanarSlideAndPushIntoStack();
		void rotateAroundXToYZeroForPlanarSlideAndPushIntoStack();
		void rotateAroundZAndPushIntoStack();
		void rotateAroundZToFindCoplanarCategorizedLineEmptyNormalAndPushIntoStack();

		void rotateAroundZAndPushIntoStack(glm::vec3* in_point);
		void rotateAroundXToYZeroAndPushIntoStack();
		void rotateAroundXForZFractureAndPushIntoStack();
		void rotateAroundXForPositiveYBorderLineAndPushIntoStack();

		void rotateEmptyNormalToPosY(glm::vec3* in_normal);
		void rotateAroundZForPosYNormalAndPushIntoStack(glm::vec3 in_normal);
		void rotateAroundYToPosXAndPushIntoStack();
		void rotatePointAroundXToPosY(glm::vec3 in_point);

		bool checkForRightAngle(glm::vec3 in_pointAtY0, glm::vec3 in_otherPoint);

		float findRotationRadiansForZFracture(glm::vec3 in_vec3);
		float findRotationRadiansForGettingToPosYThroughZ(glm::vec3 in_vec3);
		float findRotationRadainsForGettingToPosXThroughY(glm::vec3 in_vec3);
		float findRotationRadainsForGettingToPosYThroughX(glm::vec3 in_vec3);

		void flipOnXAxis();

		glm::quat createQuaternion(float radians, glm::vec3 in_angle);
		void rotateToOriginalPosition();
		void setDebugFlag(int in_debugFlag);
		glm::vec3 checkForEmptyNormalCorrection(glm::vec3 in_mrpCopy, glm::vec3 in_normalCopy);
		glm::vec3 getEmptyNormal();						// returns the calculated empty normal

		// rounding functions
		float roundToThousandths(float in_float);
		float roundRadiansForRightAngleCheck(float in_angleInRadians);

		float radianValue;				// may be optionally used by some functions.
	private:
		glm::vec3 pointACopy;
		glm::vec3 pointBCopy;
		glm::vec3 pointCCopy;

		//void executeRotationsForCyclingDirectionFinder();
		void rotateAroundZToYZero();	// Needed for when QuatRotationManager::initializeAndRunForCheckingCoplanarity 
										// ends up calling executeRotationsForCheckingCoplanarity; it expects that
										// pointBRef/Copy is the 3rd point (at index value 2)

		void rotateAroundZToYZeroV2();	// Needed for when QuatRotationManager::initializeAndRunForFindingObserverRadians ends up calling
										// executeRotationsForFindingObserverRadians; expects that the value of
										// pointBRef/Copy is the 4th point (at index value 3).

		float executeRotationsForFindingObserverRadians();
		float findRadiansForObservation();

		void executeRotationsForEmptyNormal();
		void executeRotationsForZFracture();
		
		bool executeRotationsForCheckingCoplanarity();

		PolyLogger quatRotationManagerLogger;
};

#endif
