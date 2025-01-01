#pragma once
// #include "../networkio/networkio.h"
#include "../socketNetworkIo/socketNetworkIo.h"
#include "../logSystem/log.h"
#include <nlohmann/json.hpp>
#include "../dataBase/dataBase.h"
#include "../socketManager/socketManager.h"
#include "../dataBase/dbConstants.h"

class IStrategy
{
public:
    virtual std::shared_ptr<message> handle(std::shared_ptr<message> msg) = 0;
private:
    std::shared_ptr<oldSocketManager> m_socketManager;
};

class ITextStrategy : public IStrategy
{

};

class IImageStrategy : public IStrategy
{

};

class IFileStrategy : public IStrategy
{

};

class IVoiceStrategy : public IStrategy
{

};

class IVideoStrategy : public IStrategy
{

};

class IControlStrategy : public IStrategy
{

};

class INoticeStrategy : public IStrategy
{

};

class controlUserLoginStrategy : public IControlStrategy
{
    std::shared_ptr<message> handle(std::shared_ptr<message> msg) override;
};