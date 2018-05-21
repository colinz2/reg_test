#include <iostream>

#include <string.h>
#include "pcre.h"

#include <string>
#include <vector>

#include <chrono>


std::vector<pcre*> g_pcre_patterns;

int _pcre_build(std::vector<std::string> regs)
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

int _pcre_math(const char* src, ssize_t len)
{
    int ret = 0;
    int cnt = 0, index = 0, offset = 0;
    const int OVECCOUNT = 30; /* should be a multiple of 3 */
    int ovector[OVECCOUNT] = {0, };

    const char* m = "Twain Twain Twasin Twain ";

    for (auto p : g_pcre_patterns) {
        cnt = 0;
        offset = 0;
        memset(ovector, 0, sizeof(ovector));

        auto start = std::chrono::system_clock::now();

        do {
            ret = pcre_exec(p, NULL, src, len, offset, 0, ovector, OVECCOUNT);
            if (ret > 0)
            {
                offset = ovector[1];
                //printf("ret = %d , offset = %d \n", ret, offset);
                cnt += ret;
            }
        } while(ret > 0);

        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;

        printf("%02d: match:%d takes:%f\n", index, cnt, elapsed_seconds.count());
        index++;
    }
    return  0;
}


int pcre_test(std::vector<std::string> regs, const char* src, ssize_t len)
{
    _pcre_build(regs);
    return _pcre_math(src, len);
}

