#include "imgpimping.h"

int inic (){

	if(!fork())
		return 1;

	if(!fork())
		return 2;

	if(!fork())
		return 3;

	return 0;
}
