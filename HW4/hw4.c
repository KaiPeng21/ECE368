#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*****************************************
** Name:  Chia-Hua Peng
** Email: peng81@purdue.edu								**
**										**
** Notes: 								**
**										**
******************************************/


/* LOAD AND SAVE FILES */	
long *Load_File (char *Filename, int *Size);
/*
Allocates memory and loads Array stored in file in the form:
Size
Array[0]
Array[1]
...
Array[Size-1] 
*/

void Save_File (char *Filename, long *Array, int Size);
/*
Frees memory and saves Array into a file of the form:
Array[0]
Array[1]
...
Array[Size-1] 
*/

/* SORTING ALGORITHMS */
void InsertionSort(long *Array, int first, int last, double *NumComps, double *NumMoves);
void QuickSort1(long *Array, int first, int last, double *NumComps, double *NumMoves);
void QuickSort2(long *Array, int first, int last, double *NumComps, double *NumMoves);
void QuickSort3P(long *Array, int first, int last, double *NumComps, double *NumMoves);

/* QUICKSORT FUNCTIONS*/
int find_median_index(long *Array, int first, int last, double *NumComps, double *NumMoves);



/*HELPER FUNCTIONS*/
void swap(long *Array, int low, int high,double *NumComps, double *NumMoves);
/*Swaps elements at Array[low] <-> Array[high] */

int main(int argc, char **argv)
{

	if(argc != 3)
	{
		printf("ERROR! Required command is: \n");
		printf("Sorting [name.txt] [option]");
		return(-1);
	}		
	
	int Size;
	double NumComps = 0, NumMoves = 0;
	int Option = 0;
	long *Array = Load_File (argv[1], &Size);
	
	Option = atoi(argv[2]);
	switch(Option)
	{
		case 1: 
			InsertionSort(Array, 0, Size-1, &NumComps, &NumMoves);
			break;
		case 2:
			QuickSort1(Array, 0, Size-1, &NumComps, &NumMoves);
			break;
		case 3:
			QuickSort2(Array, 0, Size-1, &NumComps, &NumMoves);
			break;
		case 4:
			QuickSort3P(Array, 0, Size-1, &NumComps, &NumMoves);
			break;
		default:
			printf("Please enter either 1,2,3 or 4\n");
			return(-1);
	}

	printf("NumComps: %lf \nNumMoves: %lf \n", NumComps, NumMoves);	

	Save_File ("Sorted.txt", Array, Size);
	
	return 0;
}


long *Load_File (char *Filename, int *Size){
	int i =0;
	long *Numbers = NULL;
	FILE *f = fopen(Filename, "r");
	if(!f){
		printf("File not found");
		exit(-1);
	}
	
	if (f == NULL) 
	{ 
		printf("File Cant be Found");
		*Size = 0;
	}
	fscanf(f, "%d", Size);
	Numbers = (long *) malloc(sizeof(long)* *Size);
	
	for(i=0; i< *Size; i++){
	fscanf(f,"%ld", (Numbers + i));
	}

	fclose(f);
	return Numbers;
	
}

void Save_File (char *Filename, long *Array, int Size){	
	FILE *f = fopen(Filename, "w");
	int i = 0;
	
	for(i=0; i < Size; i++){
		fprintf(f, "%ld \n", *(Array + i));
	}
	free(Array);
	
	fclose(f);
}





/* SORTING ALGORITHMS */

void InsertionSort(long *Array, int first, int last, double *NumComps, double *NumMoves)
{
	//Your code
	int k = 1;											++*NumMoves;
	while (k <= last){										++*NumComps;

		int j = k;										++*NumMoves;
		while(j > 0){										++*NumComps;
			

													++*NumComps;
			if (Array[j-1] > Array[j]){							
				swap(Array, j-1, j, NumComps, NumMoves);
			}else{										
				break;
			}



			j = j-1;									++*NumMoves;
		}											++*NumComps; // exit loop need an extra comparison


		k = k+1;										++*NumMoves;
	}												++*NumComps; // exit loop


}

void QuickSort1( long *Array, int first, int last, double *NumComps, double *NumMoves)
{
	//Your code
	if (first >= last){										++*NumComps;
		return;
	}												++*NumComps; // compare for else

	long pivot = Array[first];									++*NumMoves;
	
	int low = first - 1;										++*NumMoves;
	int high = last + 1;										++*NumMoves;
	
	while (1){
		do{
			low = low + 1;									++*NumMoves;
													++*NumComps;
		}while(Array[low] < pivot);								
		do{
			high = high - 1;								++*NumMoves;
													++*NumComps;
		}while(Array[high] > pivot);								

                                                                                                        ++*NumComps;
		if (low >= high){									
			break;
		}											
		swap(Array, low, high, NumComps, NumMoves);
	}
	QuickSort1(Array, first, high, NumComps, NumMoves);
	QuickSort1(Array, high+1, last, NumComps, NumMoves);

}

