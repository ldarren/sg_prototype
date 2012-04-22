#pragma once
#ifndef _VSPORE_SCENE_WEB_TEXTURE_H_
#define _VSPORE_SCENE_WEB_TEXTURE_H_

#include <awesomium/WebCore.h>
#include <irrlicht.h>

namespace meta {

class WebTexture : public Awesomium::WebViewListener
{
	int							nWidth_;
	int							nHeight_;
	Awesomium::WebView			*pWebView_;
	irr::video::IVideoDriver	*pDriver_;
	irr::video::ITexture		*pTexture_;

public:
	WebTexture(int w, int h);

	void open(Awesomium::WebCore *web, irr::video::IVideoDriver *driver);
	void update();
	void surf(const std::string &url);
	void close();

	void resize(int w, int h);
	irr::video::ITexture* read();

	int getWidth() const;
	int getHeight() const;
	Awesomium::WebView* getWebView();

	virtual void onBeginNavigation(const std::string& url);
	virtual void onBeginLoading(const std::string& url, int statusCode, const std::wstring& mimeType);
	virtual void onFinishLoading();
	virtual void onCallback(const std::string& name, const Awesomium::JSArguments& args);
	virtual void onReceiveTitle(const std::wstring& title);
	virtual void onChangeTooltip(const std::wstring& tooltip);
	virtual void onChangeCursor(const HCURSOR& cursor);
	virtual void onChangeKeyboardFocus(bool isFocused);
};

} // namespace meta

#endif // _VSPORE_SCENE_WEB_TEXTURE_H_