#include "MapBuilderLib.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include "IRect.h"
#include "IPoint.h"
#include "Delaunay.h"
#include "Kruskal.h"

using namespace std;

void UMapBuilderLib::InitMap(MapInfoType& MapInfo, int32 Width, int32 Height)
{
    /** Check width and height are valid values. **/
    if(Width == 0 || Height == 0)
    {
        return;
    }
    
    /** Clear any previously set data and free memory. **/
    if(MapInfo.IsInitialised)
    {
        UMapBuilderLib::ClearMap(MapInfo);
    }
    
    MapInfo.Width = Width;
    MapInfo.Height = Height;
    
    /** Set initialisation flag to note map info is valid. **/
    MapInfo.IsInitialised = true;
}

void UMapBuilderLib::ClearMap(MapInfoType& MapInfo)
{
    /** Delete all dynamically allocated rooms. **/
    int32 len = MapInfo.Rooms.size();
    
    for(int32 i = 0; i < len; i++)
    {
        /** Individual rooms manually deleted. **/
        delete MapInfo.Rooms[i];
    }
    /** Reset the vector back to zero (hopefully). **/
    MapInfo.Rooms.clear();
    MapInfo.Rooms.shrink_to_fit();
    
    /** Delete all dynamically allocated corridors. **/
    len = MapInfo.Corridors.size();
    
    for(int32 i = 0; i < len; i++)
    {
        /** Individual corridors manually deleted. **/
        delete MapInfo.Corridors[i];
    }
    /** Reset the vector back to zero (hopefully). **/
    MapInfo.Corridors.clear();
    MapInfo.Corridors.shrink_to_fit();
    
    /** Reset initialisation flag to note map info is no longer valid. **/
    MapInfo.IsInitialised = false;
}

void UMapBuilderLib::MakeRooms(MapInfoType& MapInfo, int32 Num, int32 minLen, int32 maxLen, int32 XOrigin, int32 YOrigin, int32 WidthMargin, int32 HeightMargin)
{
    if(!MapInfo.IsInitialised) return;
    
    if(minLen <= 0) minLen = 2;
    if(maxLen <= minLen) maxLen = minLen + 10;
    if(maxLen < MapInfo.MaxRoomWidth) maxLen = MapInfo.MaxRoomWidth;
    if(maxLen < MapInfo.MaxRoomHeight) maxLen = MapInfo.MaxRoomHeight;
    
    srand(MapInfo.Seed);
    
    default_random_engine generatorX(rand());
    uniform_int_distribution<int32> distributionX(XOrigin - WidthMargin, XOrigin + WidthMargin);
    auto RandomX = bind ( distributionX, generatorX );
    
    default_random_engine generatorY(rand());
    uniform_int_distribution<int32> distributionY(YOrigin - HeightMargin, YOrigin + HeightMargin);
    auto RandomY = bind ( distributionY, generatorY );
    
    default_random_engine generatorDim(rand());
    uniform_int_distribution<int32> distributionDim(minLen, maxLen);
    auto RandomDim = bind ( distributionDim, generatorDim );
    
    for(int32 i = 0; i < Num; i++)
    {
        int32 x = RandomX();
        int32 y = RandomY();
        int32 w = RandomDim();
        int32 h = RandomDim();
        MapInfo.Rooms.push_back(new Room(x, y, w, h));
    }
}

bool UMapBuilderLib::SeparateRooms(MapInfoType & MapInfo)
{
    IPoint force;
    int interactions = 0;
    
    vector<Room*> rooms = MapInfo.Rooms;
    int len = rooms.size();
    
    for(int i = 0; i < len; i++)
    {
        IRect* currRoom = &rooms[i]->Bounds;
        
        for(int j = 0; j < len; j++)
        {
            if(j == i) continue;
            
            IRect* nextRoom = &rooms[j]->Bounds;
            
            if(currRoom->IsIntersecting(*nextRoom))
            {
                IPoint* pen = currRoom->Penetration(*nextRoom);
                
                if(abs(pen->X) > abs(force.X))
                {
                    force.X = pen->X;
                }
                if(abs(pen->Y) > abs(force.Y))
                {
                    force.Y = pen->Y;
                }
                
                delete pen;
            }
        }
        
        if(force.X != 0 && force.Y != 0)
        {
            interactions++;
            if(abs(force.X) < abs(force.Y))
            {
                currRoom->Position.X += force.X;
            }
            else
            {
                currRoom->Position.Y += force.Y;
            }
            
            force.Zero();
        }
    }
    
    return (interactions == 0);
}

static bool IsMarkedToDelete(Room* r)
{
    return r->Enabled == false;
}

