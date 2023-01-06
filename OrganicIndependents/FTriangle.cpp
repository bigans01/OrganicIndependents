#include "stdafx.h"
#include "FTriangle.h"

void FTriangle::fracture()
{
	determineOutputLevel();		// first, determine the output level.
	setupAndRunFracturerMachine();	// now, based off the output level, select the machine to use, if it is needed;
									// if the FTriangleType of the FTriangle is already FTriangleType::ATOMIC, we will do nothing, 
									// as we are already at the lowest possible level, and it isn't possible to fracture anymore.
}


void FTriangle::determineOutputLevel()
{
	switch (triangleOriginGrid)
	{
		case FTriangleType::WORLD: { triangleOutputGrid = FTriangleType::BLUEPRINT; break; }
		case FTriangleType::BLUEPRINT: { triangleOutputGrid = FTriangleType::ORE; break; }
		case FTriangleType::ORE: { triangleOutputGrid = FTriangleType::BLOCK; break; }
		case FTriangleType::BLOCK: { triangleOutputGrid = FTriangleType::ATOMIC; break; }
	}
}

void FTriangle::setupAndRunFracturerMachine()
{
	switch (triangleOriginGrid)
	{
		case FTriangleType::WORLD:
		{
			//std::cout << "Selected WORLD fracturer. " << std::endl;

			std::shared_ptr<FTriangleFracturerBase> worldFracturer(new (WorldFracturingMachine));
			fracturerMachine = worldFracturer;
			fracturerMachine->transferFTriangleMetadata(fracturePoints[0], 
													    fracturePoints[1], 
														fracturePoints[2], 
														fractureEmptyNormal,
														fractureRequiredOrientation,
														fractureRequiredClampValue,
														fractureMaterial);
			fracturerMachine->setOutputRef(&outputContainers);
			fracturerMachine->runFracturing();

			hasFracturingCompleted = true;
			break;
		}

		case FTriangleType::BLUEPRINT:
		{
			std::cout << "Selected BLUEPRINT fracturer. " << std::endl;

			std::shared_ptr<FTriangleFracturerBase> blueprintFracturer(new (BlueprintFracturingMachine));
			fracturerMachine = blueprintFracturer;
			fracturerMachine->transferFTriangleMetadata(fracturePoints[0],
														fracturePoints[1],
														fracturePoints[2],
														fractureEmptyNormal,
														fractureRequiredOrientation,
														fractureRequiredClampValue,
														fractureMaterial);
			fracturerMachine->setOutputRef(&outputContainers);
			fracturerMachine->runFracturing();

			hasFracturingCompleted = true;
			break;
		}
	}
}

void FTriangle::printProducedPoints()
{
	fracturerMachine->printFracturerPoints();
}

std::unordered_map<EnclaveKeyDef::EnclaveKey, FTriangleContainer, EnclaveKeyDef::KeyHasher>* FTriangle::fetchOutputContainerRef()
{
	return &outputContainers;
}