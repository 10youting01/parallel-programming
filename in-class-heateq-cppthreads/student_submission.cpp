#include "Utility.h"
// ######################## TODO: include library for enabling thread and mutex ########################

// ######################## TODO END ########################

#define FILENAME        "u_sim.f64"

#define THREAD_NUM      24

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

    for (size_t t = 1; t < N_STEPS; t++) {
        if ((file != nullptr)
            && (t % print_rate == 0))
        {
            print_u(file, u);
        }

        std::thread threads[THREAD_NUM];

        // ######################## TODO: compute size that each thread needs to process ########################
        size_t total_work = TODO;
        size_t local_size = TODO;
        size_t remainder = TODO;
        // ######################## TODO END ########################

        for (int thread_id = 0; thread_id < THREAD_NUM; ++thread_id) {
            // ######################## TODO: create thread to call the working_thread function and pass the buffer element as argument ########################

            // ######################## TODO END ########################
        }

        for (int thread_id = 0; thread_id < THREAD_NUM; ++thread_id) {
            // ######################## TODO: join thread to terminate thread ########################

            // ######################## TODO END ########################
        }

        std::swap(u, u_new);
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
