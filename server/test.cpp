#include <unordered_map>
#include <string>
#include <any>
#include <functional>
#include <iostream>
using namespace std;

class test
{
    std::unordered_map<std::string , std::any> data;
public:
    void  add_data(std::string key, std::any value)
    {
        data[key] = value;
    }
    
    std::any get_data(std::string key)
    {
        return data[key];
    }
};

class sum
{
public:
    int sumnum(int a ,int b)
    {
        return a+b;
    }
};


#include <vector>
#include <memory>
int main()
{
    // sum s;
    // test t;
    // std::function<int(int,int)> test = bind(&sum::sumnum, &s, std::placeholders::_1, std::placeholders::_2);
    // std::any testany = test;
    // t.add_data("sum", testany);
    // std::function<int(int,int)> test2 = std::any_cast<std::function<int(int,int)>>(t.get_data("sum"));
    // std::cout << test2(1,2) << std::endl;
    std::shared_ptr<vector<int>> a = std::make_shared<vector<int>>();
    a->push_back(1);
    a->push_back(2);
    for( auto i : *a)
    {
        std::cout << "a: "<<i << std::endl;
    }
    auto b = std::make_shared<vector<int>>(std::move(*a));
    for( auto i : *b)
    {
        std::cout << "b: "<<i << std::endl;
    }
    for( auto i : *a)
    {
        std::cout << "a: "<<i << std::endl;
    }
    a->push_back(3);
    for( auto i : *a)
    {
        std::cout << "a: "<<i << std::endl;
    }
    return 0;
}