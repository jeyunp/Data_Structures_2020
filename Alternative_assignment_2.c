#include <stdio.h>
#include <stdlib.h>

int count = 0;				//출력 문자열 개수 count

void sort(char input_string[], int l_end, int r_end)			//집어넣은 문자열을 quick sort
{
	int i = l_end;
	int j= r_end;
	char temp;

	int pivot = (i+j)/2;				//중앙에 pivot 정함
	
	while(i<j)
	{
		while(input_string[i]<input_string[pivot])			//pivot보다 큰 값인데 pivot 앞에 있는 값 찾기
			i++;
		while(input_string[j]>input_string[pivot])			//pivot 보다 작은 값인데 pivot 뒤에 있는 값 찾기
			j--;

		temp=input_string[i];
		input_string[i]=input_string[j];
		input_string[j]=temp;
		i++;
		j--;
	}

	if(l_end<j)						//pivot 앞부분에 대해 위의 연산 재실행(pivot이 왼쪽 끝에 다다를 때까지)
		sort(input_string, l_end, j);
	if(r_end>i)						//pivot 뒷부분에 대해 위의 연산 재실행(pivot이 오른쪽 끝에 다다를 때까지)
		sort(input_string, i, r_end);
}

/*
입력된 문자열은 이미 정렬되었으므로, m길이의 문자열에서 n 길이의 조합을 만든다고 생각하면 좋다.
mask를 만들고 mask에 맞추어 문자열 출력
*/
void printString(char input_string[], int m, int n, int length, int mask[])
{
	int checkVowel=0;				//모음있는지 체크 위한 변수
	int checkConsonant=0;					//자음있는지 체크 위한 변수
	if(n==0)							//n개의 글자를 모두 고른 경우
	{
		for(int i=length-m; i<length; i++)		//mask를 공유하므로 오류를 없애기 위함.
			mask[i]=0;

		for(int i=0; i<length; i++)			//자음, 모음 체크
		{
			if(mask[i] !=0)
			{
				if(input_string[i] == 'a' || input_string[i]=='e' || input_string[i] =='i' || input_string[i]=='o' || input_string[i] =='u')
					checkVowel=1;
				else
					checkConsonant=1;
			}
		}

		if(checkVowel && checkConsonant)			//자음, 모음 둘 다 있을 경우에만 실행
		{
			for(int i=0; i<length; i++)
			{
				if(mask[i] != 0)			//mask에서 걸러서 출력
					printf("%c", input_string[i]);
			}
			printf("\n");
			count++;				//출력 카운트 +1
		}
		return;
	}

	if(m==n)					//남아 있는 모든 글자를 출력하게 되는 경우
	{
		for (int i=0; i<n; i++)			//남아 있는 글자를 모두 출력하도록 마스크 설정
			mask[length-m+i]=1;

		for(int i=0; i<length; i++)			//자음, 모음 체크
		{
			if(mask[i] !=0)
			{
				if(input_string[i] == 'a' || input_string[i]=='e' || input_string[i] =='i' || input_string[i]=='o' || input_string[i] =='u')
					checkVowel=1;
				else
					checkConsonant=1;
			}
		}

		if(checkVowel && checkConsonant)			//자음, 모음 모두 있는 경우에
		{
			for(int i=0; i<length; i++)
			{
				if(mask[i] !=0)					//마스크로 걸러서 출력
					printf("%c", input_string[i]);
			}
			printf("\n");				//출력 카운트 +1
			count++;
		}
		return;
	}
	
	//특정 요소를 포함하는 문자열/ 포함하지 않는 문자열로 나눔
	mask[length-m]=1;					//사전식 출력이므로 앞쪽 글자가 나오는 문자열이 먼저 출력되어야 함.
	printString(input_string, m-1, n-1, length, mask);

	mask[length-m]=0;
	printString(input_string, m-1, n, length, mask);
}

int main(void)
{
	int m, n;
	char * input_string;
	int * mask;

	do					//N, M 값을 받는다.
	{
		printf("N: ");
		scanf("%d", &n);
		printf("M: ");
		scanf("%d", &m);
	}while(m<n);				//M<N일 경우 반복

	input_string = (char *)malloc(sizeof(char)*m);				//글자 입력받을 배열 할당.
	mask = (int *)calloc(m, sizeof(int));					//printString함수에서 사용할 출력할 글자를 고르는 mask용 배열 할당

	printf("문자 입력:");
	for(int i = 0; i<m; i++)
		scanf(" %c", &input_string[i]);					//%c 앞에 공백을 두는 것으로 입력버퍼에 남아있는 엔터가 들어가지 않도록 함.

	sort(input_string, 0, m-1);						//입력받은 문자들을 오름차순 정렬

	printString(input_string, m, n, m, mask);				//조건에 맞춰 출력

	free(input_string);							//동적 할당한 메모리 해제
	free(mask);
	printf("%d개",count);							//몇 개의 문자열 출력했는지 출력
	return 0;
}
