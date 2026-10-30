#!/usr/bin/env python3
"""Convert a photo of Pepper into a flash-resident 128x141 JPEG header.
Usage: python firmware/assets/make_pepper_header.py firmware/assets/ID_two/pepper_source.jpg
Requires: pip install pillow
"""
from pathlib import Path
import sys
from PIL import Image, ImageOps

if len(sys.argv) != 2:
    raise SystemExit("Usage: python firmware/assets/make_pepper_header.py firmware/assets/ID_two/pepper_source.jpg")
source = Path(sys.argv[1])
target = Path(__file__).resolve().parents[1] / 'code/Cyber_ID_SuperMini/pepper_photo.h'
with Image.open(source) as im:
    im = ImageOps.exif_transpose(im).convert('RGB')
    im = ImageOps.fit(im, (128,141), method=Image.Resampling.LANCZOS)
    from io import BytesIO
    buffer = BytesIO()
    im.save(buffer, format='JPEG', quality=80, subsampling=2, progressive=False)
    payload = buffer.getvalue()
lines = []
for i in range(0, len(payload), 16):
    lines.append('  ' + ', '.join(f'0x{b:02X}' for b in payload[i:i+16]) + ',')
target.write_text('#pragma once\n#include <Arduino.h>\nconst uint8_t pepperPhoto[] PROGMEM = {\n' + '\n'.join(lines) + '\n};\nconst uint32_t pepperPhotoSize = sizeof(pepperPhoto);\n')
print(f'Wrote {target} ({len(payload)} bytes)')
