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
    int vertex_i;           /*destination vertex*/
    int edge_i;             /*edge index*/
    struct Edge *edge;
} Edge;

typedef struct {
    int size;
    Edge *A;
} AList;

AList* alist_init(int vertex_count);
void alist_add_edge(int i, int j, int edge_i, AList *alist);
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

void alist_add_edge(int i, int j, int edge_i, AList *alist) {
    printf("Adding edge %i -> %i\n", i, j);
    
    Edge *outgoing_edge = &(*alist).A[i];
    while ((*outgoing_edge).edge) {
        outgoing_edge = outgoing_edge->edge;
    }
    
    if (outgoing_edge->vertex_i == -1) {
        outgoing_edge->vertex_i = j;
    } else {
        Edge *edge = malloc(sizeof(Edge));
        edge->vertex_i = j;
        edge->edge_i = edge_i;
        edge->edge = NULL;
        outgoing_edge->edge = edge;
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
    int d;                          /*The value of the heap item to partially sort on*/
    int v_index;                    /*A reference to the index of the vertex stored in heap item*/
    int incoming_edge_i;
    int previous_vertex_i;
    unsigned int marked;
} HeapItem;

typedef struct {
    unsigned int size;
    unsigned int count;
    HeapItem *A;            /*The heap array*/
    int *D;                 /*An array of pointers mapping v_index to the HeapItem*/
} Heap;

Heap* heap_init();
void heap_insert(HeapItem *item, Heap *H);
void heap_percup(unsigned int i, Heap *H);
void heap_percdown(unsigned int i, Heap *H);
unsigned int heap_parent(unsigned int i, Heap *H);
unsigned int heap_child(unsigned int i, Heap *H);
void heap_swap(int i, int j, Heap *H);
void heap_grow(Heap *H);
void heap_print(Heap *H);
unsigned int heap_deletemin(Heap *H, HeapItem *min);
void heap_item_copy(HeapItem *a, HeapItem *b);

Heap* heap_init() {
    Heap *H = malloc(sizeof(Heap));
    H->size = 0;
    H->A = (HeapItem *)malloc(H->size * sizeof(HeapItem));
    H->D = malloc(H->size * sizeof(int));
    H->count = 0;
    
    return H;
};

void heap_item_copy(HeapItem *a, HeapItem *b) {
    a->d = b->d;
    a->v_index = b->v_index;
    a->marked = b->marked;
    a->incoming_edge_i = b->incoming_edge_i;
    a->previous_vertex_i = b->previous_vertex_i;
}

void heap_insert(HeapItem *item, Heap *H) {
    printf("\n\ninserting %d\n", item->d);
    if (H->count >= H->size) {
        heap_grow(H);
    }
    
    H->A[H->count] = *item;
    H->D[item->v_index] = H->count;
    heap_percup(H->count, H);
    H->count = H->count + 1;
    
    /*heap_print(H);*/
};

unsigned int heap_deletemin(Heap *H, HeapItem *min) {
    printf("\ndeletemin\n");
    
    if (H->count > 0) {
        heap_swap(0, H->count-1, H);
        H->count = H->count-1;
        
        min->d = H->A[H->count].d;
        min->v_index = H->A[H->count].v_index;
        min->marked = H->A[H->count].marked;
        min->incoming_edge_i = H->A[H->count].incoming_edge_i;
        min->previous_vertex_i = H->A[H->count].previous_vertex_i;
        
        H->A[H->count].d = -1;
        H->A[H->count].v_index = -1;
        H->A[H->count].marked = 0;
        
        heap_percdown(0, H);
        
        return 1;
    } else {
        return 0;
    }
};

void heap_percup(unsigned int i, Heap *H) {
    unsigned int parent_i = heap_parent(i, H);
    while (H->A[parent_i].d > H->A[i].d) {
        heap_swap(i, parent_i, H);
        i = parent_i;
        parent_i = heap_parent(i, H);
    }
}

void heap_percdown(unsigned int i, Heap *H) {
    printf("\nheap_percdown\n");
    unsigned int child_i = heap_child(i, H);
    while (H->A[child_i].d < H->A[i].d) {
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
    
    if (H->A[lchild_index].d < (*H).A[rchild_index].d) {
        child_index = lchild_index;
    } else {
        child_index = rchild_index;
    }
    
    printf("\nchild of %d is %d\n", i, child_index);
    if (child_index >= H->count) {
        return i;
    } else {
        return child_index;
    };
};

void heap_swap(int i, int j, Heap *H) {
    printf("swap: %2d and %2d\n", H->A[i].d, H->A[j].d);
    
    HeapItem *tmp = malloc(sizeof(HeapItem));
    heap_item_copy(tmp, &H->A[i]);
    H->A[i] = H->A[j];
    H->A[j] = *tmp;
    
    H->D[H->A[i].v_index] = i;
    H->D[H->A[j].v_index] = j;
};

void heap_grow(Heap *H) {
    printf("resize\n");
    H->size = (H->size + 1) * 2;
    HeapItem *newA;
    int *newD;
    newA = (HeapItem *)malloc(H->size * sizeof(HeapItem));
    newD = malloc(H->size * sizeof(int));
    
    int i;
    for (i=0; i<H->count; i++) {
        newA[i] = H->A[i];
        newD[H->A[i].v_index] = i;
    };
    
    for (i=H->count; i<H->size; i++) {
        HeapItem *item = malloc(sizeof(HeapItem));
        item->d = -1;
        item->v_index = -1;
        item->marked = 0;
        item->previous_vertex_i = -1;
        
        newA[i] = *item;
    };
    
    free(H->A);
    free(H->D);
    H->A = newA;
    H->D = newD;
};

void heap_print(Heap *H) {
    printf("\n");
    for (int i=0; i<(*H).size; i++) {
        printf("at %i: %2d\n", i, (*H).A[i].d);
    };
};

/***************************************************************************************/
/*Dijkstra Algorithm                                                                   */
/*DijkstraFlag=1 to supress output when Dijkstra is called from tour code.)            */
/***************************************************************************************/



void Dijkstra(int DijkstraFlag) {
    int i;
    
    /*Load edges into adjacency list*/
    AList *alist = alist_init(nV);
    for (i=0; i<nE; i++) {
        alist_add_edge(Estart[i], Eend[i], i, alist);
    }
    alist_print(alist);
    
    /*Load dictionary of unmarked vertices*/
    Heap *D = heap_init();
    for (i=0; i<nV; i++) {
        int init_cost = (i == Begin) ? 0 : 1000000;
        HeapItem item = {init_cost, i, -1, -1, 0};
        heap_insert(&item, D);
    }
    
    printf("\nBegin: %i\nFinish: %i\n", Begin, Finish);
    HeapItem *marked_vertices = malloc(nV * sizeof(HeapItem));
    for (i=0; i<D->size; i++) {
        HeapItem *item = malloc(sizeof(HeapItem));
        item->d = -1;
        item->v_index = -1;
        item->marked = 0;
        
        marked_vertices[i] = *item;
    };
    
    HeapItem *v = (HeapItem *)malloc(sizeof(HeapItem));
    HeapItem *w;
    while (heap_deletemin(D, v) > 0) {
        printf("min: %d\n", v->d);
        v->marked = 1;
        Edge *edge = &alist->A[v->v_index];
        
        if (!edge || edge->vertex_i == -1) {
        } else {
            while (edge) {
                printf(" -> %i", edge->vertex_i);
                printf("(%i)", EdgeCost(edge->edge_i));
                
                int w_i = edge->vertex_i;
                w = &D->A[D->D[w_i]];
                if (v->d + EdgeCost(edge->edge_i) < w->d) {
                    w->d = v->d + EdgeCost(edge->edge_i);
                    w->incoming_edge_i = edge->edge_i;
                    w->previous_vertex_i = v->v_index;
                    heap_percup(D->D[w_i], D);
                }
                
                HeapItem *tmp = malloc(sizeof(HeapItem));
                heap_item_copy(tmp, v);
                marked_vertices[v->v_index] = *tmp;
                
                if (edge->edge) {
                    edge = edge->edge;
                } else {
                    edge = NULL;
                }
            }
        }
    }
    
    /*for (int m=0; m<nV; m++) {
        printf("\ncost to get to %s (%i) is %i", Vname[m], m, marked_vertices[m].d);
        printf("\n  best arrived at by: %i", marked_vertices[m].back_vertex_i);
    }*/
    
    int edges_indices[151];
    int count = 0;
    v = &marked_vertices[Finish];
    w = &marked_vertices[v->previous_vertex_i];
    while (w) {
        edges_indices[count] = v->incoming_edge_i;
        v = w;
        if (w->previous_vertex_i == -1) {
            w = NULL;
        } else {
            w = &marked_vertices[w->previous_vertex_i];
        }
        count++;
    }
    
    for (i=count-1; i>=0; i--) {
        printf("\nfollow edge %i\n", edges_indices[i]);
        PrintLeg(Eindex[edges_indices[i]]);
    }
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
    
    HeapItem item1 = {10, 0};
    heap_insert(&item1, H);
    
    HeapItem item2 = {8, 1};
    heap_insert(&item2, H);
    
    HeapItem item3 = {11, 1};
    heap_insert(&item3, H);
    
    HeapItem item4 = {24, 1};
    heap_insert(&item4, H);
    
    HeapItem item5 = {7, 1};
    heap_insert(&item5, H);
    
    HeapItem item6 = {111, 1};
    heap_insert(&item6, H);
    
    HeapItem item7 = {4, 1};
    heap_insert(&item7, H);
    
    
    heap_print(H);
    printf("min: %d\n", heap_deletemin(H));
    heap_print(H);
    printf("min: %d\n", heap_deletemin(H));
    heap_print(H);*/
    
    /*GetVertices();
    GetEdges();
    Begin = 99;
    Finish = 62;
    Dijkstra(0);*/
    
    GetVertices();
    GetEdges();
    while (GetRequest()) {RouteOpen(); TourFlag ? Tour() : Dijkstra(0); RouteClose();}
    return(0);
}
