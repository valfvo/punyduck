#!/bin/bash

manga="dr-stone"
let "chapter=13"
let "page=0"

url="http://lelscanv.com/mangas/"$manga"/"$chapter"/"$page".jpg"

echo "Téléchargement en cours..."

while (("$chapter" < 134))
do
    if (("$page" < 10))
    then url="http://lelscanv.com/mangas/"$manga"/"$chapter"/0$page.jpg"
    else url="http://lelscanv.com/mangas/"$manga"/"$chapter"/"$page".jpg"
    fi
    let "page += 1"
    wget -q -P "Dr Stone/$chapter" "$url" || if (("$page" < 10))
        then let "page+=1"
        else let "chapter+=1"
        let "page=0"
        fi
done

echo "Téléchargement terminé !"