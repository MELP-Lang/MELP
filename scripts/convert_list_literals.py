#!/usr/bin/env python3
"""
YZ_106: List Literal Syntax Converter
Converts Python-style list syntax to PMPL standard:
    [a, b, c]  →  (a; b; c;)

PMPL Koleksiyon Syntax:
    Array:  [1; 2; 3;]     (homojen, brackets)
    List:   (1; "a"; true;)  (heterojen, parens)
    Tuple:  <1; "a"; true>  (immutable, angles)

This script converts Stage 1 modules from Python-style to PMPL standard.

Rules:
1. Find list literal patterns: [...]
2. Convert brackets: [ → (, ] → )
3. Convert separators: , → ;
4. Add trailing semicolon before closing paren
5. Preserve string literals (don't touch [ ] inside strings)
6. Preserve comments (don't touch [ ] in comments)
"""

import re
import os
import sys
from pathlib import Path

def is_inside_string(text, pos):
    """Check if position is inside a string literal"""
    # Count quotes before this position
    before = text[:pos]
    double_quotes = before.count('"') - before.count('\\"')
    single_quotes = before.count("'") - before.count("\\'")
    
    # Odd number of quotes = inside string
    return (double_quotes % 2 == 1) or (single_quotes % 2 == 1)

def is_inside_comment(text, pos):
    """Check if position is inside a comment"""
    # Find line start
    line_start = text.rfind('\n', 0, pos) + 1
    line_before_pos = text[line_start:pos]
    
    # Check for -- comment
    if '--' in line_before_pos:
        comment_start = line_before_pos.find('--')
        # Position is after -- on same line
        if comment_start < len(line_before_pos):
            return True
    
    return False

def find_matching_bracket(text, start_pos):
    """Find matching closing bracket, handling nesting"""
    depth = 1
    pos = start_pos + 1
    
    while pos < len(text) and depth > 0:
        # Skip strings and comments
        if is_inside_string(text, pos) or is_inside_comment(text, pos):
            pos += 1
            continue
        
        if text[pos] == '[':
            depth += 1
        elif text[pos] == ']':
            depth -= 1
        
        pos += 1
    
    return pos - 1 if depth == 0 else -1

def convert_list_literal(match_text):
    """Convert a single list literal from [a, b] to (a; b;)"""
    # Remove brackets
    content = match_text[1:-1].strip()
    
    if not content:
        # Empty list: [] → ()
        return "()"
    
    # Split by commas (not inside nested structures)
    elements = []
    current = ""
    depth = 0
    in_string = False
    string_char = None
    
    i = 0
    while i < len(content):
        char = content[i]
        
        # Handle string literals
        if char in ('"', "'") and (i == 0 or content[i-1] != '\\'):
            if not in_string:
                in_string = True
                string_char = char
            elif char == string_char:
                in_string = False
                string_char = None
        
        if in_string:
            current += char
            i += 1
            continue
        
        # Handle nesting
        if char in '([{':
            depth += 1
        elif char in ')]}':
            depth -= 1
        
        # Split on comma at depth 0
        if char == ',' and depth == 0:
            elements.append(current.strip())
            current = ""
        else:
            current += char
        
        i += 1
    
    # Add last element
    if current.strip():
        elements.append(current.strip())
    
    # Join with semicolons and add trailing semicolon
    result = "; ".join(elements)
    if result:
        result += ";"
    
    return f"({result})"

def convert_file(file_path, dry_run=False):
    """Convert list literals in a file"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original = content
        modified = content
        conversion_count = 0
        
        # Find all [ positions
        pos = 0
        while True:
            pos = modified.find('[', pos)
            if pos == -1:
                break
            
            # Skip if inside string or comment
            if is_inside_string(modified, pos) or is_inside_comment(modified, pos):
                pos += 1
                continue
            
            # Check if this looks like a list literal (not array type or index)
            # Context checks:
            before = modified[max(0, pos-20):pos].strip()
            
            # Skip if it's array type declaration: numeric[5]
            if before and before[-1].isalnum():
                pos += 1
                continue
            
            # Skip if it's array index: arr[i]
            # (identifier immediately before [)
            if before and (before[-1].isalnum() or before[-1] == '_'):
                pos += 1
                continue
            
            # Find matching ]
            end_pos = find_matching_bracket(modified, pos)
            if end_pos == -1:
                pos += 1
                continue
            
            # Extract list literal
            list_literal = modified[pos:end_pos+1]
            
            # Check if it contains commas (list literal pattern)
            if ',' not in list_literal:
                # No commas = not a list literal (maybe array)
                pos = end_pos + 1
                continue
            
            # Convert it
            converted = convert_list_literal(list_literal)
            
            # Replace in text
            modified = modified[:pos] + converted + modified[end_pos+1:]
            conversion_count += 1
            
            # Move past the converted part
            pos += len(converted)
        
        if conversion_count > 0:
            if not dry_run:
                # Backup original
                backup_path = f"{file_path}.backup"
                with open(backup_path, 'w', encoding='utf-8') as f:
                    f.write(original)
                
                # Write modified
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(modified)
                
                print(f"✅ {file_path}: {conversion_count} conversions")
                return conversion_count
            else:
                print(f"🔍 {file_path}: {conversion_count} conversions (dry run)")
                return conversion_count
        else:
            print(f"⏭️  {file_path}: No conversions needed")
            return 0
    
    except Exception as e:
        print(f"❌ {file_path}: Error - {e}")
        return 0

def main():
    """Main conversion script"""
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Convert list literal syntax from Python-style to PMPL standard'
    )
    parser.add_argument(
        'path',
        nargs='?',
        default='archive/stage1_api_attempt/modules',
        help='Path to file or directory (default: archive/stage1_api_attempt/modules)'
    )
    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Show what would be converted without making changes'
    )
    parser.add_argument(
        '--file',
        help='Convert single file only'
    )
    
    args = parser.parse_args()
    
    if args.dry_run:
        print("🔍 DRY RUN MODE - No files will be modified\n")
    
    total_conversions = 0
    total_files = 0
    
    if args.file:
        # Single file mode
        file_path = Path(args.file)
        if file_path.exists():
            conversions = convert_file(str(file_path), args.dry_run)
            total_conversions += conversions
            total_files += 1
        else:
            print(f"❌ File not found: {args.file}")
            return 1
    else:
        # Directory mode
        base_path = Path(args.path)
        if not base_path.exists():
            print(f"❌ Path not found: {args.path}")
            return 1
        
        # Find all .mlp files
        mlp_files = list(base_path.rglob("*.mlp"))
        
        if not mlp_files:
            print(f"❌ No .mlp files found in {args.path}")
            return 1
        
        print(f"📁 Found {len(mlp_files)} .mlp files\n")
        
        for mlp_file in sorted(mlp_files):
            conversions = convert_file(str(mlp_file), args.dry_run)
            if conversions > 0:
                total_conversions += conversions
                total_files += 1
    
    print(f"\n{'🔍 DRY RUN ' if args.dry_run else ''}SUMMARY:")
    print(f"  📊 {total_files} files modified")
    print(f"  🔄 {total_conversions} total conversions")
    
    if not args.dry_run and total_conversions > 0:
        print(f"\n💾 Backups saved as *.backup")
        print(f"⚠️  Please review changes manually!")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
