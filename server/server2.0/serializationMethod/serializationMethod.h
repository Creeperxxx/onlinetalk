#pragma once
#include <zlib.h>
#include "../logSystem/log.h"
#include "../message/message.h"
// #include "../requirement/cereal/types/string.hpp"
// #include "../requirement/cereal/types/variant.hpp"
// #include "../requirement/cereal/types/optional.hpp"
// #include "../requirement/cereal/types/vector.hpp"
// #include "../requirement/cereal/archives/binary.hpp"
#include <cereal/types/string.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/binary.hpp>

enum class SerializationMethodType
{
    CEREAL
};

class ISerializationMethod
{
public:
    virtual std::shared_ptr<std::vector<uint8_t>> serialize_message(std::shared_ptr<message> msg) = 0;
    virtual std::shared_ptr<message> deserialize_message(std::shared_ptr<std::vector<uint8_t>> data) = 0;
};

class cerealSerializationMethod : public ISerializationMethod
{
public:
    std::shared_ptr<std::vector<uint8_t>> serialize_message(std::shared_ptr<message> msg) override;
    std::shared_ptr<message> deserialize_message(std::shared_ptr<std::vector<uint8_t>> data) override;
    static uint32_t calculateCRC32(const uint8_t *data, size_t length);
};

class SerializationMethodFactory
{
public:
    static std::shared_ptr<ISerializationMethod> createSerializationMethod(const SerializationMethodType& type);
};