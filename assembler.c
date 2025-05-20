#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <inttypes.h>
//INSTRUCTIONS
#define NOP     0b00000000
#define LDA     0b00010000
#define ADD     0b00100000
#define SUB     0b00110000
#define JMP     0b01000000
#define JZ      0b01010000
#define JC      0b01100000
#define JNZ     0b01110000
#define JNC     0b10000000
#define LDAI    0b10010000
#define LDBI    0b10100000
#define LDS     0b10110000
#define RND     0b11000000
#define NOP1    0b11010000
#define OUT     0b11100000
#define HALT    0b11110000

static const char *instrs[16] = {
    "NOP", "LDA", "ADD", "SUB", "JMP", "JZ", "JC", "LDS", "RND", "STORE", "JNI", "READ", "PRINT", "CLS", "NS", "HALT"
};
uint8_t output[256] = {0};

void print_program() {
    for (int i = 0; i < 256; i += 8) {
        printf("%08b: ", i);
        for (int j = 0; j < 8; ++j) {
            printf("%08b ", output[i + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    memset(output, 0, sizeof(output));
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");
    char *line = nullptr;
    size_t size = 0;
    int n = 0;
    char *token = nullptr;
    while (getline(&line, &size, in) != -1) {
        line[strcspn(line, "\r\n")] = 0;
        if ((token = strtok(line, " "))) {
            uint8_t instr = 0;
            if (token[0] != '\n' && token[0] != ';' && token[0] != '@' && token[0] != '#') {
                bool valid = false;
                for (int i = 0; i < sizeof(instrs) / sizeof(instrs[0]); ++i) {
                    if (strcasecmp(instrs[i], token) == 0) {
                        instr = i << 4;
                        valid = true;
                        break;
                    }
                }
                if (!valid)
                    errx(EXIT_FAILURE, "INVALID INSTRUCTION %s", token);
                if ((token = strtok(nullptr, " "))) {
                    if (token[0] != '\n' && token[0] != ';') {
                        uint8_t number = strtoumax(token, nullptr, 16);
                        instr = instr | number;
                    }
                }
                output[n++] = instr;
            } else if (token[0] == '@') {
                uint8_t addr = 0;
                uint8_t val = 0;
                if ((token = strtok(nullptr, " "))) {
                    addr = (uint) strtoumax(token, nullptr, 16);
                    if (addr > 0xFF)
                        errx(EXIT_FAILURE, "Address out of bounds %x", addr);
                    if ((token = strtok(nullptr, " "))) {
                        val = (uint) strtoumax(token, nullptr, 16);
                        output[addr] = val;
                    }
                }
            } else if (token[0] == '#') {
                if ((token = strtok(nullptr, " "))) {
                    uint8_t addr = (uint) strtoumax(token, nullptr, 16);
                    if (addr > 0xFF)
                        errx(EXIT_FAILURE, "Address out of bounds %x", addr);
                    n = addr;
                }
            }
        }
    }
    print_program();
    fwrite(output, sizeof(output), 1, out);
    fclose(in);
    fclose(out);
    return 0;
}
