#pragma once

#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

enum class MessageType
{
	REQUEST,
	RESPONSE,

	// client requests
	REQUEST_FROM_CLIENT_BLUEPRINTS_FOR_OGLMBUFFERMANAGER,
	REQUEST_FROM_CLIENT_GET_BLUEPRINT_FOR_T1,
	REQUEST_FROM_CLIENT_GET_BLUEPRINT_FOR_T2,
	REQUEST_FROM_CLIENT_RUN_CONTOUR_PLAN,
	REQUEST_FROM_CLIENT_TOGGLE_IMGUI_WORLD_LOCATION,
	REQUEST_FROM_CLIENT_TOGGLE_IMGUI_TARGETED_BLOCK,
	REQUEST_FROM_CLIENT_TOGGLE_IMGUI_TEXT_INPUTS,
	REQUEST_FROM_CLIENT_TOGGLE_IMGUI_SLIDERS,
	REQUEST_FROM_CLIENT_INPUT_GENERATED,			// used when a OrganicSystem detects input from user and sends it to server; MESSAGE CHAIN: clientSendsInput, 1 of 2.

	// response from server
	RESPONSE_FROM_SERVER_PROCESS_BLUEPRINT_WHEN_RECEIVED,
	RESPONSE_FROM_SERVER_BLUEPRINT_T1_FOUND,
	RESPONSE_FROM_SERVER_BLUEPRINT_T2_FOUND,
	RESPONSE_FROM_SERVER_TOGGLE_IMGUI_WORLD_LOCATION,
	RESPONSE_FROM_SERVER_INPUT_GENERATED,			// used when an OrganicSystem sever sends a Message back to an OrganicSystem instance, indicating whether or not the 
													// the received input was valid/OK; MESSAGE CHAIN: clientSendsInput, 2 of 2.
	NOVAL,

	// request from server
	REQUEST_FROM_SERVER_SEND_BLUEPRINTS_FOR_OGLMBUFFERMANAGER,
	REQUEST_FROM_SERVER_SET_WORLD_DIRECTION,

	// server job event type
	SERVER_JOB_EVENT_UPDATE_INT,
	SERVER_JOB_EVENT_UPDATE_STRING,

	// block highlight messaging (MESSAGE CHAIN: toggleBlockHighlighting)
	REQUEST_FROM_CLIENT_TOGGLE_BLOCK_HIGHLIGHTING,
	RESPONSE_FROM_SERVER_TOGGLE_BLOCK_HIGHLIGHTING,

	// current enclave highlight messaging (MESSAGE CHAIN: toggleCurrentEnclaveHighlighting)
	REQUEST_FROM_CLIENT_TOGGLE_CURRENT_ENCLAVE_HIGHLIGHTING,
	RESPONSE_FROM_SERVER_TOGGLE_CURRENT_ENCLAVE_HIGHLIGHTING,

	// OpenGL shader machine and gear messages
	OPENGL_REGISTER_DYN_BUFFER_IN_GEAR,					// will signal an underlying Gear to register a dynamic draw buffer.
	OPENGL_REGISTER_DYN_BUFFER_MULTIDRAW_JOB_IN_GEAR,	// signals a gear to fetch a multi draw job for a named buffer from the machine, and interpret this data to form its own VAO for the draw call.
	OPENGL_DEREGISTER_DYN_BUFFER_MULTIDRAW_JOB_IN_GEAR,	// signals a gear to remove any multi draw job objects for the specified named buffer

	// OpenGLManager / JobTriggerBase messages
	OPENGLMANAGER_UPDATE_CAMERA_ENCLAVE_HIGHLIGHT,
	OPENGLMANAGER_DISABLE_CAMERA_ENCLAVE_HIGHLIGHT,

	// cleave sequence error handling
	CLEAVESEQUENCEFACTORY_SEQUENCE_INCOMPLETE,			// indicates that there was at least one incomplete cleave sequence 

	// PRMA2D Resolution (XY, XZ, YZ)
	PRMA2DRESOLVER_XY,
	PRMA2DRESOLVER_XZ,
	PRMA2DRESOLVER_YZ
};

#endif
