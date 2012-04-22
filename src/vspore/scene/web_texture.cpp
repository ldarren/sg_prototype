#include "./web_texture.h"

using namespace meta;
using namespace irr;

////////////////////// Web Texture /////////////////////
WebTexture::WebTexture(int w, int h)
:	nWidth_(w), nHeight_(h), pDriver_(0), pTexture_(0), pWebView_(0)
{
}

void WebTexture::open(Awesomium::WebCore *web, irr::video::IVideoDriver *driver)
{
	pWebView_ = web->createWebView(nWidth_, nHeight_);
	pWebView_->setListener(this);

	pDriver_ = driver;
	pTexture_ = pDriver_->getTexture(L"../res/textures/browser_home.jpg");
}

void WebTexture::update()
{
	if (!pWebView_->isDirty())
		return;

	unsigned char *tex = (unsigned char*)pTexture_->lock();

	u32 pitch = pTexture_->getPitch();
	pWebView_->render(tex, pitch, pitch/pTexture_->getSize().Width);

	pTexture_->unlock();
}

void WebTexture::surf(const std::string &url)
{
	pWebView_->loadURL(url);
	//pWebView_->loadFile("demo.html");
}

void WebTexture::close()
{
	pWebView_->destroy();
}

void WebTexture::resize(int w, int h)
{
	nWidth_ = w;
	nHeight_ = h;
	pWebView_->resize(nWidth_, nHeight_);
}

irr::video::ITexture* WebTexture::read()
{
	return pTexture_;
}

int WebTexture::getWidth() const
{
	return nWidth_;
}

int WebTexture::getHeight() const
{
	return nHeight_;
}

Awesomium::WebView* WebTexture::getWebView()
{
	return pWebView_;
}
	
void WebTexture::onBeginNavigation(const std::string& url)
{
}

void WebTexture::onBeginLoading(const std::string& url, int statusCode, const std::wstring& mimeType)
{
}

void WebTexture::onFinishLoading()
{
}

void WebTexture::onCallback(const std::string& name, const Awesomium::JSArguments& args)
{
}

void WebTexture::onReceiveTitle(const std::wstring& title)
{
}

void WebTexture::onChangeTooltip(const std::wstring& tooltip)
{
}

void WebTexture::onChangeCursor(const HCURSOR& cursor)
{
}

void WebTexture::onChangeKeyboardFocus(bool isFocused)
{
}
