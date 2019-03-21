#ifndef REGENUM_H_INCLUDED
#define REGENUM_H_INCLUDED

enum REGNAME { B = 0,
               C = 1,
               D = 2,
               E = 3,
               H = 4,
               L = 5,
               F = 6,
               A = 7
} ;

enum REGNAME_16 {
    BC = 0,
    DE = 1,
    HL = 2,
    SP = 3,
    AF = 3
};

#endif // REGENUM_H_INCLUDED
