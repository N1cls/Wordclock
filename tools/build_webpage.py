#!/usr/bin/env python3
"""Build WordClockWithWeb/webpage.h from web/index.html.

The firmware serves the web interface as a gzipped byte array from flash.
The GitHub Action runs it automatically and commits the result, so after editing web/index.html
it is enough to push. To test a changed page on your own clock before pushing, run it locally:

    python3 tools/build_webpage.py

Use --check to verify that webpage.h is up to date.
"""
import gzip
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
SRC = ROOT / "web" / "index.html"
DST = ROOT / "WordClockWithWeb" / "webpage.h"


def build() -> str:
    html = SRC.read_bytes()
    # mtime=0 keeps the output identical for identical input.
    data = gzip.compress(html, compresslevel=9, mtime=0)
    lines = []
    for i in range(0, len(data), 20):
        lines.append("  " + ", ".join(f"0x{b:02x}" for b in data[i:i + 20]) + ",")
    return (
        "// ###########################################################################################################################################\n"
        "// # WordClock web interface - GENERATED FILE, DO NOT EDIT.\n"
        "// # Source: web/index.html - edit that file. This one is rebuilt by the GitHub Action on every push,\n"
        "// # locally with: python3 tools/build_webpage.py\n"
        f"// # {len(html)} bytes HTML, {len(data)} bytes gzipped.\n"
        "// ###########################################################################################################################################\n"
        "#pragma once\n"
        "#include <Arduino.h>\n\n"
        f"#define WEBPAGE_GZ_LEN {len(data)}\n"
        "const uint8_t WEBPAGE_GZ[] PROGMEM = {\n"
        + "\n".join(lines)
        + "\n};\n"
    )


def main() -> int:
    out = build()
    if "--check" in sys.argv:
        if not DST.exists() or DST.read_text() != out:
            print("webpage.h is out of date. Run: python3 tools/build_webpage.py")
            return 1
        print("webpage.h is up to date.")
        return 0
    DST.write_text(out)
    print(f"Wrote {DST.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
