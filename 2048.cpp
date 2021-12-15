#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#define row 4 //you can change the matrix row here
#define column 4 //you can change the matrix column here

int slide_right(int array[][column],int *x){
	int point=0;
	for(int i=0;i<row;i++){
		int operation=1;
		while(operation==1){
			operation=0;
			for(int j=column-1;j>0;j--){
				if(array[i][j]==0&&array[i][j-1]!=0){
					array[i][j]=array[i][j-1];
					array[i][j-1]=0;
					*x=1;
					operation=1;
				}
			}
		}
		for(int j=column-1;j>0;j--){
			if(array[i][j]==array[i][j-1]&&array[i][j]!=0){
				array[i][j]+=array[i][j-1];
				point+=array[i][j];//Her birleþmede puan eklemesi için
				*x=1;
				array[i][j-1]=0;
			}
		}
		operation=1;
		while(operation==1){
			operation=0;
			for(int j=column-1;j>0;j--){
				if(array[i][j]==0&&array[i][j-1]!=0){
					array[i][j]=array[i][j-1];
					array[i][j-1]=0;
					*x=1;
					operation=1;
				}
			}
		}	
	}
	return point;
}
int slide_left(int array[][column],int *x){
	int point=0;
	for(int i=0;i<row;i++){
		int operation=1;
		while(operation==1){
			operation=0;
			for(int j=0;j<column-1;j++){
				if(array[i][j]==0&&array[i][j+1]!=0){
					array[i][j]=array[i][j+1];
					array[i][j+1]=0;
					*x=1;
					operation=1;
				}
			}
		}
		for(int j=0;j<column-1;j++){
			if(array[i][j]==array[i][j+1]&&array[i][j]!=0){
				array[i][j]+=array[i][j+1];
				point+=array[i][j];
				*x=1;
				array[i][j+1]=0;
			}
		}
		operation=1;
		while(operation==1){
			operation=0;
			for(int j=0;j<column-1;j++){
				if(array[i][j]==0&&array[i][j+1]!=0){
					array[i][j]=array[i][j+1];
					array[i][j+1]=0;
					*x=1;
					operation=1;
				}
			}
		}	
	}
	return point;
}

int slide_down(int array[][column],int *x){
	int point=0;
	for(int j=0;j<column;j++){
		int operation=1;
		while(operation==1){
			operation=0;
				for(int i=row-1;i>0;i--){
					if(array[i][j]==0&&array[i-1][j]!=0){
						array[i][j]=array[i-1][j];
						array[i-1][j]=0;
						*x=1;
						operation=1;
					}
				}
		}
		for(int i=row-1;i>0;i--){
			if(array[i][j]==array[i-1][j]&&array[i][j]!=0){
				array[i][j]+=array[i-1][j];
				point+=array[i][j];
				*x=1;
				array[i-1][j]=0;
			}
		}
		operation=1;
		while(operation==1){
			operation=0;
			for(int i=row-1;i>0;i--){
				if(array[i][j]==0&&array[i-1][j]!=0){
					array[i][j]=array[i-1][j];
					array[i-1][j]=0;
					*x=1;
					operation=1;
				}
			}
		}	
	}
	return point;
}
int slide_up(int array[][column],int *x){
	int point=0;
	for(int j=0;j<column;j++){
		int operation=1;
		while(operation==1){
			operation=0;
				for(int i=0;i<row-1;i++){
					if(array[i][j]==0&&array[i+1][j]!=0){
						array[i][j]=array[i+1][j];
						array[i+1][j]=0;
						*x=1;
						operation=1;
					}
				}
		}
		for(int i=0;i<row-1;i++){
			if(array[i][j]==array[i+1][j]&&array[i][j]!=0){
				array[i][j]+=array[i+1][j];
				point+=array[i][j];
				*x=1;
				array[i+1][j]=0;
			}
		}
		operation=1;
		while(operation==1){
			operation=0;
				for(int i=0;i<row-1;i++){
					if(array[i][j]==0&&array[i+1][j]!=0){
						array[i][j]=array[i+1][j];
						array[i+1][j]=0;
						*x=1;
						operation=1;
					}
				}
		}
	}
	return point;
}
int check_end(int array[][column]){
	int check_arr[row][column];
	for(int i=0;i<row;i++){
		for(int j=0;j<column;j++)
		check_arr[i][j]=array[i][j];
	}
	int x=0;
	slide_up(check_arr,&x);
	slide_down(check_arr,&x);
	slide_left(check_arr,&x);
	slide_right(check_arr,&x);
	return x;
}


