#include "./game.h"

#include <stdlib.h>
#include <time.h>

using namespace meta;
using namespace irr;
using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;

////////////////////// Keyboard hook /////////////////////
#ifdef WIN32
GameScene *gpGameScene=0;
LRESULT CALLBACK GetHookMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION)
	{
		MSG *msg = (MSG*)lParam;
		if(wParam & PM_REMOVE)
			gpGameScene->handleHook(msg->message, msg->hwnd, msg->wParam, msg->lParam);
	}

	return CallNextHookEx(gpGameScene->hKeyboardHook, nCode, wParam, lParam);
}

void GameScene::handleHook(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if(!bIsWebSelected_)
		return;

	switch(msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSDEADCHAR:
	case WM_SYSCHAR:
	case WM_IME_CHAR:
	case WM_IME_COMPOSITION:
	case WM_IME_COMPOSITIONFULL:
	case WM_IME_CONTROL:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_KEYDOWN:
	case WM_IME_KEYUP:
	case WM_IME_NOTIFY:
	//case WM_IME_REQUEST:
	case WM_IME_SELECT:
	case WM_IME_SETCONTEXT:
	case WM_IME_STARTCOMPOSITION:
	case WM_HELP:
	case WM_CANCELMODE: 
		{
			mWebTex_.getWebView()->injectKeyboardEvent(hwnd, msg, wParam, lParam);
			break;
		}
	}
}

#endif

////////////////////// Game Scene /////////////////////

GameScene::GameScene()
:	pScene_(0), pWebBrowser_(0), mWebTex_(1024, 1024), pWebBoxNode_(0),
	bIsWebSelected_(false), hKeyboardHook(0), pFPSCam_(0), pRootNode_(0), pLocalNode_(0),
	nSelectedPt_(0), nSelectedHouse_(0), pPointTex_(0), pSelPointTex_(0)
{
	gpGameScene = this;

	// Geotag octree
	GeoBBox box;
	box.mEdges[GeoBBox::EEDGE_AltMax] = 150.0f;
	box.mEdges[GeoBBox::EEDGE_AltMin] = 0.0f;
	box.mEdges[GeoBBox::EEDGE_LatMax] = 250.0f;
	box.mEdges[GeoBBox::EEDGE_LatMin] = -250.0f;
	box.mEdges[GeoBBox::EEDGE_LongMax] = 250.0f;
	box.mEdges[GeoBBox::EEDGE_LongMin] = -250.0f;
	pRootNode_ = new OctreeNode(0, 0, 0, 4, box);
	pLocalNode_ = pRootNode_->getChild(0);
}

GameScene::~GameScene()
{
	delete pRootNode_;
}

void GameScene::create(irr::IrrlichtDevice *device, NetClient *net, WorldInfo *info, const char *rcsPath)
{
	Parent::create(device, net, info, rcsPath);

	pScene_ = pDevice_->getSceneManager();

	pWebBrowser_ = new Awesomium::WebCore(Awesomium::LOG_VERBOSE);
	pWebBrowser_->setBaseDirectory("D:\\project\\vspore\\bin\\res\\web");
	pWebBrowser_->setCustomResponsePage(404, "404response.html");

	mWebTex_.open(pWebBrowser_, pDevice_->getVideoDriver());

	pPointTex_ = pScene_->getVideoDriver()->getTexture("../res/textures/point.png");
	pSelPointTex_ = pScene_->getVideoDriver()->getTexture("../res/textures/Particle.tga");
}

void GameScene::destroy()
{
	mWebTex_.close();

	delete pWebBrowser_;
	pWebBrowser_ = 0;
}

