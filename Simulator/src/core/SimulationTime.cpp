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
#include <sstream>
#include <iomanip>

/**
 * @brief Converts date and time to milliseconds after January 1st, 2000 12:00:00 AM
 *
 * @param time date and time
 * @return unsigned long long milliseconds after January 1st, 2000 12:00:00 AM
 */
unsigned long long SimulationTime::Time::dateTimeToMs()
{
    // Create a time_point representing January 1, 2000, at 12:00:00
    std::chrono::system_clock::time_point startPoint =
        std::chrono::system_clock::from_time_t(946684800);

    // Create a time_point representing the given date and time
    std::tm timeInfo{};
    timeInfo.tm_year = years - 1900;
    timeInfo.tm_mon = months - 1;
    timeInfo.tm_mday = days;
    timeInfo.tm_hour = hours;
    timeInfo.tm_min = minutes;
    timeInfo.tm_sec = seconds;

    std::chrono::system_clock::time_point targetPoint =
        std::chrono::system_clock::from_time_t(std::mktime(&timeInfo));

    // Calculate the duration between the two time_points in milliseconds
    std::chrono::milliseconds duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(targetPoint - startPoint);

    // Add the additional milliseconds
    duration += std::chrono::milliseconds(milliseconds);

    // Return the total number of milliseconds
    return duration.count();
}


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


SimulationTime::Time SimulationTime::getTime() const
{
    Time time;
    time.years = getYears();
    time.months = getMonths();
    time.days = getDays();
    time.hours = getHours();
    time.minutes = getMinutes();
    time.seconds = getSeconds();
    time.milliseconds = getMilliseconds();

    return time;
}

unsigned long long SimulationTime::getElapsedMs(Time end, Time start) const
{
    unsigned long long elapsedMsStart = timeToMs(start);
    unsigned long long elapsedMsEnd = timeToMs(end);

    return elapsedMsEnd - elapsedMsStart;
}

std::string SimulationTime::getFormattedDateTime() const
{

    int years = getYears();
    int months = getMonths();
    int days = getDays();
    int hours = getHours();
    int minutes = getMinutes();
    int seconds = getSeconds();
    int milliseconds = getMilliseconds();

    std::ostringstream formattedTimeStream;
    formattedTimeStream << std::setfill('0');
    formattedTimeStream << std::setw(4) << years << "-";
    formattedTimeStream << std::setw(2) << months << "-";
    formattedTimeStream << std::setw(2) << days << " ";
    formattedTimeStream << std::setw(2) << hours << ":";
    formattedTimeStream << std::setw(2) << minutes << ":";
    formattedTimeStream << std::setw(2) << seconds << ".";
    formattedTimeStream << std::setw(3) << milliseconds;

    return formattedTimeStream.str();
}

std::string SimulationTime::getFormattedTime() const 
{
    int hours = getHours();
    int minutes = getMinutes();
    int seconds = getSeconds();
    int milliseconds = getMilliseconds();

    std::ostringstream formattedTimeStream;
    formattedTimeStream << std::setfill('0');
    formattedTimeStream << std::setw(2) << hours << ":";
    formattedTimeStream << std::setw(2) << minutes << ":";
    formattedTimeStream << std::setw(2) << seconds << ".";
    formattedTimeStream << std::setw(3) << milliseconds;

    return formattedTimeStream.str();
}

std::string SimulationTime::getFormattedDate() const
{
    int years = getYears();
    int months = getMonths();
    int days = getDays();

    std::ostringstream formattedTimeStream;
    formattedTimeStream << std::setfill('0');
    formattedTimeStream << std::setw(4) << years << "-";
    formattedTimeStream << std::setw(2) << months << "-";
    formattedTimeStream << std::setw(2) << days;

    return formattedTimeStream.str();
}

void SimulationTime::incrementTime(double deltaTime)
{
    if (!m_paused)
    {
        m_currentTime += deltaTime * m_timeScale;
        logTime();
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
        
        logTime();
    }
    else
    {
        return;
    }
}

void SimulationTime::setTime(Time time)
{
    unsigned long long newTime = timeToMs(time);
    m_currentTime = newTime;
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

unsigned long long SimulationTime::timeToMs(Time time)
{
    unsigned long long ms = 0;
    ms += time.years * 31536000000;
    ms += time.months * 2592000000;
    ms += time.days * 86400000;
    ms += time.hours * 3600000;
    ms += time.minutes * 60000;
    ms += time.seconds * 1000;
    ms += time.milliseconds;

    return ms;
}