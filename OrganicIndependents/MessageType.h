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
	REQUEST_FROM_CLIENT_INPUT_GENERATED,				// used when a OrganicSystem detects input from user and sends it to server; MESSAGE CHAIN: clientSendsInput, 1 of 2.
	REQUEST_FROM_CLIENT_MOUSE_SCROLL_INPUT_GENERATED,	// used to identify that the message pertains to positive or negative mouse scroll input
	REQUEST_FROM_CLIENT_REMOVE_BLOCK,					// used to signify that the message contains data for requesting the removal of a block in the world

	// response from server
	RESPONSE_FROM_SERVER_PROCESS_BLUEPRINT_WHEN_RECEIVED,
	RESPONSE_FROM_SERVER_BLUEPRINT_T1_FOUND,
	RESPONSE_FROM_SERVER_BLUEPRINT_T2_FOUND,
	RESPONSE_FROM_SERVER_TOGGLE_IMGUI_WORLD_LOCATION,
	RESPONSE_FROM_SERVER_INPUT_GENERATED,			// used when an OrganicServer sends a Message back to an OrganicSystem instance, indicating whether or not the 
													// the received input was valid/OK; MESSAGE CHAIN: clientSendsInput, 2 of 2.
	RESPONSE_FROM_SERVER_MOUSE_SCROLL_INPUT_GENERATED,	// used when an OrganicServer sends a Message back to an OrganicSystem instance, indicating whether or not the 
														// the received mouse scroll was valid/OK; MESSAGE CHAIN: clientSendsMouseScroll, 2 of 2.
	RESPONSE_FROM_SERVER_REMOVE_BLOCK,				// indicates whether or not the corresponding OrganicServer approves or disapproves of an attempt to 
													// remove a block at a specified location.
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

	// ORE block composition highlighting (MESSAGE CHAIN: enableORECompositionHighlighting)
	REQUEST_FROM_CLIENT_ENABLE_ORE_COMPOSITION_HIGHLIGHTING,
	RESPONSE_FROM_SERVER_ENABLE_ORE_COMPOSITION_HIGHLIGHTING,
	REQUEST_FROM_CLIENT_DISABLE_ORE_COMPOSITION_HIGHLIGHTING,
	RESPONSE_FROM_SERVER_DISABLE_ORE_COMPOSITION_HIGHLIGHTING,

	// Block target/analysis messages (MESSAGE CHAIN: getBlockData)
	REQUEST_FROM_CLIENT_GET_BLOCK_DATA,
	RESPONSE_FROM_SERVER_GET_BLOCK_DATA,

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
	PRMA2DRESOLVER_YZ,

	// SPoly shell production messages (SPolyShellProducer)
	// See their usages in:
	// -SPolyShellProducer::convertBoundarySPolySupergroupToMessage (OrganicGLWinLib)
	SPOLYSHELLPRODUCER_BOUNDARY_PRODUCED_FROM_INPUT,		// signifies that the Message contains melted SPolys, 
															// formed from a SPolyShellProducer that had input SPolys. 
															
	SPOLYSHELLPRODUCER_BOUNDARY_PRODUCED_FROM_NO_INPUT,		// signifies that the Message contains melted SPolys, 
															// formed from a SPolyShellProducer that had *NO* input SPolys. 

	// SPoly origin 
	// See their usages in:
	// -OrganicSystem::jobCheckIfPolysExistInBlockFace (OrganicCoreLib)
	// -RJPhasedDeleteBlock::interpretPhasedJobMessage (OrganicCoreLib)
	SPOLY_ORIGINATES_FROM_EXPOSED_OR_UNEXPOSED,		// indicates that the SPoly origins were from an existing EXPOSED or UNEXPOSED block.
	SPOLY_ORIGINATES_FROM_NONEXISTENT_BECAUSE_FULL	// indicates that the SPoly origins were from a artifical box-shell (non-existent block)

};

#endif
