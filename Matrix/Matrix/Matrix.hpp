//
//  Matrix.hpp
//  Matrix
//
//  Created by Алексей Зерцалов on 10.03.2020.
//  Copyright © 2020 Алексей Зерцалов. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <stdio.h>
#include <vector>
#include "Finite.hpp"
#include "Rational.hpp"

template <unsigned N, unsigned M, typename Field = Rational>
class Matrix
{
    
public:
    
    template <unsigned _N_, unsigned _M_, typename _Field_>
    friend std::ostream& operator<< (std::ostream&, Matrix<_N_, _M_, _Field_> const&);
    
    template <unsigned _N_, unsigned _M_, typename _Field_>
    friend std::istream& operator>> (std::istream&, Matrix<_N_, _M_, _Field_>&);
    
    std::vector <std::vector <Field> > _table_;
    
    using _type_ = Matrix<N, M, Field>;
    
    template <typename U>
    struct _FiniteCheck_ { static void check() {} };
    
    template <typename U, U n>
    struct _FiniteCheck_<Finite<n>> {
        template <bool isPrime = PrimeNumberCheck<n>::is_prime>
        static void check();
        
        template <>
        static void check <true>() {}
        
    };
    
    // This function will crash if Filed = Finite<N> where N is not prime
    static void _CheckFinite_() { _FiniteCheck_<Field>::check(); }
    
    template <unsigned _N, unsigned _M>
    struct _SizeCheck_ { static void check(); };
    
    template <unsigned _N>
    struct _SizeCheck_<_N, _N> { static void check() {} };
    
    // This function will crash if N != M
    static void _CheckSize_() { _SizeCheck_<N, M>::check(); }
    
    // SecondField must be:
    // - unsigned ~ (this is for rank)
    // - vector <Field> ~ (this is for finding solution)
    // - vector <vector <Field> > ~ (this is for inverted matrix)
    template <typename SecondField>
    void _GaussMethod_(SecondField&) const;
    
    template <typename SecondField>
    void _SwapRows_(SecondField&, size_t first, size_t second)
    {
        _table_[first].swap(_table_[second]);
    }
    
    template <>
    void _SwapRows_<Field>(Field& determinant, size_t first, size_t second)
    {
        _table_[first].swap(_table_[second]);
        determinant = -determinant;
    }

    template <>
    void _SwapRows_<Matrix<N, M, Field>> (Matrix<N, M, Field>& secondMatrix, size_t first, size_t second)
    {
        _table_[first].swap(_table_[second]);
        secondMatrix._table_[first].swap(secondMatrix._table_[second]);
    }
    
    template <typename SecondField>
    void _SumWithAnotherRow_(SecondField& sec_field, size_t row_from, size_t row_to, Field element)
    {
        for (size_t indx = 0; indx < M; indx++) {
            _table_[row_to][indx] = _table_[row_to][indx] + _table_[row_from][indx] * element;
        }
    }
    
    template <>
    void _SumWithAnotherRow_<Matrix<N, M, Field>> (Matrix<N, M, Field>& secondMatrix, size_t row_from, size_t row_to, Field element)
    {
        for (size_t indx = 0; indx < M; indx++) {
            _table_[row_to][indx] = _table_[row_to][indx] + _table_[row_from][indx] * element;
            secondMatrix._table_[row_to][indx] = secondMatrix._table_[row_to][indx] + secondMatrix._table_[row_from][indx] * element;
        }
    }
    
    template <typename SecondField>
    void _ChangingResult_(SecondField& smth, size_t row_pos, size_t column_pos) {}
    
    template <>
    void _ChangingResult_<unsigned>(unsigned& rank, size_t row_pos, size_t column_pos)
    {
        rank = static_cast<unsigned>(row_pos < column_pos ? row_pos : column_pos);
    }
    
    template <typename SecondField>
    void _DivideRowByNumber_(SecondField& rank, size_t row, Field element)
    {
        for (size_t indx = 0; indx < M; indx++) {
            _table_[row][indx] /= element;
        }
    }
    
    template <>
    void _DivideRowByNumber_<Field>(Field& determinant, size_t row, Field element)
    {
        for (size_t indx = 0; indx < M; indx++) {
            _table_[row][indx] /= element;
        }
        determinant *= element;
    }
    