void UMapBuilderLib::RemoveRoomsBelowRatio(MapInfoType& MapInfo, float ratio)
{
    int len = MapInfo.Rooms.size();
    int i = 0;
    
    while(i < len)
    {
        IRect* currRoom = &MapInfo.Rooms[i]->Bounds;
        
        float currRatio;
        if(currRoom->Width < currRoom->Height)
        {
            currRatio = float(currRoom->Width) / float(currRoom->Height);
        }
        else
        {
            currRatio = float(currRoom->Height) / float(currRoom->Width);
        }
        
        if(currRatio <= ratio)
        {
            Room* tmp = MapInfo.Rooms[i];
            tmp->Enabled = false;
            
            // Move current room to end of vector.
            swap(MapInfo.Rooms[i], MapInfo.Rooms[len-1]);
            
            // Delete room and erase element from vector
            delete tmp;
            MapInfo.Rooms.erase(MapInfo.Rooms.begin() + len - 1);
            
            len--;
        }
        else
        {
            i++;
        }
    }
}

void UMapBuilderLib::FilterRooms(MapInfoType& MapInfo, RoomFilter& rf)
{
    for(Room* r : MapInfo.Rooms)
    {
        rf.Filter(*r);
    }
}

void UMapBuilderLib::RemoveFiltered(MapInfoType& MapInfo)
{
    int len = MapInfo.Rooms.size();
    int i = 0;
    
    while(i < len)
    {
        Room* currRoom = MapInfo.Rooms[i];
        
        if(currRoom->filter == false)
        {
            currRoom->Enabled = false;
            
            // Move current room to end of vector.
            swap(MapInfo.Rooms[i], MapInfo.Rooms[len-1]);
            
            // Delete room and erase element from vector
            delete currRoom;
            MapInfo.Rooms.erase(MapInfo.Rooms.begin() + len - 1);
            
            len--;
        }
        else
        {
            i++;
        }
    }
}

void UMapBuilderLib::GetRoomCenters(MapInfoType& MapInfo, std::vector<FPoint*>& point)
{
    int len = MapInfo.Rooms.size();
    for(int i = 0; i < len; i++)
    {
        Room* r = MapInfo.Rooms[i];
        
        point[i]->X = (float)r->Bounds.CenterX();
        point[i]->Y = (float)r->Bounds.CenterY();
    }
}

void UMapBuilderLib::ReduceRooms(MapInfoType& MapInfo)
{
    int len = MapInfo.Rooms.size();
    int i = 0;
    
    while(len > MapInfo.MaxRooms)
    {
        i = rand() % (len-1);
        Room* currRoom = MapInfo.Rooms[i];
        
        // Move current room to end of vector.
        currRoom->Enabled = false;
        swap(MapInfo.Rooms[i], MapInfo.Rooms[len-1]);
        
        // Delete room and erase element from vector
        delete currRoom;
        MapInfo.Rooms.erase(MapInfo.Rooms.begin() + len - 1);
        
        len--;
    }
}

void UMapBuilderLib::SeparateCorridorFeatures(MapInfoType& MapInfo)
{
    int len = MapInfo.Rooms.size();
    int i = 0;
    
    while(i < len)
    {
        Room* currRoom = MapInfo.Rooms[i];
        
        if((currRoom->Bounds.Width < MapInfo.MinRoomWidth) || (currRoom->Bounds.Height < MapInfo.MaxRoomHeight))
        {
            // Move current room to end of vector.
            currRoom->Enabled = false;
            swap(MapInfo.Rooms[i], MapInfo.Rooms[len-1]);
            
            // Convert room to corridor feature.
            MapInfo.CorridorFeatures.push_back(new CorridorFeature(currRoom));
            
            // Delete room and erase element from vector
            delete currRoom;
            MapInfo.Rooms.erase(MapInfo.Rooms.begin() + len - 1);
            
            len--;
        }
        
        i++;
    }
}

Triangulation* UMapBuilderLib::PerformDelaunayTriangulation(MapInfoType& MapInfo)
{
    // Create datastructure for calculating connections.
    Triangulation* tri = new Triangulation(MapInfo.Rooms.size());
    
    // Get a vector of all the rooms centre position.
    UMapBuilderLib::GetRoomCenters(MapInfo, tri->point);
    
    // Perform Delaunay triangulation to connect all the rooms.
    QuadraticAlgorithm qa;
    qa.triangulate(*tri);
    
    return tri;
}
list<int32>* UMapBuilderLib::CalcMinSpan(MapInfoType& MapInfo, Triangulation& tri)
{
    
    // Generate a list of the minimum edges required to connect all the rooms
    list<int32>* minSpan = Kruskal::runKruskal(tri.point, tri.edge);
    MapInfo.MinConnectedCorridors = minSpan->size();
    
    return minSpan;
}

