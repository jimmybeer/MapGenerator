
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <list>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"
#include "MapModel.h"
#include "RoomFilter.h"
#include "MapBuilderLib.h"
#include "PseudoRand.h"
#include "Delaunay.h"

using namespace std;

#define REMOVE_ON_RATIO 1
#define SEPARATE_ROOMS 2
#define FILTER_ROOMS 3
#define FILTER_CORRIDOR_FEATURES 4
#define REDUCE_ROOMS 5
#define CALC_CONNECTIONS 6
#define DELAUNAY_DRAW_DELAY 7
#define BUILD_CORRIDORS 8

static int WinWidth = 800;
static int WinHeight = 800;
static int mode = 0;

static int scaleFactor = 1;

static int roomCount = 0;
static vector<RoomFilter*> filters;
static list<int32> edges;

static sf::Color RoomFillColor = sf::Color::White;
static sf::Color RoomLineColor = sf::Color::Red;
static sf::Color CorridorFillColor = sf::Color::White;
static sf::Color CorridorLineColor = sf::Color::Blue;
static sf::Color DelaunayLineColor = sf::Color::Yellow;
static sf::Color FinalEdgeLineColor = sf::Color::Cyan;

void drawMinSpan(sf::RenderWindow& rw, list<int32>& minSpan, vector<FPoint*>& point)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(0,0), FinalEdgeLineColor),
        sf::Vertex(sf::Vector2f(0,0), FinalEdgeLineColor)
    };
    
    auto end = minSpan.end();
    for(auto itr = minSpan.begin(); itr != end; itr++)
    {
        FPoint* v1 = point[*itr];
        itr++;
        FPoint* v2 = point[*itr];
        
        line[0].position.x = v1->X * scaleFactor;
        line[0].position.y = v1->Y * scaleFactor;
        line[1].position.x = v2->X * scaleFactor;
        line[1].position.y = v2->Y * scaleFactor;
        rw.draw(line, 2, sf::Lines);
    }
    
}

void drawRoomPoints(sf::RenderWindow& rw, vector<FPoint*>& point)
{
    sf::CircleShape circle(6);
    circle.setFillColor(sf::Color::Green);
    circle.setOrigin(3.f, 3.f);
    
    int len = point.size();
    for(int i = 0; i < len; i++)
    {
        circle.setPosition(point[i]->X * scaleFactor, point[i]->Y * scaleFactor);
        rw.draw(circle);
    }
}

void drawDelaunayEdge(sf::RenderWindow& rw, vector<Edge*>& edge, vector<FPoint*>& point)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(0,0), DelaunayLineColor),
        sf::Vertex(sf::Vector2f(0,0), DelaunayLineColor)
    };
    
    int len = edge.size();
    for(int i = 0; i < len; i++)
    {
        FPoint* v1 = point[edge[i]->s];
        FPoint* v2 = point[edge[i]->t];
        
        line[0].position.x = v1->X * scaleFactor;
        line[0].position.y = v1->Y * scaleFactor;
        line[1].position.x = v2->X * scaleFactor;
        line[1].position.y = v2->Y * scaleFactor;
        rw.draw(line, 2, sf::Lines);
    }
}

void drawFilters(sf::RenderWindow& rw)
{
    for(auto f: filters)
    {
        f->DrawFilter(rw, scaleFactor);
    }
}

void drawCorridor(sf::RenderWindow& rw, Corridor* c)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(0,0), CorridorLineColor),
        sf::Vertex(sf::Vector2f(0,0), CorridorLineColor)
    };
    line[0].position.x = c->SX;
    line[0].position.y = c->SY;
    line[1].position.x = c->EX;
    line[1].position.y = c->EY;
    rw.draw(line, 2, sf::Lines);
}

void drawCorridors(sf::RenderWindow& rw)
{
    int len = MapInfo.Corridors.size();
    for(int i = 0; i < len; i++)
    {
        drawCorridor(rw, MapInfo.Corridors[i]);
    }
}

