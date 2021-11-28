#include "stdafx.h"
#include "PAtom2DXY.h"

void PAtom2DXY::expand()
{
	int lineLength = 1 + ((currentExpansionIteration - 1) * 2);		// i.e, would be "1" for iteration 1, "3" for iteration 2, "5" for 3...etc

	int leastDim = currentExpansionIteration * -1;
	int greatestDim = currentExpansionIteration;

	// build the four corners:
	insertFusableAreaFor2DXY(leastDim, greatestDim);		// upper left (neg X, pos Y)
	insertFusableAreaFor2DXY(greatestDim, greatestDim);		// upper right (pos X, pos Y)
	insertFusableAreaFor2DXY(greatestDim, leastDim);		// lower right (pos X, neg Y)
	insertFusableAreaFor2DXY(leastDim, leastDim);			// lower left (neg X, neg Y)

	// build the lines:

	// two X aligned lines:
	int x_line_start_index = leastDim + 1;
	for (int x_count = 0; x_count < lineLength; x_count++)
	{
		insertFusableAreaFor2DXY(x_line_start_index, greatestDim);	// top x line
		insertFusableAreaFor2DXY(x_line_start_index, leastDim);		// bottom x line
		x_line_start_index++;
	}

	// two Y aligned lines:
	int y_line_start_index = leastDim + 1;
	for (int y_count = 0; y_count < lineLength; y_count++)
	{
		insertFusableAreaFor2DXY(greatestDim, y_line_start_index);	// right y line
		insertFusableAreaFor2DXY(leastDim, y_line_start_index);		// left y line
		y_line_start_index++;
	}

	currentExpansionIteration++;
	std::cout << "!! Finished expansion; size of fusable area map is: " << fusableAreaMap.size();
}

EnclaveKeyDef::EnclaveKey PAtom2DXY::generateExpansionKey(int in_x, int in_y)
{
	return EnclaveKeyDef::EnclaveKey(in_x, in_y, 0);
}

ECBPolyPoint PAtom2DXY::generateExpansionPoint(int in_x, int in_y)
{
	// for XY, Z is static; but X and Y offets are based off of multiplying in_x and in_y each by the value of the expansionInterval.
	return ECBPolyPoint(
		IndependentUtils::roundToHundredth(atomCorePoint.x + (in_x * expansionInterval)), 
		IndependentUtils::roundToHundredth(atomCorePoint.y + (in_y * expansionInterval)), 
		atomCorePoint.z);
}

void PAtom2DXY::insertFusableAreaFor2DXY(int in_x, int in_y)
{
	auto key = generateExpansionKey(in_x, in_y);
	auto fusableAreaPoint = generateExpansionPoint(in_x, in_y);
	FusableArea newFusableArea(originalMassID, fusableAreaPoint);
	fusableAreaMap[key] = newFusableArea;
}