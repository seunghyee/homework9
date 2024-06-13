#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 10

// 인접 리스트 노드 구조체 정의
typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

// 인접 리스트 구조체 정의
typedef struct AdjList {
    AdjListNode* head;
} AdjList;

// 그래프 구조체 정의
typedef struct Graph {
    int numVertices;
    AdjList* array;
} Graph;

// 큐 구조체 정의 (BFS용)
typedef struct Queue {
    int items[MAX_VERTICES];
    int front, rear;
} Queue;

// 새로운 인접 리스트 노드 생성
AdjListNode* newAdjListNode(int dest) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// 그래프 생성
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->array = (AdjList*)malloc(vertices * sizeof(AdjList));

    for (int i = 0; i < vertices; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// 그래프에 간선 추가
void addEdge(Graph* graph, int src, int dest) {
    AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// 그래프 출력
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->numVertices; ++v) {
        AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// 큐 생성 (BFS용)
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

// 큐가 비어 있는지 확인
int isEmpty(Queue* queue) {
    return queue->rear == -1;
}

// 큐에 요소 추가
void enqueue(Queue* queue, int value) {
    if (queue->rear == MAX_VERTICES - 1)
        printf("\nQueue is full!");
    else {
        if (queue->front == -1)
            queue->front = 0;
        queue->rear++;
        queue->items[queue->rear] = value;
    }
}

// 큐에서 요소 제거
int dequeue(Queue* queue) {
    int item;
    if (isEmpty(queue)) {
        printf("Queue is empty.");
        item = -1;
    } else {
        item = queue->items[queue->front];
        queue->front++;
        if (queue->front > queue->rear) {
            queue->front = queue->rear = -1;
        }
    }
    return item;
}

// 깊이 우선 탐색 (DFS)
void DFS(Graph* graph, int vertex, int* visited) {
    AdjListNode* adjList = graph->array[vertex].head;
    AdjListNode* temp = adjList;

    visited[vertex] = 1;
    printf("%d ", vertex);

    while (temp != NULL) {
        int connectedVertex = temp->dest;

        if (!visited[connectedVertex]) {
            DFS(graph, connectedVertex, visited);
        }
        temp = temp->next;
    }
}

// 깊이 우선 탐색 (DFS) 래퍼 함수
void depthFirstSearch(Graph* graph, int startVertex) {
    int visited[MAX_VERTICES] = {0};

    DFS(graph, startVertex, visited);
    printf("\n");
}

// 인접 리스트를 정렬하는 함수 (오름차순)
void sortAdjList(AdjList* adjList) {
    AdjListNode* sorted = NULL;
    AdjListNode* current = adjList->head;

    // 인접 리스트의 모든 노드를 새 리스트로 삽입하며 정렬
    while (current != NULL) {
        AdjListNode* next = current->next;
        if (sorted == NULL || sorted->dest >= current->dest) {
            current->next = sorted;
            sorted = current;
        } else {
            AdjListNode* temp = sorted;
            while (temp->next != NULL && temp->next->dest < current->dest) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    adjList->head = sorted;
}

// 너비 우선 탐색 (BFS)
void breadthFirstSearch(Graph* graph, int startVertex) {
    int visited[MAX_VERTICES] = {0};
    Queue* queue = createQueue();

    visited[startVertex] = 1;
    enqueue(queue, startVertex);

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);
        printf("%d ", currentVertex);

        // 정점의 인접 리스트를 오름차순 정렬
        sortAdjList(&(graph->array[currentVertex]));

        AdjListNode* temp = graph->array[currentVertex].head;

        while (temp) {
            int adjVertex = temp->dest;

            if (!visited[adjVertex]) {
                visited[adjVertex] = 1;
                enqueue(queue, adjVertex);
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

// 메뉴 기반 프로그램 메인 함수
int main() {
    char command;
    int vertex, vertex2;
    Graph* graph = NULL;
    
    printf(" [----- [son seunghye] [2023078010]-----]");
    printf("---------------------------------------------------------------\n");
    printf("Graph Searches\n");
    printf("---------------------------------------------------------------\n");
    printf("Initialize Graph = z\n");
    printf("Insert Vertex = v Insert Edge = e\n");
    printf("Depth First Search = d Breadth First Search = b\n");
    printf("Print Graph = p Quit = q\n");
    printf("---------------------------------------------------------------\n");

    while (1) {
        printf("\nEnter a command: ");
        scanf(" %c", &command);

        switch (command) {
            case 'z':
                graph = createGraph(MAX_VERTICES);
                printf("Graph initialized to %d vertices.\n", MAX_VERTICES);
                break;

            case 'v':
                printf("The vertices are automatically managed up to %d.\n", MAX_VERTICES);
                break;

            case 'e':
                printf("Enter edge (format: v1 v2): ");
                scanf("%d %d", &vertex, &vertex2);
                if (graph && vertex < MAX_VERTICES && vertex2 < MAX_VERTICES) {
                    addEdge(graph, vertex, vertex2);
                    printf("Edge added between vertex %d and vertex %d.\n", vertex, vertex2);
                } else {
                    printf("Invalid vertex or graph not initialized.\n");
                }
                break;

            case 'd':
                if (graph) {
                    printf("Enter starting vertex for DFS: ");
                    scanf("%d", &vertex);
                    if (vertex < MAX_VERTICES) {
                        printf("DFS traversal starting from vertex %d: ", vertex);
                        depthFirstSearch(graph, vertex);
                    } else {
                        printf("Invalid starting vertex.\n");
                    }
                } else {
                    printf("Graph not initialized.\n");
                }
                break;

            case 'b':
                if (graph) {
                    printf("Enter starting vertex for BFS: ");
                    scanf("%d", &vertex);
                    if (vertex < MAX_VERTICES) {
                        printf("BFS traversal starting from vertex %d: ", vertex);
                        breadthFirstSearch(graph, vertex);
                    } else {
                        printf("Invalid starting vertex.\n");
                    }
                } else {
                    printf("Graph not initialized.\n");
                }
                break;

            case 'p':
                if (graph) {
                    printGraph(graph);
                } else {
                    printf("Graph not initialized.\n");
                }
                break;

            case 'q':
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid command. Please try again.\n");
        }
    }

    return 0;
}
