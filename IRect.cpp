#include "IRect.h"
#include <algorithm>    // std::max, std::min

bool IRect::Contains(const int32 pX, const int32 pY) const
{
    return (
	       ((pX >= Left()) && (pX <= Right())) &&
	       ((pY >= Top())  && (pY <= Bottom()))
	       );
}

bool IRect::Contains(const IRect& Rect) const
{
    return (
	       ((Rect.Left() >= Left()) && (Rect.Right() <= Right())) &&
	       ((Rect.Top() >= Top())  && (Rect.Bottom() <= Bottom()))
	       );
}

bool IRect::IsIntersecting(const IRect& Rect) const
{		
	int32 x1 = std::max(Left(), Rect.Left());
	int32 y1 = std::max(Top(), Rect.Top());
	int32 x2 = std::min(Right(), Rect.Right());
	int32 y2 = std::min(Bottom(), Rect.Bottom());
		
	if((x1 < x2) && (y1 < y2)) 
    {
	    return true;
	} 
    else 
    {
		return false;
	}
}

IRect* IRect::Intersect(const IRect& Rect) const
{		
	int32 x1 = std::max(Left(), Rect.Left());
	int32 y1 = std::max(Top(), Rect.Top());
	int32 x2 = std::min(Right(), Rect.Right());
	int32 y2 = std::min(Bottom(), Rect.Bottom());
		
	if((x1 < x2) && (y1 < y2)) 
    {
		return new IRect(x1, y1, (x2 - x1), (y2 - y1));
	} 
    else 
    {
		return nullptr;
	}
}

void IRect::IntersectWith(const IRect& Rect)
{
    int32 x2 = Right();
	int32 x4 = Rect.Right();
	int32 y2 = Bottom();
	int32 y4 = Rect.Bottom();
		
	Position.X = std::max(Left(), Rect.Left());
    Position.Y = std::max(Top(), Rect.Top());
		
	int32 x6 = std::min(x2, x4);
	int32 y6 = std::min(y2, y4);
	Width = x6 - Position.X;
	Height = y6 - Position.Y;
}

IRect* IRect::United(const IRect& Rect) const
{
	if(Rect.IsNull()) 
    {
		return new IRect(Position, Width, Height);
	}
	else if(this->IsNull()) 
    {			
		return new IRect(Rect.Position, Rect.Width, Rect.Height);
	}
	else 
    {
		int32 newX = std::min(Left(), Rect.Left());
		int32 newY = std::min(Top(), Rect.Top()); 
	    int32 newWidth = 0;
	    int32 newHeight = 0;
			
		if(Right() > Rect.Right()) 
        {
			newWidth = Right() - newX;
		} else 
        {
			newWidth = Rect.Right() - newX;
		}
			
	    if(Bottom() > Rect.Bottom()) 
        {
			newHeight = Bottom() - newY;
		} 
        else 
        {
			newHeight = Rect.Bottom() - newY;
		}
			
		return new IRect(newX, newY, newWidth, newHeight);
	}
}

IPoint* IRect::Penetration(const IRect& Rect) const
{
    IPoint* result = new IPoint();
    
	int32 x1 = std::max(Left(), Rect.Left());
	int32 y1 = std::max(Top(), Rect.Top());
	int32 x2 = std::min(Right(), Rect.Right());
	int32 y2 = std::min(Bottom(), Rect.Bottom());
		
	if((x1 < x2) && (y1 < y2)) {
		result->X = x2 - x1;
		result->Y = y2 - y1;
			
		if(Left() < Rect.Left()) {
			result->X *= -1;
	    }
		if(Top() < Rect.Top()) {
		    result->Y *= -1;
		}
	}
    
    return result;
}