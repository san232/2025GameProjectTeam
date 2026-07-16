import os
import glob
import re

files = glob.glob('2025_winapi_framework_22/*.cpp')

for file in files:
    with open(file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    changed = False

    if ('TransparentBlt' in content or 'StretchBlt' in content) and 'SpriteRenderer.h' not in content:
        content = content.replace('#include "pch.h"\n', '#include "pch.h"\n#include "SpriteRenderer.h"\n')
        changed = True

    # 1. LevelUpManager.cpp
    level_up_pattern = r'TransparentBlt\(\s*hdc\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*srcDC\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*RGB\(255,\s*0,\s*255\)\);'
    if re.search(level_up_pattern, content, re.DOTALL):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(iconTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));'
        content = re.sub(level_up_pattern, replacement, content, flags=re.DOTALL)
        changed = True

    # 2. BossKnight.cpp
    boss_knight_pattern = r'::TransparentBlt\(\s*_hdc\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*texDC\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*RGB\(255,\s*0,\s*255\)\s*\);'
    if re.search(boss_knight_pattern, content, re.DOTALL):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(m_hpBarTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));'
        content = re.sub(boss_knight_pattern, replacement, content, flags=re.DOTALL)
        changed = True

    # 3. All Bullets with ::TransparentBlt
    bullet_pattern = r'::TransparentBlt\(\s*_hdc\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*m_pTex->GetTextureDC\(\)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*RGB\(255,\s*0,\s*255\)\s*\);'
    if re.search(bullet_pattern, content, re.DOTALL):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(m_pTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));'
        content = re.sub(bullet_pattern, replacement, content, flags=re.DOTALL)
        changed = True

    # 4. StretchBlt
    stretch_pattern = r'StretchBlt\(\s*_hdc\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*pTex->GetTextureDC\(\)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*(.*?)\s*,\s*SRCCOPY\s*\);'
    if re.search(stretch_pattern, content, re.DOTALL):
        replacement = r'GET_SINGLE(SpriteRenderer)->Draw(pTex->GetSRV(), (float)(\1), (float)(\2), (float)(\3), (float)(\4), (float)(\5), (float)(\6), (float)(\7), (float)(\8), (float)(\7), (float)(\8));'
        content = re.sub(stretch_pattern, replacement, content, flags=re.DOTALL)
        changed = True

    if changed:
        content = content.replace('HDC srcDC = iconTex->GetTextureDC();', '')
        content = content.replace('HDC texDC = m_hpBarTex->GetTextureDC();', '')
        
        with open(file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Updated {file}")
