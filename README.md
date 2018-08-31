Contraction hierarchies implementation in C++
=============================================

Projekt obsahuje implementaci techniky 'Contraction Hierarchies' inspirovanou článkem "Contraction Hierarchies: Faster and Simpler Hierarchical
Routing in Road Networks" od čtveřice autorů Robert Geisberger, Peter Sanders, Dominik Schultes a Daniel Delling. Implementace umí předzpracovat
zadaný graf, tedy vytvořit samotnou 'Contraction Hierarchy' a umí jí pak i použít pro vyhodnocování dotazů (queries). Momentálně jsou funkční
dotazy na vzdálenost ('vrať nejkratší vzdálenost z bodu a do bodu b'). Teoreticky by se díky 'Contraction Hierarchies' dala vracet i samotná
nejkratší cesta (tedy kromě vzdálenosti i posloupnost vrcholů na nejkratší cestě), to ale momentálně není korektně implementováno.

Kompilace
---------
- Stáhněte si aktuální 'master'
- V libovolném IDE něco jako 'build project from sources'
- Odeberte z projektu soubory 'RandomGraphGenerator.cpp', 'RandomTripsGenerator.cpp' a 'AlternativeRandomTripsGenerator.cpp'. Jedná se o samostatné programy, které se dají samostatně zkompilovat a generují náhodné grafy nebo náhodné dvojice source - target v určité rozsahu. Dají se použít pro debugování. Pokud je necháte v projektu, nejspíše dostanete chybovou hlášku něco ve smyslu "multiple main definitions".

Projekt nepouzívá žádné externí knihovny (pouze stl), takže by měl bez větších potíží fungovat. V budoucnu možná přidám makefile aby bylo možné projekt zkompilovat i z příkazové řádky.

Použití
-------
Momentálně se při každém spuštění programu provede 'odkomentovaná' funkce v 'main.cpp'. Do budoucna by bylo vhodné implementovat nějaké user interface, alespoň na úrovni command line, ale to jsem nestíhal. Momentálně je tedy potřeba při každé změně použití program znovu překompilovat.

### Generování Contraction Hierarchy
Řeší metoda 'constructDDSGCH()'. Ta dostane graf zadaný ve formátu implementační soutěže DIMACS (popsán níže) a vygeneruje z něj soubor obsahující vygenerovanou 'Contraction Hierarchy' s příponou ".ch". U velkých grafů může předzpracování trvat poměrně dlouho a být celkem náročné na paměť.

Co kde nastavit ve funkci DDSGCH():
- Na řádce kde je 'Loader graphLoader = Loader("xyz");' tak na místo 'xyz' doplňte cestu k vašemu vstupnímu souboru (včetně přípony). Soubor musí být korektní soubor v daném formátu, program neprovádí kontroly validity vstupního souboru.
- Na řádce kde je 'graph->flushInDdsgFormat("xyz");' tak na místo 'xyz' doplnit cestu k souboru, kam se uloží daná 'Contraction Hierarchy'. K názvu souboru je automaticky přidána přípona ".ch".

### Benchmarking