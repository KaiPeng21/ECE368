#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// matrix dimensions
#define row 5
#define col 5

// direction vectors
int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1};

// the input matrix
int m[row][col];

// the labels, 0 means unlabeled
int label[row][col];


void dfs(int x, int y, int current_label){

	// Fill in this part

	label[x][y] = current_label;
	if (x-1 >= 0 &&  m[x-1][y] == 1 && label[x-1][y] == 0){
		label[x-1][y] = current_label;
		dfs(x-1, y, current_label);
	}
	if (y-1 >= 0 && m[x][y-1] == 1 && label[x][y-1] == 0){
		label[x][y-1] = current_label;
		dfs(x, y-1, current_label);
	}
	if (x+1 < row && m[x+1][y] == 1 && label[x+1][y] == 0){
		label[x+1][y] = current_label;
		dfs(x+1, y, current_label);
	}
	if (y+1 < row && m[x][y+1] == 1 && label[x][y+1] == 0){
                label[x][y+1] = current_label;
                dfs(x, y+1, current_label);
        }

}

void find_components(){

	int component = 0;
	int i, j;
	for (i = 0; i < row; ++i) 
		for (j = 0; j < col; ++j) 
			if (!label[i][j] && m[i][j]) dfs(i, j, ++component);
}

int main(){

	int i, j;

	// initialize matrices
	for(i=0; i<row; i++){
		for(j=0; j<col; j++){
			m[i][j] = 0;
			label[i][j] = 0;
		}
	}

	// enter input values (0 or 1)
	for(i=0; i<row; i++){
		printf("Enter %d input values of row %d (0 or 1)\n", col, i);
		for(j=0; j<col; j++){
			printf("(%d, %d): ", i, j);
			scanf("%d", &m[i][j]);
		}
		for(j=0; j<col; j++){
			if(m[i][j]!=0 && m[i][j]!=1){
				printf("Wrong numbers!\n");
				exit(1);
			}
		}
	}

	// implement connected component
	find_components();

	// print out the input matrix
	printf("Input:\n");
	for(i=0; i<row; i++){
		for(j=0; j<col; j++)
			printf("%d ", m[i][j]);
		printf("\n");
	}

	// print out the output matrix
	printf("Output:\n");
	for(i=0; i<row; i++){
		for(j=0; j<col; j++)
			printf("%d ", label[i][j]);
		printf("\n");
	}

	return 0;
}