void UMapBuilderLib::AddRandomEdges(MapInfoType& MapInfo, Triangulation& tri, list<int32>& minSpan)
{
    int totalCorridors = (minSpan.size() / 2) + MapInfo.MaxRandomCorridors;
    // Add some edges at random (this will allow for loops etc.)
    int edgeLen = tri.edge.size();
    while((minSpan.size() / 2) < totalCorridors)
    {
        int32 index = (int32)(rand() % edgeLen);
        minSpan.push_back(tri.edge[index]->s);
        minSpan.push_back(tri.edge[index]->t);
        
        swap(tri.edge[index], tri.edge[edgeLen-1]);
        edgeLen--;
    }
}

int32 UMapBuilderLib::DetermineX(Room& r, double ang)
{
    // if ang leads to the right of room
    if(ang >= r.ru || ang <= r.rd)
    {
        return r.Bounds.Right();
    }
    
    // if ang leads to the left of room
    else if(ang <= r.lu && ang >= r.ld)
    {
        return r.Bounds.Left();
    }
    
    // if ang leads to the top of room
    else if(ang > r.lu && ang < r.ru)
    {
        return (int32)round(r.Bounds.CenterX() + (r.Bounds.HalfHeight() * tan(ang + (M_PI/2))));
        
    }
    
    // if ang leads to the top of room
    else if(ang < r.ld && ang > r.rd)
    {
        return (int32)round(r.Bounds.CenterX() - (r.Bounds.HalfHeight() * tan(ang + (M_PI/2))));
        
    }
    return -1;
}

int32 UMapBuilderLib::DetermineY(Room& r, double ang)
{
    // if ang leads to the right of room
    if(ang >= r.ru || ang <= r.rd)
    {
        return (int32)round(r.Bounds.CenterY() + (r.Bounds.HalfWidth() * tan(ang)));
    }
    
    // if ang leads to the left of room
    else if(ang <= r.lu && ang >= r.ld)
    {
        return (int32)round(r.Bounds.CenterY() - (r.Bounds.HalfWidth() * tan(ang)));
    }
    
    // if ang leads to the bottom of room
    else if(ang > r.lu && ang < r.ru)
    {
        return r.Bounds.Top();
        
    }
    
    // if ang leads to the bottom of room
    else if(ang < r.ld && ang > r.rd)
    {
        return r.Bounds.Bottom();
    }
    
    return -1;
}

void UMapBuilderLib::CreateCorridorsBetween(MapInfoType& MapInfo, int32 Room1Index, int32 Room2Index)
{
    Room* room1 = MapInfo.Rooms[Room1Index];
    Room* room2 = MapInfo.Rooms[Room2Index];
    
    // Get center point for each room
    int32 x1 = room1->Bounds.CenterX();
    int32 y1 = room1->Bounds.CenterY();
    int32 x2 = room2->Bounds.CenterX();
    int32 y2 = room2->Bounds.CenterY();
    
    // Calculate angle from room to room;
    double angFrom1To2 = atan2(y1-y2, x1-x2);
    double angFrom2To1 = atan2(y2-y1, x2-x1);
    
    // Calculate starting and ending x,y corrds  for corridor.
    Corridor* c = new Corridor();
    c->SX = UMapBuilderLib::DetermineX(*room1, angFrom1To2);
    c->SY = UMapBuilderLib::DetermineY(*room1, angFrom1To2);
    c->EX = UMapBuilderLib::DetermineX(*room2, angFrom2To1);
    c->EY = UMapBuilderLib::DetermineY(*room2, angFrom2To1);
    
    MapInfo.Corridors.push_back(c);
}

void UMapBuilderLib::GenerateCorridors(MapInfoType& MapInfo, list<int32>& edges)
{
    // Pre calculate all the rooms angles from thier center to their corners.
    int len = MapInfo.Rooms.size();
    for(int i = 0; i < len; i++)
    {
        MapInfo.Rooms[i]->CalcCorners();
    }
    
    // Generate a corridor for each edge in the edges list.
    for(list<int32>::iterator itr = edges.begin(); itr != edges.end(); itr++)
    {
        // Each edge element contains an index to a point element in the triangulation point list, which in turn should match up to its source Room index in MapInfo
        int32 room1Ind = (*(itr));
        itr++;
        int32 room2Ind = (*(itr));
        UMapBuilderLib::CreateCorridorsBetween(MapInfo, room1Ind, room2Ind);
    }
}