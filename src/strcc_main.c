/***************************************************************************
 * Copyright (C) 2022, Stefan Eissing, <stefan@eissing.org>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "strcc.h"
#include "strcc_apr.h"
#include "strcc_curl.h"
#include "strcc_cases_httpd.h"


static size_t libc_count_matches(const strcc_lit_cmp *cmp)
{
    size_t i, n = 0;
    for (i = 0; cmp->candidate[i][0]; ++i) {
        if (!strcasecmp(cmp->literal, cmp->candidate[i])) {
            ++n;
        }
    }
    return n;
}

static size_t apr_count_matches(const strcc_lit_cmp *cmp)
{
    size_t i, n = 0;
    for (i = 0; cmp->candidate[i][0]; ++i) {
        if (!apr_cstr_casecmp(cmp->literal, cmp->candidate[i])) {
            ++n;
        }
    }
    return n;
}

static size_t curl_count_matches(const strcc_lit_cmp *cmp)
{
    size_t i, n = 0;
    for (i = 0; cmp->candidate[i][0]; ++i) {
        if (!Curl_strcasecompare(cmp->literal, cmp->candidate[i])) {
            ++n;
        }
    }
    return n;
}

static size_t iter_case(count_matches_fn *fn,
                        const strcc_lit_cmp *cmp,
                        size_t copies,
                        size_t iterations)
{
    strcc_lit_cmp *cmp_copies;
    size_t i, j, n;

    cmp_copies = calloc(copies, sizeof(strcc_lit_cmp));
    for (i = n = 0; i < copies; ++i) {
        memcpy(&cmp_copies[i], cmp, sizeof(strcc_lit_cmp));
    }
    for (i = n = 0; i < iterations; ++i) {
        for (j = 0; j < copies && i < iterations; ++j, ++i) {
            n += fn(&cmp_copies[j]);
        }
    }
    free(cmp_copies);
    return n;
}

static size_t iter_cases(count_matches_fn *fn,
                         const strcc_lit_cmp *cases[],
                         size_t copies,
                         size_t iterations)
{
    size_t i, n = 0;

    for (i = 0; cases[i]; ++i) {
        n += iter_case(fn, cases[i], copies, iterations);
    }
    return n;
}

int main(int argc, const char *const *argv)
{
    size_t n, copies = 1000, iterations = 100000;
    count_matches_fn *fn = libc_count_matches;

    if (argc > 1) {
        if (!strcmp("apr", argv[1])) {
            fn = apr_count_matches;
        }
        else if (!strcmp("curl", argv[1])) {
            fn = curl_count_matches;
        }
        else if (!strcmp("libc", argv[1])) {
            fn = libc_count_matches;
        }
        else {
            fprintf(stderr, "unknown compare variation: '%s'\n", argv[1]);
            exit(1);
        }
        if (argc > 2) {
            int iarg;
            iarg = atoi(argv[2]);
            if (iarg <= 0) {
                fprintf(stderr, "iterations must be a positive number: '%s'\n", argv[2]);
                exit(1);
            }
            iterations = (size_t)iarg;
            if (argc > 3) {
                iarg = atoi(argv[3]);
                if (iarg <= 0) {
                    fprintf(stderr, "copies must be a positive number: '%s'\n", argv[3]);
                    exit(1);
                }
                copies = (size_t)iarg;
            }
        }
    }
    (void)argc;
    (void)argv;
    n = iter_cases(fn, httpd_cases, copies, iterations);
    printf("matches: %ld\n", n);
    return 0;
}