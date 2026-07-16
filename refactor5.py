import os
import glob
import re

files = glob.glob('2025_winapi_framework_22/*.cpp')

for file in files:
    with open(file, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    changed = False

    # Comment out DrawText and TextOut
    if 'DrawText' in content or 'TextOut' in content:
        # Avoid double commenting
        lines = content.split('\n')
        for i, line in enumerate(lines):
            if ('DrawText' in line or 'TextOut' in line) and '//' not in line:
                lines[i] = '// ' + line
                changed = True
        content = '\n'.join(lines)

    if changed:
        with open(file, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Updated {file}")
