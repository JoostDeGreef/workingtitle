#pragma once

#include <array>
#include <cassert>

#include "internal/generic/Scalar.h"

template<size_t COLUMNS, size_t ROWS, typename T = Scalar>
class Matrix
{
public:
    typedef Matrix<COLUMNS, ROWS, T> this_type;

    static constexpr size_t columns = COLUMNS;
    static constexpr size_t rows = ROWS;
    static constexpr size_t elements = ROWS * COLUMNS;

    Matrix();
    Matrix(const this_type& other);
    Matrix(this_type&& other) noexcept;
    template<typename... Args> Matrix(const Args & ... args);

    Matrix & operator = (const this_type& other);
    Matrix & operator = (this_type&& other) noexcept;

    void copy(const this_type& other);
    void swap(this_type&& other);

    const T & operator () (const size_t column, const size_t row) const;
          T & operator () (const size_t column, const size_t row);

    template<size_t COLUMNS2> Matrix<COLUMNS2, ROWS, T> operator * (const Matrix<COLUMNS2, COLUMNS, T>& other) const;

    template<typename... Args> void set(const Args & ... args);
    void fill(const T& value);
    void eye();
    Matrix<ROWS, COLUMNS, T> transposed() const;
private:
    void _set(std::array<T, elements>& data, const T& value);
    template<typename ...Args> void _set(std::array<T, elements>& data, const T& value, const Args & ...args);

    std::array<T, ROWS> getColumn(const size_t & column) const;

    std::array<T, elements> data;
};

template<size_t COLUMNS, size_t ROWS, typename T>
inline Matrix<COLUMNS, ROWS, T>::Matrix()
{}

template<size_t COLUMNS, size_t ROWS, typename T> inline
Matrix<COLUMNS, ROWS, T>::Matrix(const this_type& other)
    : data(std::move(other.data))
{}

template<size_t COLUMNS, size_t ROWS, typename T> inline
Matrix<COLUMNS, ROWS, T>::Matrix(this_type&& other) noexcept
    : data(std::forward(other.data))
{}

template<size_t COLUMNS, size_t ROWS, typename T>
inline Matrix<COLUMNS, ROWS, T>& Matrix<COLUMNS, ROWS, T>::operator=(const this_type & other)
{
    copy(other);
    return *this;
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline Matrix<COLUMNS, ROWS, T>& Matrix<COLUMNS, ROWS, T>::operator=(this_type&& other) noexcept
{
    swap(other);
    return *this;
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline void Matrix<COLUMNS, ROWS, T>::copy(const this_type& other)
{
    data = other.data;
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline void Matrix<COLUMNS, ROWS, T>::swap(this_type&& other)
{
    data.swap(other.data);
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline const T& Matrix<COLUMNS, ROWS, T>::operator()(const size_t column, const size_t row) const
{
    return data.at(M*row+column);
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline T& Matrix<COLUMNS, ROWS, T>::operator()(const size_t column, const size_t row)
{
    return data.at(columns * row + column);
}

template<size_t COLUMNS, size_t ROWS, typename T>
template<typename ...Args>
inline Matrix<COLUMNS, ROWS, T>::Matrix(const Args & ...args)
{
    set(args...);
}

template<size_t COLUMNS, size_t ROWS, typename T>
template<typename ...Args>
inline void Matrix<COLUMNS, ROWS, T>::set(const Args & ...args)
{
    _set(data, args...);
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline void Matrix<COLUMNS, ROWS, T>::fill(const T& value)
{
    data.fill(value);
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline void Matrix<COLUMNS, ROWS, T>::eye()
{
    fill(0);
    for (size_t i = 0; i < M && i < N; ++i)
    {
        data.at(i * M + i) = 1;
    }
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline Matrix<ROWS, COLUMNS, T> Matrix<COLUMNS, ROWS, T>::transposed() const
{
    Matrix<ROWS, COLUMNS, T> res;
    // TODO
    return res;
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline void Matrix<COLUMNS, ROWS, T>::_set(std::array<T, elements>& data, const T& value)
{
    data[elements - 1] = value;
}

template<size_t COLUMNS, size_t ROWS, typename T>
template<typename ...Args>
inline void Matrix<COLUMNS, ROWS, T>::_set(std::array<T, elements>& data, const T& value, const Args & ...args)
{
    data[elements - sizeof...(Args) - 1] = value;
    _set(data, args...);
}

template<size_t COLUMNS, size_t ROWS, typename T>
inline std::array<T, ROWS> Matrix<COLUMNS, ROWS, T>::getColumn(const size_t & column) const
{
    assert(column<COLUMNS);
    std::array<T, ROWS> res;
    for (size_t i = column,j=0; i < elements; i += columns,++j)
    {
        res(j) = data(i);
    }
    return res;
}

template<size_t COLUMNS, size_t ROWS, typename T>
template<size_t COLUMNS2>
inline Matrix<COLUMNS2, ROWS, T> Matrix<COLUMNS, ROWS, T>::operator*(const Matrix<COLUMNS2, COLUMNS, T>& other) const
{
    Matrix<COLUMNS2, ROWS, T> res;
    for (size_t c = 0; c < other.columns; ++c)
    {
        auto column = other.getColumn();
        for (size_t r = 0; r < rows; ++r)
        {
            T value = 0;
            auto row = &data.data()[r*columns];
            for (size_t i = 0; i < columns; ++i)
            {
                value += column(i) * row(i);
            }
            res(r) = value;
        }
    }


    return res;
}

