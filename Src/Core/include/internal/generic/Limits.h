#pragma once

template<typename T, bool = std::is_integral<T>::value>
struct Limits {};

template<typename T>
struct Limits<T, true>
{
    static const T CompareEpsilon;
    static const T MinValue;
    static const T MaxValue;
};

template<typename T> const T Limits<T, true>::CompareEpsilon = (T)1;
template<typename T> const T Limits<T, true>::MinValue = std::numeric_limits<T>::lowest();
template<typename T> const T Limits<T, true>::MaxValue = std::numeric_limits<T>::max();

template<typename T>
struct Limits<T, false>
{
    static const T CompareEpsilon;
    static const T MinValue;
    static const T MaxValue;
};

template<typename T> const T Limits<T, false>::CompareEpsilon = (T)1e-10;
template<typename T> const T Limits<T, false>::MinValue = std::numeric_limits<T>::lowest();
template<typename T> const T Limits<T, false>::MaxValue = std::numeric_limits<T>::max();


