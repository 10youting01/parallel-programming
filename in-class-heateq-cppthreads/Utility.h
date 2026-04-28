#ifndef ASSIGNMENT_UTILITY_H
#define ASSIGNMENT_UTILITY_H

#include <iostream>
#include <utility>
#include <string>
#include <cstring>
#include <cstdio>
#include <random>
#include <cmath>

#define GRID_SIZE       5000000UL
#define N_STEPS         100UL
#define DELTA_T         2e-3
#define DELTA_X         1.

#define PRINT_RATE      N_STEPS / 1000

int read_seed()
{
    int seed = 0;

    printf("READY\n");
    //scanf("%d", &seed);
    std::cin >> seed;
    printf("Using seed %d\n", seed);

    if (seed == 0) {
        printf("Warning: default value 0 used as seed.\n");
    }

    return seed;
}

#if 0
void print_u_text(FILE *file, double *u)
{
    for (size_t x = 0; x < GRID_SIZE; x++) {
        fprintf(file, "%.3e;", u[x]);
    }

    putc('\n', file);
}
#endif

void print_u(FILE *file, double *u)
{
    fwrite(u, sizeof(double), GRID_SIZE, file);
}

double gaussian(double x, double mean, double std)
{
    double delta = x - mean;
    return std::exp(-0.5 * (delta*delta) / (std*std));
}

void generate_inital_data(double *u, int seed)
{
    memset(u, 0, sizeof(double) * GRID_SIZE);

    std::mt19937 rng = std::mt19937(seed);

    static std::uniform_int_distribution<int> n_peaks_dist(1, 5);
    static std::uniform_real_distribution<double> amplitude_dist_log(-1, 1);
    static std::uniform_real_distribution<double> mean_dist(DELTA_X, (GRID_SIZE - 1) * DELTA_X);
    static std::uniform_real_distribution<double> std_dist_log(0., std::log10((double)GRID_SIZE / 3.));

    int n_peaks = n_peaks_dist(rng);

    for (int i = 0; i < n_peaks; ++i) {
        double amplitude = std::pow(10, amplitude_dist_log(rng));
        double mean = mean_dist(rng);
        double std = DELTA_X * std::pow(10, std_dist_log(rng));

        printf("Gaussian peak @ amplutide=%.2f pos=%.2f, std=%.2f\n", amplitude, mean, std);

        for (size_t x = 1; x < GRID_SIZE - 1; x++) {
            u[x] += amplitude * gaussian(x * DELTA_X, mean, std);
        }
    }
}

void output_result(double *u)
{
    double sum = 0.;
    for (size_t x = 0; x < GRID_SIZE; x++) {
        sum += sin((double)(x + 1)) * u[x];
    }

    printf("%.7e\n", sum);
    printf("DONE\n");
}

#endif /* ASSIGNMENT_UTILITY_H */
