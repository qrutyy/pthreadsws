#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void* roll_dice() {
	int value = (rand() % 6) + 1;
	int* res_val = malloc(sizeof(int));
	*res_val = value;
	return (void*) res_val;
}

int main(int argc, char* argv[]) {
	int* res;
	int i = 0;
	srand(time(NULL));
	pthread_t th[4];
	
	for (i = 0; i < 4; i ++) {
		if (pthread_create(th + i, NULL, &roll_dice, NULL) != 0) {
			perror("Thread creation failed\n");
			return 1;
		}
		printf("Result: %d %p\n", *res, res);
	}

	for (i = 0; i < 4; i ++) {
		if (pthread_join(th[i], (void**) &res) != 0) {
			return 2;
		}
		printf("Result: %d %p\n", *res, res);
		free(res);
	}
		
	return 0;
}

/*
In your program, the use of a **mutex** (mutual exclusion) isn't necessary because:

1. **Independent Resources (Thread-Specific Data):**
   - Each thread is generating its own random number and storing it in its own dynamically allocated memory (`malloc`), which is specific to that thread.
   - Since each thread is working with its own memory, there is no shared resource or data between the threads that could cause race conditions. No other thread is reading or writing to the same memory location.

2. **Thread-Safe `rand()` in Modern Systems:**
   - The function `rand()` is used to generate random numbers. In many modern implementations, `rand()` is thread-safe because it either uses thread-local storage or internal locking to ensure safe access in multithreaded programs.
   - While older implementations of `rand()` may not have been thread-safe, most modern systems (like Linux and Windows) provide thread-safe random number generation functions or alternatives (like `rand_r()` or `random()`), so locking the `rand()` call with a mutex is not necessary in many cases.
   
   However, if you are unsure about your system's implementation of `rand()` and want to ensure thread safety, you could:
   - Use `rand_r()` which is a thread-safe variant, but it requires managing a seed per thread.
   - Use modern random number libraries like `<random>` in C++ or thread-safe alternatives depending on your platform.

3. **No Shared Global State:**
   - A mutex is generally used to protect access to shared global state or resources between threads. In your case, the threads don’t access any shared data or resources (e.g., a shared array or a global variable). The generated random number and the allocated memory are unique to each thread.
   
   - If the threads were writing to a shared data structure (e.g., a global array where results are stored), a mutex would be required to prevent race conditions, where two threads might attempt to write to the same location simultaneously.

### When Mutex Would Be Needed:
A mutex would be necessary if:
- Threads were updating a shared variable or data structure.
- You had non-thread-safe functions or operations that access shared resources.
- For example, if all threads were updating a global result variable, like an array where each thread writes its result to a particular index, you would need a mutex to lock access when writing to that shared array.

### Conclusion:
In your specific case:
- Each thread is working on its own local data (allocated dynamically), and there’s no shared resource.
- `rand()` is usually thread-safe in modern systems, so a mutex around it is redundant.

This is why a mutex isn't needed in your program.
*/
