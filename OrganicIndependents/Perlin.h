#pragma once

#ifndef PERLIN_H
#define PERLIN_H

#include <random>
#include <iostream>
#include "EnclaveKeyDef.h"
#include <glm/glm.hpp>
#include <algorithm>

// This header file contains utility functions for various noise algorithms and methodologies, such as PerlinNoise. As R&D continues on these methodologies,
// those functions and other items will be found in this file, and are designed to be independent of almost all other classes (some exceptions will apply, such as
// EnclaveKeyDef)

// Reference for bicubic interpolation: https://en.wikipedia.org/wiki/Bicubic_interpolation

// Description: a QuadInterpolationPointSet is a series of 4 points of data that end up forming a 
// single row to use when calculating bicubic interpolation.The A/B/C/D portions of the class
// refer to the 4 representative equations used. See https://www.paulinternet.nl/?page=bicubic
// for description of these equations.
class QuadInterpolationPointSet
{
	public:
		QuadInterpolationPointSet(float in_p0, float in_p1, float in_p2, float in_p3)
		{
			originatingValues[0] = in_p0;
			originatingValues[1] = in_p1;
			originatingValues[2] = in_p2;
			originatingValues[3] = in_p3;

			A = 2 * (in_p1 - in_p2) + .5 * (in_p2 - in_p0 + in_p3 - in_p1);
			B = 3 * (in_p2 - in_p1) - in_p2 + in_p0 - .5 * (in_p3 - in_p1);
			C = .5 * (in_p2 - in_p0);
			D = in_p1;
		}

		void printOriginValues()
		{
			std::cout << originatingValues[0] << ", " << originatingValues[1] << ", " << originatingValues[2] << ", " << originatingValues[3] << std::endl;
		}

		QuadInterpolationPointSet() {};

		float calculate(float t)
		{
			/*
			std::cout << "~~!! values are: " << std::endl;
			std::cout << "A: " << A << std::endl;
			std::cout << "B: " << B << std::endl;
			std::cout << "C: " << C << std::endl;
			std::cout << "D: " << D << std::endl;
			*/
			float valueIs = A * pow(t, 3.0) + B * pow(t, 2.0) + C * t + D;
			//std::cout << "valueIs: " << valueIs << std::endl;

			return A * pow(t, 3.0) + B * pow(t, 2.0) + C * t + D;
		}
	private:
		float A = 0.0f;
		float B = 0.0f;
		float C = 0.0f;
		float D = 0.0f;

		float originatingValues[4];
};

// Description: Given 4 instances of QuadInterpolationPointSet, which effectively
// gives what is needed to sample a tile, instances of this object can be use to 
// bicubically interpolate between the ranges of 0 and 1, where 0 and 1 reflect a range between
// the 2nd and 3rd data points of a QuadInterpolationPointSet; 0 represents the 2nd point, and 1 represents the 3rd.
class BicubicInterpolationSet
{
	public:
		BicubicInterpolationSet(QuadInterpolationPointSet in_set0,
								QuadInterpolationPointSet in_set1,
								QuadInterpolationPointSet in_set2,
								QuadInterpolationPointSet in_set3)
		{
			quadPointSets[0] = in_set0;
			quadPointSets[1] = in_set1;
			quadPointSets[2] = in_set2;
			quadPointSets[3] = in_set3;
		}

		BicubicInterpolationSet() {};

		float fetchBicubicValue(float in_xRange, float in_zRange)
		{
			// Assuming each QuadInterpolationPointSet will use the same X value (that ranges between 0 and 1), but varying Z value, do a cubic interpolation 
			// on each of the members of quadPointSets, in order to get the cubic interpolated value at X, for each quadPointSet. Store this in cubicResults.
			float cubicResults[4];
			for (int x = 0; x < 4; x++)
			{
				cubicResults[x] = quadPointSets[x].calculate(in_xRange);
			}

			// Form a new cubic QuadInterpolationPointSet from the values in cubicResults, where the first value of the array effectively serves as p0 of the new QuadInterpolationPointSet
			// All values from cubic results will be calculated form the same X value (in_xRange), which should be a value of 0 to 1, that was calculated in the code preceding this.
			// This results in 4 points that are directly aligned with each other on the Z axis -- because they were calculated from the same X --, which we can then use for the cubic interpolation on that axis.
			// This will give us our desired point that ranges between 0 and 1 in both the X and Z directions.
			return QuadInterpolationPointSet(cubicResults[0], cubicResults[1], cubicResults[2], cubicResults[3]).calculate(in_zRange);
		}

