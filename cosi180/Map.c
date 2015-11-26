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



/***************************************************************************************/
/*HEAP DATA STRUCTURE                                                                  */
/***************************************************************************************/

typedef struct {
    unsigned int size;
    unsigned int count;
    unsigned int *A;
} Heap;

Heap* heap_init(int max_size);
void heap_insert(int d, Heap *H);
void heap_percup(unsigned int i, Heap *H);
unsigned int heap_parent(int i, Heap *H);
void heap_swap(int i, int j, Heap *H);
void heap_grow(Heap *H);
void heap_print(Heap *H);

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

void heap_percup(unsigned int i, Heap *H) {
    unsigned int parent_i = heap_parent(i, H);
    while ((*H).A[parent_i] > (*H).A[i]) {
        heap_swap(i, parent_i, H);
        i = parent_i;
        parent_i = heap_parent(i, H);
    }
}

unsigned int heap_parent(int i, Heap *H) {
    if (i == 0) {
        return i;
    } else {
        return (unsigned int)(round((float)i/2.0f) - 1);
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
    
    Heap *H = heap_init(1);
    
    heap_insert(10, H);
    heap_insert(8, H);
    heap_insert(11, H);
    heap_insert(24, H);
    heap_insert(14, H);
    heap_insert(16, H);
    heap_insert(2, H);
    heap_insert(1, H);
    
    
/* GetVertices();*/
/* GetEdges();*/
/* while (GetRequest()) {RouteOpen(); TourFlag ? Tour() : Dijkstra(0); RouteClose();}*/
/* return(0);*/
}
