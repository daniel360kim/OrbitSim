/**
 * @file Timestep.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief basic timestep conversions for layers
 * @version 0.1
 * @date 2023-06-24
 * 
 * 
 */

class Timestep
{
public:
    Timestep(double time = 0.0f)
        : m_Time(time)
    {
    }

    operator double() const { return m_Time; }

    double GetSeconds() const { return m_Time; }
    double GetMilliseconds() const { return m_Time * 1000.0f; }
private:
    double m_Time;
};