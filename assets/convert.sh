#!/bin/bash

# Define magic pink color
MAGIC_PINK="#ff00ff"

# Recursively find all PNG files and process them
find . -type f -iname '*.png' | while read -r file; do
  echo "Processing: $file"
  magick "$file" -fuzz 0% -transparent "$MAGIC_PINK" "$file"
done

echo "Done replacing magic pink with transparency!"