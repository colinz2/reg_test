#include <iostream>

#include "pcre.h"

#include <string>
#include <vector>

#define OVECCOUNT 30000 /* should be a multiple of 3 */

std::vector<pcre*> g_pcre_patterns;

int pcre_build(std::vector<std::string> regs)
{
    const char *error;
    int erroffset;

    for (auto reg : regs) {
        auto p = pcre_compile(reg.c_str(), 0, &error, &erroffset, NULL);
        if (p != nullptr) {
            g_pcre_patterns.push_back(p);
        } else {
            fprintf(stderr, "pcre_compile error [%s], %s", reg.c_str(), error);
        }
    }
    return 0;
}

int pcre_math(const char* src, ssize_t len)
{
    int ret = 0;
    int ovector[OVECCOUNT];

    for (auto p : g_pcre_patterns) {
        ret = pcre_exec(p, NULL, src, len, 0, 0, ovector, OVECCOUNT);
        if (ret == PCRE_ERROR_NOMATCH)
        {
            printf("Sorry, no match ...\n");
        }
        else
        {
            printf("ret = %d \n", ret);
        }
    }
    return  0;
}

