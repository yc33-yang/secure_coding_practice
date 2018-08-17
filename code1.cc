#include<stdio.h>
#include<stdlib.h>
#include<string.h>


template <size_t n>
void strswap(char (&str1)[n], char (&str2)[n]) {
	char tem[n] = { 0, };
	strncpy(tem, str1, n);
	strncpy(str1, str2, n);
	strncpy(str2, tem, n);
	if (n > 0) 
	  str1[n-1] = str2[n-1] = 0;
}

int main(int argc, char *argv[])
{
	/* sanity check */
	printf("\n%s : %d, enter\n", __FILE__, __LINE__);

	char input1[256], input2[256], input3[256];
	printf("Gets 3 sting values from stdin:\n");
	fgets(input1, 255, stdin);
	fgets(input2, 255, stdin);
	fgets(input3, 255, stdin);

	if (strcmp(input1, input2)>0) strswap(input1, input2);
	if (strcmp(input2, input3)>0) strswap(input2, input3);
	if (strcmp(input1, input2)>0) strswap(input1, input2);

	printf("%s\n %s\n %s\n", input1, input2, input3);
	printf("\n%s : %d, exit\n", __FILE__, __LINE__);
	return 0;
}
