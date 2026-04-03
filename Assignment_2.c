#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

typedef enum{header, entry} tagfield;

typedef struct matrix_node *matrix_pointer;

typedef struct entry_node{
	int row;
	int col;
	int value;
} entry_node;

typedef struct matrix_node{
	matrix_pointer down;
	matrix_pointer right;
	tagfield tag;
	union{
		matrix_pointer next;
		entry_node entry;
	} u;
} matrix_node;

matrix_pointer hdnode[MAX_SIZE];

/*void matrix_show(matrix_pointer matrix)
{
	printf("%d %d %d\n", matrix->u.entry.row, matrix->u.entry.col, matrix->u.entry.value);
	for(matrix_pointer ptr1=matrix->right; ptr1!=matrix; ptr1=ptr1->u.next)
		for(matrix_pointer ptr2=ptr1->right; ptr2!=ptr1; ptr2=ptr2->right)
			printf("%d %d %d\n", ptr2->u.entry.row, ptr2->u.entry.col, ptr2->u.entry.value);
}*/

void init_headnode(int max)			//새로운 matrix 형성 시 과정에서 헤더 노드를 할당하는 과정을 따로 떼어냄
{
	for(int j=0; j<max; j++)		//max개의 header노드 할당받아 배열에 저장
	{
		hdnode[j]=(matrix_pointer)malloc(sizeof(matrix_node));
		hdnode[j]->tag=header;					//각 헤더 노드의 값 초기화
		hdnode[j]->u.next=hdnode[j];
		hdnode[j]->right=hdnode[j];
		hdnode[j]->down=NULL;
	}
}

void init_newnode_add(matrix_pointer newnode)			//새로운 matrix 형성 과정에서 새로운 노드를 추가하는 과정을 따로 뗴어냄
{
	matrix_pointer rowptr;
	newnode->tag=entry;					//newnode의 값 초기화
	newnode->down=NULL;
	newnode->right=hdnode[newnode->u.entry.row];			//새로 할당된 노드는 그 행에서 가장 오른쪽의 요소이므로 그 행의 헤더노드 가리킴

	rowptr=hdnode[newnode->u.entry.row];			//newnode가 있는 행의 원소를 가리킴
	while(rowptr->right!=hdnode[newnode->u.entry.row])			//rowptr은 기존에 그 행에서 있던 가장 오른쪽의 노드를 가리키게 된다.
		rowptr=rowptr->right;

	rowptr->right=newnode;					//행에서 기존 가장 오른쪽 노드가 newnode 가리키게 됨
	hdnode[newnode->u.entry.col]->u.next->down=newnode;			//열에서 기존 가장 아래 노드가 newnode 가리킴
	hdnode[newnode->u.entry.col]->u.next=newnode;				//열의 헤드노드가 newnode를  next로 가리키게 됨
}

void mat_init_finish(int max, matrix_pointer matrix)		//새로운 matrix 할당 시 마지막으로 헤더노드들과 엔트리 노드 간의 정리 수행
{
	for(int i=0; i<max-1; i++)
	{
		hdnode[i]->u.next->down=hdnode[i];			//열의 맨 아래 원소가 헤더노드 가리킴
		hdnode[i]->u.next=hdnode[i+1];				//헤더노드 간 연결
	}
	hdnode[max-1]->u.next->down=hdnode[max-1];
	hdnode[max-1]->u.next=matrix;					//마지막 헤더노드가 주어지는 행렬의 전체 헤더에 연결 
	matrix->right=hdnode[0];
}

