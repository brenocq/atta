# GitHub style colors with dark mode support
STYLE=f"""
<style>
    :root {{
         --border-color: #d1d9e0;
         --icon-open: #1a7f37;
         --icon-closed: #8250df;
         --icon-default: #59636e;
         --fgColor-default: #25292e;
         --fgColor-accent: #0969da;
         --fgColor-muted: #59636e;
         --button-background-color: #f6f8fa;
     }}

    text {{
        font-family: Arial, sans-serif;
        font-size: 14px;
    }}

    text.title {{
        fill: var(--fgColor-accent);
        font-size: 14px;
        font-weight: bold;
    }}

    text.counter {{
        fill: var(--fgColor-muted);
        font-size: 40px;
        text-anchor: middle;
    }}

    text.button {{
        font-size: 14px;
        fill: var(--fgColor-default);
    }}

    text.label {{
    }}

    rect.labelBg {{
        fill-opacity: 1.0;
    }}

    rect.button {{
        fill: var(--button-background-color);
        stroke: var(--border-color);
        stroke-width: 1px;
        rx: 6px;
        ry: 6px;
    }}

    rect.card {{
        fill: none;
        stroke: var(--border-color);
        stroke-width: 1.5px;
        rx: 6px;
        ry: 6px;
    }}

    .open {{
        fill: var(--icon-open);
    }}

    .closed {{
        fill: var(--icon-closed);
    }}

    .default {{
        fill: var(--icon-default);
    }}

    @media (prefers-color-scheme: light) {{
        text.label {{
            fill: #000000;
        }}
    }}

    @media (prefers-color-scheme: dark) {{
        :root {{
            --border-color: #3d444d;
            --icon-open: #57ab5a;
            --icon-closed: #986ee2;
            --icon-default: #9198a1;
            --fgColor-default: #d1d7e0;
            --fgColor-muted: #9198a1;
            --fgColor-accent: #478be6;
            --button-background-color: #2a313c;
        }}

        rect.labelBg {{
            fill-opacity: 0.2;
        }}
    }}
</style>
"""


# Estimated size of characters for Arial 14px
CHAR_WIDTH_TABLE_ARIAL14 = {
    'i': 4, 'I': 5, 'l': 4, '.': 4, ' ': 4, 't': 5, 'f': 5, 'j': 5, '(': 5, ')': 5,
    'r': 6, 's': 6, 'z': 6, '[': 5, ']': 5, '{': 6, '}': 6,
    'a': 7, 'b': 7, 'c': 7, 'd': 7, 'e': 7, 'g': 7, 'h': 7, 'k': 7, 'n': 7,
    'o': 7, 'p': 7, 'q': 7, 'u': 7, 'v': 7, 'x': 7, 'y': 7,
    'm': 9, 'w': 11,
    'L': 7, 'T': 7, 'F': 7, 'J': 7,
    'A': 8, 'B': 8, 'C': 8, 'D': 8, 'E': 8, 'G': 8, 'H': 8, 'K': 8, 'N': 8,
    'O': 8, 'P': 8, 'Q': 8, 'R': 8, 'S': 8, 'U': 8, 'V': 8, 'X': 8, 'Y': 8, 'Z': 8,
    'M': 10, 'W': 12,
    '0': 7, '1': 5, '2': 7, '3': 7, '4': 7, '5': 7, '6': 7, '7': 7, '8': 7, '9': 7,
    '_': 7, '-': 5, '=': 8, '+': 8, '*': 6, '#': 8, '@': 12, '!': 4, '?': 7,
    '/': 5, '\\': 5, '|': 4, '"': 5, "'": 4, ':': 4, ';': 4, ',': 4
}
DEFAULT_CHAR_WIDTH_ARIAL14 = 7 # Default for unknown characters

def estimate_text_width(text: str, size: int) -> int:
    """Estimates text width based on a lookup table (tuned for Arial 14px)."""
    # This example uses a fixed table, ignoring the 'size' parameter for now,
    # assuming the table IS for 14px. A more complex version could scale widths.
    if size != 14:
        # Simple scaling (very rough) or return error/warning
        scale_factor = size / 14.0
        return int(sum(CHAR_WIDTH_TABLE_ARIAL14.get(c, DEFAULT_CHAR_WIDTH_ARIAL14) * scale_factor for c in text))
    else:
        return sum(CHAR_WIDTH_TABLE_ARIAL14.get(c, DEFAULT_CHAR_WIDTH_ARIAL14) for c in text)

# Compute estimated text size in pixels
def estimate_text_size(text: str, font_size: int = 14) -> tuple[int, int]:
    """Returns estimated (width, height) for a single line."""
    width = estimate_text_width(text, font_size)
    height = int(font_size * 1.2)
    return width, height
