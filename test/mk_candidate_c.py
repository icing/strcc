#!/usr/bin/env python3
import json
import os
import re
import sys
from typing import List


def up_lo(c: str, low):
    return c.lower() if low else c.upper()

def seesaw_case(s: str):
    return ''.join(up_lo(c, idx & 0x1) for idx, c in enumerate(s))

def typo(c: str, typo):
    return '@' if (typo) else c

def add_typo(s: str, place):
    return ''.join(typo(c, idx == place) for idx, c in enumerate(s))


def lit_variations(literal: str) -> List[str]:
    return [
        literal, literal, literal,
        literal, literal, literal,
        literal.upper(), literal.lower(), seesaw_case(literal),
        add_typo(literal, 4), add_typo(literal, 8), add_typo(literal, 12),
        literal+'X', literal[:-1], literal[0],
    ]


def main(argv):
    rv = 0
    if len(argv) == 2:
        json_file = argv[1]
        if not os.path.isfile(json_file):
            sys.stderr.write(f"not found: {json_file}\n")
            sys.exit(1)

        with open(json_file) as fd:
            literals = json.load(fd)
        cname = re.sub(r'\.json$', '', os.path.basename(json_file))

        # static strcc_lit_cmp CMP1 = {
        #     "literal",
        #     { "LITERAL", "literal", "LiTeRaL", "literEL", "x", "LiTeRaLLiTeRaL", }
        # };
        # static const strcc_lit_cmp* test_cases1[] = {
        #     &CMP1,
        #     NULL,
        # };
        for idx, literal in enumerate(literals):
            print(f"static strcc_lit_cmp {cname}_{idx} = {{")
            print(f"  \"{literal}\", {{")
            for v in lit_variations(literal):
                print(f"    \"{v}\",")
            print("  },")
            print("};")
        print(f"const strcc_lit_cmp* {cname}_cases[] = {{")
        for idx, literal in enumerate(literals):
            print(f"  &{cname}_{idx},")
        print("  NULL,")
        print("};")
    else:
        sys.stderr.write(f"{argv[0]}: json_file\n")
        rv = 2
    sys.exit(rv)


if __name__ == "__main__":
    main(sys.argv)
