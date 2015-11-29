/*NAME AND DATE GOES HERE.*/
/*Brandeis Map*/

/*Standard system stuff - these are the ONLY ones that may be used.*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*Path to the map folder; may need to change this.*/
#include "MapPATH.h"

/*MAY NOT be modified, but should be viewed.*/
#include "Map.h" /*Map data parameters, structures, and functions.*/

/*MAY NOT be modified, and there is no need view them:*/
#include "MapData.h"   /*Functions to input the map data.*/
#include "MapInput.h"  /*Functions to get user input.*/
#include "MapOutput.h" /*Functions to produce output.*/

/*Use this to get the time to travel along an edge.*/
#define EdgeCost(X) ( (TimeFlag) ? Time(X) : Elength[X] )

/*Use this to print a leg of a route or tour.*/
void PrintLeg(int edge);



/***************************************************************************************/
/*GRAPH ADJACENCY LIST DATA STRUCTURE                                                  */
/***************************************************************************************/

typedef struct Edge {
    int vertex_i;
    struct Edge *edge;
} Edge;

typedef struct {
    int size;
    Edge *A;
} AList;

AList* alist_init(int vertex_count);
void alist_add_edge(int i, int j, AList *alist);
void _alist_add_edge(int i, int j, AList *alist);
void alist_print(AList *alist);
void edge_print(Edge *edge);

AList* alist_init(int vertex_count) {
    AList *alist = malloc(sizeof(AList));
    (*alist).size = vertex_count;
    (*alist).A = malloc((*alist).size * sizeof(Edge));
    
    
    for (int i=0; i < (*alist).size; i++) {
        Edge edge;
        edge.vertex_i = -1;
        edge.edge = NULL;
        (*alist).A[i] = edge;
    }
    
    return alist;
};

void alist_add_edge(int i, int j, AList *alist) {
    _alist_add_edge(i, j, alist);
    _alist_add_edge(j, i, alist);
};

void _alist_add_edge(int i, int j, AList *alist) {
    printf("Adding edge %i -> %i\n", i, j);
    
    Edge *outgoing_edge = &(*alist).A[i];
    while ((*outgoing_edge).edge) {
        outgoing_edge = (*outgoing_edge).edge;
    }
    
    if ((*outgoing_edge).vertex_i == -1) {
        (*outgoing_edge).vertex_i = j;
    } else {
        Edge *edge = malloc(sizeof(Edge));
        (*edge).vertex_i = j;
        (*edge).edge = NULL;
        (*outgoing_edge).edge = edge;
    }
};

void alist_print(AList *alist) {
    for (int i = 0; i < (*alist).size; i++) {
        printf("index: %i", i);
        edge_print(&(*alist).A[i]);
        printf("\n");
    }
};

void edge_print(Edge *edge) {
    if (!edge || (*edge).vertex_i == -1) {
        return;
    } else {
        while (edge) {
            printf("->%i", (*edge).vertex_i);
            
            if ((*edge).edge) {
                edge = (*edge).edge;
            } else {
                edge = NULL;
            }
        }
    }
};

/***************************************************************************************/
/*HEAP DATA STRUCTURE                                                                  */
/***************************************************************************************/

typedef struct {
    unsigned int size;
    unsigned int count;
    unsigned int *A;
} Heap;

Heap* heap_init();
void heap_insert(int d, Heap *H);
void heap_percup(unsigned int i, Heap *H);
void heap_percdown(unsigned int i, Heap *H);
unsigned int heap_parent(unsigned int i, Heap *H);
unsigned int heap_child(unsigned int i, Heap *H);
void heap_swap(int i, int j, Heap *H);
void heap_grow(Heap *H);
void heap_print(Heap *H);
unsigned int heap_deletemin(Heap *H);

Heap* heap_init() {
    Heap *H = malloc(sizeof(Heap));
    (*H).size = 2;
    (*H).A = (unsigned int *)malloc((*H).size * sizeof(unsigned int));
    (*H).count = 0;
    
    return H;
};

void heap_insert(int d, Heap *H) {
    printf("\n\ninserting %d\n", d);
    if ((*H).count >= (*H).size) {
        heap_grow(H);
    }
    
    (*H).A[(*H).count] = d;
    heap_percup((*H).count, H);
    (*H).count = (*H).count + 1;
    
    heap_print(H);
};

