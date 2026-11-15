
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define DURATION 10

// CPU-intensive work: a loop that runs 100,000 times, incrementing a counter variable.
// Sleep: a loop that runs for 10 seconds.
int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s cpu|sleep\n", argv[0]);
        return 1;
    }

    printf("PID: %ld\n", (long)getpid());
    fflush(stdout);

    if (strcmp(argv[1], "cpu") == 0) {
        struct timespec start, now;
        volatile unsigned long counter = 0;

        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
            perror("clock_gettime");
            return 1;
        }

        printf("Running CPU-intensive work...\n");
        fflush(stdout);

        do {
            for (int i = 0; i < 100000; i++) {
                counter++;
            }

            if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
                perror("clock_gettime");
                return 1;
            }

        } while (now.tv_sec - start.tv_sec < DURATION);

        printf("Counter: %lu\n", counter);
    }
    else if (strcmp(argv[1], "sleep") == 0) {
        printf("Sleeping for %d seconds...\n", DURATION);
        fflush(stdout);

        sleep(DURATION);
    }
    else {
        fprintf(stderr, "Unknown mode: %s\n", argv[1]);
        return 1;
    }

    printf("Process finished.\n");

    return 0;
}
