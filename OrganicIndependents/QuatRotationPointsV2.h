#pragma once

#ifndef QUATROTATIONPOINTSV2_H
#define QUATROTATIONPOINTSV2_H

#include "FTrianglePoint.h"
#include <iostream>

class QuatRotationPointsV2
{
	public:
		template<typename FirstOption, typename ...RemainingOptions> void insertPointRefs(FirstOption&& firstOption, RemainingOptions && ...optionParams)
		{
			std::cout << "Trying out template for QuatRotationPointsV2..." << std::endl;

			//pointsRefVector.push_back(std::forward<FirstOption>(firstOption)); 
			//if constexpr      (std::is_same<FirstOption, glm::vec3&&>::value) { std::cout << "Found glm::vec3 ref!" << std::endl; }
			if      constexpr (std::is_same_v<std::remove_cvref_t<FirstOption>, glm::vec3*>) 
			{ 
				createGlmVec3Proxy(std::forward<FirstOption>(firstOption));
				std::cout << "Found glm::vec3 ref! (glm::vec3*)" << std::endl; 
			}

			else if constexpr (std::is_same<FirstOption, FTrianglePoint*>::value) 
			{ 
				createDoublePointProxy(std::forward<FirstOption>(firstOption));
				std::cout << "Found FTrianglePoint ref!" << std::endl; 
			}

			insertPointRefs(std::forward<RemainingOptions>(optionParams)...);
			
		}
		void insertPointRefs() {};

		void applyQuaternion(glm::quat in_quat);
		void applyTranslation(glm::vec3 in_translation);
		void applyTranslationToIndexRange(glm::vec3 in_translation, int in_indexStart, int in_indexEnd);
		void printTrianglePoints();
		void applyNormalization(float in_normalizationValue);
		void applyDimensionCorrections();
		void printPoints();
		void clearPoints();
		void roundAllPointsToHundredths();
		int getSize();
		void eraseLastElement();

		glm::vec3 getFirstPoint();
		glm::vec3 getSecondPoint();
		glm::vec3 getThirdPoint();
		glm::vec3 getPointByIndex(int in_index);

	private:

		enum class PointProxyMode
		{
			PP_NOVAL,
			PP_GLMVEC3,
			PP_DOUBLEPOINT
		};

		class PointProxyBase
		{
			public:
				void setAsDouble(FTrianglePoint* in_fTrianglePoint)
				{
					ppMode = PointProxyMode::PP_DOUBLEPOINT;
					ppDoublePointRef = in_fTrianglePoint;
				}

				void setAsGlmVec3(glm::vec3* in_vec3)
				{
					ppMode = PointProxyMode::PP_GLMVEC3;
					ppVec3Ref = in_vec3;
				}

				virtual void applyQuat(glm::quat in_quat) = 0;
				virtual void translate(glm::vec3 in_translation) = 0;
				virtual void setVec3(glm::vec3 in_vec3) = 0;
				virtual glm::vec3 getVec3() = 0;
			protected:
				PointProxyMode ppMode = PointProxyMode::PP_NOVAL;
				FTrianglePoint* ppDoublePointRef = nullptr;
				glm::vec3* ppVec3Ref = nullptr;

		};

		class PPDoublePoint : public PointProxyBase
		{
			public:
				void applyQuat(glm::quat in_quat)
				{
					// First, get a glm::vec3 version of the point.
					glm::vec3 doublePointVersionedAsGlmVec3(ppDoublePointRef->point.x, ppDoublePointRef->point.y, ppDoublePointRef->point.z);

					// Now, multiply the value by the quaternion (remember, quat is used first.
					doublePointVersionedAsGlmVec3 = in_quat * doublePointVersionedAsGlmVec3;

					// Finally, set the new value.
					ppDoublePointRef->point.x = doublePointVersionedAsGlmVec3.x;
					ppDoublePointRef->point.y = doublePointVersionedAsGlmVec3.y;
					ppDoublePointRef->point.z = doublePointVersionedAsGlmVec3.z;
				}

				void translate(glm::vec3 in_translation)
				{
					ppDoublePointRef->point.x += in_translation.x;
					ppDoublePointRef->point.y += in_translation.y;
					ppDoublePointRef->point.z += in_translation.z;
				}

				void setVec3(glm::vec3 in_vec3)
				{
					ppDoublePointRef->point.x = in_vec3.x;
					ppDoublePointRef->point.y = in_vec3.y;
					ppDoublePointRef->point.z = in_vec3.z;
				}

				glm::vec3 getVec3()
				{
					return glm::vec3(ppDoublePointRef->point.x, ppDoublePointRef->point.y, ppDoublePointRef->point.z);
				}
			private:
		};

		class PPGlmVec3 : public PointProxyBase
		{
			public:
				void applyQuat(glm::quat in_quat)
				{
					glm::vec3 copiedPoint = *ppVec3Ref;
					copiedPoint = in_quat * copiedPoint;
					*ppVec3Ref = copiedPoint;
				}

				void translate(glm::vec3 in_translation)
				{
					glm::vec3 copiedPoint = *ppVec3Ref;
					copiedPoint += in_translation;
					*ppVec3Ref = copiedPoint;
				}

				void setVec3(glm::vec3 in_vec3)
				{
					*ppVec3Ref = in_vec3;
				}

				glm::vec3 getVec3()
				{
					return *ppVec3Ref;
				}
		};

		void createDoublePointProxy(FTrianglePoint* in_doublePointRef)
		{
			std::unique_ptr<PointProxyBase> newProxy = std::make_unique<PPDoublePoint>();
			newProxy->setAsDouble(in_doublePointRef);
			pointProxiesVector.push_back(std::move(newProxy));

		}

		void createGlmVec3Proxy(glm::vec3* in_glmVec3Ref)
		{
			std::unique_ptr<PointProxyBase> newProxy = std::make_unique<PPGlmVec3>();
			newProxy->setAsGlmVec3(in_glmVec3Ref);
			pointProxiesVector.push_back(std::move(newProxy));
		}

		std::vector<std::unique_ptr<PointProxyBase>> pointProxiesVector;
};


#endif