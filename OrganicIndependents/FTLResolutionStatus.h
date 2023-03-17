#pragma once

#ifndef FTLRESOLUTIONSTATUS_H
#define FTLRESOLUTIONSTATUS_H

enum class FTLResolutionStatus
{
	FTLR_NOVAL,		// default value
	FTLR_VALID,		// most cases where nothing is wrong (won't erase)
	FTLR_PURGABLE,	// cases where it's OK to purge it; OK to erase in this case
	FTLR_INCALCULABLE	// unable to determine anything (probably requires special handling)
};

#endif
