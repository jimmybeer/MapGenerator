#include "Kruskal.h"
#include "UnionFind.h"

#include <algorithm>

using namespace std;

bool sortFunc(EdgeDist& i, EdgeDist& j) { return i.distance < j.distance; }

list<int32>* Kruskal::runKruskal(vector<FPoint*>& vertices, vector<Edge*>& edges)
{
    list<int32>* finalEdge = new list<int32>();
    
    UnionFind forest(vertices.size());
    vector<EdgeDist> edgeDist((edges.size()));
    
    int32 index = 0;
    int32 len = edges.size();
    for(int32 i = 0; i < len; i++)
    {
        int32 u = edges[i]->s;
        int32 v = edges[i]->t;
        
        edgeDist[i].v1Ind = u;
        edgeDist[i].v2Ind = v;
        edgeDist[i].edgeInd = i;
        edgeDist[i].distance = Kruskal::metric_dist(*vertices[u], *vertices[v]);
    }
    
    sort(edgeDist.begin(), edgeDist.end(), sortFunc);
    
    len = edges.size() - 1;
    vector<EdgeDist>::iterator end = edgeDist.end();
    for(vector<EdgeDist>::iterator itr = edgeDist.begin(); itr != end; itr++)
    {
        int32 u = (*(itr)).v1Ind;
        int32 v = (*(itr)).v2Ind;
        
        if(forest.find(u) != forest.find(v)) {
            finalEdge->push_back(u);
            finalEdge->push_back(v);
            forest.link(u, v);
            
            // Remove edge from edge list.
            swap(edges[(*(itr)).edgeInd], edges[len]);
            len--;
        }
    }
    
    // Clean up deleted edges
    edges.erase(edges.begin() + len, edges.end());
    
    return finalEdge;
}

float Kruskal::metric_dist(FPoint& a, FPoint& b )
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx*dx + dy*dy;
}