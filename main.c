#include <cpuid.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define TESTS_NUMBER 10000000
#define NONSENSICAL_VALUE 0xDBEF

//#define DEBUG 1

void cpuid_test()
{
    uint32_t eax, ebx, ecx, edx;

#ifdef DEBUG
    if (!__get_cpuid(NONSENSICAL_VALUE, &eax, &ebx, &ecx, &edx)) {
        fprintf(stderr, "Warning: CPUID request %d not valid!\n", NONSENSICAL_VALUE);
    }
    fprintf(stdout, "eax: %d, ebx: %d, ecx: %d, edx: %d\n", eax, ebx, ecx, edx);
#endif
    __get_cpuid(NONSENSICAL_VALUE, &eax, &ebx, &ecx, &edx);
}

long int calc_time(struct timespec t1, struct timespec t2)
{
    long int tt = 1000000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec);
    return tt;
}

//#define FULL_TIME_TEST 1

int main(int argc, const char* argv[])
{
    struct timespec test_start_time, test_end_time;
    long int test_time;

#ifndef FULL_TIME_TEST
    struct timespec iter_start_time, iter_end_time;
    long int iter_time = 0;
#endif

    clock_gettime(CLOCK_REALTIME, &test_start_time);
    for (int i = 0; i < TESTS_NUMBER; i++) {
#ifndef FULL_TIME_TEST
        clock_gettime(CLOCK_REALTIME, &iter_start_time);
#endif
        cpuid_test();
#ifndef FULL_TIME_TEST
        clock_gettime(CLOCK_REALTIME, &iter_end_time);
        iter_time += calc_time(iter_start_time, iter_end_time);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &test_end_time);

#ifndef FULL_TIME_TEST
    long int avg_iter_time = iter_time / TESTS_NUMBER;
    fprintf(stdout, "Average iteration time: %ld ns \n", avg_iter_time);
#endif

    test_time = calc_time(test_start_time, test_end_time);

    fprintf(stdout, "Number of tests: %d\n", TESTS_NUMBER);
    fprintf(stdout, "Testing time: %ld ns (%f seconds) \n", test_time, test_time / 1e+9);

    return 0;
}
