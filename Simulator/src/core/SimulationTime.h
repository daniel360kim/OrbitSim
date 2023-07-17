/**
 * @file Time.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief Manages time for the simulation (orbits, planet rotation, etc.)
 * @version 0.1
 * @date 2023-07-16
 * 
 * 
 */

#pragma once

#include <string>
#include <chrono>


//Gives the true anomaly of all the planets on January 1, 2000 at 12:00:00 UTC
/*
    constexpr double MERCURY = degrees_arcmin_to_radians(175, 55.36);
    constexpr double VENUS = degrees_arcmin_to_radians(50, 24.98);
    constexpr double EARTH = degrees_arcmin_to_radians(50, 24.98);
    constexpr double MOON = degrees_arcmin_to_radians(129, 20.15);
    constexpr double MARS = degrees_arcmin_to_radians(23, 13.01);
    //TODO add the rest of the planets
*/

class SimulationTime
{
public:
    struct Time
    {
        int years;
        int months;
        int days;
        int hours;
        int minutes;
        int seconds;
        int milliseconds;
    };

    SimulationTime(double initialTime = 0.0, double timeScale = 1.0) : m_currentTime(initialTime), m_timeScale(timeScale), m_paused(false) {}
    ~SimulationTime();

    void loadTimeFromFile();

    double getCurrentTime() const { return m_currentTime; }
    double getTimeScale() const { return m_timeScale; }
    
    // Gets years, momths etc. since January 1, 2000 at 12:00:00 UTC
    int getYears() const;
    int getMonths() const;
    int getDays() const;
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;
    int getMilliseconds() const;

    Time getTime() const;

    unsigned long long getElapsedMs(Time end, Time start) const;

    std::string getFormattedTime();

    void setTimeScale(double timeScale) { m_timeScale = timeScale; }
    void incrementTime(double deltaTime);
    void incrementTimeReal();
    void pause() { m_paused = true; }
    void resume() { m_paused = false; }
    void setTime(double time) { m_currentTime = time; }
    void setTime(Time time);
    void accelerateTime(double factor) { m_timeScale *= factor; }
    void decelerateTime(double factor) { m_timeScale /= factor; }
    void resetTime() { m_currentTime = 0.0; }
    
    void resetTimeLog();
    void logTime();

private:
    unsigned long long m_currentTime; //in milliseconds time since January 1, 2000 at 12:00:00 UTC
    double m_timeScale;
    bool m_paused;

    std::chrono::time_point<std::chrono::steady_clock> m_lastTime;
    unsigned int m_iterations = 0;

    std::chrono::system_clock::time_point getEpoch() const;

    static unsigned long long timeToMs(Time time);
};