#include "hs/hs.h"
#include "stdio.h"

#include <string>
#include <vector>

int hs_test(std::vector<std::string> regs, const char* src, ssize_t len)
{
    printf("hs_version:%s \n", hs_version());
    return 0;
}