#pragma once
#include <string>   // std::string, std::to_string
#include <cmath>
#include "Helper.h"

class FPoint
{
public:
    float X;
    float Y;
    
    FPoint(float pX = 0, float pY = 0) : X(pX), Y(pY) {}
    
    FPoint(const FPoint& Point) : X(Point.X), Y(Point.Y) {}       
    FPoint& operator=(const FPoint& Rhs)
    {
        X = Rhs.X;
        Y = Rhs.Y;
        return *this;
    }
    
    FPoint operator+(const FPoint& Rhs) 
    {
        FPoint result = *this;
        result += Rhs;
        return result;
    }
   
    FPoint operator-(const FPoint& Rhs) 
    {
        FPoint result = *this;
        result -= Rhs;
        return result;
    }

    FPoint operator*(const int32 factor) 
    {
        FPoint result = *this;
        result *= factor;
        return result;
    } 

    FPoint operator/(const int32 divisor)
    {
        FPoint result = *this;
        result /= divisor;
        return result;
    }

    FPoint& operator+=(const FPoint& Rhs) 
    {
        X += Rhs.X;
        Y += Rhs.Y;
        return *this;
    }

    FPoint& operator-=(const FPoint& Rhs)
    {
        X -= Rhs.X;
        Y -= Rhs.Y;
        return *this;
    }

    FPoint& operator*=(const int32 factor) 
    {
        X *= factor;
        Y *= factor;
        return *this;
    } 

    FPoint& operator/=(const int32 divisor) 
    {
        X /= divisor;
        Y /= divisor;
        return *this;
    }
	
	float distance(const FPoint& p)
	{
	    float dx, dy;
		
		dx = p.X - X;
		dy = p.Y - Y;
		
		return sqrt((dx * dx) + (dy * dy));
	}
	
	float distanceSq(const FPoint& p)
	{
	    float dx, dy;
		
		dx = p.X - X;
		dy = p.Y - Y;
		
		return ((dx * dx) + (dy * dy));
	}
    
    inline FPoint& Zero()
    {
        X = Y = 0;
        return *this;
    }
	
	inline bool IsNull(const float epsilon = 2e-6) const 
    {
	    return ((-epsilon < X) && (X < epsilon)) && ((-epsilon < Y) && (Y < epsilon));
	}
    
    std::string toString(std::string Name="FPoint")
    {
        return Name + " X : " + std::to_string(X) + " , Y : " + std::to_string(Y);
    }
};