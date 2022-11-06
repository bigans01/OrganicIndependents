#pragma once

#ifndef PMASSRESULT_H
#define PMASSRESULT_H


class PMassResult
{
public:
	PMassResult() {};
	PMassResult(ECBPolyPoint in_ecbPolyPoint, bool in_resultFound, OperableIntSet in_resultingSet) :
		resultingPoint(in_ecbPolyPoint),
		resultFound(in_resultFound),
		resultingSet(in_resultingSet)
	{};

	void printResult()
	{
		if (resultFound == false)
		{
			std::cout << "(PMassResult): result was invalid!" << std::endl;
		}
		else if (resultFound == true)
		{
			std::cout << "(PMassResult): Printing contents of this valid result: " << std::endl;
			std::cout << "Resulting point: "; resultingPoint.printPointCoords(); std::cout << std::endl;
			std::cout << "Affected original atom IDs: " << std::endl;
			resultingSet.printSet();
		}
	}

	OperableIntSet resultingSet;
	ECBPolyPoint resultingPoint;
	bool resultFound = false;
};

#endif