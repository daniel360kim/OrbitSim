/**
 * @file RandomEnum.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief 
 * @version 0.1
 * @date 2023-07-30
 * 
 * 
 */

#pragma once

#include <random>

/**
 * @brief Returns a random enum element within the enum
 * 
 * @tparam T the enum type
 */
template <typename T>
class RandomEnum
{
public:
    RandomEnum()
    {
        std::random_device rd;
        m_generator = std::mt19937(rd());
    }
    ~RandomEnum() = default;

    T get()
    {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(T::Invalid) - 1);
        return static_cast<T>(dist(m_generator));
    }
private:
    std::mt19937 m_generator;
};


template <typename T>
class RandomNumber
{
public:
    RandomNumber()
    {
        std::random_device rd;
        m_generator = std::mt19937(rd());
    }
    ~RandomNumber() = default;

    T get(T min, T max)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(m_generator);
    }

private:
    std::mt19937 m_generator;
};