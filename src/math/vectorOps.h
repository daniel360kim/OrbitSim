/**
 * @file vectorOps.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief simple vector math functions
 * @version 0.1
 * @date 2023-06-24
 * 
 * 
 */

#ifndef VECTOROPS_H
#define VECTOROPS_H

#include <cmath>

template <typename T, size_t N>
class Vector
{
public:
    Vector();
    ~Vector();

    T elements[N];

    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(const T scalar) const;
    Vector operator/(const T scalar) const;

    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;

    T& operator[](const size_t index);
    const T& operator[](const size_t index) const;

    double magnitude() const;

    static T dot(const Vector& a, const Vector& b);
    static Vector cross(const Vector& a, const Vector& b);
};

template <typename T, size_t N>
Vector<T, N>::Vector()
{
    for (size_t i = 0; i < N; i++)
    {
        elements[i] = 0;
    }
}

template <typename T, size_t N>
Vector<T, N>::~Vector()
{
}

template <typename T, size_t N>
Vector<T, N> Vector<T, N>::operator+(const Vector& other) const
{
    Vector<T, N> result;
    for (size_t i = 0; i < N; i++)
    {
        result.elements[i] = elements[i] + other.elements[i];
    }
    return result;
} 

template <typename T, size_t N>
Vector<T, N> Vector<T, N>::operator-(const Vector& other) const
{
    Vector<T, N> result;
    for (size_t i = 0; i < N; i++)
    {
        result.elements[i] = elements[i] - other.elements[i];
    }
    return result;
}

template <typename T, size_t N>
Vector<T, N> Vector<T, N>::operator*(const T scalar) const
{
    Vector<T, N> result;
    for (size_t i = 0; i < N; i++)
    {
        result.elements[i] = elements[i] * scalar;
    }
    return result;
}

template <typename T, size_t N>
Vector<T, N> Vector<T, N>::operator/(const T scalar) const
{
    Vector<T, N> result;
    for (size_t i = 0; i < N; i++)
    {
        result.elements[i] = elements[i] / scalar;
    }
    return result;
}

template <typename T, size_t N>
bool Vector<T, N>::operator==(const Vector& other) const
{
    for (size_t i = 0; i < N; i++)
    {
        if(elements[i] != other[i])
        {
            return false;
        }
    }
    return true;
}

template <typename T, size_t N>
bool Vector<T, N>::operator!=(const Vector& other) const
{
    return !(*this == other);
}

template <typename T, size_t N>
T& Vector<T, N>::operator[](const size_t index)
{
    return elements[index];
}

template <typename T, size_t N>
const T& Vector<T, N>::operator[](const size_t index) const
{
    return elements[index];
}

template <typename T, size_t N>
double Vector<T, N>::magnitude() const
{
    double result = 0;
    for (size_t i = 0; i < N; i++)
    {
        result += elements[i] * elements[i];
    }
    return std::sqrt(result);
}

template <typename T, size_t N>
T Vector<T, N>::dot(const Vector& a, const Vector& b)
{
    T result = 0;
    for (size_t i = 0; i < N; i++)
    {
        result += a.elements[i] * b.elements[i];
    }
    return result;
}

template <typename T, size_t N>
Vector<T, N> Vector<T, N>::cross(const Vector& a, const Vector& b)
{
    static_assert(N == 3, "Cross product only works for 3D vectors");
    Vector<T, N> result;
    result.elements[0] = a.elements[1] * b.elements[2] - a.elements[2] * b.elements[1];
    result.elements[1] = a.elements[2] * b.elements[0] - a.elements[0] * b.elements[2];
    result.elements[2] = a.elements[0] * b.elements[1] - a.elements[1] * b.elements[0];
    return result;
}




















#endif