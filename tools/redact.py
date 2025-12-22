#!/usr/bin/env python3
import re
import sys
from typing import Pattern

EMAIL_RE: Pattern = re.compile(r"\b[\w.%+-]+@[\w.-]+\.[A-Za-z]{2,}\b")
API_KEY_RE: Pattern = re.compile(r"(?i)\b(api[_-]?key|secret|token|password)\b\s*[:=]\s*\S+")
LONG_HEX_RE: Pattern = re.compile(r"\b[0-9a-fA-F]{32,}\b")
CC_RE: Pattern = re.compile(r"\b(?:4[0-9]{12}(?:[0-9]{3})?|5[1-5][0-9]{14}|3[47][0-9]{13}|6(?:011|5[0-9]{2})[0-9]{12})\b")

def redact(text: str) -> str:
    text = EMAIL_RE.sub('[REDACTED_EMAIL]', text)
    text = API_KEY_RE.sub(lambda m: m.group(0).split(':')[0] + ': [REDACTED]', text)
    text = LONG_HEX_RE.sub('[REDACTED_TOKEN]', text)
    text = CC_RE.sub('[REDACTED_CC]', text)
    return text


def main(argv=None):
    argv = argv or sys.argv[1:]
    if not argv:
        data = sys.stdin.read()
    else:
        parts = []
        for p in argv:
            try:
                with open(p, 'r', encoding='utf-8') as f:
                    parts.append(f.read())
            except Exception:
                parts.append(p)
        data = '\n'.join(parts)
    print(redact(data))


if __name__ == '__main__':
    main()
