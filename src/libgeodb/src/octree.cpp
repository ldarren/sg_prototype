#include <libgeodb/octree.h>

#include <assert.h>

using namespace meta;

const U8 OctreeNode::CHILD_TYPE[OctreeNode::ECHILD_Count] = {
	0x7,	//i(0)->t(7) 111
	0x6,	//i(1)->t(6) 110
	0x4,	//i(2)->t(4) 100
	0x5,	//i(3)->t(5) 101
	0x3,	//i(4)->t(3) 011
	0x2,	//i(5)->t(2) 010
	0x0,	//i(6)->t(0) 000
	0x1,	//i(7)->t(1) 001
};

const U8 OctreeNode::CHILD_INDEX[OctreeNode::ECHILD_Count] = {
	6,	//t(0)->i(6)
	7,	//t(1)->i(7)
	5,	//t(2)->i(5)
	4,	//t(3)->i(4)
	2,	//t(4)->i(2)
	3,	//t(5)->i(3)
	1,	//t(6)->i(1)
	0,	//t(7)->i(0)
};

OctreeNode::OctreeNode(OctreeNode* parent, U8 idx, U8 lvl, U8 maxlvl, const GeoBBox &bbox)
: pParent_(parent), nIndex_(idx), nLevel_(lvl), nMaxLevel_(maxlvl), nQueryLvl_(0), pQueryRoot_(0)
{
	// we r the parent
	if (!parent)
	{
		memcpy(&mBBox_, &bbox, sizeof(mBBox_));
	}
	else
	{
		if (CHILD_TYPE[nIndex_] & 0x4)
		{
			mBBox_.mEdges[GeoBBox::EEDGE_AltMax] = bbox.mEdges[GeoBBox::EEDGE_AltMax];
			mBBox_.mEdges[GeoBBox::EEDGE_AltMin] = (bbox.mEdges[GeoBBox::EEDGE_AltMax] + bbox.mEdges[GeoBBox::EEDGE_AltMin])/2;
		}
		else
		{
			mBBox_.mEdges[GeoBBox::EEDGE_AltMax] = (bbox.mEdges[GeoBBox::EEDGE_AltMax] + bbox.mEdges[GeoBBox::EEDGE_AltMin])/2;
			mBBox_.mEdges[GeoBBox::EEDGE_AltMin] = bbox.mEdges[GeoBBox::EEDGE_AltMin];
		}
		if (CHILD_TYPE[nIndex_] & 0x2)
		{
			mBBox_.mEdges[GeoBBox::EEDGE_LatMax] = bbox.mEdges[GeoBBox::EEDGE_LatMax];
			mBBox_.mEdges[GeoBBox::EEDGE_LatMin] = (bbox.mEdges[GeoBBox::EEDGE_LatMax] + bbox.mEdges[GeoBBox::EEDGE_LatMin])/2;
		}
		else
		{
			mBBox_.mEdges[GeoBBox::EEDGE_LatMax] = (bbox.mEdges[GeoBBox::EEDGE_LatMax] + bbox.mEdges[GeoBBox::EEDGE_LatMin])/2;
			mBBox_.mEdges[GeoBBox::EEDGE_LatMin] = bbox.mEdges[GeoBBox::EEDGE_LatMin];
		}
		if (CHILD_TYPE[nIndex_] & 0x1)
		{
			mBBox_.mEdges[GeoBBox::EEDGE_LongMax] = bbox.mEdges[GeoBBox::EEDGE_LongMax];
			mBBox_.mEdges[GeoBBox::EEDGE_LongMin] = (bbox.mEdges[GeoBBox::EEDGE_LongMax] + bbox.mEdges[GeoBBox::EEDGE_LongMin])/2;
		}
		else
		{
			mBBox_.mEdges[GeoBBox::EEDGE_LongMax] = (bbox.mEdges[GeoBBox::EEDGE_LongMax] + bbox.mEdges[GeoBBox::EEDGE_LongMin])/2;
			mBBox_.mEdges[GeoBBox::EEDGE_LongMin] = bbox.mEdges[GeoBBox::EEDGE_LongMin];
		}
	}
	mLen_.fCoor[GeoCoor::EGEO_Altitude] = 1/(mBBox_.mEdges[GeoBBox::EEDGE_AltMax] - mBBox_.mEdges[GeoBBox::EEDGE_AltMin]);
	mLen_.fCoor[GeoCoor::EGEO_Latitude] = 1/(mBBox_.mEdges[GeoBBox::EEDGE_LatMax] - mBBox_.mEdges[GeoBBox::EEDGE_LatMin]);
	mLen_.fCoor[GeoCoor::EGEO_Longtitude] = 1/(mBBox_.mEdges[GeoBBox::EEDGE_LongMax] - mBBox_.mEdges[GeoBBox::EEDGE_LongMin]);

	if (nMaxLevel_ > nLevel_)
	{
		for (U8 i = 0; i < ECHILD_Count; ++i)
		{
			pChilds_[i] = new OctreeNode(this, i, nLevel_+1, nMaxLevel_, mBBox_);
		}
	}
	else
	{
		memset(pChilds_, 0, sizeof(pChilds_));
	}
}

