// senseCryptDllTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "senseCryptDll.h"

int main(int argc, char* argv[])
{
	unsigned char test[] = "fgsdfgfgsdf";
	unsigned char out[1024];
	memset(out, 0, 1024);

	int ret = S4RSAPublicEncrypt(out, test, sizeof(test));
	printf("%s\n", out);
	return ret;
}

