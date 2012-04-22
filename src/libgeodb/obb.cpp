#include <libgeodb/geodb.h>

using namespace meta;

OrientedBBox::OrientedBBox()
{
	memset(&mOBB_, 0, sizeof(GeoBBox));
	memset(&mAABB_, 0, sizeof(GeoBBox));
	memset(&mPos_, 0, sizeof(GeoCoor));
	memset(&mRot_, 0, sizeof(GeoCoor));
	memset(&mScale_, 0, sizeof(GeoCoor));
}
	
OrientedBBox::OrientedBBox(const GeoBBox &box, const GeoCoor &pos, const GeoCoor &rot, const GeoCoor &scale)
{
	defineBBox(box, pos, rot, scale);
}

void OrientedBBox::defineBBox(const GeoBBox &box, const GeoCoor &pos, const GeoCoor &rot, const GeoCoor &scale)
{
	memcpy(&mPos_, &pos, sizeof(GeoCoor));
	memcpy(&mRot_, &rot, sizeof(GeoCoor));
	memcpy(&mScale_, &scale, sizeof(GeoCoor));
}

const GeoBBox& OrientedBBox::getOBB() const
{
}

const GeoBBox& OrientedBBox::getAABB() const
{
}

const GeoTagListType& OrientedBBox::filter(const GeoTagListType &raw)
{
}
