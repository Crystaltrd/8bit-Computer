#include <stdio.h>
#include <string.h>
#define AO      0b1000000000000000
#define AI      0b0100000000000000
#define BO      0b0010000000000000
#define BI      0b0001000000000000
#define IO      0b0000100000000000
#define II      0b0000010000000000
#define CO      0b0000001000000000
#define J       0b0000000100000000
#define CE      0b0000000010000000
#define MI      0b0000000001000000
#define RO      0b0000000000100000
#define RI      0b0000000000010000
#define ALUO    0b0000000000001000
#define SUBF    0b0000000000000100
#define NEXT    0b0000000000000010
#define HALT    0b0000000000000001
// EXTRA
#define FI      0b1000000000000000
#define SI      0b0100000000000000
#define RNDO    0b0010000000000000
#define TTYOUT  0b0001000000000000
#define TTYC    0b0000100000000000
#define KBC     0b0000010000000000
#define KBO     0b0000001000000000
// FLAGS
#define IF      0b01000000
#define CF      0b00100000
#define ZF      0b00010000
//INSTRUCTIONS
#define NOP     0b0000
#define LDA     0b0001
#define ADD     0b0010
#define SUB     0b0011
#define JMP     0b0100
#define JZ      0b0101
#define JC      0b0110
#define LDS     0b0111
#define RND     0b1000
#define STORE   0b1001
#define JNI     0b1010
#define READ    0b1011
#define PRINT   0b1100
// EMPTY
#define NS      0b1110
#define HLT     0b1111

__uint16_t instructions[16 * 8][8] = {
    {CO|MI, RO|II|CE,        0,          0,               0,    0,  0,  0},  //0000 - NOP
    {CO|MI, RO|II|CE,    IO|MI,      RO|AI,               0,    0,  0,  0},  //0001 - LDA
    {CO|MI, RO|II|CE,    IO|MI,      RO|BI,         ALUO|AI,    0,  0,  0},  //0010 - ADD
    {CO|MI, RO|II|CE,    IO|MI,      RO|BI,    ALUO|AI|SUBF,    0,  0,  0},  //0011 - SUB
    {CO|MI, RO|II|CE,     IO|J,          0,               0,    0,  0,  0},  //0100 - JMP
    {CO|MI, RO|II|CE,        0,          0,               0,    0,  0,  0},  //0101 - JZ
    {CO|MI, RO|II|CE,        0,          0,               0,    0,  0,  0},  //0110 - JC
    {CO|MI, RO|II|CE,       IO,          0,               0,    0,  0,  0},  //0111 - LDS
    {CO|MI, RO|II|CE,       AI,          0,               0,    0,  0,  0},  //1000 - RND
    {CO|MI, RO|II|CE,    IO|MI,      RI|AO,               0,    0,  0,  0},  //1001 - STORE
    {CO|MI, RO|II|CE,     IO|J,          0,               0,    0,  0,  0},  //1010 - JNI
    {CO|MI, RO|II|CE,        0,          0,               0,    0,  0,  0},  //1011 - READ
    {CO|MI, RO|II|CE,       AO,          0,               0,    0,  0,  0},  //1100 - PRINT
    {CO|MI, RO|II|CE,        0,          0,               0,    0,  0,  0},  //1101 - CLS
    {CO|MI, RO|II|CE,  AO|NEXT,          0,               0,    0,  0,  0},  //1110 - NS
    {CO|MI, RO|II|CE,     HALT,          0,               0,    0,  0,  0},  //1111 - HLT
};