void mread(matrix_pointer matrix1, matrix_pointer matrix2)		//파일에서 행렬 읽어들이는 함수
{
	int max;

	FILE *fp=fopen("input.txt", "rt");
	if(fp==NULL)
	{
		printf("file open error\n");
		return;
	}

	//첫 번째 matrix입력
	fscanf(fp, "%d %d %d", &matrix1->u.entry.row, &matrix1->u.entry.col, &matrix1->u.entry.value);				//행, 열, 원소 개수 입력
	max = (matrix1->u.entry.row>matrix1->u.entry.col) ? matrix1->u.entry.row : matrix1->u.entry.col;			//행렬을 만들기 위해 행과 열의 max값 계산

	init_headnode(max);			//헤더 노드 할당

	for(int i=0; i < matrix1->u.entry.value; i++)			//원소 개수만큼 반복
	{
		matrix_pointer newnode=(matrix_pointer)malloc(sizeof(matrix_node));				//새 엔트리 노드 할당
		fscanf(fp, "%d %d %d", &newnode->u.entry.row, &newnode->u.entry.col, &newnode->u.entry.value);		//새 노드의 기본적인 값 입력받음

		init_newnode_add(newnode);						//새 노드 연결
	}

	mat_init_finish(max, matrix1);			//연결 정리

	//두 번째 matrix 입력
        fscanf(fp, "%d %d %d", &matrix2->u.entry.row, &matrix2->u.entry.col, &matrix2->u.entry.value);
        max = (matrix2->u.entry.row>matrix2->u.entry.col) ? matrix2->u.entry.row : matrix2->u.entry.col;

	init_headnode(max);

        for(int i=0; i < matrix2->u.entry.value; i++)
        {
                matrix_pointer rowptr;
                matrix_pointer newnode=(matrix_pointer)malloc(sizeof(matrix_node));
                fscanf(fp, "%d %d %d", &newnode->u.entry.row, &newnode->u.entry.col, &newnode->u.entry.value);

		init_newnode_add(newnode);
        }

        mat_init_finish(max, matrix2);

	fclose(fp);
}

void write_matrix(FILE *fp, matrix_pointer matrix)			//파일 포인터와 행렬 주어지면 행렬을 파일에 입력
{
	fprintf(fp, "%d %d %d\n", matrix->u.entry.row, matrix->u.entry.col, matrix->u.entry.value);			//행렬의 행, 열, 원소개수
	for(matrix_pointer ptr1=matrix->right; ptr1!=matrix; ptr1=ptr1->u.next)				//모든 행에 대해 반복
		for(matrix_pointer ptr2=ptr1->right; ptr2!=ptr1; ptr2=ptr2->right)				//해당 행의 모든 원소에 대해 순차적으로 반복
			fprintf(fp, "%d %d %d\n", ptr2->u.entry.row, ptr2->u.entry.col, ptr2->u.entry.value);
}


void mwrite(matrix_pointer matrix1, matrix_pointer matrix2)			//1을 통해 파일에서 읽어들인 2개 matrix 출력
{
//	matrix_show(matrix1);
//	matrix_show(matrix2);

	FILE *fp=fopen("output.txt", "at");				//파일의 기존 내용 지우지 않음

	fprintf(fp, "Write 2 matrices that read from file\n");
	write_matrix(fp, matrix1);
	write_matrix(fp, matrix2);
	fclose(fp);
}

