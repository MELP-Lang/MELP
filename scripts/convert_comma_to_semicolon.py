#!/usr/bin/env python3
"""
YZ_113: Convert comma to semicolon in MELP Stage 1 modules
Converts function call arguments: func(a, b) → func(a; b)
"""

import re
import sys
from pathlib import Path

def convert_commas_in_calls(content):
    """Convert commas to semicolons in function calls"""
    # Pattern: function_name(arg1, arg2, ...)
    # We need to be careful not to match decimal numbers like 3,14
    
    # Simple approach: replace ', ' with '; ' in function calls
    # This assumes formatted code with spaces after commas
    result = re.sub(r',\s+', '; ', content)
    
    return result

def process_file(filepath):
    """Process a single .mlp file"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original = content
        converted = convert_commas_in_calls(content)
        
        if converted != original:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(converted)
            print(f"✅ {filepath}")
            return True
        else:
            return False
            
    except Exception as e:
        print(f"❌ Error processing {filepath}: {e}", file=sys.stderr)
        return False

def main():
    base_path = Path("archive/stage1_api_attempt/modules")
    
    if not base_path.exists():
        print(f"Error: {base_path} not found", file=sys.stderr)
        sys.exit(1)
    
    print("🔧 Converting commas to semicolons in Stage 1 modules...")
    print()
    
    mlp_files = list(base_path.rglob("*.mlp"))
    converted_count = 0
    
    for filepath in mlp_files:
        if process_file(filepath):
            converted_count += 1
    
    print()
    print(f"✅ Converted {converted_count}/{len(mlp_files)} files")

if __name__ == "__main__":
    main()
