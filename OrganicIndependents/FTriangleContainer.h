#pragma once

#ifndef FTRIANGLECONTAINER_H
#define FTRIANGLECONTAINER_H


class FTriangle;
class FTriangleContainer
{
	public:
		std::map<int, FTriangle> fracturedTriangles;
};

#endif
