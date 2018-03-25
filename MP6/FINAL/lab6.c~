/* Austin Johnson
 * agjohns
 * lab6.c 
 * Lab6: Graphs
 * ECE 2230, Fall 2016
 *
 * Purpose: This file contains drivers to test the graph package.
 *
 * Bugs: None from the tests I have ran against it. 
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#include "graph.h"

/* constants used with Global variables */
#define TRUE 1
#define FALSE 0

/* Global variables for command line parameters */
int GraphType = -1;
int GraphOperation = -1;
int NumVertices = 7;
int AdjVertices = -1;
int SourceVertex = -1;
int DestVertex = -1;
int UnitNumber = -1;
int Seed = 1476379020;
int Verbose = FALSE;

/* prototypes for functions in this file only */
void getCommandLine(int argc, char **argv);
void unitDriver (void);

int main(int argc, char **argv)
{
    getCommandLine(argc, argv);
    unitDriver();
    assert(SourceVertex < NumVertices);
    assert(DestVertex < NumVertices);
    assert(AdjVertices < NumVertices);
    printf("\nSeed: %d\n", Seed);
    srand48(Seed);

    graph_t *G = graph_construct(NumVertices);

    if (GraphType == 1) {
        printf("Graph Type: Weakly-Connected with 7 Vertices\n");
        graph_add_edge(G, 1, 2, 3);
        graph_add_edge(G, 1, 6, 5);
        graph_add_edge(G, 2, 3, 7);
        graph_add_edge(G, 2, 4, 3);
        graph_add_edge(G, 2, 6, 10);
        graph_add_edge(G, 3, 4, 5);
        graph_add_edge(G, 3, 5, 1);
        graph_add_edge(G, 4, 5, 6);
        graph_add_edge(G, 5, 6, 7);
        graph_add_edge(G, 6, 0, 4);
        graph_add_edge(G, 6, 3, 8);
        graph_add_edge(G, 6, 4, 9);
    }
    
    else if (GraphType == 2) {
        printf("Graph Type: Strongly-Connected with %d Vertices\n", NumVertices);
        int link_src, link_dest;
        for (link_src = 0; link_src < NumVertices; link_src++) {
            for (link_dest = 0; link_dest < NumVertices; link_dest++) {
                if (link_src == link_dest) continue;
                graph_add_edge(G, link_src, link_dest, weight(link_src, link_dest, 0));
            }
        }
    }

    else if (GraphType == 3) {       
        printf("Graph Type: Random Graph with %d Vertices\n", NumVertices);
        if (AdjVertices == -1) {
            printf("\nInvalid Operands: Use -a to specify number of adjacent vertices.\n\n"); 
                graph_destruct(G);
                exit(-1);
        }

        int link_src, link_dest, i;
        int curr_adj = 0, sum_adj = 0, max_adj = INT_MIN, min_adj = INT_MAX;
        float v_x[NumVertices], v_y[NumVertices], dist_betw_vert;        
    
        //generate random vertex locations
        v_x[0] = 0.5, v_y[0] = 0;
        for (i = 1; i < NumVertices - 1; i++) {
            v_x[i] = drand48();    
            v_y[i] = drand48();
        }
        v_x[NumVertices-1] = 0.5, v_y[NumVertices-1] = 1;      

        //create graph
        for (link_src = 0; link_src < NumVertices; link_src++) {
            for (link_dest = 0; link_dest < NumVertices; link_dest++) {
                if (link_src == link_dest) continue;
                dist_betw_vert = distance (v_x[link_src], v_y[link_src], v_x[link_dest], v_y[link_dest]);
                graph_add_edge(G, link_src, link_dest, weight(link_src, link_dest, dist_betw_vert));
            }
        }    

        //get adjacent vertices stats
        adj_list_node_t *node = NULL;
        for (i = 0; i < NumVertices; i++) {
            node = G->adj_list[i].head;
            curr_adj = 0;
            while (node != NULL) {
                sum_adj++, curr_adj++;
                node = node->next;
            }
            if (curr_adj > max_adj) max_adj = curr_adj;
            if (curr_adj < min_adj) min_adj = curr_adj;
        }
        printf("Minimum Number of Adjacent Vertices: %d\n", min_adj);
        printf("Maximum Number of Adjacent Vertices: %d\n", max_adj);
        printf("Average Number of Adjacent Vertices: %d\n", sum_adj/NumVertices);   
    }
    else 
        printf("\nInvalid Operands: Use -g to specify type of graph.\n\n");

    if (Verbose) graph_debug_print (G);

    clock_t start, end;
    if (GraphOperation == 1) {
        printf("Graph Operation: Dijkstra's Shortest Path Algorithm\n");
        if (SourceVertex == -1 || DestVertex == -1) {
            printf("\nInvalid Operands: Use -s and -d to specify source and destination.\n\n"); 
            graph_destruct(G);
            exit(-1);
        }
        start = clock();
        dijkstra(G, SourceVertex, DestVertex);
        end = clock();
        printf("\nTime Elapsed: %g (ms)\n\n",
            1000*((double)(end-start))/CLOCKS_PER_SEC);
    }
    else if (GraphOperation == 2) {
        printf("Graph Operation: Network Diamater\n");
        start = clock();
        graph_network_diameter(G);
        end = clock();
        printf("\nTime Elapsed: %g (ms)\n\n",
            1000*((double)(end-start))/CLOCKS_PER_SEC);
    }
    else if (GraphOperation == 3) {
        printf("Graph Operation: Multiple Link-Disjoint Paths\n");
        if (SourceVertex == -1 || DestVertex == -1) {
            printf("\nInvalid Operands: Use -s and -d to specify source and destination.\n\n"); 
            graph_destruct(G);
            exit(-1);
        }
        start = clock();
        graph_multiple_paths (G, SourceVertex, DestVertex);
        end = clock();
        printf("\nTime Elapsed: %g (ms)\n\n",
            1000*((double)(end-start))/CLOCKS_PER_SEC);
    }
    graph_destruct(G);
    return 0;
}

