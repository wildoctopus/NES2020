#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define RUNNING 0
#define DONE    4

int protothread(int step) {
	printf("Executing protothread %d!\n", step);
	return step;
}

int main(int argc, char *argv[]) {

	int i = 0;
	int ret;

	do {
		i++;
		assert(ret == i)
		ret = protothread(i);

	} while (ret != DONE);

	assert(i == 4)
	printf("thread stopped after %d calls\n", i);

	return 0;
}
