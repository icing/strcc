#!/usr/bin/env python3
import json
import os
import re
import subprocess
import sys

RE_HEADER_LITERAL = re.compile(r'[^"]*header[^"]*"(?P<literal>[^"]+)".*')


def main(argv):
    rv = 0
    if len(argv) == 2:
        httpd_src = argv[1]
        if not os.path.isdir(httpd_src):
            sys.stderr.write(f"not a directory: {httpd_src}\n")
            sys.exit(1)
        p = subprocess.run([
            'fgrep', '-r',
            '-e', 'apr_table_get(',
            '-e', 'apr_table_set(',
            os.path.join(httpd_src, 'server'),
            os.path.join(httpd_src, 'modules')
        ], capture_output=True, text=True)
        if p.returncode != 0:
            sys.stderr.write(p.stderr)
            sys.exit(1)
        literals = []
        for l in p.stdout.splitlines(False):
            m = RE_HEADER_LITERAL.match(l)
            if m:
                literals.append(m.group('literal'))
        sys.stderr.write(f"found {len(literals)} literals in httpd headers handling at {httpd_src}\n")
        print(f"{json.JSONEncoder().encode(literals)}")
    else:
        sys.stderr.write(f"{argv[0]}: httpd-src-dir\n")
        rv = 2
    sys.exit(rv)


if __name__ == "__main__":
    main(sys.argv)
