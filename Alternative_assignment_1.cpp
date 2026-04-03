#include <stdio.h>
#define B_SIZE 5 

int bingoCheck(int (*bingoField)[B_SIZE]);
int playBingo(int *delNums, int (*bingoField)[B_SIZE]); 

int main(void)
{
	int bingoField[B_SIZE][B_SIZE]; 			//빙고판 배열  
	int delNums[B_SIZE*B_SIZE];				//사회자가 	부르는 수 배열  
	int count = 0;

	//bingoField에 빙고 판 숫자를, delNums에 사회자가 부르는 숫자를 집어넣는다.
	FILE* inputptr;
	inputptr = fopen("input.txt", "rt");				//main 함수 내에서 파일 입출력을 통해 입력받음  
	if (inputptr == NULL)			//에러 처리 
	{
		printf("input error");
		return 1;
	}
	
	for(int i = 0; i<B_SIZE; i++)						//빙고판 숫자 저장 
		for (int j = 0; j < B_SIZE; j++)
		{
			fscanf(inputptr, "%d", &bingoField[i][j]);
		}
			
	fgetc(inputptr);			//input.txt 중간의 빈 줄 때문. 이 라인이 없어도 프로그램이 잘 작동하기는 한다. 
	for (int i = 0; i < B_SIZE*B_SIZE; i++)
		fscanf(inputptr, "%d", &delNums[i]);

	fclose(inputptr);

	count = playBingo(delNums, bingoField);				//bingo게임을 실행하여 3줄이 지워지는 게 몇 번째인지 카운트 
	
	//출력 
	FILE* outputPtr;
	outputPtr = fopen("output.txt", "wt");
	fprintf(outputPtr, "%d", count);
	fclose(outputPtr);
	return 0;
}


int bingoCheck(int(*bingoField)[B_SIZE])				//줄이 3개 이상인지 체크하는 함수이다. 
{
	int count = 0;
	int bingoChecker = 0, bingoChecker2 = 0; 
	for (int i = 0; i < B_SIZE; i++)
	{
		for (int j = 0; j < B_SIZE; j++)					//가로방향 줄과 세로방향 줄을 동시에 처리해 루프 개수를 줄인다. 
		{																//OR연산을 통해 연속된 5칸이 모두 0인지 확인. 모두 0이면 한 줄이 지워진 것이다.
			bingoChecker = bingoChecker || bingoField[i][j];			  //가로를 체크 
			bingoChecker2 = bingoChecker2 || bingoField[j][i];			//세로를 체크 
		}
		
		if (bingoChecker == 0) 
			count++;
		if (bingoChecker2 == 0)
			count++;
		bingoChecker = 0;
		bingoChecker2 = 0;
	}

	for (int i = 0; i < B_SIZE; i++)					//두 방향의 대각선을 동시에 처리해 루프의 개수를 줄인다. 
	{
		bingoChecker = bingoChecker || bingoField[i][i];
		bingoChecker2 = bingoChecker2 || bingoField[i][B_SIZE - 1 - i];
	}

	if (bingoChecker == 0)
		count++;
	if (bingoChecker2 == 0)
		count++;
	
	if (count > 2)
		return 1;
	else
		return 0;

}

int playBingo(int *delNums, int (*bingoField)[B_SIZE])				//사회자가 부른 숫자를 지워나가면서 빙고인지 체크하는 함수 
{
	int count = 11;							//5*5에서 3줄이 만들어지기 위해서는 최소 12개의 숫자를 불러야 한다. 
	for (int i = 0; i < 11; i++)				//bingoCheck 함수를 덜 호출할 수 있다. 
		for (int j = 0;; j++)
			if (*(bingoField[0] + j) == delNums[i])
			{
				*(bingoField[0] + j) = 0;				//빙고판 배열에서 불러진 숫자 0으로 만들어 줌 
				break;
			}

	for (int i = 11;; i++)
	{
		for (int j = 0;; j++)
			if (*(bingoField[0] + j) == delNums[i])
			{
				*(bingoField[0] + j) = 0;
				break;
			}
		count++;
		if (bingoCheck(bingoField) == 1)
			break;
	}
	return count;
}