void GameScene::onWake()
{
	Parent::onWake();

	pScene_->loadScene(pPath_);

	// Hook the keyboard, because of mismatch in chromium and irrlicht keyboard events
#ifdef WIN32
	hKeyboardHook = SetWindowsHookEx(WH_GETMESSAGE, GetHookMsgProc, GetModuleHandle(0), GetCurrentThreadId());
#endif

	pWebBoxNode_ = pScene_->addCubeSceneNode(30, 0, EPICKID_Pickable, vector3df(100, 15, 0), vector3df(0, 30, 0));
	pWebBoxNode_->setMaterialTexture(0, mWebTex_.read());
	pWebBoxNode_->setMaterialFlag(irr::video::EMF_LIGHTING, false);

	// make web box selectable
	irr::scene::ITriangleSelector *sel = pScene_->createTriangleSelector(pWebBoxNode_->getMesh(), pWebBoxNode_);
	pWebBoxNode_->setTriangleSelector(sel);
	sel->drop();

	mWebTex_.surf("http://www.google.com.sg");

	// test case: point seaching
	srand( time(0) );
	GeoCoor pos;
	GeoTag *tag;
	const GeoBBox &box = pRootNode_->getBoundingBox();
	for (int i=0; i<1000; ++i)
	{
		pos.fCoor[GeoCoor::EGEO_Latitude] = box.mEdges[GeoBBox::EEDGE_LatMin]+
			(box.mEdges[GeoBBox::EEDGE_LatMax]-box.mEdges[GeoBBox::EEDGE_LatMin])*(0.01f*(rand()%100+1));
		pos.fCoor[GeoCoor::EGEO_Altitude] = box.mEdges[GeoBBox::EEDGE_AltMin]+
			(box.mEdges[GeoBBox::EEDGE_AltMax]-box.mEdges[GeoBBox::EEDGE_AltMin])*(0.01f*(rand()%100+1));
		pos.fCoor[GeoCoor::EGEO_Longtitude] = box.mEdges[GeoBBox::EEDGE_LongMin]+
			(box.mEdges[GeoBBox::EEDGE_LongMax]-box.mEdges[GeoBBox::EEDGE_LongMin])*(0.01f*(rand()%100+1));
		pPointBill_[i] = pScene_->addBillboardSceneNode();
		pPointBill_[i]->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
		pPointBill_[i]->setMaterialTexture(0, pPointTex_);
		pPointBill_[i]->setMaterialFlag(video::EMF_LIGHTING, false);
		pPointBill_[i]->setMaterialFlag(video::EMF_ZBUFFER, false);
		pPointBill_[i]->setSize(core::dimension2d<f32>(20.0f, 20.0f));
		pPointBill_[i]->setID(EPICKID_NotPickable); // This ensures that we don't accidentally ray-pick it
		pPointBill_[i]->setPosition(core::vector3df(
			pos.fCoor[GeoCoor::EGEO_Latitude],
			pos.fCoor[GeoCoor::EGEO_Altitude],
			pos.fCoor[GeoCoor::EGEO_Longtitude]));
		tag = new GeoTag;
		tag->pData = pPointBill_[i];
		pRootNode_->addTag(pos, tag);
	}

	pHouses_[0] = pScene_->getSceneNodeFromName("house_01");
	pHouses_[1] = pScene_->getSceneNodeFromName("house_02");
	pHouses_[2] = pScene_->getSceneNodeFromName("house_03");
	pHouses_[3] = pScene_->getSceneNodeFromName("house_04");

	searchNextPoint();
	selectNextHouse();
}

void GameScene::onSleep()
{
	mSelPtList_.clear();

	pDevice_->getCursorControl()->setVisible(true);
	pScene_->clear();
	
#ifdef WIN32
	UnhookWindowsHookEx(hKeyboardHook);
	hKeyboardHook = 0;
#endif

	Parent::onSleep();
}

bool GameScene::update(F32 elapsed)
{
	pWebBrowser_->update();
	mWebTex_.update();

	if (pInfo_)
	{
		if (pInfo_->pPlayer)
			pInfo_->pPlayer->update(elapsed);

		if (!pInfo_->mNetPlayerList.empty())
		{
			WorldInfo::NetPlayerListType::iterator iter;
			for (iter = pInfo_->mNetPlayerList.begin(); iter != pInfo_->mNetPlayerList.end(); ++iter)
			{
				(*iter)->update(elapsed);
			}
		}
	}

	pScene_->drawAll();

	// draw lines with fresh material and transformation setting
	video::IVideoDriver *driver = pScene_->getVideoDriver();
	video::SMaterial m; 
    m.Lighting = false; 
    driver->setMaterial(m); 	
	driver->setTransform(video::ETS_WORLD, core::matrix4());

	// test iteration
	OctreeNode *node = pRootNode_->begin(1);
	irr::core::aabbox3df	irrbbox;
	while (node)
	{
		const GeoBBox &box = node->getBoundingBox();
		GeoBBox2aabbox(box, irrbbox);
		driver->draw3DBox(irrbbox, video::SColor(255,255,255,255));
		node = node->next();
	}

	// test search
	driver->draw3DBox(mPtSelBox_, video::SColor(255,255,0,0));

	// test box seaching
	node = pRootNode_->begin(mQueryBBox_);
	if (node)
	{
		GeoBBox2aabbox(mQueryBBox_, mQueryBox_);
		GeoBBox2aabbox(node->getBoundingBox(), mResultBox_);
		driver->draw3DBox(mQueryBox_, video::SColor(255,0,255,0));
		driver->draw3DBox(mResultBox_, video::SColor(255,0,0,255));
		while (node)
		{
			const GeoBBox &box = node->getBoundingBox();
			GeoBBox2aabbox(box, irrbbox);
			driver->draw3DBox(irrbbox, video::SColor(255,128,128,255));
			node = node->next(mQueryBBox_);
		}
	}

	return true;
}

