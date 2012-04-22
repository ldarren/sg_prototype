#pragma once
#ifndef _META_ORIENTED_BOUNDING_BOX_H_
#define _META_ORIENTED_BOUNDING_BOX_H_

#include <libgeodb/geodb.h>

namespace meta {

class OrientedBBox
{
public:
	typedef std::list<GeoTag*> 
public:
	OrientedBBox();
	OrientedBBox(const GeoBBox &box, const GeoCoor &pos, const GeoCoor &rot, const GeoCoor &scale);

	void defineBBox(const GeoBBox &box, const GeoCoor &pos, const GeoCoor &rot, const GeoCoor &scale);
	const GeoBBox& getOBB() const;
	const GeoBBox& getAABB() const;
	const GeoTagListType& filter(const GeoTagListType &raw);

private:
	GeoTagListType	mGeoTagList_;
	GeoBBox			mOBB_;
	GeoBBox			mAABB_;
	GeoCoor			mPos_;
	GeoCoor			mRot_;
	GeoCoor			mScale_;
};

} // namespace meta

#endif // _META_ORIENTED_BOUNDING_BOX_H_