__uint16_t extra_instructions[16 * 8][8] = {
    {   0,      0,        0,          0,               0,    0,  0,  0}, //0000 - NOP
    {   0,      0,        0,          0,               0,    0,  0,  0}, //0001 - LDA
    {   0,      0,        0,          0,              FI,    0,  0,  0}, //0010 - ADD
    {   0,      0,        0,          0,              FI,    0,  0,  0}, //0011 - SUBS
    {   0,      0,        0,          0,               0,    0,  0,  0}, //0100 - JMP
    {   0,      0,        0,          0,               0,    0,  0,  0}, //0101 - JZ
    {   0,      0,        0,          0,               0,    0,  0,  0}, //0110 - JC
    {   0,      0,       SI,          0,               0,    0,  0,  0}, //0111 - LDS
    {   0,      0,     RNDO,          0,               0,    0,  0,  0}, //1000 - RND
    {   0,      0,        0,          0,               0,    0,  0,  0}, //1001 - STORE
    {   0,      0,        0,          0,               0,    0,  0,  0}, //1010 - JNI
    {   0,      0,       FI,          0,               0,    0,  0,  0}, //1011 - READ
    {   0,      0,   TTYOUT,          0,               0,    0,  0,  0}, //1100 - PRINT
    {   0,      0, TTYC|KBC,          0,               0,    0,  0,  0}, //1101 - CLS
    {   0,      0,        0,          0,               0,    0,  0,  0}, //1110 - NS
    {   0,      0,        0,          0,               0,    0,  0,  0}, //1111 - HLT
};

int main(void) {
    FILE *rom1 = fopen("../MICROCODE1","w+");
    FILE *rom2 = fopen("../MICROCODE2","w+");
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            instructions[i+16][j] = instructions[i][j];
            instructions[i+32][j] = instructions[i][j];
            instructions[i+48][j] = instructions[i][j];
            instructions[i+64][j] = instructions[i][j];
            instructions[i+80][j] = instructions[i][j];
            instructions[i+96][j] = instructions[i][j];
            instructions[i+112][j] = instructions[i][j];
        }
    }
    instructions[JZ|(ZF)][2] = IO|J;
    instructions[JZ|(ZF|IF)][2] = IO|J;
    instructions[JZ|(ZF|CF)][2] = IO|J;
    instructions[JZ|(ZF|CF|IF)][2] = IO|J;
    instructions[JC|(CF)][2] = IO|J;
    instructions[JC|(CF|IF)][2] = IO|J;
    instructions[JC|(CF|ZF)][2] = IO|J;
    instructions[JC|(CF|ZF|IF)][2] = IO|J;

    instructions[JNI|(IF)][2] = 0;
    instructions[JNI|(IF|ZF)][2] = 0;
    instructions[JNI|(IF|CF)][2] = 0;
    instructions[JNI|(IF|CF|ZF)][2] = 0;
    instructions[READ|(IF)][2] = AI;
    instructions[READ|(IF|ZF)][2] = AI;
    instructions[READ|(IF|CF)][2] = AI;
    instructions[READ|(IF|CF|ZF)][2] = AI;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            extra_instructions[i+16][j] = extra_instructions[i][j];
            extra_instructions[i+32][j] = extra_instructions[i][j];
            extra_instructions[i+48][j] = extra_instructions[i][j];
            extra_instructions[i+64][j] = extra_instructions[i][j];
            extra_instructions[i+80][j] = extra_instructions[i][j];
            extra_instructions[i+96][j] = extra_instructions[i][j];
            extra_instructions[i+112][j] = extra_instructions[i][j];
        }
    }

    extra_instructions[READ|(IF)][2] = KBO|FI;
    extra_instructions[READ|(IF|ZF)][2] = KBO|FI;
    extra_instructions[READ|(IF|CF)][2] = KBO|FI;
    extra_instructions[READ|(IF|CF|ZF)][2] = KBO|FI;
    printf("====ROM1====\n");
    for (int j = 0; j < sizeof(instructions)/sizeof(instructions[0]); ++j) {
        printf("%07b: ",j);
        for (int i = 0; i < 8; ++i) {
            // ReSharper disable once CppPrintfBadFormat
            printf("%016b ",instructions[j][i]);
        }
        printf("\n");
    }
    printf("====ROM1====\n");
    printf("====ROM2====\n");
    for (int j = 0; j < sizeof(extra_instructions)/sizeof(extra_instructions[0]); ++j) {
        printf("%07b: ",j);
        for (int i = 0; i < 8; ++i) {
            // ReSharper disable once CppPrintfBadFormat
            printf("%016b ",extra_instructions[j][i]);
        }
        printf("\n");
    }
    printf("====ROM2====\n");

    fwrite(instructions,sizeof(instructions),1,rom1);
    fwrite(extra_instructions,sizeof(extra_instructions),1,rom2);
    fclose(rom1);
    fclose(rom2);
    return 0;
}