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

#ifndef __strcc_h__
#define __strcc_h__

#define CANDIDATES_MAX   20
#define CANDIDATE_MAX_LEN   200

typedef struct {
    const char *literal;
    char candidate[CANDIDATES_MAX][CANDIDATE_MAX_LEN+1];
} strcc_lit_cmp;

typedef size_t count_matches_fn(const strcc_lit_cmp *cmp);

#endif