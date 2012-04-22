#pragma once
#ifndef _SERVER_CHAT_H_
#define _SERVER_CHAT_H_

#include <libnetwork/i_chat.h>
#include <string>

namespace meta {

class Chat : public IChat
{
public:
	virtual void say(const std::string &msg, const Ice::Current&) const;
};

} // namespace meta

#endif // _SERVER_CHAT_H_