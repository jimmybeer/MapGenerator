#pragma once
#include <vector>
#include <string>
#include <cmath>
#include "Helper.h"
#include "IRect.h"

static const int32 TopWall = 0;
static const int32 RightWall = 1;
static const int32 BottomWall = 2;
static const int32 LeftWall = 3;
static const int32 walls = 4;

/** Struct for storing information about a door instance. **/
typedef struct
{
    bool IsOpen;
    bool IsLocked;
    bool IsDestructable;
    bool IsHidden;
} DoorType;

/** Struct for storing information about an individual edge of a tile. **/
typedef struct
{
    bool IsBlocked;
    bool IsDestructable;
} WallType;

/** Struct describing a corridor for use during construction of map. **/
class Corridor
{
public:
    Corridor() {}
    virtual ~Corridor() {}
    
    int32 SX, SY; // Start x,y
    int32 EX, EY; // End x, y
    IRect Bounds;
};

/** Struct describing a room for use during construction of map. **/
class Room
{
public:
    Room(int32 X, int32 Y, int32 W, int32 H)
    {
        Enabled = true;
        filter = false;
        Bounds.Position.X = X;
        Bounds.Position.Y = Y;
        Bounds.Width = W;
        Bounds.Height = H;
    }
    virtual ~Room()
    {
        Corridors.clear();
        Corridors.shrink_to_fit();
    }
    
    void CalcCorners()
    {
        int32 hw = Bounds.HalfWidth();
        int32 hh = Bounds.HalfHeight();
        
        lu = atan2(hh, hw);    // Angle to top left corner
        ru = atan2(hh, -hw);   // Angle to top right corner
        rd = atan2(-hh, -hw);  // Angle to bottom right corner
        ld = atan2(-hh, hw);   // Angle to bottom left corner
    }
    
    bool Enabled;
    bool filter;
    IRect Bounds;
    double lu, ru, rd, ld;
    std::vector<Corridor*> Corridors;
};

/** Struct describing a room for use during construction of map. **/
class CorridorFeature
{
public:
    CorridorFeature(int32 X, int32 Y, int32 W, int32 H)
    {
        Bounds.Position.X = X;
        Bounds.Position.Y = Y;
        Bounds.Width = W;
        Bounds.Height = H;
    }
    virtual ~CorridorFeature()
    {
        LinkedCorridor = 0;
    }
    
    CorridorFeature(Room* room)
    {
        Bounds.Position.X = room->Bounds.Position.X;
        Bounds.Position.Y = room->Bounds.Position.Y;
        Bounds.Width = room->Bounds.Width;
        Bounds.Height = room->Bounds.Height;
    }
    
    IRect Bounds;
    Corridor* LinkedCorridor;
};

/** Structure for storing map generation settings. **/
typedef struct
{
    int32 Seed; /** Random generator seed for this map **/
    int32 Height;
    int32 Width;
    
    std::vector<Corridor*> Corridors;
    std::vector<CorridorFeature*> CorridorFeatures;
    std::vector<Room*> Rooms;
    
    int32 MinRoomWidth;
    int32 MaxRoomWidth;
    int32 MinRoomHeight;
    int32 MaxRoomHeight;
    int32 MaxRooms;
    int32 MaxRandomCorridors;
    
    int32 MinConnectedCorridors;
    
    bool IsInitialised;
    
    void setRoomSizeLimits(int32 MinWidth, int32 MaxWidth, int32 MinHeight, int32 MaxHeight)
    {
        MinRoomWidth = MinWidth;
        MaxRoomWidth = MaxWidth;
        MinRoomHeight = MinHeight;
        MaxRoomHeight = MaxHeight;
    }
    
    void setGenerationLimits(int32 MaxRoomCount = 10, int32 MaxRandomCorridorsCount = 10)
    {
        MaxRooms = MaxRoomCount;
        MaxRandomCorridors = MaxRandomCorridorsCount;
    }
    
    std::string toString()
    {
        return "MapInfo : \n"
        "\n  Seed : " + std::to_string(Seed) +
        "\n  Width : " + std::to_string(Width) +
        "\n  Height : " + std::to_string(Height) +
        "\n  MinRoomWidth : " + std::to_string(MinRoomWidth) +
        "\n  MaxRoomWidth : " + std::to_string(MaxRoomWidth) +
        "\n  MinRoomHeight : " + std::to_string(MinRoomHeight) +
        "\n  MaxRoomHeight : " + std::to_string(MaxRoomHeight) +
        "\n  MaxRooms : " + std::to_string(MaxRooms) +
        "\n  MaxRandomCorridors : " + std::to_string(MaxRandomCorridors) +
        "\n  IsInitialised : " + std::to_string(IsInitialised);
    }
    
    std::string roomsToString()
    {
        std::string str = "MapInfo Rooms: \n";
        
        int32 len = Rooms.size();
        for(int32 i = 0; i < len; i++)
            str += Rooms[i]->Bounds.toString("Bounds") + "\n";
        
        return str;
    }
} MapInfoType;

static MapInfoType MapInfo = {}; // Initialise contents to 0