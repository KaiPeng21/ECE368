
/*****************************************
** Name: 	Chia-Hua Peng
** Email:	peng81@purdue.edu							**
**										**
** Notes:								**
**If you include math.h, include -lm 	** 
gcc -Werror -Wall -lm hw5_q4.c			**
										**
******************************************/


#include <stdio.h>
#include <stdlib.h>
#define MAX_STRING_SIZE 30

#include <math.h>
#define NUM_TER 42

/* Structures Definitions */
typedef struct Territory_Info{
	int number;
	char name[MAX_STRING_SIZE];
	double x;
	double y;
}Territory_Info;

/*Other structrues I created */
int Neighbors[NUM_TER+1][NUM_TER+1] = {0};	// Neighbors[A][B] == 1 if A and B are neighbors, otherwise == 0
double Weight[NUM_TER+1][NUM_TER+1];   		// Weight[A][B] stores the distance from A to B

typedef struct _Path_Info{
	int vnum;	// vertex idenfication number
	int Visited;
	int Previous;
	double Distance;
}Path_Info;

/*Function Declarations*/
void Create_Weighted_Array(Territory_Info * Territories);
double Compute_Distance(double x1, double y1, double x2, double y2);
void Shortest_Path(Path_Info * P, int start, int end, Territory_Info * Territories);
void Recurse_Dijkstra(Path_Info * P, int v);
int Find_Unvisited_Vertex_with_Min_Dis(Path_Info * P);

void print_path_info(Path_Info *P);


//void Sort_Path_Info(Path_Info * P, int size);
//void Swap_Path_Info(Path_Info * P, int a, int b);

//Parse info from territories into an Array 
Territory_Info *Load_File(char *Filename);


//****************SUGGESTED FUNCTION TO ADD neighbors****************
//*************(YOU MAY NEED TO MODIFY THIS FUNCTION)*****************
void scan_neighbors(char *Filename);

/*Main */
int main(int argc, char *argv[]){
        if (argc < 3){
                fprintf(stderr, "Insufficient arguments.\n");
                return EXIT_FAILURE;
        }
        if (atoi(argv[1]) > NUM_TER || atoi(argv[2]) > NUM_TER){
                fprintf(stderr, "Territories number is invalid\n");
                return EXIT_FAILURE;
        }


	int i=0;
	int NumTerritories = NUM_TER;
	
	//Array that contains only the [Num],[Name],[Xcoord],[Ycoord] of each territory
	Territory_Info *Territories = NULL;
//	Territories = Load_File("test_coordinates.txt"); // I was testing my program using other file I created
	Territories = Load_File("coordinates.txt");

	//Your code


	// scan neighbors
//	scan_neighbors("test_neighbors.txt");
	scan_neighbors("neighbors.txt");	

	// compute distance between vertices in a weighted 2d array
	Create_Weighted_Array(Territories);

/*
	int j;
	for (i = 0; i <= NUM_TER; i++){
		for (j = 0; j <= NUM_TER; j++){
			printf("%d ", Neighbors[i][j]);
		}
		printf("\n");
	}
	for (i = 0; i <= NUM_TER; i++){
		for (j = 0; j <= NUM_TER; j++){
			printf("%lf ", Weight[i][j]);
		}
		printf("\n");
	}
*/

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);

	Path_Info *path_info = malloc(sizeof(Path_Info) * (NumTerritories+1));
	Shortest_Path(path_info, start, end, Territories);

	free(path_info);	
	free(Territories);

	return EXIT_SUCCESS;
}




//Read info from the territories
Territory_Info *Load_File(char *Filename){
	int NumTerritories = NUM_TER;
	double x = 0, y =0;
	Territory_Info *Territories = NULL;
	int i = 0, num = 0;
	char NAME[MAX_STRING_SIZE];
	
	FILE *f = fopen(Filename, "r");
	if (f == NULL) 
	{ 
	printf("File Cant be Found");
	}
	
	Territories = malloc(NumTerritories*sizeof(Territory_Info));
	
	for(i=0;i<NumTerritories; i++){
		fscanf(f,"%d", &num);
		fscanf(f,"%s", Territories[i].name);
		fscanf(f,"%lf", &x);
		fscanf(f,"%lf", &y);
		Territories[i].number = num;
		Territories[i].x = x;
		Territories[i].y = y;
	}
	
	fclose(f);
	return Territories;
	
}



//Suggested function to scan neighbors. You may need to parse another 
//variable, for example: void scan_neighbors(char *Filename, TYPE Graph, Territory_Info *Territories)
void scan_neighbors(char *Filename)
{
	int i,j;
	char temp_array[3];
	int vertex_number;
	int number_of_neighbors;
	int new_neighbor;
	int NumTerritories = NUM_TER;
	
	FILE *f = fopen(Filename, "r");
	if (f == NULL) 
	{ 
	printf("File Cant be Found");
	}

	for(i=0;i<NumTerritories; i++)
	{
		fscanf(f,"%d", &vertex_number);
		fscanf(f,"%d", &number_of_neighbors);
		fscanf(f,"%s", temp_array); //discard the "," in the file
		for(j=0; j<number_of_neighbors; j++)
		{
			fscanf(f,"%d", &new_neighbor);
			//add_neighbor(graph,vertex_number,new_neighbor, Territories);
			Neighbors[vertex_number][new_neighbor] = 1;
		}
	}
	
	fclose(f);
	
}

