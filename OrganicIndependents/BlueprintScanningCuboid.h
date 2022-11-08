#pragma once

#ifndef BLUEPRINTSCANNINGCUBOID_H
#define BLUEPRINTSCANNINGCUBOID_H


class BlueprintScanningCuboid
{
	public:
		BlueprintScanningCuboid(int in_cuboidDimension, EnclaveKeyDef::EnclaveKey in_centerKey) : dimension(in_cuboidDimension), currentCenterKey(in_centerKey)
		{
			totalCuboidElements = in_cuboidDimension * in_cuboidDimension*in_cuboidDimension;
			keyArray.reset(new EnclaveKeyDef::EnclaveKey[totalCuboidElements]);
			fillValuesAccordingToCenter();
		};
		std::unique_ptr<EnclaveKeyDef::EnclaveKey[]> keyArray;
		EnclaveKeyDef::EnclaveKey currentCenterKey;
		int dimension = 0;								// set by counstructor
		int totalCuboidElements = 0;					// ""
		void fillValuesAccordingToCenter();
	private:
		int translateXYZToSingle(int x, int y, int z);
};

#endif