bool GameScene::onEvent(const irr::SEvent &evt)
{
	bool playercam = pFPSCam_ != pScene_->getActiveCamera();
	if (!bIsWebSelected_ && playercam && pInfo_ && pInfo_->pPlayer &&  pInfo_->pPlayer->onEvent(evt))
		return true;

	switch (evt.EventType)
	{
	case EET_GUI_EVENT:
		break;
	case EET_MOUSE_INPUT_EVENT:
		if (!playercam)
			break;
		switch(evt.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			if (bIsWebSelected_) mWebTex_.getWebView()->injectMouseDown(Awesomium::LEFT_MOUSE_BTN);
			break;
		case EMIE_RMOUSE_PRESSED_DOWN:
			if (bIsWebSelected_) mWebTex_.getWebView()->injectMouseDown(Awesomium::RIGHT_MOUSE_BTN);
			break;
		case EMIE_MMOUSE_PRESSED_DOWN:
			if (bIsWebSelected_) mWebTex_.getWebView()->injectMouseDown(Awesomium::MIDDLE_MOUSE_BTN);
			break;
		case EMIE_LMOUSE_LEFT_UP:
			if (bIsWebSelected_) mWebTex_.getWebView()->injectMouseUp(Awesomium::LEFT_MOUSE_BTN);
			break;
		case EMIE_RMOUSE_LEFT_UP:
			if (bIsWebSelected_) mWebTex_.getWebView()->injectMouseUp(Awesomium::RIGHT_MOUSE_BTN);
			break;
		case EMIE_MMOUSE_LEFT_UP:
			if (bIsWebSelected_) mWebTex_.getWebView()->injectMouseUp(Awesomium::MIDDLE_MOUSE_BTN);
			break;
		case EMIE_MOUSE_MOVED:
			{
				vector2di actPos(evt.MouseInput.X, evt.MouseInput.Y);
				if (testWebHit(actPos))
				{
					pInfo_->pPlayer->stop();
					mWebTex_.getWebView()->injectMouseMove(actPos.X, actPos.Y);
				}
			}
			break;
		case EMIE_MOUSE_WHEEL:
			if (bIsWebSelected_) mWebTex_.getWebView()->injectMouseWheel(evt.MouseInput.Wheel);
			break;
		}
		break;
	case EET_KEY_INPUT_EVENT:
		// webtex keyboard events r handled in keyboard hook (win32 specific)
		// this portion is for game scene only
		if (evt.KeyInput.PressedDown)
		{
			switch(evt.KeyInput.Key)
			{
			case KEY_F5:
				if (playercam)
				{
					// for inspection, default cam will be LocalPlayer's 3rd cam
					pFPSCam_ = 	pScene_->addCameraSceneNodeFPS();
					pDevice_->getCursorControl()->setVisible(false);
				}
				else
				{
					if (pInfo_ && pInfo_->pPlayer)
					{
						pScene_->setActiveCamera(pInfo_->pPlayer->getCam());
						pDevice_->getCursorControl()->setVisible(true);
					}
				}
				break;
			case KEY_TAB:
				selectNextHouse();
				break;
			case KEY_CONTROL:
				searchNextPoint();
				break;
			}
		}
		break;
	case EET_JOYSTICK_INPUT_EVENT:
		break;
	case EET_LOG_TEXT_EVENT:
		break;
	case EET_USER_EVENT:
		switch(evt.UserEvent.UserData1)
		{
		case EACTID_Surf:
			LocalPlayer *p = reinterpret_cast<LocalPlayer*>(evt.UserEvent.UserData2);
			mWebTex_.surf(p->getURL());
			return true;
		}
		break;
	}

	return false;
}

bool almostEqual(float a, float b)
{
	int na = (int)(a*1000);
	int nb = (int)(b*1000);
	if (abs(na - nb) > 10) return false;
	return true;
}