void madd(matrix_pointer matrix1, matrix_pointer matrix2, matrix_pointer add)			//행렬 덧셈 수행
{
	matrix_pointer ptr1 = matrix1->right;				//각 matrix의 첫 번째 헤더 노드 가리킴
	matrix_pointer ptr2 = matrix2->right;
	int count=0;				//add에 할당되는 새로운 노드의 개수를 세는 변수

	FILE *fp = fopen("output.txt", "at");
	fprintf(fp, "Addition\n");

	if(matrix1->u.entry.row != matrix2->u.entry.row || matrix2->u.entry.col!=matrix2->u.entry.col)			//크기 맞는지 체크
	{
		fprintf(fp, "#Error!! Can NOT Compute\n");
		return;
	}

	add->u.entry.row=matrix1->u.entry.row;				//새로운 add matrix의 행, 열 개수 할당
	add->u.entry.col=matrix1->u.entry.col;

	int max = (matrix1->u.entry.row>matrix1->u.entry.col) ? matrix1->u.entry.row : matrix1->u.entry.col;			//add matrix 행, 열 최대값 구함

	init_headnode(max);			//add 위한 헤더노드 초기화

	for(; ptr1!= matrix1 && ptr2!= matrix2; ptr1=ptr1->u.next, ptr2=ptr2->u.next)			//각 열에 대한 루프-두 행렬의 크기 동일하므로 서로 같은 열에 대해 계산됨
	{
		for(ptr1=ptr1->down, ptr2=ptr2->down; ptr1->tag != header && ptr2->tag != header;)		//해당 열의 각 원소에 대한 루프-둘 중 하나가 끝에 도달할 때까지: 연결 리스트로 구현된 다항식 덧셈과 동일 알고리즘
		{
			if(ptr1->u.entry.row < ptr2->u.entry.row)			//행렬 1의 원소가 더 위에 있는 경우 그 원소 add에 대입
			{
				//add에 대입할 노드 할당 후 초기화해 대입
				matrix_pointer newnode=(matrix_pointer)malloc(sizeof(matrix_node));
				newnode->u.entry.row=ptr1->u.entry.row;
				newnode->u.entry.col=ptr1->u.entry.col;
				newnode->u.entry.value=ptr1->u.entry.value;
				init_newnode_add(newnode);
				ptr1=ptr1->down;					//행렬 1의 다음 원소 가리킴
				count++;
			}
			else if(ptr1->u.entry.row > ptr2->u.entry.row)			//행렬 2의 원소가 더 위에 있는 경우 그 원소 add에 대입
			{
				//add에 대입할 노드 할당 후 초기화해 대입
				matrix_pointer newnode=(matrix_pointer)malloc(sizeof(matrix_node));
				newnode->u.entry.row=ptr2->u.entry.row;
				newnode->u.entry.col=ptr2->u.entry.col;
				newnode->u.entry.value=ptr2->u.entry.value;
				init_newnode_add(newnode);
				ptr2=ptr2->down;				//행렬 2의 다음 원소 가리킴
				count++;
			}
			else					//두 행렬에서 원소 위치 같고 합이 0이 아닌 경우 합을 add에 대입
			{
				if(ptr1->u.entry.value+ptr2->u.entry.value !=0)
				{
					matrix_pointer newnode=(matrix_pointer)malloc(sizeof(matrix_node));
					newnode->u.entry.row=ptr1->u.entry.row;
					newnode->u.entry.col=ptr1->u.entry.col;
					newnode->u.entry.value=ptr1->u.entry.value+ptr2->u.entry.value;
					init_newnode_add(newnode);
					count++;
				}
				ptr1=ptr1->down;			//이때 두 행렬에서 모두 계산되었으므로 두 행렬에서 모두 다음 원소로 넘어간다.
				ptr2=ptr2->down;
			}
		}
		while(ptr1->tag!=header)				//행렬 1 끝까지 add에 대입
		{
			//add에 대입
			matrix_pointer newnode=(matrix_pointer)malloc(sizeof(matrix_node));
			newnode->u.entry.row=ptr1->u.entry.row;
			newnode->u.entry.col=ptr1->u.entry.col;
			newnode->u.entry.value=ptr1->u.entry.value;
			init_newnode_add(newnode);
			ptr1=ptr1->down;
			count++;
		}

		while(ptr2->tag!=header)			//행렬 2 끝까지 add에 대입
		{
			matrix_pointer newnode=(matrix_pointer)malloc(sizeof(matrix_node));
			newnode->u.entry.row=ptr2->u.entry.row;
			newnode->u.entry.col=ptr2->u.entry.col;
			newnode->u.entry.value=ptr2->u.entry.value;
			init_newnode_add(newnode);
			ptr2=ptr2->down;
			count++;
		}

	}

	add->u.entry.value=count;
        mat_init_finish(max, add);				//add 연결 정리

	write_matrix(fp, add);				//출력
	fclose(fp);
//	matrix_show(add);
}

