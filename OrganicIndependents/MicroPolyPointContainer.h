#pragma once

#ifndef MICROPOLYPOINTCONTAINER_H
#define MICROPOLYPOINTCONTAINER_H


class MicroPolyPointContainer
{
public:
	ECBPolyPoint pointArray[8];
	int numberOfPoints = 0;
	bool isContainerValid = true;

	void insertNewPoint(ECBPolyPoint in_point);
	void loadPointsFromOtherContainer(MicroPolyPointContainer* in_containerRef);
};

#endif


