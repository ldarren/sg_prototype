#include <libgeodb/geodb.h>

using namespace meta;

GeoDB::GeoDB()
{
}

GeoDB::~GeoDB()
{
}

U32 GeoDB::create(const GeoCoor &max, const GeoCoor &min)
{
	return 0;
}

void GeoDB::destroy(U32 id)
{
}

bool GeoDB::add(const GeoTag &tag)
{
	return false;
}

bool GeoDB::add(float lat, float lon, float alt, const std::string &url)
{
	return false;
}

void GeoDB::center(GeoCoor *pt)
{
}

GeoNode* GeoDB::query(const GeoCoor &max, const GeoCoor &min, float *rot)
{
	return 0;
}
