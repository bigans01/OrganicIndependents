#pragma once

#ifndef ECBPOLYTYPE_H
#define ECBPOLYTYPE_H

enum class ECBPolyType
{
	FREE,				// used when no definition is needed (such as when performing Shattering operations in ECBPolyReformer)
	ROGUE,
	SHELL,				// typical shell, outer "crust" generation
	SHELL_MASSDRIVER	// shell, outer crust, but also fills towards positive Y (or other axis...maybe? just positive Y for now).
};

#endif
