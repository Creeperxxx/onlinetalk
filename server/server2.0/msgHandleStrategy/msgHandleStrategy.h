#pragma once
#include "../networkio/networkio.h"

class IStrategy
{
public:
    virtual std::shared_ptr<message> handle(std::shared_ptr<message> msg) = 0;
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

class user_send_user : public ITextStrategy
{
public:
    std::shared_ptr<message> handle(std::shared_ptr<message> msg) override;
};