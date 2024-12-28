#include <vector>
#include <string>

class dataManager
{
public:
    void pushData();
    std::string popData();
private:
    std::vector<std::string> data;
};