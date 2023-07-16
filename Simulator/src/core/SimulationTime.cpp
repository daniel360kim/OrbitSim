/**
 * @file SimulationTime.cpp
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 *
 */

#include "SimulationTime.h"

#include <fstream>

SimulationTime::~SimulationTime()
{
    logTime();
}

void SimulationTime::loadTimeFromFile()
{
    std::ifstream timeFile("../../out/logs/time.log");
    timeFile >> m_currentTime;
    timeFile.close();
}

std::chrono::system_clock::time_point SimulationTime::getEpoch() const
{
    std::tm timeinfo = {};
    timeinfo.tm_year = 100; // year: 2000
    timeinfo.tm_mon = 0;    // month: january
    timeinfo.tm_mday = 1;   // day: 1st
    return std::chrono::system_clock::from_time_t(std::mktime(&timeinfo));
}
int SimulationTime::getYears() const
{
    auto epoch = getEpoch();
    std::chrono::milliseconds duration(m_currentTime);
    std::chrono::system_clock::time_point timePoint = epoch + duration;

    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm *timeinfoResult = std::localtime(&time);

    return timeinfoResult->tm_year - 100 + 2000; // Adjust for the year 2000 offset, add 2000 to get the actual year
}

int SimulationTime::getMonths() const
{
    auto epoch = getEpoch();
    std::chrono::milliseconds duration(m_currentTime);
    std::chrono::system_clock::time_point timePoint = epoch + duration;

    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm *timeinfoResult = std::localtime(&time);

    return timeinfoResult->tm_mon + 1;
}

int SimulationTime::getDays() const
{
    auto epoch = getEpoch();
    std::chrono::milliseconds duration(m_currentTime);
    std::chrono::system_clock::time_point timePoint = epoch + duration;

    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm *timeinfoResult = std::localtime(&time);

    return timeinfoResult->tm_mday; 
}

int SimulationTime::getHours() const
{
    auto epoch = getEpoch();
    std::chrono::milliseconds duration(m_currentTime);
    std::chrono::system_clock::time_point timePoint = epoch + duration;

    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm *timeinfoResult = std::localtime(&time);

    return timeinfoResult->tm_hour;
}

int SimulationTime::getMinutes() const
{
    auto epoch = getEpoch();
    std::chrono::milliseconds duration(m_currentTime);
    std::chrono::system_clock::time_point timePoint = epoch + duration;

    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm *timeinfoResult = std::localtime(&time);

    return timeinfoResult->tm_min;
}

int SimulationTime::getSeconds() const
{
    auto epoch = getEpoch();
    std::chrono::milliseconds duration(m_currentTime);
    std::chrono::system_clock::time_point timePoint = epoch + duration;

    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm *timeinfoResult = std::localtime(&time);

    return timeinfoResult->tm_sec;
}

int SimulationTime::getMilliseconds() const
{
    return m_currentTime % 1000;
}

std::string SimulationTime::getFormattedTime()
{

    int years = getYears();
    int months = getMonths();
    int days = getDays();
    int hours = getHours();
    int minutes = getMinutes();
    int seconds = getSeconds();
    int milliseconds = getMilliseconds();

    std::string formattedTime = std::to_string(months) + "/" + std::to_string(days) + "/" + std::to_string(years) + " " + std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds) + ":" + std::to_string(milliseconds);

    return formattedTime;
}

void SimulationTime::incrementTime(double deltaTime)
{
    if (!m_paused)
    {
        m_currentTime += deltaTime * m_timeScale;

        m_iterations++;
        if (m_iterations % 100 == 0)
        {
            logTime();
            m_iterations = 0;
        }
    }
    else
    {
        return;
    }
}

void SimulationTime::incrementTimeReal()
{
    if (!m_paused)
    {
        // if this is the first time this function is called, set lastTime to the current time so there isnt a huge jump in time
        if (m_lastTime.time_since_epoch().count() == 0)
        {
            m_lastTime = std::chrono::steady_clock::now();
            return;
        }
        // increment by how long it has been since the last time this function was called
        auto currentTime = std::chrono::steady_clock::now();
        auto timeDifference = currentTime - m_lastTime;
        m_lastTime = currentTime;

        m_currentTime += std::chrono::duration_cast<std::chrono::milliseconds>(timeDifference).count() * m_timeScale;

        m_iterations++;
        if (m_iterations % 100 == 0)
        {
            logTime();
            m_iterations = 0;
        }
    }
    else
    {
        return;
    }
}

void SimulationTime::resetTimeLog()
{
    std::ofstream timeFile("../../out/logs/time.log");
    timeFile << 0;
    timeFile.close();
}

void SimulationTime::logTime()
{
    std::ofstream timeFile("../../out/logs/time.log");
    timeFile << m_currentTime;
    timeFile.close();
}
