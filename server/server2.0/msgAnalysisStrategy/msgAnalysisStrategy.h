#pragma once
#include "../networkio/networkio.h"

class IStrategy
{
public:
    virtual std::shared_ptr<DataPacket> handle() = 0;
};

class IUserStrategy:public IStrategy
{

};

class userLoginStrategy:public IUserStrategy
{

};







class IFriendStrategy:public IStrategy
{

};

class IGroupStrategy:public IStrategy
{

};

class INotifyStrategy:public IStrategy
{

};

class IFileTransportStrategy:public IStrategy
{

};

