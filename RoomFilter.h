#pragma once
#include <SFML/Graphics.hpp>
#include "MapModel.h"

class RoomFilter
{
public:
    bool Inclusive;
    
    RoomFilter(bool inclusive = true) : Inclusive(inclusive) {}
    
    virtual void Filter(Room& pRoom) = 0;
    virtual void DrawFilter(sf::RenderWindow& rw, int scaleFactor = 1) = 0;
};

class BoxFilter : public RoomFilter {
public:
    int Left, Right;
    int Top, Bottom;
    
    BoxFilter(int x, int y, int w, int h, bool include = true) : RoomFilter(include)
    {
        Left = x;
        Right = x + w;
        Top = y;
        Bottom = y + h;
    }
    
    void Filter(Room& pRoom)
    {
        if((pRoom.Bounds.Left() >= Left) && (pRoom.Bounds.Right() <= Right) &&
           (pRoom.Bounds.Top() >= Top) && (pRoom.Bounds.Bottom() <= Bottom))
        {
            pRoom.filter = Inclusive;
        }
    }
    
    void DrawFilter(sf::RenderWindow& rw, int scaleFactor = 1)
    {
        sf::RectangleShape box(sf::Vector2f((Right-Left) * scaleFactor, (Bottom-Top) * scaleFactor));
        
        box.setFillColor(sf::Color::Transparent);
        box.setOutlineThickness(1);
        if(Inclusive)
        {
            box.setOutlineColor(sf::Color::Green);
        }
        else
        {
            box.setOutlineColor(sf::Color::Red);
        }
        box.setPosition(Left * scaleFactor, Top * scaleFactor);
        rw.draw(box);
    }
};

class CircleFilter : public RoomFilter {
public:
    int X, Y, Radius, RadiusSq;
    
    CircleFilter(int x, int y, int r, bool include = true) : RoomFilter(include)
    {
        X = x;
        Y = y;
        Radius = r;
        RadiusSq = r * r;
    }
    
    void Filter(Room& pRoom)
    {
        int dx = X - pRoom.Bounds.CenterX();
        int dy = Y - pRoom.Bounds.CenterY();
        int lenSq = (dx * dx) + (dy * dy);
        if(lenSq < RadiusSq)
        {
            pRoom.filter = Inclusive;
        }
    }
    
    void DrawFilter(sf::RenderWindow& rw, int scaleFactor = 1)
    {
        sf::CircleShape circle(Radius * scaleFactor);
        
        circle.setOrigin(Radius * scaleFactor, Radius * scaleFactor);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(1);
        if(Inclusive)
        {
            circle.setOutlineColor(sf::Color::Green);
        }
        else
        {
            circle.setOutlineColor(sf::Color::Red);
        }
        circle.setPosition(X * scaleFactor, Y * scaleFactor);
        rw.draw(circle);
    }
};

class HaloFilter : public RoomFilter {
public:
    int X, Y, OutterRadius, OutterRadiusSq, InnerRadius, InnerRadiusSq;
    
    HaloFilter(int x, int y, int innerRad, int outterRad, bool include = true) : RoomFilter(include)
    {
        X = x;
        Y = y;
        InnerRadius = innerRad;
        InnerRadiusSq = innerRad * innerRad;
        OutterRadius = outterRad;
        OutterRadiusSq = outterRad * outterRad;
    }
    
    void Filter(Room& pRoom)
    {
        int dx = X - pRoom.Bounds.CenterX();
        int dy = Y - pRoom.Bounds.CenterY();
        int lenSq = (dx * dx) + (dy * dy);
        if((lenSq > InnerRadiusSq) && (lenSq < OutterRadiusSq))
        {
            pRoom.filter = Inclusive;
        }
    }
    
    void DrawFilter(sf::RenderWindow& rw, int scaleFactor = 1)
    {
        // Draw inner circle
        sf::CircleShape circle(InnerRadius * scaleFactor);
        
        circle.setOrigin(InnerRadius, InnerRadius);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(1);
        if(Inclusive)
        {
            circle.setOutlineColor(sf::Color::Green);
        }
        else
        {
            circle.setOutlineColor(sf::Color::Red);
        }
        circle.setPosition(X * scaleFactor, Y * scaleFactor);
        rw.draw(circle);
        
        // Draw outter circle
        circle.setOrigin(OutterRadius * scaleFactor, OutterRadius * scaleFactor);
        circle.setRadius(OutterRadius * scaleFactor);
        rw.draw(circle);
    }
};