#!/usr/bin/evn python3
import os
import re
from time import clock

reg_list = [
    r"Twain",
    r"(?i)Twain",
    r"[a-z]shing",
    r"Huck[a-zA-Z]+|Saw[a-zA-Z]+",
    r"\\b\\w+nn\\b",
    r"[a-q][^u-z]{13}x",
    r"Tom|Sawyer|Huckleberry|Finn",
    r"(?i)Tom|Sawyer|Huckleberry|Finn",
    r".{0,2}(Tom|Sawyer|Huckleberry|Finn)",
    r".{2,4}(Tom|Sawyer|Huckleberry|Finn)",
    r"Tom.{10,25}river|river.{10,25}Tom",
    r"[a-zA-Z]+ing",
    r"\\s[a-zA-Z]{0,12}ing\\s",
    r"([A-Za-z]awyer|[A-Za-z]inn)\\s",
    r"[\"'][^\"']{0,30}[?!\\.][\"']",
    r"\u221E|\u2713",
    r"\\p{Sm}"                               #any mathematical symbol
]

def reg_test(file_content_str):
    start = clock()
    prog = [re.compile(r, 0) for r in reg_list]
    print("compile takes: %f" % (clock() - start))
    for i, p in enumerate(prog):
        start = clock()
        m = p.findall(file_content_str)
        print("%02d: match:%-08d takes: %f" % (i, len(m), clock() - start))

def read_file(cvd_file_name):
    with open(cvd_file_name, 'rb') as cvd_file:
        b = cvd_file.read()
        print("len b", len(b))
        return b.decode('utf-8')


def run():
    start = clock()
    file_content_str = read_file("data/mtent12.txt")
    print("file len is : %d, takes %f " % (len(file_content_str), clock() - start))

    start = clock()
    reg_test(file_content_str)
    print("reg_test takes %f "% (clock() - start))


if __name__ == '__main__':
    run()