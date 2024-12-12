#include <unordered_map>
#include <string>
#include <any>
#include <functional>
#include <iostream>
using namespace std;

// class test
// {
//     std::unordered_map<std::string , std::any> data;
// public:
//     void  add_data(std::string key, std::any value)
//     {
//         data[key] = value;
//     }
    
//     std::any get_data(std::string key)
//     {
//         return data[key];
//     }
// };

// class sum
// {
// public:
//     int sumnum(int a ,int b)
//     {
//         return a+b;
//     }
// };


// #include <vector>
// #include <memory>
// int main()
// {
//     // sum s;
//     // test t;
//     // std::function<int(int,int)> test = bind(&sum::sumnum, &s, std::placeholders::_1, std::placeholders::_2);
//     // std::any testany = test;
//     // t.add_data("sum", testany);
//     // std::function<int(int,int)> test2 = std::any_cast<std::function<int(int,int)>>(t.get_data("sum"));
//     // std::cout << test2(1,2) << std::endl;
//     std::shared_ptr<vector<int>> a = std::make_shared<vector<int>>();
//     a->push_back(1);
//     a->push_back(2);
//     for( auto i : *a)
//     {
//         std::cout << "a: "<<i << std::endl;
//     }
//     auto b = std::make_shared<vector<int>>(std::move(*a));
//     for( auto i : *b)
//     {
//         std::cout << "b: "<<i << std::endl;
//     }
//     for( auto i : *a)
//     {
//         std::cout << "a: "<<i << std::endl;
//     }
//     a->push_back(3);
//     for( auto i : *a)
//     {
//         std::cout << "a: "<<i << std::endl;
//     }
//     return 0;
// }

#include <memory>
#include <atomic>
#include <time.h>
class test
{
private: 
    atomic<time_t> m_time;
public:
    test(): m_time(time(NULL)){};
    void set_time(time_t t)
    {
        m_time.store(t);
    }
    time_t get_time() const
    {
        return m_time.load();
    }
};
struct compare
{
    bool operator()(const std::shared_ptr<test>& a, const std::shared_ptr<test>& b) const
    {
        return a->get_time() < b->get_time();
    }
};

// struct compare
// {
//     bool operator()(const test* a, const test* b) const
//     {
//         return a->get_time() > b->get_time();
//     }
// };

#include <queue>
// int main()
// {
//     test a;
//     test b;
//     a.set_time(1);
//     b.set_time(2);
//     compare c;
//     priority_queue<test*, vector<test*>, compare> pq;
//     pq.push(&a);
//     pq.push(&b);
//     cout << pq.top()->get_time() << endl;
//     return 0;
// }

#include <unordered_map>
#include <set>
int main()
{
    unordered_map<int, std::shared_ptr<test>> a;
    a[1] = make_shared<test>(); 
    multiset<std::shared_ptr<test>, compare> b;
    b.insert(a[1]);
    auto it = b.find(a[1]);
    if(it != b.end())
    {
        cout << "find" << endl;
        cout<<(*it)->get_time()<<endl;
    }
    else
    {
        cout << "not find" << endl;
    }
    return 0;
}