void QuickSort2( long *Array, int first, int last, double *NumComps, double *NumMoves)
{
	//Your code
	                                                                                                ++*NumComps;
        if (first >= last){										
		return;
	}												

	int med_index = find_median_index(Array, first, last, NumComps, NumMoves);                      ++*NumMoves;
	swap(Array, first, med_index, NumComps, NumMoves);

        long pivot = Array[first];                                                                      ++*NumMoves;

        int low = first - 1;                                                                            ++*NumMoves;
        int high = last + 1;                                                                            ++*NumMoves;

        while (1){
                do{
                        low = low + 1;                                                                  ++*NumMoves;
                                                                                                        ++*NumComps;
                }while(Array[low] < pivot);								
                do{
                        high = high - 1;                                                                ++*NumMoves;
                                                                                                        ++*NumComps;
                }while(Array[high] > pivot);		// already count before exit the loop


                                                                                                        ++*NumComps;
                if (low >= high){									
			break;										
		}											

                swap(Array, low, high, NumComps, NumMoves);
        }
        QuickSort1(Array, first, high, NumComps, NumMoves);
        QuickSort1(Array, high+1, last, NumComps, NumMoves);

}


void QuickSort3P( long *Array, int first, int last, double *NumComps, double *NumMoves)
{
	//Your code
													++*NumComps;
	if (first >= last){									
		return;
	}												

	// determine the median of the first, midpoint, and last element
	// use the median as a pivot
	// swap the pivot to the end of the array
        int med_index = find_median_index(Array, first, last, NumComps, NumMoves);                      ++*NumMoves;
//	printf("\nArray[med_index]: %ld before swap\n", Array[med_index]);
//	printf("\nArray[last]: %ld before swap\n", Array[last]);

        swap(Array, last, med_index, NumComps, NumMoves);


//	printf("\nArray[med_index]: %ld after swap\n", Array[med_index]);
//	printf("\nArray[last]: %ld after swap\n", Array[last]);
	// get the pivot value
        long pivot = Array[last];									++*NumMoves;
	
	
	int larger_start = first;									++*NumMoves;
	int i = first;											++*NumMoves;
	int equal_start = last;   									++*NumMoves;


	// three way partitioning in lower...larger...equal order
	while(i < equal_start){										++*NumComps;

		if (Array[i] < pivot){									++*NumComps; // compare <
			swap(Array, i, larger_start, NumComps, NumMoves);
			i = i + 1;									++*NumMoves;
			larger_start = larger_start + 1;						++*NumMoves;
		}
		else if (Array[i] == pivot){								*NumComps = *NumComps + 2; // compare <, then ==
			equal_start = equal_start - 1;							++*NumMoves;
			swap(Array, i, equal_start, NumComps, NumMoves);				++*NumMoves;
		}
		else{											*NumComps = *NumComps + 2; // compare <, then ==
			i = i + 1;									++*NumMoves;
		}

	}												++*NumComps; // exit loop

	// find the minimum of the larger length and the equal length
	int length = equal_start - larger_start;							++*NumMoves;
													++*NumComps;
	if (last - equal_start + 1 < length){								
		length = last - equal_start + 1;							++*NumMoves;
	}												


//	printf("\nlength: %d\n", length);
	int j = 0;											++*NumMoves;
	while(j < length){										++*NumComps;
		int a = larger_start + j;								++*NumMoves;
		int b = last - length + 1 + j;								++*NumMoves;
		swap(Array, a, b, NumComps, NumMoves);

		j = j+1;										++*NumMoves;
	}												++*NumComps; // exit loop


	QuickSort3P(Array, first, larger_start-1, NumComps, NumMoves);
	QuickSort3P(Array, last - equal_start + larger_start + 1, last, NumComps, NumMoves);
}


/* QUICKSORT FUNCTIONS*/
int find_median_index(long *Array, int first, int last, double *NumComps, double *NumMoves){
	int midpoint = (first + last) / 2;								++*NumMoves;

	int max = Array[first];			++*NumMoves;
	int min = Array[first];			++*NumMoves;
	if (Array[midpoint] > max){		++*NumComps;
		max = Array[midpoint];		++*NumMoves;
	}else if (Array[midpoint] < min){	*NumComps = *NumComps + 2; // compare >, then <
		min = Array[midpoint];		++*NumMoves;
	}else{					*NumComps = *NumComps + 2; // compare >, then <

	}

	if (Array[last] > max){			++*NumComps;
		max = Array[last];		++*NumMoves;
	}else if (Array[last] < min){		*NumComps = *NumComps + 2; // compare >, then <
		min = Array[last];		++*NumMoves;
	}else{					*NumComps = *NumComps + 2; // compare >, then <
	
	}

						++*NumComps; // != max
	if (Array[first] != max){		
						++*NumComps; // != min
		if (Array[first] != min){	
			return first;
		}


	}

						++*NumComps; // != max
	if (Array[midpoint] != max){
						++*NumComps; // != min
		if (Array[midpoint] != min){
			return midpoint;
		}
	}

	return last;



/*
	if (Array[first] > Array[midpoint]){								++*NumComps;
        	if (Array[first] < Array[last]){
			return first;
		}

	}
	else if (Array[first] < Array[midpoint] && Array[first] > Array[last]){				++*NumComps;
		return first;
	}
	else if (Array[last] > Array[midpoint] && Array[last] < Array[first]){				++*NumComps;
		return last;
	}
	else if (Array[last] < Array[midpoint] && Array[last] > Array[first]){				++*NumComps;
		return last;
	}

	return midpoint;
*/
}









/*HELPER FUNCTIONS*/


void swap(long *Array, int low, int high,double *NumComps, double *NumMoves)
{
	long temp = Array[high]; 	*NumMoves = *NumMoves + 1;
	Array[high] = Array[low]; 	*NumMoves = *NumMoves + 1;
	Array[low] = temp; 			*NumMoves = *NumMoves + 1;
}

