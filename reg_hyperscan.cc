#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <string>
#include <vector>

#include <chrono>

#include "hs/hs.h"


struct Matcher
{
    std::vector<const char*> cstrPatterns;
    std::vector<unsigned int> flags;
    std::vector<unsigned int> ids;
    std::vector<uint64_t> match_cnt;
};

static int onMatch(unsigned int id, 
                   unsigned long long from, 
                   unsigned long long to,
                   unsigned int flags, 
                   void *ctx) 
{
    // Our context points to a size_t storing the match count
    Matcher *matcher = (Matcher *)ctx;
    matcher->match_cnt[id]++;
    return 0; // continue matching
}

static int scan(hs_database_t* db_block, hs_scratch_t *scratch, const char* src, ssize_t len, Matcher* m)
{
    hs_error_t err = hs_scan(db_block, src, len, 0,
                      scratch, onMatch, m);
    if (err != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan packet. Exiting. \n");
        return -1;
    }
    return 0;
}


static hs_database_t* buildDatabase(const std::vector<const char*> &expressions,
                                    const std::vector<unsigned int> flags,
                                    const std::vector<unsigned int> ids,
                                    unsigned int mode) 
{
    hs_database_t* db = nullptr;
    hs_compile_error_t* compileErr = nullptr;
    hs_error_t err;

    err = hs_compile_multi(expressions.data(), flags.data(), ids.data(),
                           expressions.size(), mode, nullptr, &db, &compileErr);

    if (err != HS_SUCCESS) {
        if (compileErr->expression < 0) {
            fprintf(stderr, "ERROR:hs_compile_multi: %s\n", compileErr->message);
        } else {
            fprintf(stderr, "ERROR:hs_compile_multi: %03d, %s: %s\n"
                            , compileErr->expression
                    , expressions[compileErr->expression]
                    , compileErr->message);
        }
        hs_free_compile_error(compileErr);
        return nullptr;
    }
    return db;
}


//vendors/hyperscan/build/bin/hscheck -e reg_check.txt
int hs_pattern_check(const char* pattern)
{
    hs_expr_info_t *info = nullptr;
    hs_compile_error_t *compile_err = nullptr;

    hs_error_t err = hs_expression_info(pattern, HS_FLAG_DOTALL, &info, &compile_err);
    if (err != 0) {
        fprintf(stderr, "ERROR:%d,%s,%s\n", err, pattern, compile_err->message);
    }

    free(info);
    hs_free_compile_error(compile_err);
//    if (compile_err != nullptr) {
//        fprintf(stderr, "\t:%d,compile_err \n", err);
//        return -1;
//    }
    return err;
}


int hs_test(std::vector<std::string> patterns, const char* src, ssize_t len)
{
    printf("hs_version:%s , valid:%d\n", hs_version(), hs_valid_platform());

    Matcher matcher;
    unsigned int index = 0;
    for (const auto &pattern : patterns) {
        if (hs_pattern_check(pattern.c_str()) == 0) 
        {
            matcher.cstrPatterns.push_back(pattern.c_str());
            matcher.flags.push_back(HS_FLAG_DOTALL);
            matcher.ids.push_back(index++);
            matcher.match_cnt.push_back(0);
        }
    }

    hs_database_t* db = buildDatabase(matcher.cstrPatterns, matcher.flags, matcher.ids, HS_MODE_BLOCK);
    if (db == nullptr) {
        return -1;
    }

    hs_scratch_t* scratch = nullptr;
    hs_error_t err = hs_alloc_scratch(db, &scratch);
    if (err != HS_SUCCESS) {
        fprintf(stderr, "ERROR: could not allocate scratch space. err = %d \n", err);
        return -1;
    }

    auto start = std::chrono::system_clock::now();
    int ret = scan(db, scratch, src, len, &matcher);
    if (ret == 0) {
        index = 0;
        for (auto m : matcher.match_cnt) {
            printf("%02d: match:%lu \n", index++, m);
        }
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    printf("takes %f \n", elapsed_seconds.count());

    return ret;
}