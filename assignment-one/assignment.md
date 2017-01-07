# Úloha 1 - Fakultní labyrint
Po absolvování první přednášky z Algoritmů a grafů 1 jste se natolik zabrali do řešených problémů, že se vám podařilo úplně zapomenout v prostoru a čase. Pozdě v noci jste si uvědomili, že stále sedíte v posluchárně a že vlastně vůbec nevíte, kudy se nejrychleji dostat k východu. Situaci komplikuje to, že některé průchody mezi místnostmi (dveře) jsou zamčené a dají se odemknout pouze příslušnou magnetickou kartou. Naštěstí jsou studenti a zaměstnanci fakulty značně roztržití a zapomnětliví, takže se po budově dají najít poztrácené magnetické karty, které můžete použít. Přístupových karet jsou tři typy (A, B a C) a stejně tak dveří, karta typu A otevře pouze dveře typu A, a tak dále. Kartou můžete otevřít libovolný počet dveří (neopotřebovává se).

Vaším cílem je najít nejkratší cestu z posluchárny k východu, případně oznámit, že žádná cesta neexistuje. Délka cesty je měřená počtem přechodů mezi místnostmi a může se do jedné místnosti vstoupit i vícekrát. Jako správný student informatiky, než někam vyrazíte, jste se rozhodli napsat si na tento náročný úkol program.

K dispozici máte na vstupu plán budovy, která sestává z význačných míst/místností propojených mezi sebou průchody/dveřmi, a popisem, kde leží zapomenuté karty a který průchod je odemknutelný kterou kartou. Průchody mohou být jednosměrné. Na jednom průchodu je nejvýše jeden zámek. Mezi dvěma místnostmi může vést více průchodů. Po 22:00 navíc byly ve škole vypnuty informační systémy, takže nemůžete používat knihovnu STL.

## Formát vstupu:
* První řádek obsahuje mezerou oddělená čísla N X Y, kde N je počet význačných bodů N (body jsou číslovány od 0), X číslo bodu, kde začínáte, a Y číslo bodu, kde je východ (předpokládejte X≠Y, tj. N≥2).
* Poté následuje N řádků, kde i-tý obsahuje číslo si, udávající počet význačných bodů, do kterých lze přejít z význačného bodu číslo i-1 (body číslujeme od nuly), a dále na řádku následuje mezerami oddělený seznam si dvojic čísla v a znaku z, kde v je číslo bodu (můžete předpokládat, že v≠i-1) a z udává, zda je průchod zamčený ('x' není, 'a' zámek A, 'b' zámek B, 'c' zámek C).
* Pak následuje řádek začínající číslem K a po něm mezerami oddělený seznam K dvojic v z, kde v je číslo bodu a z udává, jaká magnetická karta leží na bodu v (z='a' karta A, z='b' karta B, z='c' karta C). Platí, že K≤3N.
* Na jednom význačném bodu může ležet i více karet (ať už stejných, či různých).
* Můžete předpokládat, že vstup je korektní.

## Formát výstupu:
* Na prvním řádku vypište délku nejkratší cesty (měřenou počtem průchodů) ze začátků k východu.
* Na druhém řádku vypište mezerami oddělený seznam význačných bodů na této nejkratší cestě v pořadí, v jakém jimi procházíte (na prvním místě číslo počátku a na posledním místě číslo východu).
* Pokud existuje více nejkratších cest, vypište libovolnou z nich.
* Pokud neexistuje žádná cesta do východu, vypište na samostatný řádek řetězec "No solution" (bez uvozovek).

## Ukázka práce programu:
Ukázky jsou v adresáři [tests](tests).
