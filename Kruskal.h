#pragma once
#include <vector>
#include <list>
#include "Delaunay.h"
#include "FPoint.h"
#include "Helper.h"

/**
 * An implementation of the Kruskal algorithm to find the minimum spanning tree of a graph.
 * Example usage:
 * // vertices hold data that will be used in the distance metric function
 * var verticies = [
 *  [0.38503426988609135,0.5090362404007465],
 *  [0.19520984776318073,0.786977760726586],
 *  ...
 *]
 *
 * // edges are vertex position pairs
 * var edges = [
 *  8,6,  8,12,  6,12,...
 *];
 *
 * function metric_dist( a, b )
 *{
 *  var dx = a[0] - b[0];
 *  var dy = a[1] - b[1];
 *   return dx*dx + dy*dy;
 *}
 *
 * var edgeMST = TAGAlg.Kruskal( vertices, edges, metric_dist );
 *
 * // Print minimum spanning tree
 *for (var ind in edgeMST)
 *{
 *  var u = edgeMST[ind][0];
 *  var v = edgeMST[ind][1];
 *
 *  console.log( verts[u][0] + " " + verts[u][1] );
 *  console.log( verts[v][0] + " " + verts[v][1] );
 *  console.log("");
 *}
 */

typedef struct
{
    int32 v1Ind;
    int32 v2Ind;
    int32 edgeInd;
    int32 distance;
} EdgeDist;

class Kruskal
{
public:
    /**
     * Vertices holds data that will be used in the distance metric.
     * Edges holds positions in the vertices list.
     */
    static std::list<int32>* runKruskal(std::vector<FPoint*>& Vertices, std::vector<Edge*>& edges);
    static float metric_dist(FPoint& a, FPoint& b );
};