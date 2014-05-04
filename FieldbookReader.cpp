#include "StdAfx.h"
#include "FieldbookReader.h"
#include "Utils.h"

CFieldbookReader::CFieldbookReader(CArray<TraverseObservation> &legs, CArray<OccupiedStation> &stations)
: m_legs(legs)
, m_stations(stations)
, m_routeSequence(-1)
	{
	}

CFieldbookReader::~CFieldbookReader(void)
	{
	}

bool CFieldbookReader::ReadFieldbook(const CString &filename)
	{
	m_stations.RemoveAll();
	m_legs.RemoveAll();
	m_error.Empty();

	bool ret = ParseFile(filename);
	if (!ret)
		return false;

	IdentifyDirection();

	return ret;
	}

bool CFieldbookReader::ParseFile(const CString &filename)
	{
	CStdioFile file(filename, CFile::modeRead);

	CString sLine;
	int nLine = 0;
	while (file.ReadString(sLine))
		{
		++nLine;
		CStringArray fields;
		Utils::SplitString(sLine, _T(','), fields);
		if (fields.GetCount() != (INT_PTR)FF_NumFields)
			{
			m_error.Format(_T("Line %d doesn't contain the expected number of fields."), nLine);
			return false;
			}

		TCHAR code = fields.GetAt(FF_Code).GetAt(0);
		switch (code)
			{
			case _T('{'):
				ReadNextStation(fields);
				break;
			case _T('%'):
				ReadOrientation(fields);
				break;
			case _T('u'):
				ReadUnorientedObs(fields);
				break;
			default:
				break;
			}
		}

	return true;
	}

void CFieldbookReader::ReadNextStation(const CStringArray &fields)
	{
	OccupiedStation station;
	station.setupName = fields.GetAt(FF_Name);
	station.instrHeight = _tstof(fields.GetAt(FF_Target));

	m_stations.Add(station);

	++m_routeSequence;
	}

Observation CFieldbookReader::ReadObservation(const CStringArray &fields)
	{
	Observation o;
	o.targetName = fields.GetAt(FF_Name);
	o.horizontalAngle = _tstof(fields.GetAt(FF_HorizontalCL));
	o.slopeDistance = _tstof(fields.GetAt(FF_SlopeDistance));
	o.verticalAngle = _tstof(fields.GetAt(FF_VerticalCL));
	o.targetHeight = _tstof(fields.GetAt(FF_Target));

	return o;
	}

void CFieldbookReader::ReadOrientation(const CStringArray &fields)
	{
	Observation obs = ReadObservation(fields);

	TraverseObservation tobs;
	tobs.routeSequence = m_routeSequence;
	tobs.type = TT_Orientation;
	tobs.obs = obs;

	m_legs.Add(tobs);
	}

void CFieldbookReader::ReadUnorientedObs(const CStringArray &fields)
	{
	Observation obs = ReadObservation(fields);

	TraverseObservation tobs;
	tobs.routeSequence = m_routeSequence;
	tobs.type = TT_None;
	tobs.obs = obs;

	m_legs.Add(tobs);
	}

void CFieldbookReader::IdentifyDirection()
	{
	for (INT_PTR i = 0; i < m_legs.GetCount(); ++i)
		{
		TraverseObservation &tobs = m_legs.GetAt(i);

		if (tobs.type == TT_None)
			{
			CString setupName = m_stations.GetAt(tobs.routeSequence).setupName;
			tobs.type = Utils::IdentifyObservationType(tobs.obs, setupName, m_stations);
			}
		}
	}
