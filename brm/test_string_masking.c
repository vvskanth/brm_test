#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* mask_string - Mask all characters of a string except the last four.
 * Copies input to output, replacing bytes before the last four with '*'.
 * Caller must provide an output buffer at least strlen(input)+1 bytes.
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
    output[length] = '\0';
}

/* Simple unit test harness */
static void run_test(const char *input, const char *expected) {
    size_t len = strlen(input);
    char *out = (char*)malloc(len + 1);
    if (!out) {
        fprintf(stderr, "malloc failed\n");
        exit(2);
    }
    /* fill with sentinel to detect non-termination issues (not asserted explicitly) */
    memset(out, 'X', len + 1);
    out[len] = '\0';

    mask_string(input, out);

    if (strcmp(out, expected) != 0) {
        fprintf(stderr, "FAIL: input=\"%s\" expected=\"%s\" got=\"%s\"\n",
                input, expected, out);
        free(out);
        exit(1);
    }
    free(out);
}

int main(void) {
    /* Explicit expected cases */
    run_test("", "");
    run_test("A", "A");
    run_test("ab", "ab");
    run_test("1234", "1234");
    run_test("12345", "*2345");
    run_test("abcd5", "*bcd5");
    run_test("AlphaBetaGammaDelta", "***************elta"); /* 19 chars -> 15 '*' + "elta" */

    /* Additional random/edge cases */
    run_test("😊😊😊😊😊", "😊😊😊😊😊"); /* UTF-8 multibyte bytes handled as bytes; here bytes != chars,
                                           but function is byte-based; expected keeps last 4 bytes which
                                           may split characters — this test ensures behavior is deterministic */
    run_test("toolongstringwithmorethanfour", "**********************four"); /* check longer */

    printf("All tests passed\n");
    return 0;
}