// My weighted graph data structure (2d Array)
void Create_Weighted_Array(Territory_Info * Territories){
	int i, j;
	int x1, y1, x2, y2;

	for (i = 0; i <= NUM_TER; i++){
		for(j = 0; j <= NUM_TER; j++){
			if (Neighbors[i][j] == 1){
				x1 = Territories[i-1].x;
				y1 = Territories[i-1].y;
				x2 = Territories[j-1].x;
				y2 = Territories[j-1].y;
				Weight[i][j] = Compute_Distance(x1, y1, x2, y2);


			//	printf("%d %d %d %lf %lf \n", i, j, Territories[i-1].number, Territories[i-1].x, Territories[i-1].y);
			}
			else{
				Weight[i][j] = 2000000;
			}
		}
	}
}

double Compute_Distance(double x1, double y1, double x2, double y2){
	double distance;
	
	distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

	return distance;
}

void Shortest_Path(Path_Info * P, int start, int end, Territory_Info * Territories){
	int i;
	for (i = 0; i <= NUM_TER; i++){  // P[0] is redundant, but having index number and the terretory ID the same makes life easier
		P[i].vnum = i;
		P[i].Visited = 0;
		P[i].Distance = 2000000; // infinite
		P[i].Previous = 0;
	}

	P[start].Distance = 0;
	P[start].Previous = 0;

	Recurse_Dijkstra(P, start);	

	int *path_stack = malloc(sizeof(int) * NUM_TER);
	int source = 0;
	int destination = end;
	do{
		path_stack[source] = P[destination].vnum;
		destination = P[destination].Previous; 
		source++;
	}while(P[destination].vnum != 0);	

	for (i = source - 1; i >= 0; i--){
		int location = path_stack[i];
		printf("%s \n", Territories[location-1].name);
	}
	
	free(path_stack);
	
}


void Recurse_Dijkstra(Path_Info * P, int v){
	int i;
	P[v].Visited = 1;

	// v: current vertex index, i: neighbor vertex index
	for (i = 1; i <= NUM_TER; i++){
		if (Neighbors[v][i] == 1 && P[i].Visited == 0){  // go through the unvisited neighbors
			double tmp = P[v].Distance + Weight[v][i]; // compute the distance from the beginning to v to i
			if (tmp < P[i].Distance){
				P[i].Distance = tmp;
				P[i].Previous = P[v].vnum;
			}
		}
	}
//	print_path_info(P);

	int next = Find_Unvisited_Vertex_with_Min_Dis(P);
	
	if (next == -1){return;} // all the vertex are visited, end of recursive function
	Recurse_Dijkstra(P, next);
}


int Find_Unvisited_Vertex_with_Min_Dis(Path_Info * P){
	Path_Info Pmin;

	int i = 1;
	while(i <= NUM_TER && P[i].Visited == 1){i++;}	// skip all the visited vertex ID
	if (i > NUM_TER){return -1;}			// all the vertex are visited, return -1
	Pmin = P[i];					// set the first unvisited vertex as the minimum
	while (i <= NUM_TER){
		if(P[i].Visited == 0 && P[i].Distance < Pmin.Distance){	// update the unvisited minimum path info
			Pmin = P[i];
		}
		i++;
	}

	return Pmin.vnum;
}


//
void print_path_info(Path_Info *P){
	printf("vnum\t visited\t distance\t previous\t\n");
	int i;
	for(i = 1; i <= NUM_TER; i++){
		printf("%d\t%d\t%lf\t%d\t\n", P[i].vnum, P[i].Visited, P[i].Distance, P[i].Previous);
	}
}


/*
void Recurse_Dijkstra(Path_Info * P, int v){
	int i;
	
	P[v].Visited = 1;

	numNeigh = numNeighbors[v];	// get the number of neighbors
	Path_Info *tmp_path = malloc(sizeof(Path_Info) * numNeigh);
	int j = 0;
	
	for (i = 1; i <= NUM_TER; i++){
		// v - current index, i - neighbor index
		if (Neighbors[v][i] == 1 && P[i].Visited == 0){  // go through v's non-visited neighbors
			double tmp = P[v].Distance + Weight[v][i]; // compute distance from begining to v to i
			if (tmp < P[i].Distance){		  // if it is shorter, update the neighbor's distance and the previous
				P[i].Distance = tmp;
				P[i].Previous = v;
			}

			tmp_path[j] = P[i];				   // store the neighbor vertex ID to a temperary array
			j++;
		}
	}

	Sort_Path_Info(tmp_path, numNeigh);				// sort the temperary array based on the distance information



	free(tmp_path);
}
*/
/*
void Sort_Path_Info(Path_Info * P, int size){
	int i, j;
	for (i = 1; i < size; i++){
		j = i;
		while (j > 0 && P[j-1].Distance > P[j].Distance){
			Swap_Path_Info(P, j, j-1);
			j--;
		}
	}
}

void Swap_Path_Info(Path_Info * P, int a, int b){
	Path_Info tmp = P[a];
	P[a] = P[b];
	P[b] = tmp;
}
*/