void mMulti(matrix_pointer matrix1, matrix_pointer matrix2, matrix_pointer product)			//행렬 간 곱셈
{
	matrix_pointer ptr1=NULL;			//각각 행렬 1, 2 가리키는 용도의  포인터
	matrix_pointer ptr2=NULL;
	int count=0;

	FILE *fp=fopen("output.txt", "at");
	fprintf(fp, "Multiplication\n");

	if(matrix1->u.entry.col != matrix2->u.entry.row)		//크기 확인
	{
		fprintf(fp, "#Error!! Can NOT Compute\n");
		return;
	}

	product->u.entry.row=matrix1->u.entry.row;				//product 행렬의 행, 열 초기화
	product->u.entry.col=matrix2->u.entry.col;
	int max = (matrix1->u.entry.row>matrix1->u.entry.col) ? matrix1->u.entry.row : matrix1->u.entry.col;			//헤더노드 할당 위한 max 값

	init_headnode(max);			//헤더노드 할당

	//행렬곱 연산 진행
	for(ptr1=matrix1->right; ptr1 != matrix1; ptr1=ptr1->u.next)			//행렬 1의 각 행에 대해 반복
	{
		for(ptr2=matrix2->right; ptr2 != matrix2; ptr2=ptr2->u.next)			//행렬 2의 각 열에 대해 반복
		{
			matrix_pointer a=ptr1->right;
			matrix_pointer b=ptr2->down;
			int newnode_value=0;

			while(a->tag!=header&&b->tag!=header)				//행렬곱 연산을 진행할 때, 주어진 행과 열에서 서로 대응되는 요소가 있을 경우 곱해서 reduction.
			{
				if(a->u.entry.col < b->u.entry.row)
					a=a->right;
				else if(a->u.entry.col > b->u.entry.row)
					b=b->down;
				else
				{
					newnode_value+=a->u.entry.value*b->u.entry.value;		//서로 대응되는 경우
					a=a->right;
					b=b->down;
				}
			}

			if(newnode_value!=0)				//주어진 행과 열에 대해 곱연산을 마치고 나서 0이 아닌 경우 새로운 노드 만들어 product에 대입
			{
				matrix_pointer newnode = (matrix_pointer)malloc(sizeof(matrix_node));
				newnode->u.entry.row=ptr1->right->u.entry.row;				//새로운 노드의 행은 연산을 진행한 행렬1의  행
				newnode->u.entry.col=ptr2->down->u.entry.col;				//새로운 노드의 열은 연산을 진행한 행렬2의 열
				newnode->u.entry.value=newnode_value;
				init_newnode_add(newnode);
				count++;
			}
		}
	}
	product->u.entry.value=count;				//product의 원소개수 대입
	mat_init_finish(max, product);				//product 내 연결 정리
//	matrix_show(product);
	write_matrix(fp, product);				//출력
	fclose(fp);
}

int main(void)
{
	matrix_node matrix1;
	matrix_node matrix2;
	matrix_node add;
	matrix_node product;
	int n;
	FILE *fp=fopen("output.txt", "wt");				//혹시라도 기존의 파일에 있을 내용을 지워주기 위함
	fclose(fp);

	while(1)
	{
		printf("=======================M E N U=========================\n");
		printf("1. Matrix Read\n");
		printf("2. Matrix Write\n");
		printf("3. Matrix Add\n");
		printf("4. Matrix Multiply\n");
		printf("5. Program Exit\n");
		printf("=======================================================\n");
		printf("Select a menu number:");

		scanf("%d", &n);

		switch(n)
		{
		case 1 :					//파일에서 읽어들임
			mread(&matrix1, &matrix2);
			break;
		case 2 :					//읽어들인 행렬을 파일에 씀
			mwrite(&matrix1, &matrix2);
			break;
		case 3 :
			madd(&matrix1, &matrix2, &add);			//덧셈
			break;
		case 4 :
			mMulti(&matrix1, &matrix2, &product);			//곱셈
			break;
		case 5 :					//프로그램 종료
			return 0;
		}
	}
	return 0;
}
