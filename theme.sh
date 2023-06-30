#!/bin/bash

# Generate grayscale colors
for ((i = 0; i < 32; i++)); do
    color=$(printf '#%06x' $((i * 0x080808))) # adjusted calculation to fit 32 steps
    echo "color$i $color"
done

# Define color sections
sections=(
    "Red"
    "Yellow"
    "Green"
    "Cyan"
    "Blue"
    "Magenta"
    "Pink"
)

# Generate colors for each section
n=32
for section in "${sections[@]}"; do
    echo "=== $section ==="
    for ((i = 0; i < 16; i++)); do
        r=0; g=0; b=0;

        case "$section" in
            "Red")
                r=255
                g=$((i * 16))
                ;;
            "Yellow")
                r=$((255 - i * 16))
                g=255
                ;;
            "Green")
                g=255
                b=$((i * 16))
                ;;
            "Cyan")
                g=$((255 - i * 16))
                b=255
                ;;
            "Blue")
                r=$((i * 16))
                b=255
                ;;
            "Magenta")
                r=255
                b=$((255 - i * 16))
                ;;
            "Pink")
                g=$((i * 16))
                b=255
                ;;
        esac

        color=$(printf '#%02x%02x%02x' "$r" "$g" "$b")
        echo "color$n $color"
        ((n++))
    done
done
