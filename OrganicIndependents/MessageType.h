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
	OPENGLMANAGER_UPDATE_ORE_COMPOSITION_HIGHLIGHT,
	OPENGLMANAGER_DISABLE_ORE_COMPOSITION_HIGHLIGHT,

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
	SPOLY_ORIGINATES_FROM_NONEXISTENT_BECAUSE_FULL,	// indicates that the SPoly origins were from a artifical box-shell (non-existent block)

	// ImGui messages
	IMGUI_SEND_INPUT_FEEDBACK_TO_IMGUI_INPUT_TEXT,	// indicates that this message contains keystroke/input feedback destined for a ImGuiInputText object (OrganicGLWinLib).
													// The message should contain three values: 
													// 1. a string for the name of the keyed ImGuiInputTextPanel as it exists in ImGuiInputTextPanelContainer
													// 2. a string for the name of the keyed ImGuiInputText as it exists in the ImGuiInputTextPanel from step 1 above
													// 3. an int value representing the OpenGL keystroke enumeration to process (i.e, GLFW_KEY_W)
													
	// Multi-purpose codes
	CLEAVESEQUENCE_CATEGORIZED_LINES,				// Can be used in multiple places; this message signifies that the message contains one 
													// or more categorized lines.

	// Blueprint data messages
	BDM_BLUEPRINT_HEADER,	// Stores metadata about the blueprint, such as the number of ECBPolys, and the total number of OREs in the blueprint.
	BDM_BLUEPRINT_ECBPOLY,	// Stores data required for an ECBPoly to be rebuilt from a Message.
	BDM_ORE_HEADER,			// Stores data about an OrganicRawEnclave, such as the number of different types of blocks,
							// it's currentLodState, currentAppendedState, and the currentDependencyState. The first 3 int's of this Message need to form an EnclaveKey
							// that represents the location of the ORE (i.e, an ORE at key 5,7,2)			
			
	BDM_ORE_SKELETONSGM,	// Stores data that can be used to populate an empty instance of EnclaveTriangleSkeletonSupergroupManager.
							// The structure of this constructed Message should be as follows: the blueprint key, the ore key, the number of skeletons, 
							// followd by skeleton data that is written-in via a loop. 
							// (see the function EnclaveTriangleSkeletonSupergroupManager::convertSkeletonSGMToBDM for the writing methodology of this Message type)

	BDM_BLOCK_FANMANAGER,	// This type indicates that the Message includes all necessary metadata for the FanManager to be reconstructed. 
							// This type should be constructed via the FanManager::writeFanManagerToBDMFormat() function. 

	BDM_BLOCK_UNTAGGED,		// This Message would indicate that it contains all data necessary to construct an EnclaveBlock, but it doesn't contain the keys (Blueprint, ORE, Block) 
							// that specify the world position that is relevant to the data

	BDM_BLOCK_TAGGED,		// The Message should contain everything the BDM_BLOCK_UNTAGGED does, but it should also have the specific Blueprint, ORE, and block keys at the front of the message, in that order;
							// Remember, that to put it in that order when the Message already contains data, that you will have to call Message::insertEnclaveKeyFront to insert the block key.

	BDM_SKELETONBLOCK_TAGGED,	// The Message contains data relative to a skeleton block; it should have the specific Blueprint, ORE and block key that this pertains to as well.


	// Direct buffer data to Gear transmission
	SHADERMACHINE_BUFFER_DATA_COLLECTION_HIGHLIGHT	// Should contain, in the following order:
													//	-an EnclaveKey value representing the blueprint to highlight
													//	-an int representing the number of triangles to draw.
													

};

#endif
