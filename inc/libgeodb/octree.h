#pragma once
#ifndef _META_GEO_DB_LIB_OCTREE_H_
#define _META_GEO_DB_LIB_OCTREE_H_

#include <libgeodb/geodb.h>
#include <meta/types.h>

namespace meta {

class OctreeNode
{
public:
	enum
	{
		ECHILD_TNW,
		ECHILD_TNE,
		ECHILD_TSE,
		ECHILD_TSW,
		ECHILD_BNW,
		ECHILD_BNE,
		ECHILD_BSE,
		ECHILD_BSW,

		ECHILD_Count
	};

	static const U8 CHILD_TYPE[ECHILD_Count];		// index -> type
	static const U8 CHILD_INDEX[ECHILD_Count];		// type -> index

public:
	OctreeNode(OctreeNode* parent, U8 idx, U8 lvl, U8 maxlvl, const GeoBBox &bbox);
	virtual ~OctreeNode();

	OctreeNode* getParent();

	U8 getLevel() const;
	void setMaxLevel(U8 lvl);

	const GeoBBox& getBoundingBox() const;

	U8 isDirectChild(OctreeNode *node);
	bool isChild(OctreeNode *node);
	OctreeNode** getAllChilds();
	OctreeNode* getChild(U8 id);
	/// always return a value, return 0 only if coor is out of bound
	/// at last level getChild return it's own address
	OctreeNode* getChild(const GeoCoor &coor);

	// query
	OctreeNode* begin(const GeoBBox &box);
	OctreeNode* begin(U8 lvl);
	OctreeNode* next(const GeoBBox &box, OctreeNode *root=0, OctreeNode *caller=0);
	OctreeNode* next(U8 lvl=0, OctreeNode *root=0, OctreeNode *caller=0);

	OctreeNode* search(const GeoCoor &coor, bool outofscope=true);

	// geotag
	bool addTag(const GeoCoor &coor, GeoTag *tag);
	const GeoTagListType& getTags() const;

private:
	// identity
	U8 nLevel_;
	U8 nMaxLevel_;
	U8 nIndex_;

	// query data
	U8 nQueryLvl_;
	OctreeNode *pQueryRoot_;

	// link
	OctreeNode *pParent_;
	OctreeNode *pChilds_[ECHILD_Count];

	// payload
	GeoBBox mBBox_;
	GeoCoor mLen_;
	GeoTagListType mGeoTagList_;
};

} // namespace meta

#endif // _META_GEO_DB_LIB_OCTREE_H_