#pragma once

#ifndef SEGMENTMETA_H
#define SEGMENTMETA_H


class SegmentMeta
{
public:
	ECBPolyPoint beginVertex;	// the begin vertex of the primary line segment
	ECBPolyPoint endVertex;	// the end vertex
	unsigned char lineID = 100;		// the line ID; default value is 100 (could be higher), indicating no value is stored
};

#endif
