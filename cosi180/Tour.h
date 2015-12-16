/*NAME AND DATE GOES HERE.*/
/*Brandeis Map - Tour .h file*/

void add_edges_to_heap(AList *alist, Heap *E, int i);
void spanning_tree_tour(AList *spanning_tree, int start);
int get_inverse_edge(AList * alist, int i);
void Prim();
void Triangle();

AList *alist;       /*Make adjacency graph available to helper methods*/

void add_edges_to_heap(AList *alist, Heap *E, int i) {
    Edge *edge = malloc(sizeof(Edge));
    edge = &alist->A[i];
    if (!edge || edge->vertex_i == -1) {
    } else {
        while (edge) {
            HeapItem *item = malloc(sizeof(HeapItem));
            item->d = EdgeCost(Eindex[i]);
            item->index = edge->edge_i;
            heap_insert(item, E);
            
            if (edge->edge) {
                edge = edge->edge;
            } else {
                edge = NULL;
            }
        }
    }
}

void spanning_tree_tour(AList *spanning_tree, int i) {
    Edge *edge = malloc(sizeof(Edge));
    edge = &spanning_tree->A[i];
    if (!edge || edge->vertex_i == -1) {
    } else {
        while (edge) {
            PrintLeg(Eindex[edge->edge_i]);
            int inverse_i = get_inverse_edge(alist, edge->edge_i);

            if (edge->vertex_i != -1) {
                spanning_tree_tour(spanning_tree, edge->vertex_i);
            }
            edge = edge->edge;
            
            PrintLeg(Eindex[inverse_i]);
        }
    }
}

int get_inverse_edge(AList *alist, int i) {
    Edge *e = &alist->A[Eend[i]];
    while (e != NULL && e->vertex_i != -1) {
        if (e->vertex_i == Estart[i]) {
            return e->edge_i;
        } else {
            e = e->edge;
        }
    }
    return -1;
}

void Prim () {
    int i, v, w;
    
    /*Load edges into adjacency list*/
    alist = alist_init(nV);
    for (i=0; i<nE; i++) {
        alist_add_edge(Estart[i], Eend[i], i, alist);
    }
    
    /*The resulting spanning tree*/
    AList *spanning_tree = alist_init(nV);
    int spanning_tree_edge_count = 0;
    
    /*Load dictionary/heap of traversable edges*/
    Heap *E = heap_init();
    
    /*Initialize edge heap with adjacencies to starting node*/
    add_edges_to_heap(alist, E, Begin);
    
    /*Track which vertices are in tree*/
    int *vertices_in_tree;
    vertices_in_tree = malloc(nV * sizeof(int));
    for (i = 0; i<nV; i++) {
        vertices_in_tree[i] = 0;
    }
    vertices_in_tree[Begin] = 1;
    
    HeapItem *item = (HeapItem *)malloc(sizeof(HeapItem));
    while (spanning_tree_edge_count < nV - 1 && heap_deletemin(E, item) == 1) {
        v = Estart[item->index];
        w = Eend[item->index];
        
        if (vertices_in_tree[w] == 1) {
            continue;
        } else {
            alist_add_edge(v, w, item->index, spanning_tree);
            add_edges_to_heap(alist, E, w);
            spanning_tree_edge_count++;
            vertices_in_tree[w] = 1;
        }
    }
    
    /*Traverse spanning tree*/
    spanning_tree_tour(spanning_tree, Begin);
}

void Triangle() {
    
}

void Tour () {
    char s[MaxString];
    
    printf("1. Prim\n");
    printf("2. Triangle optimization of MST\n");
    printf("Selection: ");
    fflush(stdout); fgets(s,MaxString,stdin);
    if (s[0] == '1') {
        Prim();
    } else if (s[0] == '2') {
        Triangle();
    }
}
