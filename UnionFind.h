#pragma once

class UnionFind
{
    int *id, cnt, *sz;
	
public:
    // Create an empty union find data structure with N isolated sets.
	UnionFind(int N)
	{
	    cnt = N;
		id = new int[N];
		sz = new int[N];
		for(int i = 0; i < N; i++)
		{
		    id[i] = i;
			sz[i] = i;
		}
	}
	
	~UnionFind()
	{
	    delete [] id;
		delete [] sz;
	}
	
	// Return the root id of component corresponding to object p, compress path at same time.
	int find(int p)
	{
	    while(p != id[p])
		{
		    id[p] = id[id[p]]; // path compression
		    p = id[p];
		}
		return p;
	}
	
	void link(int X, int Y)
	{
	    int i = find(X);
		int j = find(Y);
		if(i == j) return;
		
		// make smaller root point to larger one
		if(sz[i] < sz[j]) 
		{
		    id[i] = j;
			sz[j] += sz[i];
		}
		else
		{
		    id[j] = i;
			sz[i] += sz[j];
		}
		cnt--;
	}
	
	// Are objects x and y in the same set?
	bool connected(int X, int Y)
	{
	    return find(X) == find(Y);
	}
	
	// Return the number of disjoint sets.
	int count() 
	{
	    return cnt;
	}
};