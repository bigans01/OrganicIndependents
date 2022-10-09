#include "stdafx.h"
#include "WorldFracturingMachine.h"

void WorldFracturingMachine::runFracturing()
{
	determineUncalibratedBlueprintKeys();	// Step 1: get the uncalibrated keys.
	calibrateOriginBlueprintKeys();			// Step 2: calibrate the keys.
	translateTriangleByBlueprintKeys();		// first, do any required translations.
	runWorldTracing();
}

void WorldFracturingMachine::determineUncalibratedBlueprintKeys()
{
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = IndependentUtils::getUncalibratedBlueprintKeyForPoint(originFTrianglePoints[x]);
	}
}

void WorldFracturingMachine::calibrateOriginBlueprintKeys()
{
	CalibratableBlueprintKeyPair pairA;
	pairA.initialize(originFTriangleKeys[0], originFTriangleKeys[1], originFTrianglePoints[0], originFTrianglePoints[1], originFTrianglePoints[2]);
	pairA.calibrate();
	originFTriangleLineKeypairs[0] = pairA.getBeginAndEndKeys();

	CalibratableBlueprintKeyPair pairB;
	pairB.initialize(originFTriangleKeys[1], originFTriangleKeys[2], originFTrianglePoints[1], originFTrianglePoints[2], originFTrianglePoints[0]);
	pairB.calibrate();
	originFTriangleLineKeypairs[1] = pairB.getBeginAndEndKeys();

	CalibratableBlueprintKeyPair pairC;
	pairC.initialize(originFTriangleKeys[2], originFTriangleKeys[0], originFTrianglePoints[2], originFTrianglePoints[0], originFTrianglePoints[1]);
	pairC.calibrate();
	originFTriangleLineKeypairs[2] = pairC.getBeginAndEndKeys();

	// once everyting has been calibrated, we can store the keys for each point.
	std::cout << "Calibrated keys for each point are: " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		originFTriangleKeys[x] = originFTriangleLineKeypairs[x].keyA;
		std::cout << "Point " << x << ": ";
		originFTrianglePoints[x].printPointCoords();
		std::cout << " Key: ";
		originFTriangleKeys[x].printKey();
		std::cout << std::endl;
	}

	std::cout << "(WorldFracturingMachine::calibrateOriginBlueprintKeys) -> finished calibrating blueprint keys; keys are: " << std::endl;
	
	std::cout << "FTriangle line 0: " << std::endl;
	std::cout << "point A: "; originFTrianglePoints[0].printPointCoords(); std::cout << std::endl;
	std::cout << "point B: "; originFTrianglePoints[1].printPointCoords(); std::cout << std::endl;
	std::cout << "point A key: "; originFTriangleLineKeypairs[0].keyA.printKey(); std::cout << std::endl;
	std::cout << "point B key: "; originFTriangleLineKeypairs[0].keyB.printKey(); std::cout << std::endl;

	std::cout << "FTriangle line 1: " << std::endl;
	std::cout << "point A: "; originFTrianglePoints[1].printPointCoords(); std::cout << std::endl;
	std::cout << "point B: "; originFTrianglePoints[2].printPointCoords(); std::cout << std::endl;
	std::cout << "point A key: "; originFTriangleLineKeypairs[1].keyA.printKey(); std::cout << std::endl;
	std::cout << "point B key: "; originFTriangleLineKeypairs[1].keyB.printKey(); std::cout << std::endl;

	std::cout << "FTriangle line 2: " << std::endl;
	std::cout << "point A: "; originFTrianglePoints[2].printPointCoords(); std::cout << std::endl;
	std::cout << "point B: "; originFTrianglePoints[0].printPointCoords(); std::cout << std::endl;
	std::cout << "point A key: "; originFTriangleLineKeypairs[2].keyA.printKey(); std::cout << std::endl;
	std::cout << "point B key: "; originFTriangleLineKeypairs[2].keyB.printKey(); std::cout << std::endl;	
}

void WorldFracturingMachine::translateTriangleByBlueprintKeys()
{

}

void WorldFracturingMachine::runWorldTracing()
{
	FTriangleWorldTracer worldTracer;
	worldTracer.initialize(&stagerMap, &fracturerPoints, originFTriangleLineKeypairs, originFTrianglePoints);
	worldTracer.runLineTracing();
}
