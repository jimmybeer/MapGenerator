#include "Delaunay.h"
#include <cstdlib>
#include <ctime>
#include <cfloat>
#include <iostream>

const int Triangulation::Undefined = -1;
const int Triangulation::Universe = 0;

Triangulation::Triangulation(int nPoints)
{
    // Allocate points.
    this->nPoints = nPoints;
    point.resize(nPoints);
    for (int i = 0; i < nPoints; i++)
        point[i] = new FPoint();
    
    // Allocate edges.
    maxEdges = 3 * nPoints - 6;	// Max number of edges.
    edge.resize(maxEdges);
    for (int i = 0; i < maxEdges; i++)
        edge[i] = new Edge();
    nEdges = 0;
}

Triangulation::~Triangulation()
{
    deleteContainerContents(edge);
    deleteContainerContents(point);
}

void Triangulation::setNPoints(int nPoints)
{
    deleteContainerContents(edge);
    deleteContainerContents(point);
    // Allocate points.
    this->nPoints = nPoints;
    point.resize(nPoints);
    for (int i = 0; i < nPoints; i++)
        point[i] = new FPoint();
    
    // Allocate edges.
    maxEdges = 3 * nPoints - 6;	// Max number of edges.
    edge.resize(maxEdges);
    for (int i = 0; i < maxEdges; i++)
        edge[i] = new Edge();
    nEdges = 0;
}

void Triangulation::randomPoints(int maxX, int maxY) {
    srand(time(NULL));
    
    for (int i = 0; i < nPoints; i++)
    {
        point[i]->X = (float)(rand() % maxX);
        point[i]->Y = (float)(rand() % maxY);
    }
    nEdges = 0;
}

void Triangulation::copyPoints(const Triangulation& t)
{
    deleteContainerContents(edge);
    deleteContainerContents(point);
    // Allocate points.
    this->nPoints = t.nPoints;
    point.resize(nPoints);
    for (int i = 0; i < nPoints; i++)
        point[i] = t.point[i];
    
    // Allocate edges.
    maxEdges = 3 * nPoints - 6;	// Max number of edges.
    edge.resize(maxEdges);
    for (int i = 0; i < maxEdges; i++)
        edge[i] = new Edge();
    nEdges = 0;
}

void Triangulation::addTriangle(int s, int t, int u)
{
    this->addEdge(s, t);
    this->addEdge(t, u);
    this->addEdge(u, s);
}

int Triangulation::addEdge(int s, int t)
{
    return this->addEdge(s, t, Triangulation::Undefined, Triangulation::Undefined);
}

int Triangulation::addEdge(int s, int t, int l, int r)
{
    int e;
    
    // Add edge if not already in the triangulation.
    e = this->findEdge(s, t);
    if (e == Triangulation::Undefined)
    {
        if (s < t)
        {
            edge[nEdges]->s = s;
            edge[nEdges]->t = t;
            edge[nEdges]->l = l;
            edge[nEdges]->r = r;
            return nEdges++;
        }
        else
        {
            edge[nEdges]->s = t;
            edge[nEdges]->t = s;
            edge[nEdges]->l = r;
            edge[nEdges]->r = l;
            return nEdges++;
        }
    }
    else
        return Triangulation::Undefined;
}

int Triangulation::findEdge(int s, int t)
{
    bool edgeExists = false;
    int i;
    
    for (i = 0; i < nEdges; i++)
    {
        if ((edge[i]->s == s && edge[i]->t == t) ||
            (edge[i]->s == t && edge[i]->t == s))
        {
            edgeExists = true;
            break;
        }
    }
    
    if (edgeExists)
        return i;
    else
        return Triangulation::Undefined;
}

void Triangulation::updateLeftFace(int eI, int s, int t, int f)
{
    if (!((edge[eI]->s == s && edge[eI]->t == t) ||
          (edge[eI]->s == t && edge[eI]->t == s)))
        cout << "updateLeftFace: adj. matrix and edge table mismatch\n";
    if (edge[eI]->s == s && edge[eI]->l == Triangulation::Undefined)
        edge[eI]->l = f;
    else if (edge[eI]->t == s && edge[eI]->r == Triangulation::Undefined)
        edge[eI]->r = f;
    else
        cout << "updateLeftFace: attempt to overwrite edge info\n";
}

void Triangulation::print()
{
    this->printPoints();
    this->printEdges();
}

void Triangulation::printPoints()
{
    cout << "Points:\n";
    for (int i = 0; i < nPoints; i++)
    {
        cout << i << " " << point[i]->X << " " << point[i]->Y << "\n";
    }
}

