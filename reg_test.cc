#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <vector>

extern int pcre_build(std::vector<std::string> regs);
extern int pcre_math(const char* src, ssize_t len);
extern void hs_test();

const char patterns[][128] = {
    "Twain",
    "(?i)Twain",
    "[a-z]shing",
    "Huck[a-zA-Z]+|Saw[a-zA-Z]+",
    "\\b\\w+nn\\b",
    "[a-q][^u-z]{13}x",
    "Tom|Sawyer|Huckleberry|Finn",
    "(?i)Tom|Sawyer|Huckleberry|Finn",
    ".{0,2}(Tom|Sawyer|Huckleberry|Finn)",
    ".{2,4}(Tom|Sawyer|Huckleberry|Finn)",
    "Tom.{10,25}river|river.{10,25}Tom",
    "[a-zA-Z]+ing",
    "\\s[a-zA-Z]{0,12}ing\\s",
    "([A-Za-z]awyer|[A-Za-z]inn)\\s",
    "[\"'][^\"']{0,30}[?!\\.][\"']",
    "\u221E|\u2713",
    "\\p{Sm}", // any mathematical symbol
};


struct Mmap
{
    int fd;
    char* data;
    size_t length;
};


int load_file(const char* filename, Mmap* mp)
{
    ssize_t file_size = -1;
    struct stat statbuff;

    if (stat(filename, &statbuff) < 0) {
        return -1;
    } else {
        file_size = statbuff.st_size;
    }

    mp->fd = open(filename, O_RDONLY);
    if (mp->fd < 0) {
        free(mp);
        return -1;
    }
    mp->data = (char*) malloc(file_size);
    mp->length = file_size;
    read(mp->fd, mp->data, file_size);
    close(mp->fd);
    return 0;
}

int main()
{
    Mmap mmap;
    std::vector<std::string> regs;
    load_file("data/mtent12.txt", &mmap);

    for (int i = 0; i < sizeof(patterns) / 128; i++) {
        regs.push_back(std::string(patterns[i]));
    }

    pcre_build(regs);
    pcre_math(mmap.data, mmap.length);

    hs_test();

    return 0;
}

