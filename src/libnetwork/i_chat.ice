#ifndef _CHAT_INTERFACE_ICE_
#define _CHAT_INTERFACE_ICE_

module meta
{

interface IChat
{
	["cpp:const"] void say(string msg);
};

};

#endif // _CHAT_INTERFACE_ICE_