void Triangulation::printEdges()
{
    cout << "Edges:\n";
    for (int i = 0; i < nEdges; i++)
    {
        cout << i << " " << edge[i]->s << " " << edge[i]->t << "\n";
    }
}

void QuadraticAlgorithm::triangulate(Triangulation& tri)
{
    int seedEdge, currentEdge;
    int nFaces;
    int s, t;
    
    // Initialise.
    nFaces = 0;
    s = 0;
    t = 0;
    
    // Find closest neighbours and add edge to triangulation.
    this->findClosestNeighbours(tri.point, tri.nPoints, s, t);
    
    // Create seed edge and add it to the triangulation.
    seedEdge = tri.addEdge(s, t, Triangulation::Undefined, Triangulation::Undefined);
    
    currentEdge = 0;
    while (currentEdge < tri.nEdges)
    {
        if (tri.edge[currentEdge]->l == Triangulation::Undefined)
        {
            completeFacet(currentEdge, tri, nFaces);
        }
        if (tri.edge[currentEdge]->r == Triangulation::Undefined)
        {
            completeFacet(currentEdge, tri, nFaces);
        }
        currentEdge++;
    }
}

// Find the two closest points.
void QuadraticAlgorithm::findClosestNeighbours(vector<FPoint*>& p, int nPoints, int& u, int& v)
{
    int i, j;
    float d, min;
    int s, t;
    
    s = t = 0;
    min = FLT_MAX;
    for (i = 0; i < nPoints-1; i++)
    {
        for (j = i+1; j < nPoints; j++)
        {
            d = p[i]->distanceSq(*p[j]);
            if (d < min)
            {
                s = i;
                t = j;
                min = d;
            }
        }
    }
    u = s;
    v = t;
}

/*
 * Complete a facet by looking for the circle free point to the left
 * of the edge "e_i".  Add the facet to the triangulation.
 *
 * This function is a bit long and may be better split.
 */
void QuadraticAlgorithm::completeFacet(int eI, Triangulation& tri, int nFaces)
{
    float cP;
    
    // Cache s and t.
    if (tri.edge[eI]->l == Triangulation::Undefined)
    {
        s = tri.edge[eI]->s;
        t = tri.edge[eI]->t;
    }
    else if (tri.edge[eI]->r == Triangulation::Undefined)
    {
        s = tri.edge[eI]->t;
        t = tri.edge[eI]->s;
    }
    else
    {
        // Edge already completed.
        return;
    }
    
    // Find a point on left of edge.
    for (u = 0; u < tri.nPoints; u++)
    {
        if (u == s || u == t)
            continue;
        if (Vector::crossProduct(*tri.point[s], *tri.point[t], *tri.point[u]) > 0.0f)
            break;
    }
    
    // Find best point on left of edge.
    bP = u;
    if (bP < tri.nPoints)
    {
        bC.circumCircle(*tri.point[s], *tri.point[t], *tri.point[bP]);
        
        for (u = bP+1; u < tri.nPoints; u++)
        {
            if (u == s || u == t)
            {
                continue;
            }
            
            cP = Vector::crossProduct(*tri.point[s], *tri.point[t], *tri.point[u]);
            
            if (cP > 0.0f)
            {
                if (bC.inside(*tri.point[u]))
                {
                    bP = u;
                    bC.circumCircle(*tri.point[s], *tri.point[t], *tri.point[u]);
                }
            }
        }
    }
    
    // Add new triangle or update edge info if s-t is on hull.
    if (bP < tri.nPoints)
    {
        // Update face information of edge being completed. 
        tri.updateLeftFace(eI, s, t, nFaces);
        nFaces++;
        
        // Add new edge or update face info of old edge. 
        eI = tri.findEdge(bP, s);
        if (eI == Triangulation::Undefined)
        {
            // New edge. 
            eI = tri.addEdge(bP, s, nFaces, Triangulation::Undefined);
        }
        else
        {
            // Old edge. 
            tri.updateLeftFace(eI, bP, s, nFaces);
        }
        
        // Add new edge or update face info of old edge. 
        eI = tri.findEdge(t, bP);
        if (eI == Triangulation::Undefined)
        {
            // New edge.  
            eI = tri.addEdge(t, bP, nFaces, Triangulation::Undefined);
        }
        else
        {
            // Old edge.  
            tri.updateLeftFace(eI, t, bP, nFaces); 
        }
    } 
    else
    {
        tri.updateLeftFace(eI, s, t, Triangulation::Universe); 
    }
}
