#!/bin/bash

rm -f szamolj feladat7 ki.txt 2>/dev/null

echo -e "\n--- lab_4: 5. Feladat tesztelése ---"
if gcc -Wall feladat7.c parancssor.c -o feladat7; then
    ./feladat7
else
    echo "Hiba a feladat7.c fordítása közben!"
fi

echo -e "\n--- lab_5: 6. Feladat: szamolj.c tesztelése ---"
if gcc -Wall szamolj.c -o szamolj; then
    ./szamolj be.txt ki.txt
    echo -n "A ki.txt tartalma: " && cat ki.txt && echo ""
else
    echo "Hiba a szamolj.c fordítása közben!"
fi
