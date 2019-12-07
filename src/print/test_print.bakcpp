#include "SEscPrintCom.h"
#include <string.h>

int main(int argc, char* argv[])
{
	SEscPrintCom pnt;
	if(!pnt.Open("dev=/dev/ttyS0;baud=19200;chk=N;"))
	{
		printf("open com printer error!\n");
		return 0;
	}
	pnt.PrintText("中华人民共和国ABCD");
	pnt.Close();
	printf("print ok!|n");
	return 0;
}