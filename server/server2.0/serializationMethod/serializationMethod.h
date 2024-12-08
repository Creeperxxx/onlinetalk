#pragma once
#include <zlib.h>
#include "../logSystem/log.h"
#include "../message/message.h"

class IserializationMethod
{
public:
    virtual std::shared_ptr<std::vector<uint8_t>> serialize_message(std::shared_ptr<message> msg) = 0;
    virtual std::shared_ptr<message> deserialize_message(std::shared_ptr<std::vector<uint8_t>> data) = 0;
};

class serializationMethodV1 : public IserializationMethod
{
public:
    std::shared_ptr<std::vector<uint8_t>> serialize_message(std::shared_ptr<message> msg) override;
    std::shared_ptr<message> deserialize_message(std::shared_ptr<std::vector<uint8_t>> data) override;
    uint32_t calculateCRC32(const uint8_t *data, size_t length);
};