void drawRoom(sf::RenderWindow& rw, IRect& rect)
{
    sf::RectangleShape room(sf::Vector2f(rect.Width * scaleFactor, rect.Height * scaleFactor));
    
    int32 x1 = rect.Position.X * scaleFactor;
    int32 y1 = rect.Position.Y * scaleFactor;
    int32 x2 = x1 + (rect.Width * scaleFactor);
    int32 y2 = y1 + (rect.Height * scaleFactor);
    
    room.setFillColor(RoomFillColor);
    room.setPosition(x1, y1);
    rw.draw(room);
    
    // Draw cell lines
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(0,0), RoomLineColor),
        sf::Vertex(sf::Vector2f(0,0), RoomLineColor)
    };
    
    line[0].position.x = x1;
    line[0].position.y = y1;
    line[1].position.x = x2;
    line[1].position.y = y1;
    rw.draw(line, 2, sf::Lines);
    line[1].position.x = x1;
    line[1].position.y = y2;
    rw.draw(line, 2, sf::Lines);
    line[0].position.x = x2;
    line[0].position.y = y2;
    rw.draw(line, 2, sf::Lines);
    line[1].position.x = x2;
    line[1].position.y = y1;
    rw.draw(line, 2, sf::Lines);
    // Draw horizonal lines.
    /*for(int y = y1; y <= y2; y += scaleFactor)
     {
     line[0].position.x = x1;
     line[0].position.y = y;
     line[1].position.x = x2;
     line[1].position.y = y;
     rw.draw(line, 2, sf::Lines);
     }
     // Draw vertical lines.
     for(int x = x1; x <= x2; x += scaleFactor)
     {
     line[0].position.x = x;
     line[0].position.y = y1;
     line[1].position.x = x;
     line[1].position.y = y2;
     rw.draw(line, 2, sf::Lines);
     }*/
}

void drawRooms(sf::RenderWindow& rw)
{
    
    for(int i = 0; i < roomCount; i++)
    {
        drawRoom(rw, MapInfo.Rooms[i]->Bounds);
    }
    
    if(roomCount < MapInfo.Rooms.size())
    {
        roomCount++;
        if(roomCount == MapInfo.Rooms.size())
        {
            mode++;
        }
    }
}

void drawCorridorFeature(sf::RenderWindow& rw, IRect& rect)
{
    sf::RectangleShape room(sf::Vector2f(rect.Width * scaleFactor, rect.Height * scaleFactor));
    
    int32 x1 = rect.Position.X * scaleFactor;
    int32 y1 = rect.Position.Y * scaleFactor;
    int32 x2 = x1 + (rect.Width * scaleFactor);
    int32 y2 = y1 + (rect.Height * scaleFactor);
    
    room.setFillColor(CorridorFillColor);
    room.setPosition(x1, y1);
    rw.draw(room);
    
    // Draw cell lines
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(0,0), CorridorLineColor),
        sf::Vertex(sf::Vector2f(0,0), CorridorLineColor)
    };
    
    line[0].position.x = x1;
    line[0].position.y = y1;
    line[1].position.x = x2;
    line[1].position.y = y1;
    rw.draw(line, 2, sf::Lines);
    line[1].position.x = x1;
    line[1].position.y = y2;
    rw.draw(line, 2, sf::Lines);
    line[0].position.x = x2;
    line[0].position.y = y2;
    rw.draw(line, 2, sf::Lines);
    line[1].position.x = x2;
    line[1].position.y = y1;
    rw.draw(line, 2, sf::Lines);
    /*
     // Draw horizonal lines.
     for(int y = y1; y <= y2; y += scaleFactor)
     {
     line[0].position.x = x1;
     line[0].position.y = y;
     line[1].position.x = x2;
     line[1].position.y = y;
     rw.draw(line, 2, sf::Lines);
     }
     // Draw vertical lines.
     for(int x = x1; x <= x2; x += scaleFactor)
     {
     line[0].position.x = x;
     line[0].position.y = y1;
     line[1].position.x = x;
     line[1].position.y = y2;
     rw.draw(line, 2, sf::Lines);
     }*/
}

void drawCorridorFeatures(sf::RenderWindow& rw)
{
    int len = MapInfo.CorridorFeatures.size();
    for(int i = 0; i < len; i++)
    {
        drawCorridorFeature(rw, MapInfo.CorridorFeatures[i]->Bounds);
    }
}

