#include "mysqlpool.h"

class ImysqlMethods
{
public:
    virtual bool query(const std::string& sql) = 0;
};

class mysqlMethodsV1: public ImysqlMethods
{

};