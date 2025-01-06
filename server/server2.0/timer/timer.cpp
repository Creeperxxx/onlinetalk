#include "timer.h"

timerNode::timerNode(std::chrono::milliseconds interval, std::function<void()> task, bool isrepeat)
{
    m_interval = interval;
    m_task = task;
    m_is_repeat = isrepeat;
    m_expire_time = std::chrono::steady_clock::now() + m_interval;
}

void timerNode::do_work()
{
    m_task();
}

bool timerNode::is_repeat()
{
    return m_is_repeat;
}

std::chrono::time_point<std::chrono::steady_clock> timerNode::get_expire_time() const
{
    return m_expire_time;
}

// void timerList::add_timer(timerNode node)
// void timerList::add_timer(timerNode* node)
void timerList::add_timer(std::shared_ptr<timerNode> node)
{
    std::lock_guard<std::mutex> lock(m_timer_set_mutex);
    m_timer_set->insert(node);
}

// void timerList::del_timer(timerNode node)
// {
//     std::lock_guard<std::mutex> lock(m_timer_set_mutex);
//     m_timer_set.erase(node);
// }

void timerList::run()
{
    int timer_num = 0;
    bool sleep_flag;
    while (m_run.load())
    {
        // 是否为空
        // 取出头，判断
        //  std::lock_guard<std::mutex> lock(m_timer_set_mutex);
        sleep_flag = true;
        std::unique_lock<std::mutex> lock(m_timer_set_mutex);
        while (m_timer_set->begin() != m_timer_set->end())
        {
            sleep_flag = false;
            auto curr_node = *m_timer_set->begin();
            if (curr_node == nullptr)
            {
                m_timer_set->erase(m_timer_set->begin());
                continue;
            }
            if (curr_node->is_expire() == false)
            {
                lock.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(TIMERLIST_SLEEPTIME));
                break;
            }
            auto future = std::async(std::launch::async, &timerNode::do_work, curr_node);
            m_timer_set->erase(m_timer_set->begin());
            if (curr_node->is_repeat() == true)
            {
                curr_node->update_expire_time();
                add_timer(curr_node);
            }
            auto status = future.wait_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(TIMERLIST_FUTURE_MAXEXPIRE));
            if (status == std::future_status::timeout)
            {
                LOG_ERROR("%s:%s:%d // 定时器set中任务的future超时了", __FILE__, __FUNCTION__, __LINE__);
            }
        }
        if (sleep_flag == true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(TIMERLIST_SLEEPTIME));
        }

        // while(set_isempty() == false)
        // {
        //     std::lock_guard<std::mutex> lock(m_timer_set_mutex);
        //     auto curr_node = *m_timer_set->begin();
        //     if(curr_node == nullptr)
        //     {
        //         m_timer_set->erase(m_timer_set->begin());
        //         continue;
        //     }
        //     if(curr_node->is_expire() == false)
        //     {
        //         std::this_thread::sleep_for(std::chrono::milliseconds(TIMERLIST_SLEEPTIME));
        //         continue;
        //     }
        //     // (*curr_node)->run();
        //     auto future = std::async(std::launch::async,&timerNode::run,curr_node.get());
        //     m_timer_set->erase(m_timer_set->begin());
        //     if(curr_node->is_repeat() == true)
        //     {
        //         curr_node->update_expire_time();
        //         add_timer(curr_node);
        //     }
        // }
        // std::this_thread::sleep_for(std::chrono::milliseconds(TIMERLIST_SLEEPTIME));
    }
}

// bool timerList::set_isempty()
// {
//     std::lock_guard<std::mutex> lock(m_timer_set_mutex);
//     return m_timer_set->empty();
// }

bool timerNode::is_expire()
{
    return std::chrono::steady_clock::now() >= m_expire_time;
}

void timerNode::update_expire_time()
{
    m_expire_time = std::chrono::steady_clock::now() + m_interval;
}

// int timerList::get_timer_num()
// {
//     std::lock_guard<std::mutex> lock(m_timer_set_mutex);
//     return m_timer_set->size();
// }

timerList::timerList()
{
    init();
}

timerList &timerList::get_instance()
{
    static timerList instance;
    return instance;
}

void timerList::stop()
{
    m_run.store(false);
}

timerList::~timerList()
{
    stop();
}

void timerList::init()
{
    m_timer_set = std::make_unique<std::multiset<std::shared_ptr<timerNode>, decltype(compare)>>(compare);
    m_run.store(true);
}

void timerList::start()
{
    auto lambda = [this]()
    {
        this->run();
    };
    threadPool::get_instance().commit(lambda);
}