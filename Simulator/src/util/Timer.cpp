/**
 * @file Timer.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-08
 * 
 * 
 */

#include "Timer.h"

Timer::Timer(std::string name)
    : m_Name(name), m_Stopped(false)
{
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    if (!m_Stopped)
        Stop();
}

void Timer::Stop()
{
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    m_duration = end - start;

    m_Stopped = true;
}

void Timer::printResults()
{
    std::cout << m_Name << " took " << m_duration << " microseconds" << std::endl;
}


TimerManager::TimerManager()
{

}

TimerManager::~TimerManager()
{

}

void TimerManager::AddTimer(Timer timer)
{
    m_Timers.push_back(timer);
}

void TimerManager::PrintResults()
{
    for (auto& timer : m_Timers)
        timer.Stop();

    for (auto& timer : m_Timers)
    {
        std::cout << timer.getName() << " took " << timer.GetDuration() << " microseconds" << std::endl;
    }
}

