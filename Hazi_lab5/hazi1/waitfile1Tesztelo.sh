#!/bin/bash

gcc waitfile1.c -o waitfile1

FAIL_COUNT=0
ITERATIONS=1000

echo "Tesztelés indítása $ITERATIONS futtatással..."

for ((i=1; i<=ITERATIONS; i++))
do
    # Lefuttatjuk a programot és elmentjük a kimenetét egy változóba
    # A 2>/dev/null elrejti az esetleges hibaüzeneteket
    OUTPUT=$(./waitfile1 2>/dev/null)

    # Megnézzük, hogy a kimenet tartalmazza-e a "Helló" szöveget
    # Ha a kimenet hossza 0 (-z flag), akkor nem sikerült az olvasás
    if [ -z "$OUTPUT" ]; then
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi

    # Haladás jelzése minden 100. körben
    if (( i % 100 == 0 )); then
        echo "$i futtatás kész..."
    fi
done

echo "-----------------------------------------------"
echo "Összes futtatás: $ITERATIONS"
echo "Sikertelen olvasások száma (üres kimenet): $FAIL_COUNT"
echo "Hibaarány: $(echo "scale=2; $FAIL_COUNT / $ITERATIONS * 100" | bc)%"