int main(){
	srand(time(NULL));
	int score=0;
	int matrix[row][column]={0};
	//generating random 2 and 4 with %70 %30 into 0 value
	int rand_r=rand()%row;
	int rand_c=rand()%column;
	if(rand()%10<7)
	matrix[rand_r][rand_c]=2;
	else
	matrix[rand_r][rand_c]=4;
	
	//scores
	printf("Score:%d\n",score);
	//graphic
	for(int i=0;i<column*5+1;i++)
		printf("-");
	printf("\n");
	for(int i=0;i<row;i++){
		for(int j=0;j<column;j++){
		printf("|");
		if(matrix[i][j]==0){
			printf("    ");
		}
		else
		printf("%4d",matrix[i][j]);
		}
		printf("|\n");
		for(int i=0;i<column*5+1;i++)
		printf("-");
		printf("\n");
	}
	printf("Press 'W'for up,'S'for down,'D'for right,'A'for left. Please use uppercase characters.\n");
	do{
		//Move
		char move;
		do{
		printf("Move? ");
		move=getch();
		printf("%c\n",move);
		if(move!='W'&&move!='A'&&move!='S'&&move!='D')
		printf("Undeclared input, please give a proper input.Remember to use uppercase letters.\n");
		}while(move!='W'&&move!='A'&&move!='S'&&move!='D');
		int x=0;
		switch(move){
			case 'W':score+=slide_up(matrix,&x);
			break;
			case 'S':score+=slide_down(matrix,&x);
			break;
			case 'D':score+=slide_right(matrix,&x);
			break;
			case 'A':score+=slide_left(matrix,&x);
			break;
		}
		//generating random 2 and 4 with %70 %30 into 0 value	
		while(x==1){//eðer x 1 deðilse yani hareketle hiç bi deðiþiklik olmadýysa yeni random sayý üretmemesi için
			int rand_r=rand()%row;
			int rand_c=rand()%column;
			if(matrix[rand_r][rand_c]==0){
				if(rand()%10<7)
				matrix[rand_r][rand_c]=2;
				else
				matrix[rand_r][rand_c]=4;
				x=0;//eðer if döngüsünde matrixte 0 deðeri bulamazsa x 0 olmuyo while döngüsü bozulmuyo ve 0 deðeri bulup oraya random 2 veya 4 atýyana kadar döngü devam ediyo.
			}
		}
		//oyunun çizimi
		for(int i=0;i<column*5+1;i++)
			printf("-");
		printf("\n");
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
			printf("|");
			if(matrix[i][j]==0){
				printf("    ");
			}
			else
			printf("%4d",matrix[i][j]);
			}
			printf("|\n");
			for(int i=0;i<column*5+1;i++)
			printf("-");
			printf("\n");
			
		}
		//scores
		printf("Score:%d\n",score);
		/*testing if there is no 0 values.
		 If there is no 0 values it will check if there is a valid move.
		 If there is no valid mýove game will over.*/
		int fulled=1;
		for(int i=0;i<row;i++){
			for(int j=0;j<column;j++){
				if(matrix[i][j]==0){
					fulled=0;
				}
			}
		}
		if(fulled==1){
			if(check_end(matrix)==0){
				printf("GAME IS OVER!");
				break;//it will break the do-while loop
			}
		}
		
	}while(1);
	
	return 0;
	
}
