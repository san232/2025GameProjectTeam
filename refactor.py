import os
import glob
import re

files = glob.glob('2025_winapi_framework_22/*.cpp')

for file in files:
    with open(file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    changed = False

    # Add SpriteRenderer.h if needed
    if ('TransparentBlt' in content or 'StretchBlt' in content) and 'SpriteRenderer.h' not in content:
        content = content.replace('#include "pch.h"\n', '#include "pch.h"\n#include "SpriteRenderer.h"\n')
        changed = True

    # Replace StretchBlt
    # StretchBlt(_hdc, 0, 0, rect.right, rect.bottom, pTex->GetTextureDC(), 0, 0, texWidth, texHeight, SRCCOPY);
    stretch_pattern = r'StretchBlt\s*\(\s*_hdc\s*,\s*0\s*,\s*0\s*,\s*rect\.right\s*,\s*rect\.bottom\s*,\s*pTex->GetTextureDC\(\)\s*,\s*0\s*,\s*0\s*,\s*texWidth\s*,\s*texHeight\s*,\s*SRCCOPY\s*\);'
    if re.search(stretch_pattern, content):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(pTex->GetSRV(), 0, 0, (float)rect.right, (float)rect.bottom, 0, 0, (float)texWidth, (float)texHeight, (float)texWidth, (float)texHeight);'
        content = re.sub(stretch_pattern, replacement, content)
        changed = True

    # Replace TransparentBlt in bullets
    # ::TransparentBlt(_hdc, dx, dy, width, height, m_pTex->GetTextureDC(), 0, 0, width, height, RGB(255, 0, 255));
    trans_pattern1 = r'::TransparentBlt\s*\(\s*_hdc\s*,\s*dx\s*,\s*dy\s*,\s*width\s*,\s*height\s*,\s*m_pTex->GetTextureDC\(\)\s*,\s*0\s*,\s*0\s*,\s*width\s*,\s*height\s*,\s*RGB\s*\(\s*255\s*,\s*0\s*,\s*255\s*\)\s*\);'
    if re.search(trans_pattern1, content):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(m_pTex->GetSRV(), (float)dx, (float)dy, (float)width, (float)height, 0, 0, (float)width, (float)height, (float)width, (float)height);'
        content = re.sub(trans_pattern1, replacement, content)
        changed = True
        
    trans_pattern2 = r'::TransparentBlt\s*\(\s*_hdc\s*,\s*left\s*,\s*top\s*,\s*width\s*,\s*height\s*,\s*texDC\s*,\s*0\s*,\s*0\s*,\s*texWidth\s*,\s*texHeight\s*,\s*RGB\s*\(\s*255\s*,\s*0\s*,\s*255\s*\)\s*\);'
    if re.search(trans_pattern2, content):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(m_hpBarTex->GetSRV(), (float)left, (float)top, (float)width, (float)height, 0, 0, (float)texWidth, (float)texHeight, (float)texWidth, (float)texHeight);'
        content = re.sub(trans_pattern2, replacement, content)
        changed = True

    # LevelUpManager.cpp
    trans_pattern3 = r'TransparentBlt\(\s*hdc\s*,\s*drawX\s*,\s*drawY\s*,\s*ICON_SIZE\s*,\s*ICON_SIZE\s*,\s*srcDC\s*,\s*0\s*,\s*0\s*,\s*texWidth\s*,\s*texHeight\s*,\s*RGB\s*\(\s*255\s*,\s*0\s*,\s*255\s*\)\s*\);'
    if re.search(trans_pattern3, content):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(iconTex->GetSRV(), (float)drawX, (float)drawY, (float)ICON_SIZE, (float)ICON_SIZE, 0, 0, (float)texWidth, (float)texHeight, (float)texWidth, (float)texHeight);'
        content = re.sub(trans_pattern3, replacement, content)
        changed = True

    if changed:
        # Also fix any remaining GetTextureDC manually if missed
        content = content.replace('HDC srcDC = iconTex->GetTextureDC();', '')
        content = content.replace('HDC texDC = m_hpBarTex->GetTextureDC();', '')
        
        with open(file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Updated {file}")