bool almostEqualV(const vector3df &a, const vector3df &b)
{
	int na = (int)(a.X*1000);
	int nb = (int)(b.X*1000);
	if (abs(na - nb) > 10) return false;
	na = (int)(a.Y*1000);
	nb = (int)(b.Y*1000);
	if (abs(na - nb) > 10) return false;
	na = (int)(a.Z*1000);
	nb = (int)(b.Z*1000);
	if (abs(na - nb) > 10) return false;
	return true;
}

bool GameScene::testWebHit(vector2di &pos)
{
	bIsWebSelected_ = false;
	ISceneCollisionManager* const coll = pScene_->getSceneCollisionManager();

	// mouse ray
	core::line3df ray = coll->getRayFromScreenCoordinates(pos, pInfo_->pPlayer->getCam());
	// intersection point
	core::vector3df intersectPt; 
	// Used to show with triangle has been hit
	core::triangle3df hitTri;

	IMeshSceneNode* selNode = (IMeshSceneNode*)coll->getSceneNodeAndCollisionPointFromRay(
							ray,
							intersectPt, // This will be the position of the collision
							hitTri, // This will be the triangle hit in the collision
							EPICKID_Pickable, // This ensures that only nodes that we have set up to be pickable are considered
							0 // Check the entire scene (this is actually the implicit default)
							);

	if(selNode)
	{
		vector2df tcoorA, tcoorB, tcoorC;

		// transform hit triangle and intersection point to object space
		matrix4 rot = selNode->getRelativeTransformation();
		hitTri.pointA -= selNode->getPosition();
		hitTri.pointB -= selNode->getPosition();
		hitTri.pointC -= selNode->getPosition();
		intersectPt -= selNode->getPosition();
		rot.inverseRotateVect(hitTri.pointA);
		rot.inverseRotateVect(hitTri.pointB);
		rot.inverseRotateVect(hitTri.pointC);
		rot.inverseRotateVect(intersectPt);

		// get texture coordinate of hit trainagle
		IMeshBuffer *mbuff = selNode->getMesh()->getMeshBuffer(0);
		const S3DVertex *vertlist = (S3DVertex*)mbuff->getVertices();
		u32 vcount = mbuff->getVertexCount();
		int fcount = 0x111;
		for (u32 i=0; i<vcount; ++i)
		{
			if (fcount & 0x001 && almostEqualV(vertlist[i].Pos, hitTri.pointA))
			{
				tcoorA = mbuff->getTCoords(i);
				fcount = fcount & 0x110;
			}
			else if (fcount & 0x010 && almostEqualV(vertlist[i].Pos, hitTri.pointB))
			{
				tcoorB = mbuff->getTCoords(i);
				fcount = fcount & 0x101;
			}
			else if (fcount & 0x100 && almostEqualV(vertlist[i].Pos, hitTri.pointC))
			{
				tcoorC = mbuff->getTCoords(i);
				fcount = fcount & 0x011;
			}
			// get texture coordinate of intersection point
			if (fcount == 0x000)
			{
				core::quaternion rotM;

				vector3df normal = hitTri.getNormal();
				if (almostEqual(normal.X, 0) && almostEqual(normal.Y, 0)) // irrlicht's quarternion failed to find rotation axis
					rotM.fromAngleAxis(DEGTORAD*180, vector3df(0,1,0));
				else
					rotM.rotationFromTo(normal, vector3df(0,0,1));

				hitTri.pointA = rotM * hitTri.pointA;
				hitTri.pointB = rotM * hitTri.pointB;
				hitTri.pointC = rotM * hitTri.pointC;
				intersectPt = rotM * intersectPt;

				// **Note opengl's texture coordinate, 0,0 is at bottom left
				// TODO: this algo doesn;t work on complex mesh
				if ((tcoorA.X != tcoorB.X) && (hitTri.pointA.Y != hitTri.pointB.Y))
					pos.Y = (int)mWebTex_.getHeight() * 
						(tcoorA.X - (tcoorA.X-tcoorB.X)/(hitTri.pointA.Y - hitTri.pointB.Y) * (hitTri.pointA.Y - intersectPt.Y));
				else
					pos.Y = (int)mWebTex_.getHeight() * 
						(tcoorA.X - (tcoorA.X-tcoorC.X)/(hitTri.pointA.Y - hitTri.pointC.Y) * (hitTri.pointA.Y - intersectPt.Y));
				if ((tcoorA.Y != tcoorB.Y) && (hitTri.pointA.X != hitTri.pointB.X))
					pos.X = (int) mWebTex_.getWidth() - mWebTex_.getWidth() * 
						(tcoorA.Y - (tcoorA.Y-tcoorB.Y)/(hitTri.pointA.X - hitTri.pointB.X) * (hitTri.pointA.X - intersectPt.X));
				else
					pos.X = (int) mWebTex_.getWidth() - mWebTex_.getWidth() * 
						(tcoorA.Y - (tcoorA.Y-tcoorC.Y)/(hitTri.pointA.X - hitTri.pointC.X) * (hitTri.pointA.X - intersectPt.X));

				// check texture orientation
				vector3df euler;
				rotM.rotationFromTo(vector3df(tcoorA.X, tcoorA.Y, 0) - vector3df(tcoorB.X, tcoorB.Y, 0), vector3df(tcoorB.X, tcoorB.Y, 0) - vector3df(tcoorC.X, tcoorC.Y, 0));
				rotM.toEuler(euler);
				if (euler.Z > 0)
					pos.Y = mWebTex_.getHeight() - pos.Y;
				else
					pos.X = mWebTex_.getWidth() - pos.X;
				
				bIsWebSelected_ = true;
				break;
			}
		}
	}
	return bIsWebSelected_;
}

