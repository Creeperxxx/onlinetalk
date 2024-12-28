#include <set>
#include <functional>
#include <cstdint>
#include <chrono>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <future>
#include "../logSystem/log.h"

inline const int TIMERLIST_SLEEPTIME = 1000 * 1; //定时器列表默认sleep的时间
inline const int TIMERLIST_FUTURE_MAXEXPIRE = 1000 * 60;  //定时器的回调函数的future的等待时间

class timerNode
{
public:
    timerNode(std::chrono::milliseconds interval, std::function<void()> task, bool is_repeat = false);
    void do_work();
    bool is_repeat();
    bool is_expire();
    void update_expire_time();
    std::chrono::time_point<std::chrono::steady_clock> get_expire_time() const ;
private:
    // time_t m_expireTime;
    // time_t m_interval;
    std::chrono::time_point<std::chrono::steady_clock> m_expire_time;
    std::chrono::milliseconds m_interval;
    std::function<void()> m_task;
    bool m_is_repeat;
};

class timerList
{
public:
    // void add_timer(timerNode* node);
    void add_timer(std::shared_ptr<timerNode> node);
    static timerList& get_instance();
    void run(); 
    timerList(const timerList&) = delete;
    timerList& operator=(const timerList&) = delete;
    void stop();
private:
    timerList();
    ~timerList();

    // void del_timer(timerNode node);
    // bool set_isempty();
    // int get_timer_num();
    bool compare = [](const std::shared_ptr<timerNode> a, const std::shared_ptr<timerNode> b) {
        return a->get_expire_time()<b->get_expire_time();
    };

private:
    // std::multiset<timerNode,decltype(compare)> m_timer_set;
    // std::unique_ptr<std::multiset<timerNode*,decltype(compare)>> m_timer_set;
    std::unique_ptr<std::multiset<std::shared_ptr<timerNode>,decltype(compare)>> m_timer_set;
    std::mutex m_timer_set_mutex;
    std::atomic<bool> m_run;
};

