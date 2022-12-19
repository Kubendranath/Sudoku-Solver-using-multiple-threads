/* Do not make any changes in the skeleton. Your submission will be invalidated if the skeleton is changed */


# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
#include<pthread.h>

struct data{
    int size;
    int n;
    int m;
    };   
 

int answer[36][36];
void read_grid_from_file(int size, char *ip_file, int grid[36][36]) {
	FILE *fp;
	int i, j;
	fp = fopen(ip_file, "r");
	for (i=0; i<size; i++) 
		for (j=0; j<size; j++) {
			fscanf(fp, "%d", &grid[i][j]);
	}
} 

void print_grid(int size, int grid[36][36]) {
	int i, j;
	/* The segment below prints the grid in a standard format. Do not change */
	for (i=0; i<size; i++) {
		for (j=0; j<size; j++)
			printf("%d\t", grid[i][j]);
		printf("\n");
	}
}
   
   
 void *isValid(void* args){
 	struct data *d = args;
 	int * value = malloc(sizeof(int));
 	*value =0;
 	int size = d->size;
 	int n = d->n;
 	int m = d->m;
 	
        int row[size+1], col[size+1], box[size+1];
        for(int i=0;i<=size;i++){
            row[i]=0;
            col[i]=0;
            box[i]=0;
        }
        for(int j=0;j<size;j++){
            if(answer[n][j]==0)continue;
            row[answer[n][j]]++;
            if(row[answer[n][j]]>1)return (void*) value;
        }
        for(int i=0;i<size;i++){
             if(answer[i][m]==0)continue;
            col[answer[i][m]]++;
            if(col[answer[i][m]]>1)return (void*) value;
        }
        n = n - (n%(int)sqrt(size)) ;
        m = m - (m%(int)sqrt(size));
        for(int i=n;i<n+sqrt(size);i++){
            for(int j=m;j<m+sqrt(size);j++){
                if(answer[i][j]==0)continue;
                box[answer[i][j]]++;
                if( box[answer[i][j]]>1)return (void*) value;
            }
        }
        *value = 1;
        return (void*) value;
}

 bool solve_sudoku(int size,int i, int j){
      if(i==size)return true;
      if(j==size)return solve_sudoku(size,i+1,0);
      if(answer[i][j]!=0)return solve_sudoku(size,i,j+1);
      struct data arg[size+1];
      pthread_t p[size+1];
      
      for(int ch=1;ch<=size;ch++){
      	 arg[i].size  =  size;
      	 arg[i].n  =  i;
      	 arg[i].m  =  j;
         answer[i][j]=ch;
         int *val;
         pthread_create(&p[i],NULL,&isValid,(void*)&arg[i]); 
         pthread_join(p[i],(void**) &val);
         
         if(*val){
             if(solve_sudoku(size,i,j+1))return true; 
         } 
          answer[i][j]=0;
          free(val);
      
      }
      
      return false;
}


int main(int argc, char *argv[]) {
	int grid[36][36], size, i, j;
	
	if (argc != 3) {
		printf("Usage: ./sudoku.out grid_size inputfile");
		exit(-1);
	}
	
	size = atoi(argv[1]);
	read_grid_from_file(size, argv[2], grid);
	
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            answer[i][j] = grid[i][j]; 
        }
    }

	solve_sudoku(size,0,0);
	
	print_grid(size, answer);
}