		void printFieldInterpPoints()
		{
			for (int x = 4; x > 0; x--)
			{
				int currentIndex = (x - 1);
				std::cout << "Row " << currentIndex << ": " << std::endl;
				quadPointSets[currentIndex].printOriginValues();
			}
		}

	private:
		QuadInterpolationPointSet quadPointSets[4];
};

class PerlinNoise
{
	public:
		// Below: create a Perlin-style gradient, given a seed value and a location in space to create the gradient at.
		// The point to create the gradient at should be a location on a regular grid.
		glm::vec2 generate2DPerlinGradient(int in_seed_value,
			                          EnclaveKeyDef::Enclave2DKey in_twoDKey)
		{
			// Step 1: initialize a new "super" generator with the given seed super seed;
			std::mt19937_64 superGenerator(in_seed_value);
			std::uniform_int_distribution<> superFloatDistribution(0, 1000);
			float superParentValue = superFloatDistribution(superGenerator);

			// Step 2: get a value for another generator, by getting the hash of the current 2d key.
			auto twoDKeyHash = (std::hash<int>()(in_twoDKey.a)
				^ (std::hash<int>()(in_twoDKey.b) << 1)
				) >> 1;

			// The two separate hashes produced by the following Enclave2DKey values will apparently cause 
			// two separate mt19937_64 objects to produce the same values:
			//
			// -2048, 0          (hash computation of 5139928519672806836)
			// -2048, -1024      (hash computation of 8839207905642333590)
			twoDKeyHash += 1;

			// Step 3: create a mt, where it is seeded from the current key's hash value;
			// the range of the distribution will be from 0 to pi.
			std::mt19937_64 currentKeyGenerator(twoDKeyHash);
			std::uniform_int_distribution<> currentKeyDistribution(0, 1000);
			float currentKeyValue = currentKeyDistribution(currentKeyGenerator);


			// By this point we have two ints
			// -constant int from the parent seed
			// -an int generated from a seed calculated from the hash of the 2d key we are looking at
			//
			// We will now multiply these two values together, to get a new int, and create a new generator off of that.
			std::mt19937_64 finalGenerator(superParentValue * currentKeyValue);

			// Create a new float value from the finalGenerator, using float distribution. 
			// The float value should be from to two pi; this value can be used with cos/sin to find a unit vector.
			std::uniform_real_distribution<> finalFloatRange(0.0f, 2 * 3.14f);
			float selectedFloatRange = finalFloatRange(finalGenerator);

			float cosVal = cos(selectedFloatRange);
			float sinVal = sin(selectedFloatRange);

			std::cout << "Unit vector values: " << cosVal << ", " << sinVal << std::endl;
			return glm::vec2(cosVal, sinVal);
		};

