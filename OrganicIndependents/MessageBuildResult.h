#pragma once

#ifndef MESSAGEBUILDRESULT_H
#define MESSAGEBUILDRESULT_H


class MessageBuildResult
{
public:
		MessageBuildResult() {};
		MessageBuildResult(bool in_isMessageValid) :
			isMessageValid(in_isMessageValid)
		{};
		MessageBuildResult(bool in_isMessageValid, Message in_message) :
			isMessageValid(in_isMessageValid),
			resultingMessage(in_message)
		{};
		bool isMessageValid = false;
		Message resultingMessage;
};

#endif
