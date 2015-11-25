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
    int A[10];
} Heap;

Heap heap_init(int max_size);
void heap_insert(int d, Heap H);

Heap heap_init(int max_size) {
    Heap H;
    H.size = max_size;
    H.count = 0;
    
    return H;
};

void heap_insert(int d, Heap H){
    printf("insert: %i\n", d);
    H.A[H.count] = d;
    H.count++;
    
    printf("value at location %i is %i\n", H.count-1, d);
};

void heap_percup(int i, Heap H) {
    
}

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
    
    Heap H = heap_init(1);
    heap_insert(10, H);
    
/* GetVertices();*/
/* GetEdges();*/
/* while (GetRequest()) {RouteOpen(); TourFlag ? Tour() : Dijkstra(0); RouteClose();}*/
/* return(0);*/
}
