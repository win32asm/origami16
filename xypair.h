//
// Created by botanic on 8/5/16.
//

#ifndef ICFPC2016_XYPAIR_H
#define ICFPC2016_XYPAIR_H

class xypair {
public:
    xypair(){
        mpq_inits(x, y, nullptr);
    }
    xypair(const xypair&xp) {
        mpq_inits(x, y, nullptr);
        mpq_set(x, xp.x);
        mpq_set(y, xp.y);
    }
    ~xypair(){
        mpq_clears(x, y, nullptr);
    }
    mpq_t x, y;

    bool parse(mpq_t rslt, char *from, char **after) {
        char *eon = from;
        while (isdigit(*eon) || *eon == '/' || *eon == '-') ++eon;
        if (*eon) { *eon = '\0'; ++eon; }
        if (0 == mpq_set_str(rslt, from, 0)) {
            if (after) *after = eon;
            return true;
        }
        return false;
    }

    bool parse(char *from, char **after){
        char *a1;
        if (!parse(x, from, &a1)) return false;
        if (! *a1) return false;
        return parse(y, a1, after);
    }
};

#include <cctype>
#include <gmpxx.h>

#endif //ICFPC2016_XYPAIR_H
