// Ali Haktan SIGIN - 21070001004
#include <stdio.h>
#include <stdlib.h>

// Node structure
struct Node {
    int vertex;
    int weight;
    struct Node* next;
};

// Graph structure
struct Graph {
    int numVertices;
    struct Node** adjacencyList;
};

// Edge structure
struct Edge {
    int src, dest, weight;
};

// Swap function (will be used in Heapify & Heapsort functions)
void swap(struct Edge* a, struct Edge* b) {
    struct Edge t = *a;
    *a = *b;
    *b = t;
}

// Heapify edges according to their weight
void heapify(struct Edge arr[], int n, int i) {
    int largest = i;
    int leftChild = (2 * i) + 1;
    int rightChild = (2 * i) + 2;

    if (leftChild < n && arr[leftChild].weight > arr[largest].weight)
        largest = leftChild;

    if (rightChild < n && arr[rightChild].weight > arr[largest].weight)
        largest = rightChild;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// Heapsort function
void heapSort(struct Edge arr[], int n) {
    int i;
    for (i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

// Function to create a node
struct Node* createNode(int v, int weight) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = V;

    // Create an array of adjacency lists
    graph->adjacencyList = (struct Node**)malloc(V * sizeof(struct Node*));

    // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->adjacencyList[i] = NULL;

    return graph;
}

// Function to add an edge between source and destination vertices with a weight
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    // Add an edge from src to dest
    struct Node* newNode = createNode(dest, weight);
    newNode->next = graph->adjacencyList[src];
    graph->adjacencyList[src] = newNode;

    // As we are working with an undirected graph, we should also add edges from dest to src with the same weights.
    newNode = createNode(src, weight);
    newNode->next = graph->adjacencyList[dest];
    graph->adjacencyList[dest] = newNode;
}

// Function to find the subset of an element i (with path compression)
int find(int parent[], int i) {
    if (parent[i] != i)
        parent[i] = find(parent, parent[i]);
    return parent[i];
}

// Function that performs union of two subsets (by rank)
void Union(int parent[], int rank[], int x, int y) {
    int xRoot = find(parent, x);
    int yRoot = find(parent, y);

    if (rank[xRoot] < rank[yRoot])
        parent[xRoot] = yRoot;
    else if (rank[xRoot] > rank[yRoot])
        parent[yRoot] = xRoot;
    else {
        parent[yRoot] = xRoot;
        rank[xRoot]++;
    }
}

// Kruskal's algorithm using union by rank
void kruskalMST(struct Graph* graph) {
    int numVertices = graph->numVertices;
    int numEdges = numVertices * (numVertices - 1) / 2; // Maximum possible edges in a complete graph

    struct Edge* edges = (struct Edge*)malloc(numEdges * sizeof(struct Edge));

    int e = 0;
    for (int v = 0; v < numVertices; ++v) {
        struct Node* temp = graph->adjacencyList[v];
        while (temp) {
            if (v < temp->vertex) {
                edges[e].src = v;
                edges[e].dest = temp->vertex;
                edges[e].weight = temp->weight;
                e++;
            }
            temp = temp->next;
        }
    }

    heapSort(edges, e);

    struct Edge result[numVertices]; // To store the MST
    int parent[numVertices];
    int rank[numVertices];

    for (int v = 0; v < numVertices; ++v) {
        parent[v] = v;
        rank[v] = 0;
    }

    int i = 0, count = 0;
    while (count < numVertices - 1 && i < e) {
        struct Edge nextEdge = edges[i++];

        int x = find(parent, nextEdge.src);
        int y = find(parent, nextEdge.dest);

        if (x != y) {
            result[count++] = nextEdge;
            Union(parent, rank, x, y);
        }
    }

    printf("Edges in the MST using Kruskal's algorithm with union by rank:\n");
    int totalWeight = 0;
    for (i = 0; i < count; ++i) {
        printf("%d - %d (weight: %d)\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight = totalWeight + result[i].weight;
    }
    printf("Total weight of the MST: %d\n", totalWeight);

    free(edges);
}

// Function to free allocated memory for the graph
void freeGraph(struct Graph* graph) {
    if (graph) {
        if (graph->adjacencyList) {
            int v;
            for (v = 0; v < graph->numVertices; ++v) {
                struct Node* temp = graph->adjacencyList[v];
                while (temp) {
                    struct Node* prev = temp;
                    temp = temp->next;
                    free(prev);
                }
            }
            free(graph->adjacencyList);
        }
        free(graph);
    }
}

int main() {
    int V = 9; // Number of vertices

    // Create a graph with V vertices
    struct Graph* graph = createGraph(V);

    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 7, 8, 7);
    addEdge(graph, 7, 6, 1);
    addEdge(graph, 6, 5, 2);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 3, 5, 14);
    addEdge(graph, 4, 5, 10);

    kruskalMST(graph);

    // Free allocated memory for the graph
    freeGraph(graph);

    return 0;
}