OctreeNode::~OctreeNode()
{
	nLevel_ = 0;
	nMaxLevel_ = 0;
	nIndex_ = 0;

	pParent_ = 0;

	for (U8 i = 0; i < ECHILD_Count; ++i)
		delete pChilds_[i];
	memset(pChilds_, 0, sizeof(pChilds_));

	memset(&mBBox_, 0, sizeof(mBBox_));
	
	// is root, delete all tags
	if (!pParent_)
	{
		GeoTagListType::iterator iter;
		for (iter = mGeoTagList_.begin(); iter != mGeoTagList_.end(); ++iter)
			delete *iter;
	}
	mGeoTagList_.clear();
}

OctreeNode* OctreeNode::getParent()
{
	return pParent_;
}

U8 OctreeNode::getLevel() const
{
	return nLevel_;
}

void OctreeNode::setMaxLevel(U8 lvl)
{
	if (nMaxLevel_ != lvl)
	{
		// expanding required
		if (nMaxLevel_ < lvl)
		{
			if (nLevel_ == nMaxLevel_)
			{
				assert(pChilds_[0] == 0);
				for (U8 i = 0; i < ECHILD_Count; ++i)
					pChilds_[i] = new OctreeNode(this, i, nLevel_+1, lvl, mBBox_);

				nMaxLevel_ = lvl;
				return;
			}
		}
		else // reducing required
		{
			if (nLevel_ == lvl)
			{
				assert(pChilds_[0] != 0);
				for (U8 i = 0; i < ECHILD_Count; ++i)
					delete pChilds_[i];

				nMaxLevel_ = lvl;
				return;
			}
		}

		if (nLevel_ < lvl && pChilds_[0] != 0)
			for (U8 i = 0; i < ECHILD_Count; ++i)
				pChilds_[i]->setMaxLevel(lvl);

		nMaxLevel_ = lvl;
	}
}

const GeoBBox& OctreeNode::getBoundingBox() const
{
	return mBBox_;
}

U8 OctreeNode::isDirectChild(OctreeNode *node)
{
	// empty childs list
	if (!pChilds_[0])
		return ECHILD_Count;

	for (U8 i = 0; i < ECHILD_Count; ++i)
		if (pChilds_[i] == node)
			return pChilds_[i]->nIndex_;

	return ECHILD_Count;
}

bool OctreeNode::isChild(OctreeNode *node)
{
	assert(node);

	if (nLevel_ >= node->nLevel_)
		return false;

	U8 i;
	// juz 1 level diff
	if ((nLevel_+1) == node->nLevel_)
	{
		for (i=0; i<ECHILD_Count; ++i)
		{
			if (node == pChilds_[i])
				return true;
		}
	}
	else
	{
		for (i=0; i<ECHILD_Count; ++i)
		{
			if (pChilds_[i]->isChild(node))
				return true;
		}
	}

	return false;
}

OctreeNode** OctreeNode::getAllChilds()
{
	return pChilds_;
}

OctreeNode* OctreeNode::getChild(U8 id)
{
	assert(id < ECHILD_Count);
	return pChilds_[id];
}
	
