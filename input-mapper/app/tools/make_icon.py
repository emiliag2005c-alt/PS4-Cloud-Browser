#!/usr/bin/env python3
"""Generate a simple opaque 512x512 RGB icon without external dependencies."""
import math
import struct
import sys
import zlib


SIZE = 512
pixels = bytearray(SIZE * SIZE * 3)


def put(x, y, color):
    if 0 <= x < SIZE and 0 <= y < SIZE:
        offset = (y * SIZE + x) * 3
        pixels[offset:offset + 3] = bytes(color)


def rect(x0, y0, x1, y1, color):
    for y in range(max(0, y0), min(SIZE, y1)):
        for x in range(max(0, x0), min(SIZE, x1)):
            put(x, y, color)


def rounded_rect(x0, y0, x1, y1, radius, color):
    for y in range(y0, y1):
        for x in range(x0, x1):
            cx = min(max(x, x0 + radius), x1 - radius - 1)
            cy = min(max(y, y0 + radius), y1 - radius - 1)
            if (x - cx) ** 2 + (y - cy) ** 2 <= radius ** 2:
                put(x, y, color)


def circle(cx, cy, radius, color, thickness=0):
    outer = radius * radius
    inner = (radius - thickness) ** 2 if thickness else -1
    for y in range(max(0, cy - radius), min(SIZE, cy + radius + 1)):
        for x in range(max(0, cx - radius), min(SIZE, cx + radius + 1)):
            d = (x - cx) ** 2 + (y - cy) ** 2
            if d <= outer and (thickness == 0 or d >= inner):
                put(x, y, color)


def line(x0, y0, x1, y1, color, thickness=4):
    steps = max(abs(x1 - x0), abs(y1 - y0), 1)
    for i in range(steps + 1):
        x = round(x0 + (x1 - x0) * i / steps)
        y = round(y0 + (y1 - y0) * i / steps)
        circle(x, y, thickness, color)


# Deep blue background with a subtle vertical gradient.
for y in range(SIZE):
    t = y / (SIZE - 1)
    shade = int(14 + 13 * t)
    rect(0, y, SIZE, y + 1, (shade, shade + 8, shade + 24))

# A compact keyboard and mouse mark on a deep blue background.
rounded_rect(48, 145, 464, 365, 34, (20, 34, 58))
rounded_rect(66, 165, 446, 345, 22, (8, 17, 34))
# Keyboard body and keycaps.
rounded_rect(88, 195, 330, 310, 18, (45, 197, 231))
for row, count in enumerate((10, 9, 7)):
    y = 211 + row * 27
    width = 20 if row == 0 else (22 if row == 1 else 24)
    gap = 4
    total = count * width + (count - 1) * gap
    x0 = 209 - total // 2
    for col in range(count):
        rounded_rect(x0 + col * (width + gap), y,
                     x0 + col * (width + gap) + width, y + 17,
                     4, (8, 17, 34))
rounded_rect(145, 293, 274, 302, 4, (8, 17, 34))
# Mouse body, wheel and center split.
rounded_rect(342, 188, 420, 315, 36, (235, 248, 255))
rounded_rect(350, 196, 412, 307, 28, (45, 197, 231))
line(381, 198, 381, 251, (8, 17, 34), 3)
rounded_rect(375, 211, 387, 237, 5, (8, 17, 34))
circle(381, 270, 5, (8, 17, 34))
# Small status accent.
circle(421, 119, 13, (82, 220, 151))
def png_chunk(kind, data):
    payload = kind + data
    return struct.pack(">I", len(data)) + payload + struct.pack(">I", zlib.crc32(payload) & 0xffffffff)


def save(path):
    raw = bytearray()
    stride = SIZE * 3
    for y in range(SIZE):
        raw.append(0)
        raw.extend(pixels[y * stride:(y + 1) * stride])
    header = struct.pack(">IIBBBBB", SIZE, SIZE, 8, 2, 0, 0, 0)
    png = b"\x89PNG\r\n\x1a\n"
    png += png_chunk(b"IHDR", header)
    png += png_chunk(b"IDAT", zlib.compress(bytes(raw), 9))
    png += png_chunk(b"IEND", b"")
    with open(path, "wb") as output:
        output.write(png)


if __name__ == "__main__":
    save(sys.argv[1] if len(sys.argv) > 1 else "sce_sys/icon0.png")
