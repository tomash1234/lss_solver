# Řešení Soustavy Linearních Rovnic

Program přečte ze vsutupu rozšířenou matici soustavy a vypočítá partikularní řešení a
 případně také řešení homogenní soustavy. Výpočet je prováděn pomocí Gaussovy eliminační metody.


## Použití

Program přečte matici ze souboru a spočítá řešení
```
.\lss -f in/in01.txt

```

Program může přečíst matici i ze standartního vstupu

```
cat in/in01.txt | .\lss 

```
Pokud je vstup zadán uživatelem na klavesnici je doporučováno použít přepínáš **--verbose**,
 program výpiše co má uživatel zabrat a také mu ukáže průběžné výpočtz 

```
.\lss --verbose

```

## Formát vstupu
Máme soustavu Ax = b, potom rozšířená matici vypadá takto: Ab

První řádek obsahuje 2 celé čísla - počet řádku a sloupců.
Další řádky 

```
[počet řádků n] [počet sloupců m]
A1_1 A1_2 ... A1_m-1 b_1
A2_1 A2_2 ... A2_m-1 b_2
          ...
An_1 An_2 ... An_m-1 b_n-1
```
Pro testování a měření se ve složce '**in**' nachazi 8 vstupní souborů.

## Implementace

* lss.cpp - soubor s main funkcí, čte data, zpracovává argumenty vypisuje výsledek
* mat.hpp/.cpp - definice třídy mat a základní operace s maticemi a s řádky matice
* gaussian_elimination.hpp/.cpp - implemetace gaussovy elimanační metody, včetní zparalizované verze

Implemetnaci s více vlákny jsem pojal tak, že jsem si zjistil, která část výpočtu zabírá nejvíce času a tu jsem pak zparalizoval.
Jedná se o převod matice do horního trojúhelníkového tvaru. S více vláknami jsem implementoval dělení řádků pivotem.
 Tato operace lze rozdělit do nezívsilých části, proto nejsou potřeba žádné mutexy, což urychluje výpočet. Vliv použítí vláken je patrý až  při výpočtu
 velkých matic (500x500 a více), při menších matic je výhodnější použít jedno vlákno.

Použítí přepínač pro zapnutí multivláknové implemetnace **--multithread**
```
.\lss -f in/in08.txt --multithread

```

## Měření

Pro měření jsem použil matici o rozměřech 999x1000, uloženou v souboru in/in08.txt.
Měření jsem prováděl na stroji s CPU i5 1.6 GHz s Turbo Boost up to 3.4 GHz s 4 fyzickými jádry a zapnutým HT (tedy 8 vláken) a 
proběhlo vůči kodu z commitu 0bb6e532.

Jedno vlákno:	4753 ms
Multithraed:	1868 ms