    template <>
    void _DivideRowByNumber_<Matrix<N, M, Field>>(Matrix<N, M, Field>& secondMatrix, size_t row, Field element)
    {
        for (size_t indx = 0; indx < M; indx++) {
            _table_[row][indx] /= element;
            secondMatrix._table_[row][indx] /= element;
        }
    }
    
    
public:
    
    Matrix();
    Matrix(Field);
    Matrix(const Matrix<N, M, Field>&);
    
    _type_ operator+ (const _type_&) const;
    _type_& operator+= (const _type_&);
    _type_ operator- (const _type_&) const;
    _type_& operator-= (const _type_&);
    
    _type_ operator* (Field) const;
    _type_& operator*= (Field);
    
    template <unsigned K>
    Matrix<N, K, Field> operator* (const Matrix<M, K, Field>&) const;
    template <unsigned K>
    Matrix<N, K, Field>& operator*= (const Matrix<M, K, Field>&);
    
    Field det() const;
    
    Matrix<M, N, Field> transposed() const;
    
    unsigned rank() const;
    
    _type_ inverted() const;
    _type_ invert();
    
    Field trace();
    
    std::vector<Field> getRow(unsigned);
    std::vector<Field> getColumn(unsigned);
    
    std::vector<Field>& operator[](const size_t);
    const std::vector<Field>& operator[](const size_t) const;
    // TODO: operator[][] overloading
    // https://en.cppreference.com/w/cpp/language/operators
    // this is reference for this overloading
};

template <unsigned N, typename Field>
using SquareMatrix = Matrix<N, N, Field>;

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix()
{
    _CheckFinite_();
    _table_.resize(N, std::vector<Field>(N));
}
 
