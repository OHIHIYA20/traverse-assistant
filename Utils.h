#pragma once
#include "Structures.h"

namespace Utils
	{
	void SplitString(const CString &value, TCHAR delimiter, CStringArray &tokens);
	ETraverseType IdentifyObservationType(const Observation &obs, CString setupName, const CArray<OccupiedStation> &stations);
	};
