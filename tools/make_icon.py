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

# Browser window.
rounded_rect(70, 105, 442, 407, 30, (8, 17, 34))
rounded_rect(78, 113, 434, 399, 24, (20, 34, 58))
rect(80, 151, 432, 158, (36, 57, 86))
circle(105, 133, 7, (255, 105, 105))
circle(130, 133, 7, (255, 202, 90))
circle(155, 133, 7, (82, 220, 151))

# Address bar.
rounded_rect(185, 122, 405, 144, 10, (12, 23, 42))
rect(202, 132, 370, 135, (76, 102, 137))

# Stylized globe / web symbol.
circle(256, 278, 91, (45, 197, 231), 7)
circle(256, 278, 61, (45, 197, 231), 5)
line(169, 278, 343, 278, (45, 197, 231), 4)
line(256, 190, 256, 366, (45, 197, 231), 4)
# Curved longitude approximations.
for xoff in (-32, 32):
    for y in range(215, 342):
        x = 256 + xoff * math.sqrt(max(0.0, 1 - ((y - 278) / 88) ** 2))
        put(round(x), y, (45, 197, 231))

# Bright cursor accent.
circle(344, 333, 18, (235, 248, 255))
line(338, 326, 365, 353, (8, 17, 34), 5)
line(350, 340, 359, 363, (8, 17, 34), 4)

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
