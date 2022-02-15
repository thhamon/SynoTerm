#!/bin/sh

# rm /home/thierry/Recherche/Program/SynoTerm0.5.5-devel/WordNet.syn

cd  /home/thierry/Recherche/Ressources/EN/WordNet-3.0/dict/

for d in "data.adj  data.adv  data.noun  data.verb" ; do
    grep -v "^ " $d  | cut -d'|' -f 1 | cut -d@ -f 1 | /home/thierry/Recherche/Program/SynoTerm/WN2SynoTerm.pl >> /home/thierry/Recherche/Program/SynoTerm/WordNet-3.0.syn
done