OctreeNode* OctreeNode::getChild(const GeoCoor &coor)
{
	U8 type=0;
	float alt = (coor.fCoor[GeoCoor::EGEO_Altitude] - mBBox_.mEdges[GeoBBox::EEDGE_AltMin])*mLen_.fCoor[GeoCoor::EGEO_Altitude];
	if (alt < 0 || alt > 1)
		return 0;
	else if (alt > 0.5f)
		type = type | 0x4;

	float lat = (coor.fCoor[GeoCoor::EGEO_Latitude] - mBBox_.mEdges[GeoBBox::EEDGE_LatMin])*mLen_.fCoor[GeoCoor::EGEO_Latitude];
	if (lat < 0 || lat > 1)
		return 0;
	else if (lat > 0.5f)
		type = type | 0x2;

	float lon = (coor.fCoor[GeoCoor::EGEO_Longtitude] - mBBox_.mEdges[GeoBBox::EEDGE_LongMin])*mLen_.fCoor[GeoCoor::EGEO_Longtitude];
	if (lon < 0 || lon > 1)
		return 0;
	else if (lon > 0.5f)
		type = type | 0x1;

	if (nLevel_ == nMaxLevel_)
		return this;

	return pChilds_[CHILD_INDEX[type]];
}

OctreeNode* OctreeNode::begin(const GeoBBox &box)
{
	GeoCoor pt8[ECHILD_Count] = {
		{box.mEdges[GeoBBox::EEDGE_LatMax],box.mEdges[GeoBBox::EEDGE_AltMax],box.mEdges[GeoBBox::EEDGE_LongMax]},
		{box.mEdges[GeoBBox::EEDGE_LatMin],box.mEdges[GeoBBox::EEDGE_AltMax],box.mEdges[GeoBBox::EEDGE_LongMax]},
		{box.mEdges[GeoBBox::EEDGE_LatMin],box.mEdges[GeoBBox::EEDGE_AltMax],box.mEdges[GeoBBox::EEDGE_LongMin]},
		{box.mEdges[GeoBBox::EEDGE_LatMax],box.mEdges[GeoBBox::EEDGE_AltMax],box.mEdges[GeoBBox::EEDGE_LongMin]},
		{box.mEdges[GeoBBox::EEDGE_LatMax],box.mEdges[GeoBBox::EEDGE_AltMin],box.mEdges[GeoBBox::EEDGE_LongMax]},
		{box.mEdges[GeoBBox::EEDGE_LatMin],box.mEdges[GeoBBox::EEDGE_AltMin],box.mEdges[GeoBBox::EEDGE_LongMax]},
		{box.mEdges[GeoBBox::EEDGE_LatMin],box.mEdges[GeoBBox::EEDGE_AltMin],box.mEdges[GeoBBox::EEDGE_LongMin]},
		{box.mEdges[GeoBBox::EEDGE_LatMax],box.mEdges[GeoBBox::EEDGE_AltMin],box.mEdges[GeoBBox::EEDGE_LongMin]},
	};

	U8 i;
	OctreeNode *cparent = search(pt8[0]);
	if (!cparent)
		return 0;

	for (i=1; i<ECHILD_Count; ++i)
	{
		while (!cparent->search(pt8[i], false))
		{
			if (cparent->pParent_)
				cparent = cparent->pParent_;
			else
				return 0;
		}
	}

	return cparent->next(box, cparent, cparent);
}

OctreeNode* OctreeNode::begin(U8 lvl)
{ 
	U8 l = nLevel_+lvl;
	return next(l > nMaxLevel_ ? nMaxLevel_ : l, this, this);
}

