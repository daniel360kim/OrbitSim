/**
 * @file Timer.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief timer class for profiling
 * @version 0.1
 * @date 2023-07-08
 * 
 * 
 */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <vector>
#include <string>
#include <iostream>

class Timer
{
public:
    Timer(std::string name);
    ~Timer();

    std::string getName() const { return m_Name; }

    void Stop();
    long long GetDuration() { return m_duration; }

    void printResults();
    
private:
    std::string m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped = false;
    long long m_duration;
};

class TimerManager
{
public:
    TimerManager();
    ~TimerManager();

    void AddTimer(Timer timer);
    void PrintResults();


private:
    std::vector<Timer> m_Timers;

    
};


#endif