unsigned int heap_deletemin(Heap *H) {
    printf("\ndeletemin\n");
    if ((*H).count > 0) {
        heap_swap(0, (*H).count-1, H);
        (*H).count = (*H).count-1;
        unsigned int min = (*H).A[(*H).count];
        (*H).A[(*H).count] = 0;
        heap_percdown(0, H);
        
        return min;
    } else {
        return -1;
    }
};

void heap_percup(unsigned int i, Heap *H) {
    unsigned int parent_i = heap_parent(i, H);
    while ((*H).A[parent_i] > (*H).A[i]) {
        heap_swap(i, parent_i, H);
        i = parent_i;
        parent_i = heap_parent(i, H);
    }
}

void heap_percdown(unsigned int i, Heap *H) {
    printf("\nheap_percdown\n");
    unsigned int child_i = heap_child(i, H);
    while ((*H).A[child_i] < (*H).A[i]) {
        heap_swap(i, child_i, H);
        i = child_i;
        child_i = heap_child(i, H);
    }
}

unsigned int heap_parent(unsigned int i, Heap *H) {
    if (i == 0) {
        return i;
    } else {
        return (unsigned int)(round((float)i/2.0f) - 1);
    };
};

unsigned int heap_child(unsigned int i, Heap *H) {
    unsigned int lchild_index = (unsigned int)(round((float)i*2.0f) + 1);
    unsigned int rchild_index = (unsigned int)(round((float)i*2.0f) + 2);
    unsigned int child_index;
    
    if ((*H).A[lchild_index] < (*H).A[rchild_index]) {
        child_index = lchild_index;
    } else {
        child_index = rchild_index;
    }
    
    printf("\nchild of %d is %d\n", i, child_index);
    if (child_index >= (*H).count) {
        return i;
    } else {
        return child_index;
    };
};

void heap_swap(int i, int j, Heap *H) {
    printf("swap: %2d and %2d\n", (*H).A[i], (*H).A[j]);
    int tmp = (*H).A[i];
    (*H).A[i] = (*H).A[j];
    (*H).A[j] = tmp;
};

void heap_grow(Heap *H) {
    printf("resize\n");
    (*H).size = (*H).size * 2;
    unsigned int *tmpArray;
    tmpArray = (unsigned int *)malloc((*H).size * sizeof(unsigned int));
    
    for (int i=0; i<(*H).count; i++) {
        tmpArray[i] = (*H).A[i];
    };
    
    free((*H).A);
    (*H).A = tmpArray;
};

void heap_print(Heap *H) {
    printf("\n");
    for (int i=0; i<(*H).size; i++) {
        printf("at %i: %2d\n", i, (*H).A[i]);
    };
};

/***************************************************************************************/
/*Dijkstra Algorithm                                                                   */
/*DijkstraFlag=1 to supress output when Dijkstra is called from tour code.)            */
/***************************************************************************************/

void Dijkstra(int DijkstraFlag) {
}


/***************************************************************************************/
/*CAMPUS TOUR                                                                          */
/***************************************************************************************/
#include "Tour.h"


/***************************************************************************************/
/*MAIN PROGRAM (don't modify)                                                          */
/***************************************************************************************/
int main() {
    
    /*Heap *H = heap_init();
    
    heap_insert(10, H);
    heap_insert(8, H);
    heap_insert(11, H);
    heap_insert(24, H);
    heap_insert(7, H);
    heap_insert(111, H);
    heap_insert(4, H);
    
    
    heap_print(H);
    printf("min: %d\n", heap_deletemin(H));
    heap_print(H);
    printf("min: %d\n", heap_deletemin(H));
    heap_print(H);*/
    
    AList *alist = alist_init(10);
    alist_add_edge(1, 2, alist);
    alist_add_edge(1, 4, alist);
    alist_print(alist);
    
/* GetVertices();*/
/* GetEdges();*/
/* while (GetRequest()) {RouteOpen(); TourFlag ? Tour() : Dijkstra(0); RouteClose();}*/
/* return(0);*/
}
