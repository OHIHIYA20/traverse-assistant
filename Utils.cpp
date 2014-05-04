#include "stdafx.h"
#include "Utils.h"

namespace Utils
	{

	void SplitString(const CString &value, TCHAR delimiter, CStringArray &tokens)
	{
		int start = 0;
		while (true)
		{
			int end = value.Find(delimiter, start);
			if (end == -1)
			{
				CString token = value.Mid(start);
				token.Trim();
				tokens.Add(token);
				break;
			}
			else
			{
				CString token = value.Mid(start, end - start);
				token.Trim();
				tokens.Add(token);
				start = end + 1;
			}
		}
	}

	ETraverseType IdentifyObservationType(const Observation &obs, CString setupName, const CArray<OccupiedStation> &stations)
	{
		int nLeg = -1;
		int nStations = stations.GetCount();

		for (INT_PTR i = 0; i < nStations; ++i)
		{
			CString legName = stations.GetAt(i).setupName;
			if (legName == setupName)
				{
				nLeg = i;

				CString previousStation, nextStation;
				if (nLeg > 0)
					previousStation = stations.GetAt(nLeg - 1).setupName;
				if (nLeg < nStations - 1)
					nextStation = stations.GetAt(nLeg + 1).setupName;

				if (obs.targetName == previousStation)
					return TT_Backward;
				else if (obs.targetName == nextStation)
					return TT_Forward;
				}
		}
		
		if (nLeg == -1)
			return TT_None;
		else /* if targetName point exists in db */
			return TT_Orientation;
	}

	};