int main(int, char const**)
{
    Triangulation* tri;
    list<int32>* minSpan;
    int delayCount = 120;
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "SFML window");
    
    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    UMapBuilderLib::InitMap(MapInfo, WinWidth, WinHeight);
    MapInfo.setRoomSizeLimits(5, 15, 5, 15);
    MapInfo.setGenerationLimits(20,5);
    UMapBuilderLib::SetSeed(MapInfo, PseudoRand::setSeed());
    UMapBuilderLib::MakeRooms(MapInfo, 100, 3, 40, WinWidth / 2, WinHeight / 2, 30, 30);
    
    //filters.push_back(new BoxFilter(20, 50, 120, 15));
    //filters.push_back(new BoxFilter(20, 90, 120, 15));
    //filters.push_back(new BoxFilter(50, 20, 15, 120));
    //filters.push_back(new BoxFilter(90, 20, 15, 120));
    //filters.push_back(new CircleFilter(80,80, 30));
    //filters.push_back(new HaloFilter(140, 140, 30, 60));
    
    cout << MapInfo.toString() << "\n";
    cout << "After room generation #rooms=" << MapInfo.Rooms.size() << "\n";
    
    window.setFramerateLimit(60);
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        // Clear screen
        window.clear();
        
        if(mode == BUILD_CORRIDORS)
        {
            UMapBuilderLib::GenerateCorridors(MapInfo, *minSpan);
            mode++;
        }
        
        if(mode == DELAUNAY_DRAW_DELAY)
        {
            if(--delayCount < 1)
            {
                mode++;
            }
        }
        
        if(mode == CALC_CONNECTIONS)
        {
            tri = UMapBuilderLib::PerformDelaunayTriangulation(MapInfo);
            cout << "After triangulation #totalEdges=" << tri->edge.size() << "\n";
            
            minSpan = UMapBuilderLib::CalcMinSpan(MapInfo, *tri);
            cout << "After Kruskal #totalEdges=" << tri->edge.size() << "\n";
            cout << "                #minEdges=" << (minSpan->size()/2) << "\n";
            
            UMapBuilderLib::AddRandomEdges(MapInfo, *tri, *minSpan);
            cout << "After random insertion #totalEdges=" << tri->edge.size() << "\n";
            cout << "                         #minEdges=" << (minSpan->size()/2) << "\n";
            mode++;
        }
        
        if(mode == REDUCE_ROOMS)
        {
            UMapBuilderLib::ReduceRooms(MapInfo);
            roomCount = MapInfo.Rooms.size();
            cout << "After room reduction #rooms=" << MapInfo.Rooms.size() << "\n";
            mode++;
        }
        
        if(mode == FILTER_CORRIDOR_FEATURES)
        {
            UMapBuilderLib::SeparateCorridorFeatures(MapInfo);
            roomCount = MapInfo.Rooms.size();
            cout << "After separating corridor features #rooms=" << MapInfo.Rooms.size() << "\n";
            cout << "                        #CorridorFeatures=" << MapInfo.CorridorFeatures.size() << "\n";
            mode++;
        }
        
        if(mode == FILTER_ROOMS)
        {
            if(filters.size() > 0)
            {
                for(auto f : filters)
                {
                    UMapBuilderLib::FilterRooms(MapInfo, *f);
                }
                UMapBuilderLib::RemoveFiltered(MapInfo);
            }
            roomCount = MapInfo.Rooms.size();
            cout << "After filtering #rooms=" << MapInfo.Rooms.size() << "\n";
            mode++;
        }
        
        if(mode == SEPARATE_ROOMS)
        {
            if(UMapBuilderLib::SeparateRooms(MapInfo))
            {
                mode++;
            }
        }
        
        if(mode == REMOVE_ON_RATIO)
        {
            UMapBuilderLib::RemoveRoomsBelowRatio(MapInfo, (1.f / 3.f));
            cout << "After ratio reduction #rooms=" << MapInfo.Rooms.size() << "\n";
            roomCount = MapInfo.Rooms.size();
            mode++;
        }
        
        drawRooms(window);
        
        if(mode >= FILTER_ROOMS)
        {
            drawFilters(window);
        }
        if(mode >= FILTER_CORRIDOR_FEATURES)
        {
            drawCorridorFeatures(window);
        }
        if(mode > CALC_CONNECTIONS && mode <= DELAUNAY_DRAW_DELAY)
        {
            drawRoomPoints(window, tri->point);
            drawDelaunayEdge(window, tri->edge, tri->point);
        }
        if(mode > DELAUNAY_DRAW_DELAY && mode <= BUILD_CORRIDORS)
        {
            drawMinSpan(window, *minSpan, tri->point);
        }
        if(mode > BUILD_CORRIDORS)
        {
            drawCorridors(window);
        }
        
        // Update the window
        window.display();
    }
    
    delete tri;
    delete minSpan;
    
    return EXIT_SUCCESS;
}
