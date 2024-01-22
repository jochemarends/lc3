#ifndef OPCODES_H
#define OPCODES_H

namespace lc3 {
    enum class opcode {
        ADD  = 0b0001,
        AND  = 0b0101,
        BR   = 0b0000,
        JMP  = 0b1100,
        JSR  = 0b0100,
        LD   = 0b0010,
        LDI  = 0b1010,
        LDR  = 0b0110,
        LEA  = 0b1110,
        NOT  = 0b1001,
        RTI  = 0b1000,
        ST   = 0b0011,
        STI  = 0b1011,
        STR  = 0b0111,
        TRAP = 0b1111,
    };
}

#endif
