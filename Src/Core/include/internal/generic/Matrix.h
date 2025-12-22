#pragma once

#include <array>

#include "internal/generic/Scalar.h"

template<size_t M, size_t N, typename T = Scalar>
class Matrix
{
public:
    typedef Matrix<M, N, T> this_type;

    Matrix(const this_type& other);
    Matrix(this_type&& other) noexcept;
    template<typename... Args> Matrix(const Args & ... args);

    Matrix & operator = (const this_type& other);
    Matrix & operator = (this_type&& other) noexcept;

    void copy(const this_type& other);
    void swap(this_type&& other);

    const T & operator () (const size_t column, const size_t row) const;
          T & operator () (const size_t column, const size_t row);

    template<size_t U> Matrix<U, N, T> operator * (const Matrix<U, M, T>& other) const;

    template<typename... Args> void set(const Args & ... args);
    void fill(const T& value);
    void eye();
private:
    void _set(std::array<T, M* N>& data, const T& value);
    template<typename ...Args> void _set(std::array<T, M* N>& data, const T& value, const Args & ...args);

    std::array<T, M*N> data;
};

template<size_t M, size_t N, typename T> inline
Matrix<M, N, T>::Matrix(const this_type& other)
    : data(std::move(other.data))
{}

template<size_t M, size_t N, typename T> inline
Matrix<M, N, T>::Matrix(this_type&& other) noexcept
    : data(std::forward(other.data))
{}

template<size_t M, size_t N, typename T>
inline Matrix<M, N, T>& Matrix<M, N, T>::operator=(const this_type & other)
{
    copy(other);
    return *this;
}

template<size_t M, size_t N, typename T>
inline Matrix<M, N, T>& Matrix<M, N, T>::operator=(this_type&& other) noexcept
{
    swap(other);
    return *this;
}

template<size_t M, size_t N, typename T>
inline void Matrix<M, N, T>::copy(const this_type& other)
{
    data = other.data;
}

template<size_t M, size_t N, typename T>
inline void Matrix<M, N, T>::swap(this_type&& other)
{
    data.swap(other.data);
}

template<size_t M, size_t N, typename T>
inline const T& Matrix<M, N, T>::operator()(const size_t column, const size_t row) const
{
    return data.at(M*row+column);
}

template<size_t M, size_t N, typename T>
inline T& Matrix<M, N, T>::operator()(const size_t column, const size_t row)
{
    return data.at(M * row + column);
}

template<size_t M, size_t N, typename T>
template<typename ...Args>
inline Matrix<M, N, T>::Matrix(const Args & ...args)
{
    set(args...);
}

template<size_t M, size_t N, typename T>
template<typename ...Args>
inline void Matrix<M, N, T>::set(const Args & ...args)
{
    _set(data, args...);
}

template<size_t M, size_t N, typename T>
inline void Matrix<M, N, T>::fill(const T& value)
{
    data.fill(value);
}

template<size_t M, size_t N, typename T>
inline void Matrix<M, N, T>::eye()
{
    fill(0);
    for (size_t i = 0; i < M && i < N; ++i)
    {
        data.at(i * M + i) = 1;
    }
}

template<size_t M, size_t N, typename T>
inline void Matrix<M, N, T>::_set(std::array<T, M* N>& data, const T& value)
{
    data[M * N - 1] = value;
}

template<size_t M, size_t N, typename T>
template<typename ...Args>
inline void Matrix<M, N, T>::_set(std::array<T, M* N>& data, const T& value, const Args & ...args)
{
    data[M * N - sizeof...(Args) - 1] = value;
    _set(data, args...);
}

template<size_t M, size_t N, typename T>
template<size_t U>
inline Matrix<U, N, T> Matrix<M, N, T>::operator*(const Matrix<U, M, T>& other) const
{
    // TODO
    return Matrix<U, N, T>();
}

