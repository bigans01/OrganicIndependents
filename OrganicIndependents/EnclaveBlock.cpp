#include "stdafx.h"
#include "EnclaveBlock.h"

EnclaveBlock::EnclaveBlock()
{

}

EnclaveBlock::EnclaveBlock(Message* in_blockDataMessageRef)
{
	// REMEMBER: Message must be already opened from OUTSIDE this function, or program will crash.
	// Read the flags, and then purge the value from the Message, so that it can be passed on to the FanManager for processing.
	blockflags.flags = unsigned char(in_blockDataMessageRef->readInt());

	in_blockDataMessageRef->removeIntsFromFrontAndResetIter(1);
	manager.constructManagerFromMessage(in_blockDataMessageRef);
}

EnclaveBlock::EnclaveBlock(Message in_blockDataMessage)
{
	in_blockDataMessage.open();

	blockflags.flags = unsigned char(in_blockDataMessage.readInt());

	in_blockDataMessage.removeIntsFromFrontAndResetIter(1);
	manager.constructManagerFromMessage(&in_blockDataMessage);
}

Message EnclaveBlock::writeEnclaveBlockToBDMMessage()
{
	Message convertedBlockDataMessage(MessageType::BDM_BLOCK_UNTAGGED);
	// Convert the value of blockflags.flags first, insert that, and then append the Message from a call to 
	// manager.writeFanManagerToBDMFormat().
	//
	// Remember, BDM_BLOCK_UNTAGGED does NOT contain the individual block key.
	convertedBlockDataMessage.insertInt(int(blockflags.flags));
	convertedBlockDataMessage.appendOtherMessage(&manager.writeFanManagerToBDMFormat());

	return convertedBlockDataMessage;
}


int EnclaveBlock::getNumberOfBBFans()
{
	return manager.getNumberOfTotalFans();
}

int EnclaveBlock::resetBlock()
{
	int previousNumberOfTriangles = getNumberOfTotalTriangles();
	blockflags.resetFlags();

	FanManager emptyManager;
	manager = emptyManager;

	return previousNumberOfTriangles;
}

int EnclaveBlock::getNumberOfTotalTriangles()
{
	return manager.getNumberOfTotalTriangles();
}

int EnclaveBlock::getNumberOfTertiariesInTriangleAtIndex(int in_index)
{
	return manager.getNumberOfTertiariesInTriangleAtIndex(in_index);
}

ECBPolyPoint EnclaveBlock::getEmptyNormalFromTriangle(int in_index)
{
	return manager.getEmptyNormalFromTriangle(in_index);
}

void EnclaveBlock::processTertiaryData(TertiaryTriangleContainer in_polyMetaData, TriangleMaterial in_materialID)
{
	manager.processTertiaryData(in_polyMetaData, in_materialID);
}

void EnclaveBlock::insertBBFanFromRawEnclave(OrganicWrappedBBFan in_wrappedFan)
{
	// EBC,NEW
	manager.insertBBFanFromRawEnclave(in_wrappedFan);

}

int EnclaveBlock::insertFanGroup(OrganicFanGroup in_fanGroupToAdd)
{
	int totalTrianglesInserted = 0;
	for (auto& currentFan : in_fanGroupToAdd.fans)
	{
		manager.insertBBFanFromRawEnclave(currentFan);
		totalTrianglesInserted += currentFan.poly.numberOfTertiaries;
	}
	return totalTrianglesInserted;
}

FanBase* EnclaveBlock::retrieveSecondaryFromIndex(int in_index)
{
	// EBC,NEW
	return manager.retrieveSecondaryFromIndex(in_index);

}

int EnclaveBlock::getTotalNumberOfTertiariesInBlock()
{
	// EBC,NEW
	return manager.getTotalNumberOfTertiariesInBlock();

}

PointSearchData EnclaveBlock::checkIfPointExists(EnclaveBlockVertex in_blockVertex)
{

	// EBC,NEW
	return manager.checkIfPointExists(in_blockVertex);

}

PointSearchData EnclaveBlock::checkIfNearbyPointExists(EnclaveBlockVertex in_blockVertex)
{

	// EBC,NEW
	return manager.checkIfNearbyPointExists(in_blockVertex);
}

BlockSearchMeta EnclaveBlock::checkForExactPoint(ECBPolyPoint in_point, int in_debugFlag)
{
	// EBC, NEW
	return manager.checkForExactPoint(in_point, in_debugFlag);

}

bool EnclaveBlock::checkIfPolysExistOnBoundary(BoundaryOrientation in_boundaryOrientationToCheck)
{
	bool doPolysExistInBoundary = false;
	int numberOfFans = getNumberOfBBFans();
	for (int currentFanIndex = 0; currentFanIndex < numberOfFans; currentFanIndex++)
	{
		auto targetFanPtr = retrieveSecondaryFromIndex(currentFanIndex);
		if (targetFanPtr->faceAlignment.getBoundaryIndicatorValue() == in_boundaryOrientationToCheck)
		{
			doPolysExistInBoundary = true;
			break;
		}
	}
	return doPolysExistInBoundary;
}

EnclaveBlockVertex EnclaveBlock::fetchPoint(unsigned char in_pointIndex)
{
	// EBC, NEW
	return manager.fetchPoint(in_pointIndex);
}

EnclaveBlockVertex EnclaveBlock::fetchPoint(int in_pointIndex)
{
	// EBC, NEW
	return manager.fetchPoint(in_pointIndex);
}




void EnclaveBlock::setBlockMode(BlockSubType in_modeToSet)
{
	blockMode = in_modeToSet;
}

BlockSubType EnclaveBlock::getBlockMode()
{
	return blockMode;
}


BlockSearchMeta EnclaveBlock::checkIfNearbyPointExistsOnLine(ECBPolyPoint in_point, EnclaveKeyDef::EnclaveKey in_key, int in_debugFlag)
{
	// EBC, NEW
	return manager.checkIfNearbyPointExistsOnLine(in_point, in_key, in_debugFlag);
}

ECBPolyPoint EnclaveBlock::convertVertexToPolyPoint(EnclaveBlockVertex in_blockVertex)
{
	ECBPolyPoint pointToReturn;
	pointToReturn.x = IndependentUtils::convertPreciseCoordToFloat(in_blockVertex.x);
	pointToReturn.y = IndependentUtils::convertPreciseCoordToFloat(in_blockVertex.y);
	pointToReturn.z = IndependentUtils::convertPreciseCoordToFloat(in_blockVertex.z);
	return pointToReturn;
}

void EnclaveBlock::listSecondaries()
{
	manager.listSecondaries();
}

void EnclaveBlock::listPoints()
{

}