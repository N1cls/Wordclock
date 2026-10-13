#!/usr/bin/env python3
"""Build WordClockWithWeb/webpage.h from web/index.html.

The firmware serves the web interface as a gzipped byte array from flash.
The GitHub Action runs it automatically and commits the result, so after editing web/index.html
it is enough to push. To test a changed page on your own clock before pushing, run it locally:

    python3 tools/build_webpage.py

webpage.h is only rewritten when the page inside it differs from web/index.html, so different
Python or zlib versions (which may compress slightly differently) do not cause pointless changes.

Options:
    --check   only verify that webpage.h contains the current web/index.html (exit code 1 if not)
    --force   always rewrite webpage.h
"""
import gzip
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
SRC = ROOT / "web" / "index.html"
DST = ROOT / "WordClockWithWeb" / "webpage.h"


def page() -> bytes:
    # Same result on Windows (CRLF line endings in the working copy) and Linux (LF).
    return SRC.read_bytes().replace(b"\r\n", b"\n")


def page_in_header():
    """The HTML contained in the existing webpage.h, or None if it is missing or unreadable."""
    try:
        text = DST.read_text()
        array = text[text.index("{") + 1:text.rindex("}")]
        data = bytes(int(x, 16) for x in re.findall(r"0x([0-9a-fA-F]{2})", array))
        return gzip.decompress(data)
    except (OSError, ValueError, EOFError, gzip.BadGzipFile):
        return None


def build(html: bytes) -> str:
    # mtime=0 keeps the output identical for identical input.
    data = bytearray(gzip.compress(html, compresslevel=9, mtime=0))
    # Byte 9 of the gzip header names the operating system. Python versions fill it differently
    # (3 = Unix or 255 = unknown), so it is set to a fixed value.
    data[9] = 255
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
    html = page()
    up_to_date = page_in_header() == html
    if "--check" in sys.argv:
        print("webpage.h is up to date." if up_to_date else "webpage.h is out of date. Run: python3 tools/build_webpage.py")
        return 0 if up_to_date else 1
    if up_to_date and "--force" not in sys.argv:
        print("webpage.h is up to date.")
        return 0
    DST.write_text(build(html))
    print(f"Wrote {DST.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
