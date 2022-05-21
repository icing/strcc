# strcc

A comparison of `strcasecmp()` and alternate implementations - on a limited data set, strangely
inspired by HTTP use cases.

## Installation

Check out the this repository, run the usual autoconf/automake magic words:

```
> autoreconf -i
> ./configure --enable-werror
> make
```

## Run it

By default, it uses the standard `strcasecmp()`:

```
> LOCALE=C time src/strcc_bench
libc: matches: 229272795
        4,73 real         4,64 user         0,07 sys
```

Run the custom function from the Apache Runtime (as in httpd):

```
> LOCALE=C time src/strcc_bench apr
apr: matches: 229272795
        2,66 real         2,59 user         0,06 sys
```

Run the custom function from curl:

```
> LOCALE=C time src/strcc_bench curl
curl: matches: 229272795
        3,00 real         2,71 user         0,05 sys
```

This runs on a set of strings extracted from HTTPD sources (further description
about reasoning below). This is totally not representative, but maybe good for
HTTP transfer usage.

The iterations run can be specified in the second argument. To run it only 1000 times, you invoke:

```
> LOCALE=C time src/strcc_bench curl 1000
```

To make it a bit more realistic, the sample strings are copied 1000 times (by default). 
You can specify the number of copies as the third argument:

```
# run the iterations on a single data set
> LOCALE=C time src/strcc_bench curl 10000 1
```


## Test Sets

The domain `strcasecmp()` is running in plays a determining role. The length of the strings
encountered, the frequency of compares of an equal, matching and non-matching strings varies
greatly in applications.

In most applications, `strcasecmp()` is done with a fixed set of known strings against
input received from the "outside" (network, file). The properties of this fixed set becomes
a dominant factor. 

The other is how often the input matches or how narrowly it mismatches. Optimizations
for longer strings obviously do not matter if the first character is always different.

### HTTP

The HTTP protocol uses case-insensitive method and header values, domain names. There is commonly
a small set of fixed strings that are checked against against values received over the network (either
in a request or in a response).

Headers are commonly hashed, but lookup still requires a `strcasecmp` on a matching hash entry. Since
we only want to test the string compares, the efficiency of hash value calculation and matches are
disregarded. Assuming the hashing is done well, the string compares mostly run into cases where
the header name matches. 

The form of the match differs with the HTTP protocol version used. In HTTP/1.x
names mostly arrive in the camel case notation used in the standards. In HTTP/2 and 3, header 
field names on the wire are lower-case only. But code checking the headers is often ignorant
of the protocol version. Even though `content-length` arrives as such on a HTTP/2 request, server
code commonly checks for the presence of `Content-Length`.

HTTP method names are few and short, this compares are commonly hand-optimized and do not use
`strcasecmp`. It seems therefore valid to focus on header field names only.

A hash map of headers in HTTP is already calculated case-insensitive. If it is done well,
a lookup will most frequently either miss, hit a matching field or a field that already varies
in the first few characters.

