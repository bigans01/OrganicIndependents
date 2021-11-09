#pragma once

#ifndef POINTRESOLUTIONMETHOD_H
#define POINTRESOLUTIONMETHOD_H

#include "Message.h"
#include "PRResult.h"

class PointResolutionMethod
{
	public:
		virtual void setupMethod(Message in_message) = 0;
		virtual void run() = 0;
		virtual PRResult getResult() = 0;
	private:

};

#endif
