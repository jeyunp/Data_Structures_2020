#define MAX_TERMS 100
#include <stdio.h>
#include <stdlib.h>

typedef struct term{
	int degree;
	int coef;
	struct term * link;
}term; 				//각 polynomial들의  term이 되는 구조체

typedef struct{
	int length;			//이번 과제에서는 사용할 일이 없었지만 추후 이 코드를 업그레이드한다고 할 때 필요할 수 있다.
	term *head;
	term *tail;
}ListHeader;			//polynomial들은 linked list 로 구현된다. 

void poly_init(ListHeader *poly)
{
	poly->length=0;
	poly->head=NULL;
	poly->tail=NULL;
}				//리스트 초기화

void free_poly(ListHeader *poly)
{
	term *delptr = poly->head;
        while(delptr!=NULL)				//주어진 리스트의 head 부터 차례대로 메모리 할당 해제해 나감
        {
                term *link = delptr->link;
                free(delptr);
                delptr=link;
        }
}				//리스트를 모두 사용하고 나면 메모리 동적 할당을 해제해 준다.

void insert_node_last(ListHeader *poly, int coef, int degree)			//주어진 리스트(다항식)의 맨 뒤에 새로운 항(노드) 삽입
{
	term *temp = (term *)malloc(sizeof(term));				//새로운 term위한 메모리 할당
	if(temp == NULL)
	{
		printf("Memory allocation error");
		return ;
	}
	temp->coef=coef;				//주어진 계수와 차수 값을 적용
	temp->degree=degree;
	temp->link=NULL;
	if(poly->tail == NULL)				//리스트가 빈 경우와 비지 않은 경우를 나누어 적용
	{
		poly->head=temp;
		poly->tail=temp;
	}
	else
	{
		poly->tail->link=temp;
		poly->tail=temp;
	}
	poly->length++;
}

ListHeader add(ListHeader *poly1, ListHeader *poly2)
{
	term *ptr1=poly1->head;
	term *ptr2=poly2->head;

	ListHeader poly_add;				//계산결과를 이 polynomial에 대입
	poly_init(&poly_add);
	while(ptr1 != NULL && ptr2 != NULL)				//두 리스트를 읽어들이면서 모두 끝에 도달하지 않은 경우
	{
		if(ptr1->degree>ptr2->degree)				//두 polynomial에서 현재 가리켜지는 term 중 차수가 킅 term은 그대로 대입
		{
			insert_node_last(&poly_add, ptr1->coef, ptr1->degree);
			ptr1=ptr1->link;				//이때 선택된 polynomial에서 다음 term으로 넘어감
		}
		else if(ptr1->degree < ptr2->degree)
		{
			insert_node_last(&poly_add, ptr2->coef, ptr2->degree);
			ptr2=ptr2->link;
		}
		else
		{

			if(ptr1->coef+ptr2->coef!=0)				//두 다항식에서 현재 가리켜지는 항의 차수가 같고 계수의 합이 0이 아닌 경우
				insert_node_last(&poly_add, ptr1->coef+ptr2->coef, ptr1->degree);		//두 계수 더해서 대입
			ptr1=ptr1->link;
			ptr2=ptr2->link;
		}

	}
	while(ptr1!=NULL)				//ptr2가 위에서 끝에 먼저 도달한 경우
	{
		insert_node_last(&poly_add, ptr1->coef, ptr1->degree);
		ptr1=ptr1->link;
	}
	while(ptr2!=NULL)				//ptr1이 위에서 먼저 끝에 도달한 경우
	{
		insert_node_last(&poly_add, ptr2->coef, ptr2->degree);
		ptr2=ptr2->link;
	}
	return poly_add;
}

ListHeader product(ListHeader *poly1, ListHeader *poly2)
{
        term *ptr1=poly1->head;

        ListHeader poly_prod;
	ListHeader poly_temp;
        poly_init(&poly_prod);

	for(;ptr1!=NULL;ptr1=ptr1->link)
	{
		poly_init(&poly_temp);
		term *ptr2=poly2->head;
		for(;ptr2!=NULL; ptr2=ptr2->link)			//poly_temp 다항식에 poly1의 어느 한 항과 poly2곱한 식 대입
		{
			int coef = ptr1->coef * ptr2->coef;
			int degree = ptr1->degree+ptr2->degree;
			insert_node_last(&poly_temp, coef, degree);
		}
		poly_prod=add(&poly_temp, &poly_prod);			//poly_prod에 poly1의 각 항에 대한 poly_temp 다항식을 모두 더함

		free_poly(&poly_temp);

	}

	return poly_prod;
}

int main(void)
{
	ListHeader poly1;
	ListHeader poly2;
	ListHeader poly_add;
	ListHeader poly_product;
	int coef, degree;
	term *ptr=NULL;;

	poly_init(&poly1);
	poly_init(&poly2);
	poly_init(&poly_add);
	poly_init(&poly_product);


	FILE *fp1 = fopen("input.txt", "rt");
	if(fp1==NULL)
	{
		printf("File open error!\n");
		return 1;
	}
	
	while(1)	//첫 번째 polynomial 입력받기. \n을 만나면 루프를 탈출
	{
		if(fgetc(fp1) == '\n')
			break;
		fseek(fp1, -1, SEEK_CUR);
		
		fscanf(fp1, " %d %d", &coef, &degree);
		insert_node_last(&poly1, coef, degree);
		fgetc(fp1);
	}

	while(1)		//두 번째 polynomial 입력받기. EOF 만나면 루프를 탈출
	{
		if(fscanf(fp1, "%d %d", &coef, &degree) == EOF)
			break;
		insert_node_last(&poly2, coef, degree);
	}
	fclose(fp1);


	poly_add=add(&poly1, &poly2);			//다항식 덧셈 계산
	
	poly_product=product(&poly1, &poly2);			//다항식 곱셈 계산

	FILE *fp2 = fopen("output.txt", "wt");
	if(fp2==NULL)
	{
		printf("File open error!\n");
		return 1;
	}

	ptr=poly_add.head;
	fprintf(fp2, "Addition\n");				//다항식 덧셈 출력
	for(;ptr!=NULL; ptr=ptr->link)				//각 term들에 순차적으로 접근
		fprintf(fp2, "%d %d\n", ptr->coef, ptr->degree);

	ptr=poly_product.head;					//다항식 곱셈 출력
	fprintf(fp2, "Multiplication\n");
	for(;ptr!=NULL; ptr=ptr->link)
		fprintf(fp2, "%d %d\n", ptr->coef, ptr->degree);
	fclose(fp2);

	free_poly(&poly1);
	free_poly(&poly2);
	free_poly(&poly_add);
	return 0;
}

