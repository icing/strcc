# Copyright 2022 Stefan Eissing (<stefan@eissing.org>)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign

#---------------------------------------------------------------------------------------------------
# a2md executable
#---------------------------------------------------------------------------------------------------

bin_PROGRAMS       = strcc_bench

strcc_bench_CFLAGS  = -g -I../src -std=c99 -D_GNU_SOURCE @WERROR_CFLAGS@

STRCC_BENCH_OBJECTS = \
    strcc_main.c \
    strcc_apr.c \
    strcc_curl.c \
    strcc_cases_httpd.c

STRCC_BENCH_HFILES = \
    version.h \
    strcc.h \
    strcc_apr.h \
    strcc_curl.h \
    strcc_cases_httpd.h

strcc_bench_SOURCES = $(STRCC_BENCH_HFILES) $(STRCC_BENCH_OBJECTS)

