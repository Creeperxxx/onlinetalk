#include "RedisPool.h"

class IredisMethods
{
    virtual bool insert(const std::string& key, const std::string& value) = 0;

};
/*
redis方法
1. 插入键值对。要求值可以为5种数据类型
2. 查询键值对。
3. 删除键值对。
4. 更新键值对。
5. 
*/
class redisMethodsV1 : public IredisMethods
{
    bool insert(const std::string &key, const std::string &value) override;
};