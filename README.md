# praca_magisterska

## instalacja modułu lena 
https://cttc-lena.gitlab.io/nr/html/getting-started.html

## uruchomienie skryptów
1. Repozytorium zapisz w katalogu /scratch pod nazwą [nazwa_katalogu]
2. Będąc w głównym katalogu ns3 (czyli np. "ns-3-dev", albo "ns-3.XX" (gdzie XX to numer wersji)), uruchom:
   2.1 ./ns3 configure --disable-werror --enable-examples --enable-tests
   2.2 ./ns3 run [nazwa_katalogu]/praca_magisterska/[nazwa_skryptu]
       (gdzie nazwa_skryptu jest bez rozszerzenia .cc)