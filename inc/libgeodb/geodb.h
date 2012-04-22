#pragma once
#ifndef _META_GEO_DB_LIBRARY_H_
#define _META_GEO_DB_LIBRARY_H_

#include <meta/types.h>
#include <string>
#include <list>

namespace meta {

struct GeoCoor
{
	enum
	{
		EGEO_Latitude,
		EGEO_Altitude,
		EGEO_Longtitude,
	};
	float fCoor[3];
};

struct GeoTag
{
	std::string sUrl;		///< max url length is 4000 bytes
	void *pData;			///< temp for scenenode pointer
};
typedef std::list<GeoTag*> GeoTagListType;

struct GeoNode
{
	GeoTag mTag;

	GeoNode *pPrev;
	GeoNode *pNext;
};

struct GeoBBox
{
	enum
	{
		EEDGE_LatMax,	///< pos x
		EEDGE_LatMin,	///< neg x
		EEDGE_AltMax,	///< pos y
		EEDGE_AltMin,	///< neg y
		EEDGE_LongMax,	///< pos z
		EEDGE_LongMin,	///< neg z

		EEDGE_Count
	};
	float mEdges[EEDGE_Count];
};

struct GeoDB
{
	GeoDB();
	virtual ~GeoDB();

	U32 create(const GeoCoor &, const GeoCoor &);
	void destroy(U32 id);

	bool add(float lat, float lon, float alt, const std::string &url);
	bool add(const GeoTag &);
	bool Update(float lat, float lon, float alt, const std::string &url);
	bool Update(GeoTag *tag);

	void center(GeoCoor *pt);
	GeoNode* query(const GeoCoor &max, const GeoCoor &min, float *rot);
};

} // namespace meta

#endif // _META_GEO_DB_LIBRARY_H_