void GameScene::searchNextPoint()
{
	pPointBill_[nSelectedPt_]->setMaterialTexture(0, pPointTex_);

	++nSelectedPt_;
	if (nSelectedPt_ >= PT_COUNT)
		nSelectedPt_ = 0;

	pPointBill_[nSelectedPt_]->setMaterialTexture(0, pSelPointTex_);

	const vector3df &pt = pPointBill_[nSelectedPt_]->getPosition();
	GeoCoor coor = {{pt.X, pt.Y, pt.Z}};

	OctreeNode *node = pRootNode_->search(coor);
	if (node)
	{
		const GeoBBox &box = node->getBoundingBox();
		GeoBBox2aabbox(box, mPtSelBox_);
	}
}

void GameScene::selectNextHouse()
{
	pHouses_[nSelectedHouse_]->setDebugDataVisible(EDS_OFF);

	SelBillList::iterator iter;
	for (iter = mSelPtList_.begin(); iter != mSelPtList_.end(); ++iter)
		(*iter)->setMaterialTexture(0, pPointTex_);
	mSelPtList_.clear();

	++nSelectedHouse_;
	if (nSelectedHouse_ >= 4)
		nSelectedHouse_ = 0;

	pHouses_[nSelectedHouse_]->setDebugDataVisible(EDS_HALF_TRANSPARENCY);

	const aabbox3df &box = pHouses_[nSelectedHouse_]->getBoundingBox();
	const vector3df &rot = pHouses_[nSelectedHouse_]->getRotation();
	const vector3df &pos = pHouses_[nSelectedHouse_]->getPosition();

	vector3df max = box.MaxEdge*0.1f + pos;
	vector3df min = box.MinEdge*0.1f + pos;
	
	GeoBBox sbox = {{max.X, min.X, max.Y, min.Y, max.Z, min.Z}};
	memcpy(&mQueryBBox_, &sbox, sizeof(GeoBBox));

	OctreeNode *node = pRootNode_->begin(mQueryBBox_);
	GeoTagListType::const_iterator tl;
	while (node)
	{
		const GeoTagListType &tags = node->getTags();
		for (tl = tags.begin(); tl != tags.end(); ++tl)
		{
			IBillboardSceneNode *bn = reinterpret_cast<IBillboardSceneNode*>((*tl)->pData);
			mSelPtList_.push_back(bn);
			bn->setMaterialTexture(0, pSelPointTex_);
		}
		node = node->next(mQueryBBox_);
	}
}

void GameScene::GeoBBox2aabbox(const GeoBBox &gbox, irr::core::aabbox3df &irrbox)
{
	irrbox.MaxEdge = core::vector3df(gbox.mEdges[GeoBBox::EEDGE_LatMax],gbox.mEdges[GeoBBox::EEDGE_AltMax],gbox.mEdges[GeoBBox::EEDGE_LongMax] );
	irrbox.MinEdge = core::vector3df(gbox.mEdges[GeoBBox::EEDGE_LatMin],gbox.mEdges[GeoBBox::EEDGE_AltMin],gbox.mEdges[GeoBBox::EEDGE_LongMin] );
}