		// Below: Uses a seeded value with an EnclaveKey2D key as input to produce two unit vectors.
		// Return their similiarity, which falls between between 0 and 1.
		float getSeededVecSimiliarity(int in_seed_value,
			EnclaveKeyDef::Enclave2DKey in_twoDKey)
		{
			float returnValue = 0.0f;

			// Step 1: initialize a new "super" generator with the given seed super seed;
			std::mt19937_64 superGenerator(in_seed_value);
			std::uniform_int_distribution<> superFloatDistribution(0, 1000);
			float superParentValue = superFloatDistribution(superGenerator);

			// Step 2: get a value for another generator, by getting the hash of the current 2d key.
			auto twoDKeyHash = (std::hash<int>()(in_twoDKey.a)
				^ (std::hash<int>()(in_twoDKey.b) << 1)
				) >> 1;

			// The two separate hashes produced by the following Enclave2DKey values will apparently cause 
			// two separate mt19937_64 objects to produce the same values:
			//
			// -2048, 0          (hash computation of 5139928519672806836)
			// -2048, -1024      (hash computation of 8839207905642333590)
			twoDKeyHash += 1;

			// Step 3: create a mt, where it is seeded from the current key's hash value;
			// the range of the distribution will be from 0 to pi.
			std::mt19937_64 currentKeyGenerator(twoDKeyHash);
			std::uniform_int_distribution<> currentKeyDistribution(0, 1000);
			float currentKeyValue = currentKeyDistribution(currentKeyGenerator);

			// By this point we have two ints
			// -constant int from the parent seed
			// -an int generated from a seed calculated from the hash of the 2d key we are looking at
			//
			// We will now multiply these two values together, to get a new int, and create a new generator off of that.
			std::mt19937_64 finalGenerator(superParentValue * currentKeyValue);

			// Create a new float value from the finalGenerator, using float distribution. 
			// The float value should be from to two pi; this value can be used with cos/sin to find a unit vector.
			std::uniform_real_distribution<> finalFloatRange(0.0f, 2 * 3.14f);
			float selectedFloatRangeVec1 = finalFloatRange(finalGenerator);

			float cosValVec1 = cos(selectedFloatRangeVec1);
			float sinValVec1 = sin(selectedFloatRangeVec1);

			glm::vec2 unitVec1(cosValVec1, sinValVec1);

			float selectedFloatRangeVec2 = finalFloatRange(finalGenerator);

			float cosValVec2 = cos(selectedFloatRangeVec2);
			float sinValVec2 = sin(selectedFloatRangeVec2);

			glm::vec2 unitVec2(cosValVec2, sinValVec2);

			returnValue = getVectorSimiliarity(unitVec1, unitVec2);

			std::cout << "(DEBUG): EnclaveKey2D value: ";
			in_twoDKey.printKey();
			std::cout << "| twoDKeyHash: " << twoDKeyHash << " | currentKeyValue: " << currentKeyValue << " | float range vec1: " << selectedFloatRangeVec1 << " | float range vec2: " << selectedFloatRangeVec2 << " | returnValue: " << returnValue << std::endl;

			return returnValue;
		}

		// Below: returns a value between 0 and 1, that represents how similiar two unit vectors are.
		// A value of 1 indicates that they are exactly the same, and a value of 0 means they are polar oppposites.
		float getVectorSimiliarity(glm::vec2 in_vecA, glm::vec2 in_vecB)
		{		
			// get the dot product.
			float abDot = dot(in_vecA, in_vecB);

			// get the magnitude of vector A.
			float magA = sqrt(pow(in_vecA.x, 2.0) + pow(in_vecA.y, 2.0));

			// get the magnitude of vector B.
			float magB = sqrt(pow(in_vecB.x, 2.0) + pow(in_vecB.y, 2.0));

			// calculate the vector similarity, which should be the dot product divided, by (magA * magB)
			float similarityRatio = (abDot / (magA * magB));

			// now, take the similarity value and plug into acos, and multiply it by radians to get the degrees.
			float similiarDegrees = acos(similarityRatio);
			similiarDegrees *= 57.2958;

			return 1 - (similiarDegrees / 180);		
		}

		// Below: interpolate cubically, between points p1 and p2, using the value of t.
		// T must be a range between 0 and 1; a value of 0 will be equal to in_p1, and a value of 1 wil return a value equal to in_p2.

		float interpolateCubically(float in_p0, float in_p1, float in_p2, float in_p3, float t)
		{
			float returnValue = 0.0f;

			float A = 2 * (in_p1 - in_p2) + .5 * (in_p2 - in_p0 + in_p3 - in_p1);
			float B = 3 * (in_p2 - in_p1) - in_p2 + in_p0 - .5 * (in_p3 - in_p1);
			float C = .5 * (in_p2 - in_p0);
			float D = in_p1;

			returnValue = A * pow(t, 3.0) + B * pow(t, 2.0) + C * t + D;

			return returnValue;
		}

    private:
};

#endif
