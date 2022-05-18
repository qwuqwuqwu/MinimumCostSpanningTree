#include <iostream>
#include <deque>
#include <bits/stdc++.h>

#define MAX ( 10 )

using namespace std;

struct Edge {
    int nDest[ 2 ];
    int nWeight;
};

class myComparator
{
public:
    bool operator() ( const Edge& e1, const Edge& e2 ) {
        return e1.nWeight >= e2.nWeight;
    }
};

deque< Edge > g_nEdges;
deque< Edge > g_nMCST;
priority_queue< Edge, vector< Edge >, myComparator > g_MinHeap;
deque< int > g_nAdjList[ MAX ];

deque< int > g_CC[ MAX ];
int g_nCCCount = 0;

int g_nN = 0; // total node counts
int g_nEdgesCount = 0;
int g_nMinCost = 0;
bool g_bCollected[ MAX ] = { false };

void Algorithm( void )
{
    for( int i = 0; i < g_nN; i++ ) {
        g_CC[ i ].push_back( i );
    }
    g_nCCCount = g_nN;

    for( int i = 0; i < g_nEdgesCount; i++ ) {
        g_MinHeap.push( g_nEdges.at( i ) );
    }
    
    // Kruskal's Algorithm
    int nCollectedEdgeCount = 0;
    while( nCollectedEdgeCount != ( g_nN - 1 ) ) {
        // find min edges
        Edge MinEdge = g_MinHeap.top(); g_MinHeap.pop();
        int nIdx1 = MinEdge.nDest[ 0 ];
        int nIdx2 = MinEdge.nDest[ 1 ];
        int nIdx1CCGroup = -1;
        int nIdx2CCGroup = -1;
        bool bFound1 = false;
        bool bFound2 = false;

        int nIdx = -1;

        // determine if it can form a cycle
        // find vertex 1's CC group index
        for( int i = 0; i < g_nCCCount; i++ ) {
            int nVerticesCount = g_CC[ i ].size();
            for( int j = 0; j < nVerticesCount; j++ ) {
                if( g_CC[ i ].at( j ) == nIdx1 ) {
                    nIdx1CCGroup = i;
                    bFound1 = true;
                    break;
                }
            }
            if( bFound1 == true ) {
                break;
            }
        }

        // find vertex 2's CC group index
        for( int i = 0; i < g_nCCCount; i++ ) {
            int nVerticesCount = g_CC[ i ].size();
            for( int j = 0; j < nVerticesCount; j++ ) {
                if( g_CC[ i ].at( j ) == nIdx2 ) {
                    nIdx2CCGroup = i;
                    bFound2 = true;
                    break;
                }
            }
            if( bFound2 == true ) {
                break;
            }
        }

        // if two vertices are already in the same cc group
        // this min edge forms a cycle, do not add it
        if( nIdx1CCGroup == nIdx2CCGroup ) {
            // discard this edge
            continue;
        }
        else {
            // merge two CC group
            // smaller one merge to larger one
            int nSize1 = g_CC[ nIdx1CCGroup ].size();
            int nSize2 = g_CC[ nIdx2CCGroup ].size();
            if( nSize1 > nSize2 ) {
                for( int i = 0; i < nSize2; i++ ) {
                    g_CC[ nIdx1CCGroup ].push_back( g_CC[ nIdx2CCGroup ].back() );
                    g_CC[ nIdx2CCGroup ].pop_back();
                }

            }
            else {
                for( int i = 0; i < nSize1; i++ ) {
                    g_CC[ nIdx2CCGroup ].push_back( g_CC[ nIdx1CCGroup ].back() );
                    g_CC[ nIdx1CCGroup ].pop_back();
                }
            }
        }

        // pick this edge
        g_nMCST.push_back( MinEdge );
        nCollectedEdgeCount++;
        g_nMinCost += MinEdge.nWeight;
        g_bCollected[ nIdx1 ] = true;
        g_bCollected[ nIdx2 ] = true;
    }
}

int main( void ) {
    cin >> g_nN;
    cin >> g_nEdgesCount;
    for( int i = 0; i < g_nEdgesCount; i++ ) {
        int nStartIdx = 0;
        int nEndIdx = 0;
        int nWeight = 0;
        cin >> nStartIdx >> nEndIdx >> nWeight;
        Edge e;
        e.nDest[ 0 ] = nStartIdx;
        e.nDest[ 1 ] = nEndIdx;
        e.nWeight = nWeight;
        g_nEdges.push_back( e );
        g_nAdjList[ nStartIdx ].push_back( nEndIdx );
        g_nAdjList[ nEndIdx ].push_back( nStartIdx );
    }

    // Kruskal's Algorithm
    Algorithm();

    // print minimum cost and path
    cout << "Min cost = " << g_nMinCost << endl;
    int nPathSize = g_nMCST.size();
    cout << "Dest 1, Dest 2, Weight" << endl;
    for( int i = 0; i < nPathSize; i++ ) {
        cout << g_nMCST.at( i ).nDest[ 0 ] << ", " << g_nMCST.at( i ).nDest[ 1 ] << ", " << g_nMCST.at( i ).nWeight << endl;
    }

    return 0;
}