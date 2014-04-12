#pragma once

#include "tinyxml.h"

struct XmlDetails
{
	CString filename;
	TiXmlDocument xmlDocument;
	const TiXmlElement *surveyXml;

	XmlDetails() : surveyXml(NULL) {}
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

struct TraverseObservation /*: public UI_Element*/
{
	int stationSetup;
	ETraverseType type;
	int routeSequence;
	Observation obs;

	TraverseObservation() : stationSetup(0), type(TT_NONE), routeSequence(0) {}
};

class TraverseRoute
{
	CStringArray m_stationNames;
};

//static int StationOrderingFunc(const UI_Element *a, const UI_Element *b)
//{
//	TraverseObservation *ta = dynamic_cast<TraverseObservation *>(a);
//	TraverseObservation *tb = dynamic_cast<TraverseObservation *>(b);
//	if (ta && tb)
//	{
//		if (ta->routeSequence == tb->RouteSequence)
//			return ta->type < tb->type)
//		else
//			return ta->routeSequence < tb->RouteSequence
//	}
//	return 0;
//}
