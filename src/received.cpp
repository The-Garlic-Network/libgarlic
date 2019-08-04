
#include "include/received.hpp"

void _received::operator<<(tgnmsg message)
{
	this->mute.lock();
	this->list.push_back(message);
	this->mute.unlock();
}