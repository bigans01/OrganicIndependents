#pragma once

#ifndef STRINGTOMESSAGEBUILDER_H
#define STRINGTOMESSAGEBUILDER_H

#include "MessageBuildResult.h"
#include <iostream>

class StringToMessageBuilder
{
	public:
		MessageBuildResult buildMessageFromString(std::string in_string);
	private:

};

#endif