void unitDriver (void) {
    switch (UnitNumber) {
        case 0: //Graph 1 Dijkstra's Algorithm
            GraphType = 1;
            GraphOperation = 1;
            SourceVertex = 1;
            DestVertex = 5;
            break;
        case 1:  //Graph 1 Network Diameter
            GraphType = 1; 
            GraphOperation = 2;
            break;
        case 2: //Graph 1 Multiple Link-Disjoint 
            GraphType = 1;  
            GraphOperation = 3;
            SourceVertex = 1;
            DestVertex = 5;
            break;
        case 3: //Dijkstra's Analysis Graph 1 P1
            GraphType = 2;
            GraphOperation = 1;
            NumVertices = 10000;
            SourceVertex = 0;
            DestVertex = 9999;
            break;
        case 4: //Dijkstra's Analysis Graph 1 P2
            GraphType = 2;
            GraphOperation = 1;
            NumVertices = 5000;
            SourceVertex = 0;
            DestVertex = 4999;
            break;
        case 5: //Dijkstra's Analysis Graph 3 P1
            GraphType = 3;
            GraphOperation = 1; 
            NumVertices = 15000;
            AdjVertices = 20;
            SourceVertex = 0;
            DestVertex = 14999;
            break;
        case 6: //Dijkstra's Analysis Graph 3 P2
            GraphType = 3;
            GraphOperation = 1; 
            NumVertices = 7500;
            AdjVertices = 20;
            SourceVertex = 0;
            DestVertex = 7499;
            break;
        case 7: //Network Diameter Graph 3 P1
            GraphType = 3;
            GraphOperation = 2;
            NumVertices = 600;
            AdjVertices = 20;
            break;
        case 8: //Network Diameter Graph 3 P2
            GraphType = 3;
            GraphOperation = 2;
            NumVertices = 300;
            AdjVertices = 20;
            break;
        case 9: //Graph 3 Node Density (low number of AdjVertices)
            GraphType = 3; //set the seed on the command line --> -u9 -r seed
            GraphOperation = 2;
            NumVertices = 100;
            AdjVertices = 7;
            break;
        case 10: //Graph 3 Node Density (high number of AdjVertices)
            GraphType = 3; //set the seed on the command line --> -u10 -r seed
            GraphOperation = 2;
            NumVertices = 100;
            AdjVertices = 20;
            break;
        case 11: //Graph 2 NumPaths = NumVertices - 1 
            GraphType = 2;
            GraphOperation = 3;
            NumVertices = 15;
            SourceVertex = 14;
            DestVertex = 0;
            break;
        case 12: //Graph 3 NumPaths is approx. AdjVertices / 2
            GraphType = 3; //set the AdjVertices on the command line --> -u12 -a R       
            GraphOperation = 3;
            NumVertices = 1000;
            SourceVertex = 0;
            DestVertex = 999;
            break;
        case 13: //Printing GraphType 1
            GraphType = 1;
            Verbose = TRUE;
            break;
        case 14: //Printing GraphType 2
            GraphType = 2;
            Verbose = TRUE;
            break;
        case 15: //Printing GraphType 3     
            GraphType = 3;
            AdjVertices = 3; 
            Verbose = TRUE;
            break;
        case 16: //Graph 2 Dijkstra's Algorithm
            GraphType = 2;
            GraphOperation = 1;
            SourceVertex = 0;
            DestVertex = 6;
            break;
        case 17: //Graph 2 Network Diameter
            GraphType = 2; 
            GraphOperation = 2; 
            break;
        case 18: //Graph 3 Dijkstra's Algorithm Disconnected
            GraphType = 3; 
            GraphOperation = 1;
            AdjVertices = 3;
            SourceVertex = 0;
            DestVertex = 6;
            break;
        case 19: //Graph 3 Dijkstra's Algorithm Connected
            GraphType = 3;
            GraphOperation = 1;
            AdjVertices = 5;
            SourceVertex = 0;
            DestVertex = 6;
            break;
        case 20: //Graph 3 Network Diameter Disconnected
            GraphType = 3;
            GraphOperation = 2;
            NumVertices = 10; 
            AdjVertices = 5;
            break;
        case 21: //Graph 3 Network Diameter Connected
            GraphType = 3;
            GraphOperation = 2;
            NumVertices = 10;
            AdjVertices = 7;
            break;
        case 22: //Graph 3 Multiple Link-Disjoint Paths Disconnected
            GraphType = 3;
            GraphOperation = 3;
            AdjVertices = 3;
            SourceVertex = 0;
            DestVertex = 6;
            break;
        case 23: //Graph 3 Multiple Link-Disjoint Paths Connected
            GraphType = 3;
            GraphOperation = 3;
            NumVertices = 10;
            AdjVertices = 7;
            SourceVertex = 0;
            DestVertex = 9;
            break;   
        default: break; //don't run a unit driver
    }
}