template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix(Field element)
{
    _CheckFinite_();
    _table_.resize(N, std::vector<Field>(N, element));
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix(const _type_& rhs)
{
    _CheckFinite_();
    _table_ = rhs._table_;
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::operator+ (const _type_& rhs) const
{
    Matrix<N, M, Field> result(*this);
    
    result += rhs;
    
    return result;
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator+= (const _type_& rhs)
{
    for (size_t indxRow = 0; indxRow < N; indxRow++) {
        for (size_t indxColumn = 0; indxColumn < M; indxColumn++) {
            _table_[indxRow][indxColumn] += rhs._table_[indxRow][indxColumn];
        }
    }
    
    return (*this);
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::operator- (const _type_& rhs) const
{
    Matrix<N, M, Field> result(*this);
    
    result -= rhs;
    
    return result;
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator-= (const _type_& rhs)
{
    for (size_t indxRow = 0; indxRow < N; indxRow++) {
        for (size_t indxColumn = 0; indxColumn < M; indxColumn++) {
            _table_[indxRow][indxColumn] -= rhs._table_[indxRow][indxColumn];
        }
    }
    
    return (*this);
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::operator* (Field rhs) const
{
    Matrix<N, M, Field> result(*this);
    
    result *= rhs;
    
    return result;
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator*= (Field rhs)
{
    for (size_t indxRow = 0; indxRow < N; indxRow++) {
        for (size_t indxColumn = 0; indxColumn < M; indxColumn++) {
            _table_[indxRow][indxColumn] *= rhs;
        }
    }
    
    return (*this);
}

template <unsigned N, unsigned M, typename Field>
template <unsigned K>
Matrix<N, K, Field> Matrix<N, M, Field>::operator* (const Matrix<M, K, Field>& rhs) const
{
    Matrix<N, K, Field> result;
    
    for (size_t indxRow = 0; indxRow < N; indxRow++) {
        for (size_t indxColumn = 0; indxColumn < K; indxColumn++) {
            for (size_t indxM = 0; indxM < M; indxM++) {
                result._table_[indxRow][indxColumn] += _table_[indxRow][indxM] *
                                                   rhs._table_[indxM][indxColumn];
            }
        }
    }
    
    return result;
}

template <unsigned N, unsigned M, typename Field>
template <unsigned K>
Matrix<N, K, Field>& Matrix<N, M, Field>::operator*= (const Matrix<M, K, Field>& rhs)
{
    return (*this) = (*this) * rhs;
}

template <unsigned N, unsigned M, typename Field>
template <typename SecondField>
void Matrix<N, M, Field>::_GaussMethod_(SecondField & result) const
{
    Matrix<N, M, Field> matrix(*this);
    
    size_t row = 0;
    size_t column = 0;
    
    while (row < N && column < M) {
        
        // Finding nonzero element table[i][row]
        size_t index_nonzero_row = row;
        
        for (; index_nonzero_row < N; index_nonzero_row++) {
            if (matrix._table_[index_nonzero_row][column] != static_cast<Field>(0)) {
                break;
            }
        }
        
        if (index_nonzero_row == N) {
            column++;
            continue;
        }
        
        // Swaping this row
        if (index_nonzero_row != row) {
            matrix._SwapRows_<SecondField>(result, row, index_nonzero_row);
        }
        
        // Making 1 in this element
        matrix._DivideRowByNumber_(result, row, matrix._table_[row][column]);
        
        // Making zeroes under that element
        for (size_t indx_row = 0; indx_row < N; indx_row++) {
            if (indx_row == row) {
                continue;
            }
            
            matrix._SumWithAnotherRow_<SecondField>(result, row, indx_row, -(matrix._table_[indx_row][column]));
        }
        
        row++;
        column++;
    }
    
    matrix._ChangingResult_(result, row, column);
}

template <unsigned N, unsigned M, typename Field>
Field Matrix<N, M, Field>::det() const
{
    _CheckSize_();
    
    Field determinant = 1u;
    
    _GaussMethod_(determinant);
    
    return determinant;
}

template <unsigned N, unsigned M, typename Field>
Matrix<M, N, Field> Matrix<N, M, Field>::transposed() const
{
    Matrix<M, N, Field> result;
    
    for (size_t indxRow = 0; indxRow < N; indxRow++) {
        for (size_t indxColumn = 0; indxColumn < M; indxColumn++) {
            result._table_[indxColumn][indxRow] = _table_[indxRow][indxColumn];
        }
    }
    
    return result;
}

template <unsigned N, unsigned M, typename Field>
unsigned Matrix<N, M, Field>::rank() const
{
    unsigned rank = 0;
    
    _GaussMethod_(rank);
    
    return rank;
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::inverted() const
{
    _CheckSize_();
    
    Matrix<N, M, Field> result;
    for (size_t row = 0; row < N; row++) {
        for (size_t column = 0; column < M; column++) {
            result._table_[row][column] = (row == column ? 1u : 0u);
        }
    }
    
    _GaussMethod_(result);
    
    return result;
}

template <unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::invert()
{
    _CheckSize_();
    
    return (*this) = (*this).inverted();
}

template <unsigned N, unsigned M, typename Field>
Field Matrix<N, M, Field>::trace()
{
    _CheckSize_();
    
    Field result;
    for (size_t indx = 0; indx < N; indx++) {
        result += _table_[indx][indx];
    }
    
    return result;
}

template <unsigned N, unsigned M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getRow (unsigned numRow)
{
    return _table_[numRow];
}

template <unsigned N, unsigned M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getColumn (unsigned numColumn)
{
    std::vector<Field> result(M);
    for (size_t indx = 0; indx < N; indx++) {
        result[indx] = _table_[numColumn][indx];
    }
    return result;
}

template <unsigned N, unsigned M, typename Field>
std::vector<Field>& Matrix<N, M, Field>::operator[] (const size_t index)
{
    return _table_[index];
}

template <unsigned N, unsigned M, typename Field>
const std::vector<Field>& Matrix<N, M, Field>::operator[] (const size_t index) const
{
    return _table_[index];
}

template <unsigned N, unsigned M, typename Field>
std::ostream& operator<< (std::ostream& out, Matrix<N, M, Field> const& elem) {
    for (size_t indxRow = 0; indxRow < N; indxRow++) {
        for (size_t indxColumn = 0; indxColumn < M; indxColumn++) {
            out << elem._table_[indxRow][indxColumn] << " ";
        }
        out << "\n";
    }
    return out;
}

template <unsigned N, unsigned M, typename Field>
std::istream& operator>> (std::istream& in, Matrix<N, M, Field>& elem) {
    for (size_t indxRow = 0; indxRow < N; indxRow++) {
        for (size_t indxColumn = 0; indxColumn < M; indxColumn++) {
            in >> elem._table_[indxRow][indxColumn];
        }
    }
    return in;
}

#endif /* Matrix_hpp */
