#pragma once

#include "tinyxml.h"

struct XmlDetails
{
	CString filename;
	TiXmlDocument xmlDocument;
	const TiXmlElement *surveyXml;

	XmlDetails() : surveyXml(NULL) {}
	void Reset() {filename.Empty(); surveyXml = NULL; }
};

enum ETraverseType
{
	TT_ORIENTATION, TT_FORWARD, TT_BACKWARD, TT_NETWORK, TT_NONE
};

struct Observation
{
	CString targetName;
	CString description;
	CString tag;
	double targetHeight;
	double horizontalAngle;
	double verticalAngle;
	double slopeDistance;

	Observation() : targetHeight(0.0), horizontalAngle(0.0), verticalAngle(0.0), slopeDistance(0.0) {}
};

struct TraverseObservation
{
	ETraverseType type;
	int routeSequence;
	Observation obs;

	TraverseObservation() : type(TT_NONE), routeSequence(0) {}

	static bool SortByType(const TraverseObservation &a, const TraverseObservation &b)
	{
		if (a.routeSequence == b.routeSequence)
		{
			if (a.type == b.type)
				return a.obs.targetName.Compare(b.obs.targetName) < 0;
			else
				return (int)a.type < (int)b.type;
		}
		else
			return a.routeSequence < b.routeSequence;
	}
};

struct OccupiedStation
	{
	CString setupName;
	int setupNumber;
	double instrHeight;

	OccupiedStation() : setupNumber(0), instrHeight(0.0) {}
	void Reset() { setupName.Empty(); setupNumber = 0; instrHeight = 0.0; }
	};
