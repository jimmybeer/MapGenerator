#pragma once
#include "IPoint.h"

class IRect
{
public:
    IPoint Position;
    int32 Width;
    int32 Height;
    
    IRect() : Position(0, 0), Width(0), Height(0) {}
    IRect(int32 X, int32 Y, int32 W, int32 H) : Position(X, Y), Width(W), Height(H) {}
    IRect(const IPoint& Pos, int32 W, int32 H) : Position(Pos), Width(W), Height(H) {}
    
    IRect(const IRect& Rect) : Position(Rect.Position), Width(Rect.Width), Height(Rect.Height) {}       
    IRect& operator=(const IRect& Rhs)
    {
        Position = Rhs.Position;
        Width = Rhs.Width;
        Height = Rhs.Height;
        return *this;
    }
    
    inline const bool IsNull() const { return (Width <= 0) && (Height <= 0); }
    
    inline const int32 HalfWidth() const { return Width / 2; }
    inline const int32 HalfHeight() const { return Height / 2; }
    inline const int32 CenterX() const { return Position.X + HalfWidth(); }
    inline const int32 CenterY() const { return Position.Y + HalfHeight(); }
    inline const int32 Left() const { return Position.X; }
    inline const int32 Right() const { return Position.X + Width; }
    inline const int32 Top() const { return Position.Y; }
    inline const int32 Bottom() const { return Position.Y + Height; }
    
    bool Contains(const IPoint& Pos) const { return Contains(Pos.X, Pos.Y); }
    bool Contains(const int32 pX, const int32 pY) const;
    bool Contains(const IRect& Rect) const;
    
    bool IsIntersecting(const IRect& Rect) const;
    IRect* Intersect(const IRect& Rect) const;
    void IntersectWith(const IRect& Rect);
    IRect* United(const IRect& Rect) const;
    
    IPoint* Penetration(const IRect& Rect) const;
       
    std::string toString(std::string Name="IRect")
    {
        return Name + " X : " + std::to_string(Position.X) + 
                  " , Y : " + std::to_string(Position.Y) +
                  " , Width : " + std::to_string(Width) + 
                  " , Height : " + std::to_string(Height);
    }
};