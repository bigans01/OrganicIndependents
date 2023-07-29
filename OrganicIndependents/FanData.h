#pragma once

#ifndef FANDATA_H
#define FANDATA_H



class FanData
{
	public:
		FanData() {};
		FanData(Message* in_messageRefToReadFrom)
		{
			// the values to read from, should be in this order:
			//	-the number of tertiaries
			//  -the total points
			//	-point index data that belongs in pointArray (the size of the loop would be equal to the total points)
			numberOfTertiaries = in_messageRefToReadFrom->readInt();
			int numberOfPoints = in_messageRefToReadFrom->readInt();
			for (int x = 0; x < numberOfPoints; x++)
			{
				pointArray[x] = unsigned int(in_messageRefToReadFrom->readInt());
			}

			materialID = TriangleMaterial(in_messageRefToReadFrom->readInt());
			faceAlignment = BoundaryPolyIndicator(in_messageRefToReadFrom->readInt());
			emptyNormal = in_messageRefToReadFrom->readPoint();
		}

		FanData(unsigned char in_intArrayPtr[],
			TriangleMaterial in_materialID,
			unsigned char in_numberOfTertiaries,
			//unsigned char in_faceAlignment,
			BoundaryPolyIndicator in_faceAlignment,
			ECBPolyPoint in_emptyNormal)
		{
			numberOfTertiaries = in_numberOfTertiaries;
			for (int x = 0; x < 8; x++)
			{
				pointArray[x] = in_intArrayPtr[x];
			}
			materialID = in_materialID;
			faceAlignment = in_faceAlignment;
			emptyNormal = in_emptyNormal;
		}

		FanData(unsigned int in_intArrayPtr[],
			TriangleMaterial in_materialID,
			unsigned char in_numberOfTertiaries,
			//unsigned char in_faceAlignment,
			BoundaryPolyIndicator in_faceAlignment,
			ECBPolyPoint in_emptyNormal)
		{
			numberOfTertiaries = in_numberOfTertiaries;
			for (int x = 0; x < 8; x++)
			{
				pointArray[x] = in_intArrayPtr[x];
			}
			materialID = in_materialID;
			faceAlignment = in_faceAlignment;
			emptyNormal = in_emptyNormal;
		}

		unsigned char numberOfTertiaries = 0;	// The number of points is equal to the number of tertiaries + 2. So 2 tertaries = 4 points. 
		unsigned int pointArray[8] = { 0 };		// stores indices of the points that make up the fan.
		TriangleMaterial materialID = TriangleMaterial::NOVAL;	// material of the fan.
		BoundaryPolyIndicator faceAlignment;	// stores what border the fan is on, if it applies.
		ECBPolyPoint emptyNormal;				// for the normal

		Message convertFanDataToMessage()
		{
			Message fanData;

			// Step 1: read, convert, and store the following (in this order): 
			// -the number of tertiaries
			// -the number of points, which is equal to the number of tertiaries + 2.
			// -each value in the pointArray, the size of the loop being equal to the number of ponits
			// -the material ID
			// -face alignment
			// -empty normal.


			fanData.insertInt(int(numberOfTertiaries));
			int totalPoints = numberOfTertiaries + 2;
			fanData.insertInt(int(totalPoints));

			for (int x = 0; x < totalPoints; x++)
			{
				fanData.insertInt(pointArray[x]);
			}

			fanData.insertInt(int(materialID));
			fanData.insertInt(int(faceAlignment.getIndicatorData()));
			fanData.insertPoint(emptyNormal);

			return fanData;
		}

};

#endif
