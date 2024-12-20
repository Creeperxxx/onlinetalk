#pragma once

#include "../msgHandle/msgHandle.h"

class msgAnalysis
{
public:
    msgAnalysis(){}
    ~msgAnalysis(){}
    static void analysis(const std::shared_ptr<msgHandle>& msg);
    
}