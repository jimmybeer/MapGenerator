#pragma once

#include <stdlib.h>
#include <vector>
#include <list>
#include "MapModel.h"
#include "RoomFilter.h"
#include "FPoint.h"
#include "Delaunay.h"
#include "Helper.h"

//UCLASS()
class UMapBuilderLib //: public UBlueprintFunctionLibrary
{
    //GENERATED_UCLASS_BODY()
public:
    static void InitMap(MapInfoType& MapInfo, int32 Width, int32 Height);
    static void ClearMap(MapInfoType& MapInfo);
    
    static void SetSeed(MapInfoType& MapInfo, int32 Seed) { MapInfo.Seed = Seed; }
    static int32 SetRandomSeed(MapInfoType& MapInfo) { MapInfo.Seed = rand(); return MapInfo.Seed; }
    
    static void MakeRooms(MapInfoType& MapInfo, int32 Num, int32 minLen, int32 maxLen, int32 XOrigin, int32 YOrigin, int32 WidthMargin = 0, int32 HeightMargin = 0);
    static bool SeparateRooms(MapInfoType& MapInfo);
    static void RemoveRoomsBelowRatio(MapInfoType& MapInfo, float ratio);
    static void FilterRooms(MapInfoType& MapInfo, RoomFilter& rf);
    static void RemoveFiltered(MapInfoType& MapInfo);
    static void GetRoomCenters(MapInfoType& MapInfo, std::vector<FPoint*>& point);
    static void ReduceRooms(MapInfoType& MapInfo);
    static void SeparateCorridorFeatures(MapInfoType& MapInfo);
    static Triangulation* PerformDelaunayTriangulation(MapInfoType& MapInfo);
    static std::list<int32>* CalcMinSpan(MapInfoType& MapInfo, Triangulation& tri);
    static void AddRandomEdges(MapInfoType& MapInfo, Triangulation& tri, std::list<int32>& minSpan);
    static void GenerateCorridors(MapInfoType& MapInfo, std::list<int32>& edges);
    static void CreateCorridorsBetween(MapInfoType& MapInfo, int32 Room1Index, int32 Room2Index);
    
private:
    static int32 DetermineX(Room& r, double ang);
    static int32 DetermineY(Room& r, double ang);
};