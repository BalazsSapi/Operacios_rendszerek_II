#!/bin/bash

# 1. Előkészületek: takarítás
echo "--- Előkészületek ---"
rm -f negyf offset waitfile1 kiazelso ki.txt a.txt message.txt teszt_offset.txt 2>/dev/null

# 2. 5. feladat: Négy folyamat (negyf.c)
echo -e "\n--- 5. Feladat: negyf.c tesztelése ---"
if gcc -Wall negyf.c -o negyf; then
    ./negyf
    [ -f ki.txt ] && echo -n "A ki.txt tartalma: " && cat ki.txt
else
    echo "Hiba a negyf.c fordítása közben!"
fi

# 3. 6. feladat: Közös fájl azonosító (offset.c)
echo -e "\n--- 6. Feladat: offset.c tesztelése ---"
touch teszt_offset.txt
if gcc -Wall offset.c -o offset; then
    ./offset teszt_offset.txt
    echo -n "A fájl végleges tartalma: " && cat teszt_offset.txt && echo ""
else
    echo "Hiba az offset.c fordítása közben!"
fi

# 4. 8.1 feladat: waitfile1.c és a részletes TESZTELŐ SZKRIPT
echo -e "\n--- 8.1 Feladat: waitfile1.c intenzív tesztelése ---"
#./waitfile1Tesztelo.sh


# 5. 7. feladat (OPCIONÁLIS): Ki az első (kiazelso.c)
echo -e "\n--- 7. Feladat (OPCIONÁLIS) kiazelso.c ---"

if gcc -Wall kiazelso.c -o kiazelso; then
    ./kiazelso 10000
else
    echo "Hiba a kiazelso.c fordítása közben!"
fi

awk 'BEGIN{ca=0; cf=0}{a=$0; getline; f=$0; if (a=="1") {ca++;}
else{ cf++;}; } END {print ca,cf,cf/(ca+cf)}' ki.txt

echo -e "\n--- Minden teszt befejeződött ---"