OctreeNode* OctreeNode::next(const GeoBBox &box, OctreeNode *root, OctreeNode *caller)
{
	if (root != 0)
		pQueryRoot_ = root;

	bool lastchildcall = caller && caller == pChilds_[ECHILD_Count-1];
	bool childcall = isDirectChild(caller) != ECHILD_Count;
	bool lastlevel = nLevel_ == nMaxLevel_;
	bool parentcall = caller && caller == pParent_;
	bool imroot = this == pQueryRoot_;

	// end condition: i'm root and last child call
	if (imroot && lastchildcall)
		return 0;

	// route up condition 1: empty caller
	// route up condition 2: last child call
	if (!caller || lastchildcall)
	{
		if (pParent_)
			return pParent_->next(box, pQueryRoot_, this);
		else
			return 0;
	}

	U8 cond1a = 0x00;
	U8 cond1b = 0x00;
	U8 cond2a = 0x00;
	U8 cond2b = 0x00;
	for (U8 i=0; i<GeoBBox::EEDGE_Count; i+=2)
	{
		cond1a = cond1a << 1;
		cond1b = cond1b << 1;
		cond2a = cond2a << 1;
		cond2b = cond2b << 1;

		if (mBBox_.mEdges[i] > box.mEdges[i] && mBBox_.mEdges[i+1] < box.mEdges[i])
			cond1a |= 0x01;
		if (mBBox_.mEdges[i] > box.mEdges[i+1] && mBBox_.mEdges[i+1] < box.mEdges[i+1])
			cond1b |= 0x01;

		if (box.mEdges[i] > mBBox_.mEdges[i] && box.mEdges[i+1] < mBBox_.mEdges[i])
			cond2a |= 0x01;
		if (box.mEdges[i] > mBBox_.mEdges[i+1] && box.mEdges[i+1] < mBBox_.mEdges[i+1])
			cond2b |= 0x01;
	}

	bool overlapped = cond2a == 0x07 && cond2b == 0x07;
	bool inside = cond1a == 0x07 && cond1b == 0x07;
	bool outside = (cond1a | cond1b | cond2a | cond2b) != 0x07;
	bool intersected = !overlapped && !inside && !outside;

	// true condition 1: not childcall
	// true condition 2: bounding box is overlapped the search box or intersected or enclose with search box and max level reached
	if (caller && !childcall && (overlapped || ((intersected || inside) && lastlevel)))
	{
		return this;
	}

	// route up condition 3: bounding box is completely not inside the search box
	if (outside)
	{
		if (pParent_)
			return pParent_->next(box, pQueryRoot_, this);
		else
			return 0;
	}

	// route down to next child condition 1: intersected and not last level, caller is child by not last child
	// route down to first child condition 1: intersected and not last level, caller is parent
	if ((caller && isDirectChild(caller)!=ECHILD_Count))
	{
		return pChilds_[caller->nIndex_+1]->next(box, pQueryRoot_, this);
	}
	else if (imroot || parentcall)
	{
		return pChilds_[0]->next(box, pQueryRoot_, this);
	}

	return 0;
}

OctreeNode* OctreeNode::next(U8 lvl, OctreeNode *root, OctreeNode *caller)
{
	assert(nMaxLevel_ >= lvl);
	if (lvl != 0)
		nQueryLvl_ = lvl;
	if (root != 0)
		pQueryRoot_ = root;

	bool lastchildcall = caller && caller == pChilds_[ECHILD_Count-1];
	bool matchlevel = nLevel_ == nQueryLvl_;
	bool parentcall = caller && caller == pParent_;
	bool imroot = this == pQueryRoot_;

	// true condition, parent call and level is the same
	if (parentcall && matchlevel)
		return this;

	// end condition, is root and caller is last child
	if (imroot && lastchildcall)
		return 0;

	// route up condition, 1) empty caller, level match 2) last child call and i'm not root
	if ((!caller && matchlevel) || (!imroot && lastchildcall))
		return pParent_->next(nQueryLvl_, pQueryRoot_, this);

	// route down to next child condition: child but not last child
	if (caller && isDirectChild(caller)!=ECHILD_Count && !parentcall && !lastchildcall)
		return pChilds_[caller->nIndex_+1]->next(nQueryLvl_, pQueryRoot_, this);

	// route down to first child condition: parent call and level smaller
	if ((imroot || parentcall) && nLevel_ < nQueryLvl_)
		return pChilds_[0]->next(nQueryLvl_, pQueryRoot_, this);

	return 0;
}

OctreeNode* OctreeNode::search(const GeoCoor &coor, bool outofscope)
{
	OctreeNode *child = getChild(coor);
	if (!child)
	{
		if (outofscope && pParent_)
			return pParent_->search(coor);
		else
			return 0;
	}
	else if (nLevel_ == nMaxLevel_)
	{
		return this;
	}

	return child->search(coor);
}

bool OctreeNode::addTag(const GeoCoor &coor, GeoTag *tag)
{
	OctreeNode *child = getChild(coor);

	if (child)
	{
		mGeoTagList_.push_back(tag);
		if (nLevel_ == nMaxLevel_)
			return true;
		else
			return child->addTag(coor, tag);
	}

	return false;
}

const OctreeNode::GeoTagListType& OctreeNode::getTags() const
{
	return mGeoTagList_;
}
