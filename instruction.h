#ifndef VMOS6502_INSTR_H
#define VMOS6502_INSTR_H

#include <cstdint>

typedef uint8_t byte;

struct Instruction {
    const byte opcode;
    const char *name;
    const byte mode;
    const byte size;
    const byte cycles;
    const byte page_cross_cycles;

    constexpr Instruction(const byte _opcode, const char _name[4], const byte _mode, const byte _size,
                          const byte _cycles, const byte _page_crossed_cycles) : opcode(_opcode), name(_name),
                                                                                 mode(_mode), size(_size),
                                                                                 cycles(_cycles), page_cross_cycles(
                    _page_crossed_cycles) {}
};

constexpr Instruction instructions[256] = {
        {0,   "BRK", 6,  1, 7, 0},
        {1,   "ORA", 7,  2, 6, 0},
        {2,   "KIL", 6,  0, 2, 0},
        {3,   "SLO", 7,  0, 8, 0},
        {4,   "NOP", 11, 2, 3, 0},
        {5,   "ORA", 11, 2, 3, 0},
        {6,   "ASL", 11, 2, 5, 0},
        {7,   "SLO", 11, 0, 5, 0},
        {8,   "PHP", 6,  1, 3, 0},
        {9,   "ORA", 5,  2, 2, 0},
        {10,  "ASL", 4,  1, 2, 0},
        {11,  "ANC", 5,  0, 2, 0},
        {12,  "NOP", 1,  3, 4, 0},
        {13,  "ORA", 1,  3, 4, 0},
        {14,  "ASL", 1,  3, 6, 0},
        {15,  "SLO", 1,  0, 6, 0},
        {16,  "BPL", 10, 2, 2, 1},
        {17,  "ORA", 9,  2, 5, 1},
        {18,  "KIL", 6,  0, 2, 0},
        {19,  "SLO", 9,  0, 8, 0},
        {20,  "NOP", 12, 2, 4, 0},
        {21,  "ORA", 12, 2, 4, 0},
        {22,  "ASL", 12, 2, 6, 0},
        {23,  "SLO", 12, 0, 6, 0},
        {24,  "CLC", 6,  1, 2, 0},
        {25,  "ORA", 3,  3, 4, 1},
        {26,  "NOP", 6,  1, 2, 0},
        {27,  "SLO", 3,  0, 7, 0},
        {28,  "NOP", 2,  3, 4, 1},
        {29,  "ORA", 2,  3, 4, 1},
        {30,  "ASL", 2,  3, 7, 0},
        {31,  "SLO", 2,  0, 7, 0},
        {32,  "JSR", 1,  3, 6, 0},
        {33,  "AND", 7,  2, 6, 0},
        {34,  "KIL", 6,  0, 2, 0},
        {35,  "RLA", 7,  0, 8, 0},
        {36,  "BIT", 11, 2, 3, 0},
        {37,  "AND", 11, 2, 3, 0},
        {38,  "ROL", 11, 2, 5, 0},
        {39,  "RLA", 11, 0, 5, 0},
        {40,  "PLP", 6,  1, 4, 0},
        {41,  "AND", 5,  2, 2, 0},
        {42,  "ROL", 4,  1, 2, 0},
        {43,  "ANC", 5,  0, 2, 0},
        {44,  "BIT", 1,  3, 4, 0},
        {45,  "AND", 1,  3, 4, 0},
        {46,  "ROL", 1,  3, 6, 0},
        {47,  "RLA", 1,  0, 6, 0},
        {48,  "BMI", 10, 2, 2, 1},
        {49,  "AND", 9,  2, 5, 1},
        {50,  "KIL", 6,  0, 2, 0},
        {51,  "RLA", 9,  0, 8, 0},
        {52,  "NOP", 12, 2, 4, 0},
        {53,  "AND", 12, 2, 4, 0},
        {54,  "ROL", 12, 2, 6, 0},
        {55,  "RLA", 12, 0, 6, 0},
        {56,  "SEC", 6,  1, 2, 0},
        {57,  "AND", 3,  3, 4, 1},
        {58,  "NOP", 6,  1, 2, 0},
        {59,  "RLA", 3,  0, 7, 0},
        {60,  "NOP", 2,  3, 4, 1},
        {61,  "AND", 2,  3, 4, 1},
        {62,  "ROL", 2,  3, 7, 0},
        {63,  "RLA", 2,  0, 7, 0},
        {64,  "RTI", 6,  1, 6, 0},
        {65,  "EOR", 7,  2, 6, 0},
        {66,  "KIL", 6,  0, 2, 0},
        {67,  "SRE", 7,  0, 8, 0},
        {68,  "NOP", 11, 2, 3, 0},
        {69,  "EOR", 11, 2, 3, 0},
        {70,  "LSR", 11, 2, 5, 0},
        {71,  "SRE", 11, 0, 5, 0},
        {72,  "PHA", 6,  1, 3, 0},
        {73,  "EOR", 5,  2, 2, 0},
        {74,  "LSR", 4,  1, 2, 0},
        {75,  "ALR", 5,  0, 2, 0},
        {76,  "JMP", 1,  3, 3, 0},
        {77,  "EOR", 1,  3, 4, 0},
        {78,  "LSR", 1,  3, 6, 0},
        {79,  "SRE", 1,  0, 6, 0},
        {80,  "BVC", 10, 2, 2, 1},
        {81,  "EOR", 9,  2, 5, 1},
        {82,  "KIL", 6,  0, 2, 0},
        {83,  "SRE", 9,  0, 8, 0},
        {84,  "NOP", 12, 2, 4, 0},
        {85,  "EOR", 12, 2, 4, 0},
        {86,  "LSR", 12, 2, 6, 0},
        {87,  "SRE", 12, 0, 6, 0},
        {88,  "CLI", 6,  1, 2, 0},
        {89,  "EOR", 3,  3, 4, 1},
        {90,  "NOP", 6,  1, 2, 0},
        {91,  "SRE", 3,  0, 7, 0},
        {92,  "NOP", 2,  3, 4, 1},
        {93,  "EOR", 2,  3, 4, 1},
        {94,  "LSR", 2,  3, 7, 0},
        {95,  "SRE", 2,  0, 7, 0},
        {96,  "RTS", 6,  1, 6, 0},
        {97,  "ADC", 7,  2, 6, 0},
        {98,  "KIL", 6,  0, 2, 0},
        {99,  "RRA", 7,  0, 8, 0},
        {100, "NOP", 11, 2, 3, 0},
        {101, "ADC", 11, 2, 3, 0},
        {102, "ROR", 11, 2, 5, 0},
        {103, "RRA", 11, 0, 5, 0},
        {104, "PLA", 6,  1, 4, 0},
        {105, "ADC", 5,  2, 2, 0},
        {106, "ROR", 4,  1, 2, 0},
        {107, "ARR", 5,  0, 2, 0},
        {108, "JMP", 8,  3, 5, 0},
        {109, "ADC", 1,  3, 4, 0},
        {110, "ROR", 1,  3, 6, 0},
        {111, "RRA", 1,  0, 6, 0},
        {112, "BVS", 10, 2, 2, 1},
        {113, "ADC", 9,  2, 5, 1},
        {114, "KIL", 6,  0, 2, 0},
        {115, "RRA", 9,  0, 8, 0},
        {116, "NOP", 12, 2, 4, 0},
        {117, "ADC", 12, 2, 4, 0},
        {118, "ROR", 12, 2, 6, 0},
        {119, "RRA", 12, 0, 6, 0},
        {120, "SEI", 6,  1, 2, 0},
        {121, "ADC", 3,  3, 4, 1},
        {122, "NOP", 6,  1, 2, 0},
        {123, "RRA", 3,  0, 7, 0},
        {124, "NOP", 2,  3, 4, 1},
        {125, "ADC", 2,  3, 4, 1},
        {126, "ROR", 2,  3, 7, 0},
        {127, "RRA", 2,  0, 7, 0},
        {128, "NOP", 5,  2, 2, 0},
        {129, "STA", 7,  2, 6, 0},
        {130, "NOP", 5,  0, 2, 0},
        {131, "SAX", 7,  0, 6, 0},
        {132, "STY", 11, 2, 3, 0},
        {133, "STA", 11, 2, 3, 0},
        {134, "STX", 11, 2, 3, 0},
        {135, "SAX", 11, 0, 3, 0},
        {136, "DEY", 6,  1, 2, 0},
        {137, "NOP", 5,  0, 2, 0},
        {138, "TXA", 6,  1, 2, 0},
        {139, "XAA", 5,  0, 2, 0},
        {140, "STY", 1,  3, 4, 0},
        {141, "STA", 1,  3, 4, 0},
        {142, "STX", 1,  3, 4, 0},
        {143, "SAX", 1,  0, 4, 0},
        {144, "BCC", 10, 2, 2, 1},
        {145, "STA", 9,  2, 6, 0},
        {146, "KIL", 6,  0, 2, 0},
        {147, "AHX", 9,  0, 6, 0},
        {148, "STY", 12, 2, 4, 0},
        {149, "STA", 12, 2, 4, 0},
        {150, "STX", 13, 2, 4, 0},
        {151, "SAX", 13, 0, 4, 0},
        {152, "TYA", 6,  1, 2, 0},
        {153, "STA", 3,  3, 5, 0},
        {154, "TXS", 6,  1, 2, 0},
        {155, "TAS", 3,  0, 5, 0},
        {156, "SHY", 2,  0, 5, 0},
        {157, "STA", 2,  3, 5, 0},
        {158, "SHX", 3,  0, 5, 0},
        {159, "AHX", 3,  0, 5, 0},
        {160, "LDY", 5,  2, 2, 0},
        {161, "LDA", 7,  2, 6, 0},
        {162, "LDX", 5,  2, 2, 0},
        {163, "LAX", 7,  0, 6, 0},
        {164, "LDY", 11, 2, 3, 0},
        {165, "LDA", 11, 2, 3, 0},
        {166, "LDX", 11, 2, 3, 0},
        {167, "LAX", 11, 0, 3, 0},
        {168, "TAY", 6,  1, 2, 0},
        {169, "LDA", 5,  2, 2, 0},
        {170, "TAX", 6,  1, 2, 0},
        {171, "LAX", 5,  0, 2, 0},
        {172, "LDY", 1,  3, 4, 0},
        {173, "LDA", 1,  3, 4, 0},
        {174, "LDX", 1,  3, 4, 0},
        {175, "LAX", 1,  0, 4, 0},
        {176, "BCS", 10, 2, 2, 1},
        {177, "LDA", 9,  2, 5, 1},
        {178, "KIL", 6,  0, 2, 0},
        {179, "LAX", 9,  0, 5, 1},
        {180, "LDY", 12, 2, 4, 0},
        {181, "LDA", 12, 2, 4, 0},
        {182, "LDX", 13, 2, 4, 0},
        {183, "LAX", 13, 0, 4, 0},
        {184, "CLV", 6,  1, 2, 0},
        {185, "LDA", 3,  3, 4, 1},
        {186, "TSX", 6,  1, 2, 0},
        {187, "LAS", 3,  0, 4, 1},
        {188, "LDY", 2,  3, 4, 1},
        {189, "LDA", 2,  3, 4, 1},
        {190, "LDX", 3,  3, 4, 1},
        {191, "LAX", 3,  0, 4, 1},
        {192, "CPY", 5,  2, 2, 0},
        {193, "CMP", 7,  2, 6, 0},
        {194, "NOP", 5,  0, 2, 0},
        {195, "DCP", 7,  0, 8, 0},
        {196, "CPY", 11, 2, 3, 0},
        {197, "CMP", 11, 2, 3, 0},
        {198, "DEC", 11, 2, 5, 0},
        {199, "DCP", 11, 0, 5, 0},
        {200, "INY", 6,  1, 2, 0},
        {201, "CMP", 5,  2, 2, 0},
        {202, "DEX", 6,  1, 2, 0},
        {203, "AXS", 5,  0, 2, 0},
        {204, "CPY", 1,  3, 4, 0},
        {205, "CMP", 1,  3, 4, 0},
        {206, "DEC", 1,  3, 6, 0},
        {207, "DCP", 1,  0, 6, 0},
        {208, "BNE", 10, 2, 2, 1},
        {209, "CMP", 9,  2, 5, 1},
        {210, "KIL", 6,  0, 2, 0},
        {211, "DCP", 9,  0, 8, 0},
        {212, "NOP", 12, 2, 4, 0},
        {213, "CMP", 12, 2, 4, 0},
        {214, "DEC", 12, 2, 6, 0},
        {215, "DCP", 12, 0, 6, 0},
        {216, "CLD", 6,  1, 2, 0},
        {217, "CMP", 3,  3, 4, 1},
        {218, "NOP", 6,  1, 2, 0},
        {219, "DCP", 3,  0, 7, 0},
        {220, "NOP", 2,  3, 4, 1},
        {221, "CMP", 2,  3, 4, 1},
        {222, "DEC", 2,  3, 7, 0},
        {223, "DCP", 2,  0, 7, 0},
        {224, "CPX", 5,  2, 2, 0},
        {225, "SBC", 7,  2, 6, 0},
        {226, "NOP", 5,  0, 2, 0},
        {227, "ISC", 7,  0, 8, 0},
        {228, "CPX", 11, 2, 3, 0},
        {229, "SBC", 11, 2, 3, 0},
        {230, "INC", 11, 2, 5, 0},
        {231, "ISC", 11, 0, 5, 0},
        {232, "INX", 6,  1, 2, 0},
        {233, "SBC", 5,  2, 2, 0},
        {234, "NOP", 6,  1, 2, 0},
        {235, "SBC", 5,  0, 2, 0},
        {236, "CPX", 1,  3, 4, 0},
        {237, "SBC", 1,  3, 4, 0},
        {238, "INC", 1,  3, 6, 0},
        {239, "ISC", 1,  0, 6, 0},
        {240, "BEQ", 10, 2, 2, 1},
        {241, "SBC", 9,  2, 5, 1},
        {242, "KIL", 6,  0, 2, 0},
        {243, "ISC", 9,  0, 8, 0},
        {244, "NOP", 12, 2, 4, 0},
        {245, "SBC", 12, 2, 4, 0},
        {246, "INC", 12, 2, 6, 0},
        {247, "ISC", 12, 0, 6, 0},
        {248, "SED", 6,  1, 2, 0},
        {249, "SBC", 3,  3, 4, 1},
        {250, "NOP", 6,  1, 2, 0},
        {251, "ISC", 3,  0, 7, 0},
        {252, "NOP", 2,  3, 4, 1},
        {253, "SBC", 2,  3, 4, 1},
        {254, "INC", 2,  3, 7, 0},
        {255, "ISC", 2,  0, 7, 0}
};

#endif