//
// Created by Vincent Bode on 21/04/2020.
// Updated by Dennis-Florian Herr 28/04/2022.
//

#ifndef ASSIGNMENTS_VV_AES_H
#define ASSIGNMENTS_VV_AES_H

#include <cstdint>
#include <limits>

#define BLOCK_SIZE 7
#define UNIQUE_CHARACTERS 256
#define ROUNDS 10
#define ITERATIONS 400000

/*
 * This is the message given to you to encrypt to verify the correctness and speed of your approach. Unfortunately,
 * there are no state secrets to be found here. Those would have probably made a pretty penny on the black market.
 */
uint8_t message[BLOCK_SIZE][BLOCK_SIZE] = {
        {'T', 'H', 'E', ' ', 'D', 'E', 'A'},
        {'T', 'H', ' ', 'S', 'T', 'A', 'R'},
        {'H', 'A', 'S', ' ', 'A', ' ', 'W'},
        {'W', 'E', 'A', 'K', 'N', 'E', 'S'},
        {'S', '.', ' ', 'H', 'I', 'T', ' '},
        {'T', 'H', 'E', ' ', 'R', 'E', 'A'},
        {'C', 'T', 'O', 'R', '.', ' ', ' '}
};

/*
 * The set of all keys generated at runtime and the index of the current key.
 */
int currentKey = 0;
uint8_t allKeys[ROUNDS][BLOCK_SIZE][BLOCK_SIZE];

/*
 * Use this like a 2D-Matrix key[BLOCK_SIZE][BLOCK_SIZE];
 * The key is only handed to you when it's time to actually encrypt something.
 */
uint8_t (*key)[BLOCK_SIZE];

/*
 * This is the source list of characters. If you wish to translate a character, find its index in this list. The
 * corresponding output character resides at the same index in the substituted character list.
 */
uint8_t originalCharacter[] = { 190, 150, 24, 31, 42, 180, 112, 135, 98, 53, 35, 163, 22, 57, 1, 75, 128, 17, 217, 199, 229, 106, 252, 236, 61, 177, 0, 45, 96, 66, 214, 64, 216, 92, 20, 189, 226, 85, 131, 63, 201, 210, 82, 30, 89, 254, 154, 41, 44, 198, 230, 137, 18, 119, 14, 54, 200, 251, 183, 74, 94, 37, 215, 211, 59, 90, 125, 34, 167, 73, 235, 110, 95, 49, 50, 174, 105, 155, 164, 3, 7, 65, 146, 191, 240, 168, 231, 239, 15, 67, 121, 107, 186, 225, 84, 81, 165, 68, 222, 133, 11, 208, 126, 43, 140, 108, 185, 32, 33, 91, 28, 114, 99, 243, 104, 149, 46, 87, 55, 166, 23, 86, 124, 97, 62, 12, 143, 234, 241, 176, 153, 39, 184, 250, 173, 48, 38, 47, 102, 169, 21, 115, 249, 118, 101, 130, 237, 138, 88, 72, 77, 188, 109, 218, 242, 93, 246, 16, 79, 8, 204, 175, 117, 147, 123, 194, 71, 145, 122, 233, 116, 5, 202, 171, 36, 227, 100, 159, 10, 206, 203, 78, 224, 142, 127, 207, 244, 193, 132, 2, 76, 221, 196, 26, 9, 27, 148, 228, 134, 212, 160, 179, 151, 60, 178, 58, 141, 248, 56, 156, 129, 29, 238, 157, 19, 245, 83, 103, 4, 13, 152, 111, 120, 69, 220, 187, 253, 161, 162, 172, 113, 247, 139, 195, 213, 219, 209, 52, 170, 158, 255, 6, 181, 70, 80, 25, 192, 205, 182, 136, 51, 232, 144, 223, 197, 40 };
/*
 * This is the output list of characters. If you wish to translate a character, find its index in the original list.
 * The corresponding output character resides at the same index in this list.
 */