/* read in command line arguments and store in global variables for easy
 * access by other functions.
 */
void getCommandLine(int argc, char **argv)
{
    /* optopt--if an unknown option character is found
     * optind--index of next element in argv 
     * optarg--argument for option that requires argument 
     * "x:" colon after x means argument required
     */
    int c;
    int index;

    while ((c = getopt(argc, argv, "g:n:a:h:s:d:u:vr:")) != -1)
        switch(c) {
            case 'v': Verbose = TRUE;                 break;
            case 'r': Seed = atoi(optarg);            break;
            case 'n': 
                if (atoi(optarg) < 0) {
                    fprintf(stderr, "invalid number of vertices: %s\n", optarg);  
                    exit(-1);
                }                  
                else NumVertices = atoi(optarg);      break;
            case 'a': 
                if (atoi(optarg) <= 0) {
                    fprintf(stderr, "invalid number of adjacent vertices: %s\n", optarg);
                    exit(-1);
                }
                else AdjVertices = atoi(optarg);      break;
            case 's': 
                if (atoi(optarg) < 0) {
                    fprintf(stderr, "invalid source vertex: %s\n", optarg);
                    exit(-1);
                }
                else SourceVertex = atoi(optarg);     break;
            case 'd': 
                if (atoi(optarg) < 0) {
                    fprintf(stderr, "invalid destination vertex: %s\n", optarg);
                    exit(-1);
                }
                else DestVertex = atoi(optarg);       break;
            case 'u': 
                if (atoi(optarg) < 0 || atoi(optarg) > 23) { 
                    fprintf(stderr, "invalid driver: %s\n", optarg); 
                    exit(-1);
                }
                else UnitNumber = atoi(optarg);       break;
            case 'g':
                if (atoi(optarg) == 1) 
                    GraphType = 1;
                else if (atoi(optarg) == 2) 
                    GraphType = 2;
                else if (atoi(optarg) == 3)
                    GraphType = 3;
                else {
                    fprintf(stderr, "invalid graph type: %s\n", optarg);
                    exit(-1);
                }
                break;
            case 'h':
                if (atoi(optarg) == 1)
                    GraphOperation = 1;
                else if (atoi(optarg) == 2)
                    GraphOperation = 2;
                else if (atoi(optarg) == 3)
                    GraphOperation = 3;
                else {
                    fprintf(stderr, "invalid graph operation: %s\n", optarg);
                    exit(-1);
                }
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unknown option %c.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            default:
                printf("Lab6 command line options\n");
                printf("General options ---------\n");
                printf("  -v        turn on verbose prints (default off)\n");
                printf("  -s 123    seed for random number generator\n");
                printf("  -g [1|2|3]\n");
                printf("            Type of graph\n");
                printf("  -h [1|2|3]\n");
                printf("            Graph Operation\n");
                printf("  -n N      number of vertices in the graph\n");
                printf("  -a R      approximate number of adjacent vertices between [0:N)\n");
                printf("  -s S      number of source vertex S between [0:N)\n");
                printf("  -d D      number of destination vertex D between [0:N)\n");
                printf("  -u 0      run unit test driver\n");
                exit(1);
        }
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}

/* vi:set ts=8 sts=4 sw=4 et: */
