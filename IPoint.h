#pragma once
#include <string>   // std::string, std::to_string
#include <cmath>
#include "Helper.h"

class IPoint
{
public:
    int32 X;
    int32 Y;
    
    IPoint(int32 pX = 0, int32 pY = 0) : X(pX), Y(pY) {}
    
    IPoint(const IPoint& Point) : X(Point.X), Y(Point.Y) {}
    IPoint& operator=(const IPoint& Rhs)
    {
        X = Rhs.X;
        Y = Rhs.Y;
        return *this;
    }
    
    IPoint operator+(const IPoint& Rhs)
    {
        IPoint result = *this;
        result += Rhs;
        return result;
    }
    
    IPoint operator-(const IPoint& Rhs)
    {
        IPoint result = *this;
        result -= Rhs;
        return result;
    }
    
    IPoint operator*(const int32 factor)
    {
        IPoint result = *this;
        result *= factor;
        return result;
    }
    
    IPoint operator/(const int32 divisor)
    {
        IPoint result = *this;
        result /= divisor;
        return result;
    }
    
    IPoint& operator+=(const IPoint& Rhs)
    {
        X += Rhs.X;
        Y += Rhs.Y;
        return *this;
    }
    
    IPoint& operator-=(const IPoint& Rhs)
    {
        X -= Rhs.X;
        Y -= Rhs.Y;
        return *this;
    }
    
    IPoint& operator*=(const int32 factor)
    {
        X *= factor;
        Y *= factor;
        return *this;
    }
    
    IPoint& operator/=(const int32 divisor)
    {
        X /= divisor;
        Y /= divisor;
        return *this;
    }
    
    int32 distance(const IPoint& p)
    {
        int32 dx, dy;
        
        dx = p.X - X;
        dy = p.Y - Y;
        
        return sqrt((dx * dx) + (dy * dy));
    }
    
    int32 distanceSq(const IPoint& p)
    {
        int32 dx, dy;
        
        dx = p.X - X;
        dy = p.Y - Y;
        
        return ((dx * dx) + (dy * dy));
    }
    
    inline IPoint& Zero()
    {
        X = Y = 0;
        return *this;
    }
    
    inline bool IsNull() const
    {
        return ((X == 0) && (Y == 0));
    }
    
    std::string toString(std::string Name="IPoint")
    {
        return Name + " X : " + std::to_string(X) + " , Y : " + std::to_string(Y);
    }
};