#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


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
    printf("Adding edge %i -> %i\n", i, j);
    Edge edge;
    edge.vertex_i = j;
    edge.edge = NULL;
    
    if ((*alist).A[i].edge != NULL) {
        printf("edge not null");
    } else {
        printf("edge null");
    }
    
    (*alist).A[i] = edge;
    (*alist).A[i].edge = NULL;
    
    if ((*alist).A[i].edge != NULL) {
        printf("edge not null");
    } else {
        printf("edge null");
    }
};

void alist_print(AList *alist) {
    for (int i = 0; i < (*alist).size; i++) {
        printf("index: %i\n", i);
        
        /*if (&(*alist).A[i]) {
            printf("ep not null");
        } else {
            printf("ep null");
        }*/
        
        edge_print(&(*alist).A[i]);
    }
};

void edge_print(Edge *edge) {
    printf("  edges:");
    
    while (edge) {
        if (edge && (*edge).vertex_i != -1) {
            printf("->%i", (*edge).vertex_i);
            
            if ((*edge).edge) {
                edge = (*edge).edge;
            } else {
                edge = NULL;
            }
        } else {
            edge = NULL;
        }
    }
    printf("\n");
};

int main() {
    
    AList *alist = alist_init(10);
    alist_add_edge(1, 2, alist);
    alist_print(alist);
    
}
