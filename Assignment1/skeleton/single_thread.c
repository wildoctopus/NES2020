#include <stdio.h>
#include <stdlib.h>

#define RUNNING 0
#define DONE    1

int protothread() {
	printf("protothread!\n");
	return DONE;
}

int main(int argc, char *argv[]) {

	int i = 0;
	int ret;

	do {
		i++;
		ret = protothread();

	} while (ret != DONE);

	printf("thread stopped after %d calls\n", i);

	return 0;
}
