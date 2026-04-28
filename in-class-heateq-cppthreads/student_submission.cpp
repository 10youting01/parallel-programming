#include "Utility.h"
// ######################## TODO: include library for enabling thread and mutex ########################
#include <thread>
#include <barrier>
#include <vector>
// ######################## TODO END ########################

#define FILENAME        "u_sim.f64"

#ifndef THREAD_NUM
#define THREAD_NUM      4
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define LOCAL_OPTIMIZE __attribute__((optimize("O3,no-tree-vectorize,fp-contract=off")))
#else
#define LOCAL_OPTIMIZE
#endif

LOCAL_OPTIMIZE
void compute_stencil(double *__restrict__ u, double *__restrict__ u_new, double a, const size_t local_size)
{
    for (size_t x = 0; x < local_size; x++) {
        u_new[x] = u[x] + a * (u[x + 1] - 2.*u[x] + u[x - 1]);
    }
}

double *simulation(double *__restrict__ u0, double *__restrict__ u1, double a, FILE *file, int print_rate)
{
    double *u = u0;
    double *u_new = u1;

    const size_t total_work = GRID_SIZE - 2;
    unsigned int thread_count = std::thread::hardware_concurrency();

    if (thread_count == 0) {
        thread_count = 1;
    }

    if (thread_count > THREAD_NUM) {
        thread_count = THREAD_NUM;
    }

    if (thread_count > total_work) {
        thread_count = static_cast<unsigned int>(total_work);
    }

    std::vector<size_t> starts(thread_count);
    std::vector<size_t> work_sizes(thread_count);
    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    const size_t local_size = total_work / thread_count;
    const size_t remainder = total_work % thread_count;
    size_t offset = 0;

    for (unsigned int thread_id = 0; thread_id < thread_count; ++thread_id) {
        const size_t thread_work = local_size + (thread_id < remainder ? 1 : 0);
        starts[thread_id] = 1 + offset;
        work_sizes[thread_id] = thread_work;
        offset += thread_work;
    }

    std::barrier start_barrier(thread_count + 1);
    std::barrier done_barrier(thread_count + 1);

    for (unsigned int thread_id = 0; thread_id < thread_count; ++thread_id) {
        threads.emplace_back([&, thread_id]() {
            const size_t start = starts[thread_id];
            const size_t work_size = work_sizes[thread_id];

            for (size_t t = 1; t < N_STEPS; ++t) {
                start_barrier.arrive_and_wait();
                compute_stencil(u + start, u_new + start, a, work_size);
                done_barrier.arrive_and_wait();
            }
        });
    }

    for (size_t t = 1; t < N_STEPS; t++) {
        if ((file != nullptr)
            && (print_rate > 0)
            && (t % print_rate == 0))
        {
            print_u(file, u);
        }

        start_barrier.arrive_and_wait();
        done_barrier.arrive_and_wait();

        std::swap(u, u_new);
    }

    for (std::thread &thread : threads) {
        thread.join();
    }

    return u_new;
}

int main()
{
    double a = DELTA_T / (DELTA_X * DELTA_X);
    double *u0 = new double[GRID_SIZE];
    double *u1 = new double[GRID_SIZE];

    FILE *file = nullptr;
    // uncomment to write out time evolution of heat field
    //file = fopen(FILENAME, "w");

    //if (file == nullptr) {
    //    printf("Could not create simulation file (%s)!\n", FILENAME);
    //}

    int seed = read_seed();

    if (seed == 0) {
        fprintf(stderr, "Warning: default value 0 used as seed.\n");
    }

    generate_inital_data(u0, seed);
    double *un = simulation(u0, u1, a, file, PRINT_RATE);

    if (file != nullptr){
        print_u(file, un);
        fclose(file);

        std::string meta_filename(FILENAME);
        meta_filename.append(".meta");

        FILE *meta_file = fopen(meta_filename.c_str(), "w");
        fprintf(meta_file, "%lu\n", GRID_SIZE);
        fprintf(meta_file, "%.8e\n", DELTA_T);
        fprintf(meta_file, "%.8e\n", DELTA_X);
        fprintf(meta_file, "%lu\n", N_STEPS);
        fprintf(meta_file, "%lu\n", PRINT_RATE);
        fclose(meta_file);
    }

    output_result(un);

    delete[] u0;
    delete[] u1;

    return 0;
}
