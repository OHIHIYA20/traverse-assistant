#include "StdAfx.h"
#include "FieldbookReader.h"
#include "Utils.h"

CFieldbookReaderWriter::CFieldbookReaderWriter(CArray<TraverseObservation> &legs, CArray<OccupiedStation> &stations)
: m_legs(legs)
, m_stations(stations)
, m_routeSequence(-1)
	{
	}

CFieldbookReaderWriter::~CFieldbookReaderWriter(void)
	{
	}

bool CFieldbookReaderWriter::ReadFieldbook(const CString &filename)
	{
	m_stations.RemoveAll();
	m_legs.RemoveAll();
	m_error.Empty();
	m_routeSequence = -1;

	bool ret = ParseFile(filename);
	if (!ret)
		return false;

	IdentifyDirection();

	return ret;
	}

bool CFieldbookReaderWriter::ParseFile(const CString &filename)
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

void CFieldbookReaderWriter::ReadNextStation(const CStringArray &fields)
	{
	OccupiedStation station;
	station.setupName = fields.GetAt(FF_Name);
	station.instrHeight = _tstof(fields.GetAt(FF_Target));

	m_stations.Add(station);

	++m_routeSequence;
	}

Observation CFieldbookReaderWriter::ReadObservation(const CStringArray &fields)
	{
	Observation o;
	o.targetName = fields.GetAt(FF_Name);
	o.horizontalAngle = _tstof(fields.GetAt(FF_HorizontalCL));
	o.slopeDistance = _tstof(fields.GetAt(FF_SlopeDistance));
	o.verticalAngle = _tstof(fields.GetAt(FF_VerticalCL));
	o.targetHeight = _tstof(fields.GetAt(FF_Target));

	return o;
	}

void CFieldbookReaderWriter::ReadOrientation(const CStringArray &fields)
	{
	Observation obs = ReadObservation(fields);

	TraverseObservation tobs;
	tobs.routeSequence = m_routeSequence;
	tobs.type = TT_Orientation;
	tobs.obs = obs;

	m_legs.Add(tobs);
	}

void CFieldbookReaderWriter::ReadUnorientedObs(const CStringArray &fields)
	{
	Observation obs = ReadObservation(fields);

	TraverseObservation tobs;
	tobs.routeSequence = m_routeSequence;
	tobs.type = TT_None;
	tobs.obs = obs;

	m_legs.Add(tobs);
	}

void CFieldbookReaderWriter::IdentifyDirection()
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

bool CFieldbookReaderWriter::WriteFieldbook(CString &csv)
	{
	csv.Empty();
	m_error.Empty();
	m_routeSequence = -1;

	for (INT_PTR i = 0; i < m_legs.GetCount(); ++i)
		{
		TraverseObservation tobs = m_legs.GetAt(i);

		if (tobs.routeSequence != m_routeSequence)
			{
			OccupiedStation station = m_stations.GetAt(tobs.routeSequence);
			WriteNextStation(station, csv);
			m_routeSequence = tobs.routeSequence;
			}
		else
			{
			switch (tobs.type)
				{
				case TT_Orientation:
					WriteNextObservation(tobs.obs, _T('%'), csv);
					break;
				case TT_Forward:
				case TT_Backward:
					WriteNextObservation(tobs.obs, _T('u'), csv);
					break;
				case TT_None:
				default:
					break;
				}
			}
		}

	return true;
	}

void CFieldbookReaderWriter::WriteNextStation(const OccupiedStation &station, CString &csv)
	{
	CString line;
	line.Format(_T("{,%s,,,,,,%.3f\n"), station.setupName, station.instrHeight);

	csv += line;
	}

void CFieldbookReaderWriter::WriteNextObservation(const Observation &obs, TCHAR code, CString &csv)
	{
	CString line;
	line.Format(_T("%c,%s,%.4f,,%.3f,%.4f,,%.3f\n"), 
		code,
		obs.targetName,
		obs.horizontalAngle,
		obs.slopeDistance,
		obs.verticalAngle,
		obs.targetHeight);

	csv += line;
	}
