// Function for the mask the string except for the last four characters
#include <stdio.h>

/**
 * mask_string - Mask all characters of a string except the last four.
 *
 * @input:  Pointer to a NUL-terminated input string. Must not be NULL.
 *
 * @output: Pointer to a writable buffer where the resulting NUL-terminated
 *          masked string will be stored. The buffer must be large enough to
 *          hold strlen(input) + 1 bytes.
 *
 * Description:
 * - Replaces each character in input with '*' except for the final four
 *   characters, which are copied unchanged.
 * - If the input length is <= 4, the input is copied unchanged to output.
 * - The output is always NUL-terminated by this function.
 *
 * Return: void. No error is reported; the caller is responsible for providing
 *         valid pointers and sufficient output buffer capacity.
 */
void mask_string(const char *input, char *output) {
    int length = 0;
    while (input[length] != '\0') {
        length++;
    }

    for (int i = 0; i < length; i++) {
        if (i < length - 4) {
            output[i] = '*';
        } else {
            output[i] = input[i];
        }
    }
    output[length] = '\0'; // Null-terminate the output string
}

int main() {
    const char *original = "AlphaBetaGammaDelta";
    char masked[100];

    mask_string(original, masked);
    printf("Original: %s\n", original);
    printf("Masked: %s\n", masked);

    return 0;
}

