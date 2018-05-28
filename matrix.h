#ifndef MARTIX_H_INCLUDED
#define MARTIX_H_INCLUDED

#include "figures.h"

#define M_PI 3.14159265359

template <class T>
class Matrix
{
    static const int size = 3;
    std::array<std::array<T, size>, size> cf;
    //T cf[size][size];

    void setIdentity()
    {
        for(int r = 0; r < size; r++)//r - row
            for(int c = 0; c < size; c++)//c - column
                if(r == c)
                    cf[r][c] = 1;
                else
                    cf[r][c] = 0;
    }

    void copyMx(const Matrix& rhm)
    {
        for(int r = 0; r < size; r++)//r - row
            for(int c = 0; c < size; c++)//c - column
                cf[r][c] = rhm.cf[r][c];
    }

    public:
    Matrix()
    {
        setIdentity();
    }
    Matrix(const Matrix& rhm)
    {
        copyMx(rhm);
    }

    static Matrix scaleMx(T scaleX, T scaleY)
    {
        Matrix<T> retMatrix;
        retMatrix.cf[0][0] = scaleX;
        retMatrix.cf[1][1] = scaleY;

        return retMatrix;
    }
    static Matrix translateMx(T offsetX, T offsetY)
    {
        Matrix<T> retMatrix;
        retMatrix.cf[2][0] = offsetX;
        retMatrix.cf[2][1] = offsetY;

        return retMatrix;
    }
    static Matrix rotateMx(T angle)
    {
        angle *= M_PI/180;
        Matrix<T> retMatrix;
        retMatrix.cf[0][0] = cos(angle);
        retMatrix.cf[0][1] = -sin(angle);
        retMatrix.cf[1][0] = sin(angle);
        retMatrix.cf[1][1] = cos(angle);

        return retMatrix;
    }

    friend std::ostream& operator <<(ostream& os, const Matrix<T>& mx)
    {
        for(int r = 0; r < size; r++)//r - row
        {
            for(int c = 0; c < size; c++)//c - column
            {
                os << right << setw(10) << mx.cf[r][c];
            }
            os << "\n";
        }
        return os;
    }

    friend Matrix operator * (const Matrix& lhm, const Matrix& rhm)
    {
        Matrix retMatrix;
        for(int r = 0; r < static_cast<int>(size); r++)//r - row
        {
            for(int c = 0; c < static_cast<int>(size); c++)//c - column
            {
                T value = 0;
                for(int k = 0; k < static_cast<int>(size); k++)
                    value += lhm.cf[r][k] * rhm.cf[k][c];

                retMatrix.cf[r][c] = value;
            }
        }

        return retMatrix;
    }

    Matrix& operator = (const Matrix& rhm) { copyMx(rhm); return *this; }
    Matrix& operator *= (const Matrix& rhm) { return *this = *this * rhm;}


    FPoint transform(const FPoint& pt) const
    {
        FPoint retPoint;
        retPoint.x = pt.x * cf[0][0] + pt.y * cf[1][0] + 1 * cf[2][0];
        retPoint.y = pt.x * cf[0][1] + pt.y * cf[1][1] + 1 * cf[2][1];

        return retPoint;
    }

};

#endif // MARTIX_H_INCLUDED
