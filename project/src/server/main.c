#include "db.h"
#include "network.h"
#include <stdio.h>

int main()
{
	dblnit();
	int sockFd = serverlnit();
	waitClient(sockFd);
	return 0;
}
