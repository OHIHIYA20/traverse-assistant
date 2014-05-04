#pragma once

#include "Structures.h"

class CFieldbookReader
	{
	CArray<TraverseObservation> &m_legs;
	CArray<OccupiedStation> &m_stations;
	int m_routeSequence;

	enum EFieldbookFields { FF_Code = 0, FF_Name, FF_HorizontalCL, FF_HorizontalCR, FF_SlopeDistance, FF_VerticalCL, FF_VerticalCR, FF_Target, FF_NumFields};

	bool ParseFile(const CString &filename);
	void ReadNextStation(const CStringArray &fields);
	Observation ReadObservation(const CStringArray &fields);
	void ReadOrientation(const CStringArray &fields);
	void ReadUnorientedObs(const CStringArray &fields);
	void IdentifyDirection();

	public:
		CFieldbookReader(CArray<TraverseObservation> &legs, CArray<OccupiedStation> &stations);
		~CFieldbookReader(void);

		bool ReadFieldbook(const CString &filename);

		CString m_error;
	};
