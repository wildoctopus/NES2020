#include <stdio.h>
#include <stdlib.h>

#define RUNNING 0
#define DONE    1


typedef struct thread {
	int (*run)();
	struct thread *next;
} thread_t;

thread_t *threads = NULL;

void threads_add_tail(thread_t *t) {
	if (!threads) {
		threads = t;
		t->next = NULL;
		return;
	}

	thread_t *current = threads;
	while (current->next != NULL) {
		current = current->next;
	}

	current->next = t;
	t->next = NULL;
}

int thread_available() {
	if (threads) {
		return 1;
	} else {
		return 0;
	}
}

thread_t* threads_pop_front() {
	thread_t* front = threads;
	threads = front->next;
	return front;
}

int protothread1() {
    static int step = 1;
    char thread_id[] = "protohread1";
    switch(step)
    {
    case 1:
        printf("Executed Step %d in thread %s \n", step, thread_id);
        step = 2;
        return RUNNING;
    case 2:
        printf("Executed Step %d in thread %s \n", step, thread_id);
        step = 3;
        return RUNNING;
    case 3:
        printf("Executed Step %d in thread %s \n", step, thread_id);
        step = 4;
        return RUNNING;
    default:
        printf("Executed Step %d in thread %s \n", step, thread_id);
        step++;
        break;

    }
    return DONE;

}

int protothread2() {
    static int step = 1;
    int a = 0;
    char thread_id[] = "protothread2";
    switch(step)
    {
    case 1:
    	a++;
        printf("Executed Step %d in %s and local variable a with value %d \n", step, thread_id, a);
        step = 2;
        return RUNNING;
    default:
    	a++;        
    	printf("Executed Step %d in %s and local variable a with value %d \n", step, thread_id, a);
        step = 3;
        break;

    }
    return DONE;
}

int main(int argc, char *argv[]) {

	// create two protothreads
	thread_t *t1 = (thread_t*)malloc(sizeof(thread_t));
	t1->run = &protothread1;
	thread_t *t2 = (thread_t*)malloc(sizeof(thread_t));
	t2->run = &protothread2;

	threads_add_tail(t1);
	threads_add_tail(t2);

	while (thread_available()) {

		// get first thread and exectue
		thread_t* t = threads_pop_front();
		int ret = t->run();
		// remove completed threads from pool
		if (ret != DONE) {
			threads_add_tail(t);
		}
	}

	return 0;
}
