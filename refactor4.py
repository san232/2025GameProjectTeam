import os
import glob
import re

files = glob.glob('2025_winapi_framework_22/*.cpp')

for file in files:
    with open(file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    changed = False

    if ('Rectangle' in content or 'Ellipse' in content) and 'SpriteRenderer.h' not in content:
        content = content.replace('#include "pch.h"\n', '#include "pch.h"\n#include "SpriteRenderer.h"\n')
        changed = True

    # 1. Rectangle
    rect_pattern = r'(?:::\s*)?Rectangle\s*\(\s*_hdc\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,)]+)\s*\);'
    # We replace Rectangle(_hdc, left, top, right, bottom);
    # with GET_SINGLE(SpriteRenderer)->DrawRect((float)(left), (float)(top), (float)((right)-(left)), (float)((bottom)-(top)), XMFLOAT4(1,0,0,1));
    # Note: The color is hardcoded to Red/Black, I'll just use XMFLOAT4(1,0,0,1) for simplicity since it's mostly debug/HP bars.
    
    # Actually, we can use a generic color based on context, but red is fine for now.
    def rect_replace(match):
        left, top, right, bottom = match.groups()
        return f'GET_SINGLE(SpriteRenderer)->DrawRect((float)({left}), (float)({top}), (float)(({right})-({left})), (float)(({bottom})-({top})), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));'

    new_content = re.sub(rect_pattern, rect_replace, content)
    if new_content != content:
        content = new_content
        changed = True

    # 2. Ellipse
    ellipse_pattern = r'(?:::\s*)?Ellipse\s*\(\s*_hdc\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,)]+)\s*\);'
    def ellipse_replace(match):
        left, top, right, bottom = match.groups()
        return f'GET_SINGLE(SpriteRenderer)->DrawRect((float)({left}), (float)({top}), (float)(({right})-({left})), (float)(({bottom})-({top})), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));'
        
    new_content = re.sub(ellipse_pattern, ellipse_replace, content)
    if new_content != content:
        content = new_content
        changed = True

    if changed:
        with open(file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Updated {file}")
