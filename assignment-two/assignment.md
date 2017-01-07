# Krmení v ZOO
V ZOO nastal čas krmení, které probíhá tak, že do výběhu přijede několik náklaďáků se senem a okolo nich se sbíhají zvířátka, která chtějí krmení.

Každý náklaďák má seznam zvířátek (identifikovaných přirozenými čísly), která chtějí krmení, spolu s jejich celočíselnou sílou (čím větší číslo, tím je zvířátko silnější). Zvířátka k náklaďákům průběžně přicházejí. Různá zvířátka mají různou sílu a podle zákonů džungle dostávají krmení v pořadí od těch nejsilnějších k nejslabším. Občas z náklaďáku vyleze zřízenec, který naopak dohlíží na to, aby i slabá zvířátka něco dostala, a tehdy vydává krmení zvířátkům v pořadí od nejslabších k nejsilnějším. Pokud zřízenec opět zaleze, vše se vrátí k původním pořádkům (tj. silnější zvířátka se dostávají na řadu dříve). Po nakrmení zvířátko od náklaďáku odejde. Čas od času v náklaďáku dojde seno a tehdy je třeba zvířátka okolo něj poslat k jinému náklaďáku. Také nové náklaďáky přijíždějí, což poznáte tak, že přijde nějaký požadavek na tento náklaďák.

V ZOO pochopitelně musí být pořádek, je potřeba sledovat a evidovat, kdy bylo jaké zvířátko nakrmeno, a vy jste pověřeni úkolem vrcholné důležitosti - napsat informační systém pro ZOO. Ten je řízen příkazy na vstupu, které mají následující formát.

## Formát vstupu a výstupu
* Vstup sestává z určitého počtu řádek na vstupu, kde každá kóduje jeden příkaz, viz níže uvedené příkazy v uvozovkách:
  * "0 K ID S"
    * kde K, ID, S jsou přirozená čísla. Tento příkaz říká přidej náklaďáku K do seznamu zvířátko ID se silou S.
  * "1 K"
    * kde K je přirozené číslo. Tento příkaz znamená nakrm (a odstraň) od náklaďáku K nejsilnější zvířátko a vypiš jeho ID na výstup na samostatný řádek.
  * "2 K"
    * kde K je přirozené číslo Tento příkaz znamená nakrm (a odstraň) od náklaďáku K nejslabší zvířátko a vypiš jeho ID na výstup na samostatný řádek.
  * "3 K1 K2"
    * kde K1, K2 jsou přirozená čísla (může se stát ze K1 = K2) Tento příkaz znamená přidej zvířátka od náklaďáku K2 k náklaďáku K1.
  * "4"
    * Tento příkaz znamená vypni informační systém; je zaručeně jako poslední na vstupu
* Můžete se spolehnout, že vstup je zadán korektně.
* Může se stát, že nezodpovědný pracovník zadá příkaz nakrmení zvířátka od náklaďáku, kde žádné nestojí. V takovém případě je třeba vypsat na výstup na samostatný řádek řetězec "empty" (bez uvozovek).
* Bohužel se nedozvíte, že přijel nový náklaďák, zjistíte to teprve až na něj přijde požadavek. V takovém případě je jeho seznam prázdný.
* Číslo náklaďáku K je 1 ≤ K ≤ 10 000.
* Číslo zvířátka ID je 0 ≤ ID < 232, nicméně není zaručeno, že jsou identifikátory unikátní.
* Síla S je 0 < S < 232 - 1.
* Identifikátory se mohou recyklovat, tedy jednou nakrmený identifikátor se může objevit znovu.
* Pokud v seznamu jednoho náklaďáku příkazu 1 nebo 2 vyhovuje více zvířátek, má přednost zvířátko s nižším ID.
* Knihovna STL nevyhověla kontrole hlavního hygienika a nelze ji tedy používat.
