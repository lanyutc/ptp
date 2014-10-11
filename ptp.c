#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

/********************************
 *
 * pointer to pointer test for array
 *
 **************/
void test_ptp_array(int **test_array, const int row, const int column)
{
	int i, j;
	for (i = 0; i < column; i++) {
		for (j = 0; j < row; j++) {
			printf("&array[%d][%d]=0x%08x array[%d][%d]=%d\n",
					i,j,&test_array[i][j],i,j,test_array[i][j]);
		}
	}
}

void test_p_array(int *test_array, const int row, const int column)
{
	int i, j;
	for (i = 0; i < column; i++) {
		for (j = 0; j < row; j++) {
			int idx = i * row + j;
			printf("&array[%d]=0x%08x array[%d]=%d\n",
					idx, &test_array[idx], idx, test_array[idx]);
		}
	}
}

void test_array()
{
	/* allocate m*n int array */
	const int m = 2, n = 5;
	int **array = NULL;
	array = (int **)malloc(m * sizeof(int *));

	int i, j;
	for (i = 0; i < m; i++) {
		array[i] = (int *)malloc(n * sizeof(int));
	}

	int idx = 0;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			array[i][j] = idx++;
		}
	}

	printf("======================== array info\n");
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("&array[%d][%d]=0x%08x array[%d][%d]=%d\n",
					i,j,&array[i][j],i,j,array[i][j]);
		}
	}
	

	printf("\n======================== malloc array\n");
	test_ptp_array(array, n, m);


	printf("\n======================== fixed array\n");
	int test_array[2][5] = {
		{1,2,3,4,5},{6,7,8,9,10}
	};
	test_p_array((int *)test_array, n ,m);


	for (i = 0; i < m; i++) {
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
}



/********************************
 *
 * pointer to pointer test for list
 *
 **************/
struct list_t {
	int value_a;
	int value_b;
	struct list_t *next;
};

/* return value must be struct list_t */
struct list_t *common_list_del(struct list_t *head, const int a, const int b)
{
	struct list_t *cur = head;
	struct list_t *prev = NULL;
	while (cur) {
		struct list_t * const next = cur->next;
		if (cur->value_a == a && cur->value_b == b) {
			if (prev) {
				prev->next = next;
			} else {
				head = next;
			}
			/* free cur */
		} else {
			prev = cur;
		}
		cur = next;
	}

	return head;
}

/* return value could be void or struct list_t*/
struct list_t *ptp_list_del(struct list_t **head, const int a, const int b)
{
	struct list_t **cur = head;
	while (*cur) {
		struct list_t *entry = *cur;
		if (entry->value_a == a && entry->value_b == b) {
			*cur = entry->next;
			/* free entry */
		} else {
			cur = &(entry->next);
		}
	}

	return *head;
}

/* return value could be void or struct list_t*/
struct list_t *references_list_del(struct list_t *&head, struct list_t *cp_head, const int a, const int b)
{
	struct list_t *cur = cp_head;
	struct list_t *prev = NULL;
	while (cur) {
		struct list_t * const next = cur->next;
		if (cur->value_a == a && cur->value_b == b) {
			if (prev) {
				prev->next = next;
			} else {
				head = next;
			}
			/* free cur*/
		} else {
			prev = cur;
		}
		cur = next; 
	}
	return head;
}

void test_list()
{
	struct list_t A;
	struct list_t B;
	struct list_t C;
	struct list_t D;

	A.value_a = 1;
	A.value_b = 2;
	A.next = &B;

	B.value_a = 3;
	B.value_b = 4;
	B.next = &C;

	C.value_a = 5;
	C.value_b = 6;
	C.next = &D;

	D.value_a = 7;
	D.value_b = 8;
	D.next = NULL;

	printf("\n======================== list info\n");
	struct list_t *entry = &A;
	for ( ; entry != NULL; entry = entry->next) {
		printf("a:%d b:%d\n",entry->value_a,entry->value_b);
	}

	printf("\n======================== DEL A(1,2)\n");
	struct list_t *head = &A;
	head = common_list_del(head, A.value_a, A.value_b);
	entry = head;
	for ( ; entry != NULL; entry = entry->next) {
		printf("a:%d b:%d\n",entry->value_a,entry->value_b);
	}

	printf("\n======================== DEL C(5,6)\n");
	head = ptp_list_del(&head, C.value_a, C.value_b);
	entry = head;
	for ( ; entry != NULL; entry = entry->next) {
		printf("a:%d b:%d\n",entry->value_a,entry->value_b);
	}

	printf("\n======================== DEL D(7,8)\n");
	references_list_del(head, head, D.value_a, D.value_b);
	entry = head;
	for ( ; entry != NULL; entry = entry->next) {
		printf("a:%d b:%d\n",entry->value_a,entry->value_b);
	}
}



/********************************
 *
 * pointer to pointer test for malloc
 *
 **************/
void wrong_malloc(char *p, const unsigned int size)
{
	p = (char *)malloc(size);
}

void correct_malloc(char **p, const unsigned int size)
{
	*p = (char *)malloc(size);
}

char *char_malloc(const unsigned int size)
{
	char *p = (char *)malloc(size);
	return p;
}

void test_malloc()
{
	printf("\n======================== test malloc\n");

	char *p = NULL;

	printf("size (p)=%lu\n", malloc_usable_size(p));
	wrong_malloc(p, 64);
	printf("size (p)=%lu\n", malloc_usable_size(p));

	correct_malloc(&p, 64);
	printf("size (p)=%lu\n", malloc_usable_size(p));

	p = char_malloc(96);
	printf("size (p)=%lu\n", malloc_usable_size(p));
}


int main()
{
	test_array();
	test_list();
	test_malloc();
	return 0;
}
