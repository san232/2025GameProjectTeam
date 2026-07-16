import os
import glob
import re

files = glob.glob('2025_winapi_framework_22/*.cpp')

for file in files:
    with open(file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    changed = False

    # Replace TransparentBlt multi-line calls
    # We can just remove whitespace and newlines inside the macro call
    # But a simpler way is to replace `::TransparentBlt(_hdc` with a single line
    
    # Let's match ::TransparentBlt(_hdc up to the closing );
    pattern = r'::TransparentBlt\s*\(\s*_hdc\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*m_pTex->GetTextureDC\(\)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*RGB\(255,\s*0,\s*255\)\s*\);'
    
    # We replace with SpriteRenderer call
    new_content = re.sub(pattern, r'GET_SINGLE(SpriteRenderer)->Draw(m_pTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));', content, flags=re.DOTALL)
    
    if new_content != content:
        content = new_content
        changed = True

    # For BossKnight
    pattern2 = r'::TransparentBlt\s*\(\s*_hdc\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*texDC\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*RGB\(255,\s*0,\s*255\)\s*\);'
    new_content = re.sub(pattern2, r'GET_SINGLE(SpriteRenderer)->Draw(m_hpBarTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));', content, flags=re.DOTALL)
    
    if new_content != content:
        content = new_content
        changed = True
        
    # LevelUpManager
    pattern3 = r'TransparentBlt\s*\(\s*hdc\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*srcDC\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*RGB\(255,\s*0,\s*255\)\s*\);'
    new_content = re.sub(pattern3, r'GET_SINGLE(SpriteRenderer)->Draw(iconTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));', content, flags=re.DOTALL)
    
    if new_content != content:
        content = new_content
        changed = True

    # StretchBlt
    pattern4 = r'StretchBlt\s*\(\s*_hdc\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*pTex->GetTextureDC\(\)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*([^,]+)\s*,\s*SRCCOPY\s*\);'
    new_content = re.sub(pattern4, r'GET_SINGLE(SpriteRenderer)->Draw(pTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));', content, flags=re.DOTALL)
    
    if new_content != content:
        content = new_content
        changed = True

    if changed:
        if 'SpriteRenderer.h' not in content:
            content = content.replace('#include "pch.h"', '#include "pch.h"\n#include "SpriteRenderer.h"')
            
        with open(file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Updated {file}")
