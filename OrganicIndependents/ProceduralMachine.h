#pragma once

#ifndef PROCEDURALMACHINE_H
#define PROCEDURALMACHINE_H

/*
* 
* Description: This class is meant to take in absolute world locations in the world, and check to see 
* if it needs to do anything based on that location in the world. Each ProceduralMachine can have its own implementation
* of how it will respond to the world position; i.e, some ProceduralMachine descendants may respond when the blueprint location
* changes, or only when the sector of the blueprint changes.
* 
* The response should serve as an indicator that there MAY be work to do; it is up to the caller to determine if it should do the work
* when the ProceduralMachine sends back it's query data. 
* 
* If the decision is made to do the work, notify the same ProceduralMachine that sent the work request that it may do so.

*/

class ProceduralMachine
{
	public:

	private:
};

#endif
