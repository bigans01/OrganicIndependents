#pragma once

#ifndef RESOLVERLINK_H
#define RESOLVERLINK_H

#include "ECBPolyPoint.h"

class ResolverLink
{
	public:
		ResolverLink() {};
		ResolverLink(ECBPolyPoint in_originalPoint) : originalPoint(in_originalPoint), resolvedPoint(in_originalPoint) {};
		ECBPolyPoint originalPoint;
		ECBPolyPoint resolvedPoint;
};

#endif
