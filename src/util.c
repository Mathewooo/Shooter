#include "util.h"
#include "common/common.h"

long randomBound(long min, long max) {
    unsigned long num_bins = (max - min) + 1L, num_rand = RAND_MAX + 1L, bin_size = num_rand / num_bins,
            defect = num_rand % num_bins;
    long x; do x = random();
    while (num_rand - defect <= (unsigned long) x);
    return min + (x / bin_size);
}

