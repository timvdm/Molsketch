#!/bin/bash
iconsFromTheme=( $(git -C .. grep -E "fromTheme\\s*\\(\\s*\"[^\"]+" | grep -Po '(?<=fromTheme\(")[^"]+') )
if [[ $(git -C .. grep fromTheme | wc -l) != ${#iconsFromTheme[*]} ]]
then
    echo "Mismatch in number of fromTheme calls and icons found."
    git -C .. --no-pager grep fromTheme
    exit 1
fi

for icon in ${iconsFromTheme[*]}
do
    [[ -e ../molsketch/src/oxygen-icons/$icon.svg ]] || echo "File for $icon is missing"
    grep "<file alias=\"scalable/$icon.svg\">oxygen-icons/$icon.svg</file>" ../molsketch/src/oxygenicons.qrc > /dev/null|| echo "Resource entry for icon $icon is missing"
done
echo "Icon check done."