uint8_t substitutedCharacter[] = { 206, 167, 222, 134, 125, 87, 29, 132, 53, 54, 240, 192, 94, 113, 12, 78, 128, 155, 96, 83, 214, 212, 207, 148, 208, 27, 162, 133, 103, 28, 47, 0, 151, 147, 146, 156, 187, 210, 88, 114, 196, 204, 153, 77, 32, 49, 20, 198, 37, 99, 227, 159, 61, 242, 219, 40, 69, 189, 237, 183, 178, 168, 246, 169, 252, 205, 176, 218, 122, 30, 46, 11, 152, 190, 116, 164, 201, 21, 51, 59, 229, 31, 108, 225, 86, 243, 16, 224, 234, 62, 56, 3, 101, 232, 25, 217, 106, 98, 163, 64, 255, 223, 160, 220, 73, 251, 180, 117, 67, 127, 173, 170, 13, 35, 141, 52, 135, 209, 197, 63, 55, 80, 22, 253, 175, 17, 110, 131, 93, 92, 249, 5, 33, 245, 115, 226, 36, 104, 70, 236, 89, 2, 82, 42, 230, 238, 71, 136, 143, 150, 43, 91, 235, 121, 45, 24, 23, 9, 188, 157, 140, 109, 76, 191, 129, 202, 174, 44, 130, 239, 254, 181, 166, 203, 39, 60, 213, 72, 250, 38, 231, 145, 216, 58, 215, 123, 74, 105, 165, 95, 172, 97, 48, 57, 50, 138, 79, 6, 10, 75, 65, 195, 124, 107, 66, 182, 120, 1, 34, 149, 154, 26, 221, 139, 177, 200, 90, 4, 248, 119, 194, 14, 68, 41, 126, 15, 84, 144, 118, 233, 7, 184, 137, 161, 211, 158, 112, 186, 81, 193, 228, 100, 179, 247, 244, 18, 102, 241, 19, 85, 111, 171, 199, 8, 185, 142 };

 uint8_t polynomialCoefficients[BLOCK_SIZE][BLOCK_SIZE] = {
        { 1, 2, 3, 4, 5, 6, 7},
        { 8, 7, 6, 5, 4, 3, 2},
        { 1, 2, 3, 4, 5, 6, 7},
        { 8, 7, 6, 5, 4, 3, 2},
        { 8, 8, 8, 8, 8, 8, 8},
        { 4, 5, 6, 8, 6, 4, 2}
};

/*
 * Generate some keys that can be used for encryption based on the seed set from the input.
 */
void generate_keys() {
    // Fill the key block
    for(auto & currentKey : allKeys) {
        for (auto & row : currentKey) {
            for (unsigned char & column : row) {
                column = rand() % std::numeric_limits<uint8_t>::max();
            }
        }
    }
}

void readInput() {
    std::cout << "READY" << std::endl;
    unsigned int seed = 0;
    std::cin >> seed;

    std::cerr << "Using seed " << seed << std::endl;
    if(seed == 0) {
        std::cerr << "Warning: default value 0 used as seed." << std::endl;
    }

    // Set the pseudo random number generator seed used for generating encryption keys
    srand(seed);

    generate_keys();
}

void writeOutput() {
    // Output the current state of the message in hexadecimal.
    for (int row = 0; row < BLOCK_SIZE; row++) {
        std::cout << std::hex << (int) message[row][0] << (int) message[row][1] << (int) message[row][2]
                  << (int) message[row][3];
    }
    // This stops the timer.
    std::cout << std::endl << "DONE" << std::endl;
}

/*
 * This is a utility method. It determines the next key to use from the set of pre-generated keys. In a real
 * cryptographic system, the subsequent keys are generated from a key schedule from the original key. To keep the code
 * short, we have omitted this behavior.
 */
void set_next_key() {
    key = &allKeys[currentKey][0];
    currentKey = (currentKey + 1) % ROUNDS;
}

#endif //ASSIGNMENTS_VV_AES_H
