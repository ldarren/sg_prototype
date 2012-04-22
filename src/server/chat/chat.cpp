#include "./chat.h"

using namespace meta;
using namespace std;

void Chat::say(const std::string &msg, const Ice::Current&) const
{
	cout<<msg<<endl;
}
