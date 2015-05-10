#pragma once
#include <vector>
#include <string>
#include "FPoint.h"
#include "Helper.h"

using namespace std;

/*
 * Edge class. Edges have two vertices, s and t, and two faces,
 * l (left) and r (right). The triangulation representation and
 * the Delaunay triangulation algorithms require edges.
 */
class Edge {
public:
    int32 s, t;
    int32 l, r;
    
    Edge() { s = t = 0; }
    Edge(int32 S, int32 T) : s(S), t(T) {}
};

/*
 * Vector class.  A few elementary vector operations.
 */
class Vector {
public:
    float u, v;
    
    Vector() { u = v = 0.0f; }
    Vector(const FPoint& p1, const FPoint& p2)
    {
        u = p2.X - p1.X;
        v = p2.Y - p1.Y;
    }
    Vector(float U, float V) : u(U), v(V) {}
    
    float dotProduct(const Vector& V)
    {
        return u * V.u + v * V.v;
    }
    
    static float dotProduct(const FPoint& p1, const FPoint& p2, const FPoint& p3)
    {
        float u1, v1, u2, v2;
        
        u1 =  p2.X - p1.X;
        v1 =  p2.Y - p1.Y;
        u2 =  p3.X - p1.X;
        v2 =  p3.Y - p1.Y;
        
        return u1 * u2 + v1 * v2;
    }
    
    float crossProduct(const Vector& V)
    {
        return u * V.v - v * V.u;
    }
    
    static float crossProduct(const FPoint& p1, const FPoint& p2, const FPoint& p3)
    {
        float u1, v1, u2, v2;
        
        u1 =  p2.X - p1.X;
        v1 =  p2.Y - p1.Y;
        u2 =  p3.X - p1.X;
        v2 =  p3.Y - p1.Y;
        
        return u1 * v2 - v1 * u2;
    }
    
    void setFPoints(const FPoint& p1, const FPoint& p2) {
        u = p2.X - p1.X;
        v = p2.Y - p1.Y;
    }
};

/*
 * Circle class. Circles are fundamental to computation of Delaunay
 * triangulations.  In particular, an operation which computes a
 * circle defined by three points is required.
 */
class Circle {
public:
    FPoint c;
    float r;
    
    Circle() : r(0.0f) {}
    Circle(const FPoint& C, float R) : c(C), r(R) {}
    //public FPoint center() { return c; }
    //public float radius() { return r; }
    void set(const FPoint& C, float R)
    {
        c = C;
        r = R;
    }
    
    /*
     * Tests if a point lies inside the circle instance.
     */
    bool inside(const FPoint& p)
    {
        if (c.distanceSq(p) < (r * r))
            return true;
        else
            return false;
    }
    
    /*
     * Compute the circle defined by three points (circumcircle).
     */
    void circumCircle(const FPoint& p1, const FPoint& p2, const FPoint& p3)
    {
        float cp;
        
        cp = Vector::crossProduct(p1, p2, p3);
        if (cp != 0.0f)
        {
            float p1Sq, p2Sq, p3Sq;
            float num;
            float cx, cy;
            
            p1Sq = p1.X * p1.X + p1.Y * p1.Y;
            p2Sq = p2.X * p2.X + p2.Y * p2.Y;
            p3Sq = p3.X * p3.X + p3.Y * p3.Y;
            num = p1Sq*(p2.Y - p3.Y) + p2Sq*(p3.Y - p1.Y) + p3Sq*(p1.Y - p2.Y);
            cx = num / (2.0f * cp);
            num = p1Sq*(p3.X - p2.X) + p2Sq*(p1.X - p3.X) + p3Sq*(p2.X - p1.X);
            cy = num / (2.0f * cp);
            
            c.X = cx;
            c.Y = cy;
        }
        
        // Radius
        r = c.distance(p1);
    }
};



/*
 * Triangulation class.  A triangulation is represented as a set of
 * points and the edges which form the triangulation.
 */
class Triangulation {
public:
    static const int Undefined;
    static const int Universe;
    int nPoints;
    vector<FPoint*> point;
    int nEdges;
    int maxEdges;
    vector<Edge*> edge;
    
    Triangulation(int nPoints);
    ~Triangulation();
    
    /*
     * Sets the number of points in the triangulation.  Reuses already
     * allocated points and edges.
     */
    void setNPoints(int nPoints);
    
    /*
     * Generates a set of random points to triangulate.
     */
    void randomPoints(int maxX, int maxY);
    
    /*
     * Copies a set of points.
     */
    void copyPoints(const Triangulation& t);
    
    void addTriangle(int s, int t, int u);
    
    int addEdge(int s, int t);
    
    /*
     * Adds an edge to the triangulation. Store edges with lowest
     * vertex first (easier to debug and makes no other difference).
     */
    int addEdge(int s, int t, int l, int r);
    
    int findEdge(int s, int t);
    
    /*
     * Update the left face of an edge.
     */
    void updateLeftFace(int eI, int s, int t, int f);
    
    void print();
    void printPoints();
    void printEdges();
};

/*
 * TriangulationAlgorithm class.  Absract.  Superclass for
 * actual algorithms.  Has several abstract function members -
 * including the triangulation member which actually computes
 * the triangulation.
 */
class TriangulationAlgorithm
{
public:
    string algName;
    
    TriangulationAlgorithm(string name)
    {
        algName = name;
    }
    
    virtual void reset()
    {
    }
    
    virtual void triangulate(Triangulation& t) = 0;
};

/*
 * QuadraticAlgorithm class.  O(n^2) algorithm.
 */
class QuadraticAlgorithm : public TriangulationAlgorithm
{
private:
    int s, t, u, bP;
    Circle bC;
    int nFaces;
    
public:
    QuadraticAlgorithm() : TriangulationAlgorithm("O(n^2)") {}
    
    void reset() {
        nFaces = 0;
        TriangulationAlgorithm::reset();
    }
    
    void triangulate(Triangulation& tri);
    
    // Find the two closest points.
    void findClosestNeighbours(vector<FPoint*>& p, int nPoints, int& u, int& v); 
    
    /* 
     * Complete a facet by looking for the circle free point to the left
     * of the edge "e_i".  Add the facet to the triangulation.
     *
     * This function is a bit long and may be better split.
     */
    void completeFacet(int eI, Triangulation& tri, int nFaces);
};