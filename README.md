Contraction hierarchies implementation in C++
=============================================

Projekt obsahuje implementaci techniky `Contraction Hierarchies` inspirovanou článkem "Contraction Hierarchies: Faster and Simpler Hierarchical
Routing in Road Networks" od čtveřice autorů Robert Geisberger, Peter Sanders, Dominik Schultes a Daniel Delling. Implementace umí předzpracovat
zadaný graf, tedy vytvořit samotnou 'Contraction Hierarchy' a umí jí pak i použít pro vyhodnocování dotazů (queries). Momentálně jsou funkční
dotazy na vzdálenost ('vrať nejkratší vzdálenost z bodu a do bodu b'). Teoreticky by se díky 'Contraction Hierarchies' dala vracet i samotná
nejkratší cesta (tedy kromě vzdálenosti i posloupnost vrcholů na nejkratší cestě), to ale momentálně není korektně implementováno.

Samotná projekt je rozdělený na dva hlavní moduly. Spustitelnou C++ aplikaci, která umožňuje vygenerování 'Contraction Hierarchy' pro libovolný graf v jednom
z podporovaných formátů, a dále shared library, která umožňuje volat dotazy nad danou Contraction Hierarchy z Javy. Toto dělení bylo zvoleno proto,
že samotné generování Contraction Hierarchy je poměrně náročné a pro velké grafy může dlouho trvat, proto dává smysl si nejprve potřebnou hierarchii předgenerovat
s využitím C++ aplikace, a následně už tuto hierarchii použít pro dotazy v Javě.


Kompilace
---------

### Nejjednodušší varianta
Nejjednodušším způsobem jak zkompilovat celý projekt je přiložený `CMakeLists.txt`. Ten obsahuje dva targety, knihovnu `contractionHierarchies` a aplikaci `contractionHierarchiesPreprocessor`. Tyto targety mohou být použity samostatně. Postup při využití `CMakeLists.txt` je zhruba následující:

- Stáhněte si aktuální `master`
- Využijte přiložený `CMakeLists.txt`. Ten by měl zajistit zkompilování knihovny `contractionHierarchies` pro Váš systém, kterou je následně možné volat z Javy pro vyhodnocování dotazů a dále zkompilování aplikace `contractionHierarchiesPreprocessor` která slouží ke generování hierarchií.
- Při volání `contractionHierarchies` z Javy je pak potřeba zajistit, že cesta k dané knihovně (soubor `libcontractionHierarchies.so` v Linuxu, `contractionHierarchies.dll` ve Windows...) je obsažena v `java.library.path` při spouštění Java aplikace, která bude `contractionHierarchies` volat (dá se například použít přepínač `-Djava.library.path` při spouštění Java aplikace).

### Alternativní způsob

- V případě, že z nějakého důvodu není možné použít CMake, například protože není na daném systému k dispozici, je možné aplikaci zkompilovat ručně.
- V takovém případě je nejjednodušší v libovolném IDE zvolit něco jako 'build project from sources'
- Následně je z projektu nutné odebrat soubory `RandomGraphGenerator.cpp`, `RandomTripsGenerator.cpp` a `AlternativeRandomTripsGenerator.cpp`. Jedná se o samostatné programy, které se dají samostatně zkompilovat a generují náhodné grafy nebo náhodné dvojice source - target v určité rozsahu. Dají se použít pro debugování. Pokud je necháte v projektu, nejspíše dostanete chybovou hlášku něco ve smyslu "multiple main definitions".
- Pokud chcete vygenerovat pouze shared library pro následné volání z Javy, nezahrnujte `main.cpp` do kompilace. Naopak, je potřeba zkompilovat veškeré ostatní soubory s příponou `.cpp` a `.cxx` ve všech podadresářích `src`. Pokud chcete vygenerovat spustitelný program který dokáže zkonstruovat Contraction Hierarchy pro daný graf, pak je naopak potřeba `main.cpp` do kompilace zahrnout. 

### Poznámka

Z C++ knihoven využívá program pouze `stl`, což by neměl být problém. Pro kompilaci je ovšem nutné mít na systému nainstalovaný `JDK`. V `CMakeLists.txt` je volání `find_package(JNI)`, pro které je potřeba `JDK` v daném systému.


Použití
-------

### Aplikace contractionHierarchiesPreprocessor
Aplikace `contractionHierarchiesPreprocessor` umožňuje relativně snadné vytvoření 'Contraction Hierarchy' pro daný vstupní soubor. Po spuštění této aplikace bez dalších argumentů aplikace pouze vypíše správné použití. Momentálně aplikace umožňuje pouze generování hierarchií (v budoucnu by mohlo být například umožněno i nějaké benchmarkování).

- V případě, že chce člověk vygenerovat hierarchii, stačí v konzoli zavolat aplikaci s podobnými argumenty jako v následujícím příkladu: `./contractionHierarchiesPreprocessor c i X Prague_map.xeng Prague_map`.
- V příkladu výše 'c' znamená create, 'i' znamená že se má tvořit integer 'Contraction Hierarchy' (alternativně 'f' by znamenalo floating point 'Contraction Hierarchy'), 'X' znamená že vstupní soubor je ve formátu XenGraph (alternativně 'D' znamená formát DIMACS), a konečně poslední dva argumenty jsou cesta ke vstupnímu souboru (buď relativní vzhledem k working directory, nebo absolutní) a cesta kam má být uložena výsledná hierarchie (na konec se automaticky přidá přípona '.ch' nebo '.chg', takže v případě daného příkladu by vznikl soubor 'Prague_map.ch', neboť jsme požádali o integer 'Contraction Hierarchy').

### Knihovna contractionHierarchies
Kromě spustitelné aplikace by měl `cmake` vygenerovat též příslušnou knihovnu. Knihovna se generuje samozřejmě v závislosti na systému, na kterém je `cmake` spuštěn, tedy výsledkem na Linuxu bude soubor `libcontractionHierarchies.so`, na Windows pravděpodobně `contractionHierarchies.dll`, na Macu pravděpodobně soubor s příponou `.dylib`. Tato knihovna vystavuje rozhraní pro volání dotazů na vzdálenost mezi dvěma vrcholy. Pro volání dotazů z Javy je nutné tuto knihovnu mít v `java.library.path`, například zkopírováním na vhodné místo, nebo upravením `java.library.path`, poté ji v Javě načíst pomocí `System.loadLibrary("contractionHierarchies")`, poté by již mělo být možné používat query algoritmus z Javy. Pro ukázku funkcionality a možnost ověření, že knihovna správně funguje, obsahuje tento projekt složku `javatest`, která obsahuje jednoduchou Java aplikaci, která by po volání `mnv compile` a následném `mvn test` měla provést několik testů, které ověří, že vše správně funguje. Samotný kód v daných testech pak může sloužit jako jakýsi vzor pro to, jak knihovnu z Javy správně použít. Pozor na to, že pokud uživatel v Javě vytvoří instanci nějakého QueryManageru, je potřeba na něm před ukončením programu zavolat funkci `clearStructures()`. Je to proto, že při vytvoření instance QueryManageru v Javě se za pomoci knihovny vytvoří též odpovídající instance QueryManageru v C++, která si alokuje nějakou paměť a pokud nedojde k zavolání `clearStructures()`, tato paměť nebude nikdy uvolněna.


Popis vstupních formátů
-----------------------

### Formát XenGraph pro grafy vytvořené z edges.geojson a nodes.geojson
Pro běžné použití na grafech pro které existuje dvojice souborů `edges.geojson` a `nodes.geojson` využívám velmi jednoduchý textový formát. Existuje Python skript, který dokáže pro danou dvojici souborů
`edges.geojson` a `nodes.geojson` vygenerovat soubory `graph.xeng` a `graph.xeni` které mohou být následně přímo použity pro vygenerování 'Contraction Hierarchy'.

Hlavní je soubor pro grafy (přípona `.xeng`) jehož formát je následující:
- Soubor začíná řádkou `XGI n e` v případě celočíselných vah hran, případně `XGF n e` v případě floating point vah hrat, s tím že 'n' je číslo určující počet vrcholů a 'e' je číslo určující počet hran.
- Následuje 'e' řádek ve formátu `s t w f` reprezentujících jednotlivé hrany, pro které 's' je ID source uzlu (tedy z rozsahu 0 až 'n' - 1), 't' je ID target uzlu ze stejného rozsahu, 'w' je váha hrany, což je celé číslo v případě že soubor začíná `XGI` nebo desetinné číslo v případě `XGF`, a 'f' je flag určující, zda daná hrana je jednosměrná. Hodnota '1' flagu 'f' znamená, že daná hrana je jednosměrná, hodnota '0' znamená obousměrnou hranu. Při načítání grafu ze souboru se za každou hranu s hodnotou flagu '0' přidají do grafu dvě hrany, tedy hrana z 's' do 't' plus i opačná hrana z 't' do 's'.

Kromě samotného grafu ještě existuje soubor s příponou `.xeni` který slouží pro mapování uzlů. V `edges.geojson` a `nodes.geojson` se používají indexy uzlů které mají řádově 16 decimálních cifer. V rámci 'Contraction Hierarchies' pracuji s indexy uzlů od 0 do 'n' - 1. Tento soubor tedy slouží pro mapování z daných dlouhých indexů na indexy z rozsahu se kterým pracuji. Formát souboru je velice jednoduchý:
- Soubor začíná řádkou `XID n`, kde opět 'n' odpovídá počtu vrcholů.
- Následuje 'n' řádek, z nichž každá obsahuje pouze jedno číslo, odpovídající originálnímu ID vrcholu, který se mapuje na ID odpovídající danému řádku. Jinými slovy ID na druhém řádku se mapuje na ID 0, ID na třetím řádku se mapuje na ID 1, až ID na řádku 'n' + 1 se mapuje na ID 'n' - 1.

Toto mapování se momentálně využívá při dotazování z Javy. Protože v Javě (respektive v Agentpolis) se interně pro reprezentaci silniční sítě využívají ID z `edges.geojson` a `nodes.geojson`, je v tomto projektu implementován `IntegerDistanceQueryManagerWithMapping` a `FPointDistanceQueryManagerWithMapping`, u kterých platí, že je možné se dotazovat na vzdálenost mezi dvěmi nody pomocí jejich originálních dlouhých ID, a DistanceQueryManagerWithMapping sám převede ID. Pro tyto třídy je tedy logicky navíc potřeba soubor `.xeni`, ze kterého si načtou toto mapování (to probíhá v konstruktoru těchto tříd).

### Formát grafu z implementační soutěže DIMACS 
(Ukázkové grafy v tomto formátu mohou být staženy zde: http://www.dis.uniroma1.it/challenge9/download.shtml)

- Soubor začíná řádkou `p sp n e` kde místo 'n' je číslo určující počet vrcholů a místo 'e' je číslo určující počet hran.
- Následuje 'e' řádek ve tvaru `a u v w` reprezentujících jednotlivé hrany, s tím že 'u' je číslo source uzlu, 'v' je číslo target uzlu a 'w' je váha hrany (délka, čas potřebný pro její projetí...). Čísla 'u' a 'v' musí být z rozsahu 1 až n. V rámci formátu pro soutěž DIMACS se vrcholy indexovaly od jedničky. Program indexuje vrcholy od nuly, takže se každý načtený vrchol automaticky sníží o 1. Hrany se načítají orientovaně, takže pokud chci mít hranu 'u <-> v' obousměrnou, musím jí zadat jako hranu z 'u' do 'v' a znovu jako hranu z 'v' do 'u'.
- Kdekoliv v soubrou se dále mouhou vyskytovat řádky začínající znakem `c`. Tyto řádky reprezentují komentáře a program je pouze zahazuje.


### Formát souboru s dotazy
- Na první řádce bude číslo `x` reprezentující počet párů source target (tedy dotazů)
- Následuje 'x' řádek z nichž každá obsahuje dvě čísla oddělená mezerou `a b` kde 'a' je source a 'b' je target. Oba vrcholy musí být z rozsahu 0 až n-1 kde 'n' je počet vrcholů v pouužitém grafu. Zde se vrcholy indexují od nuly.
- Pro vygenerování libovolně velké sady dotazů se dá využít soubor `RandomTripsGenerator.cpp`.

### Formát 'Contraction Hierarchies'
- Pro ukládání celočíselné 'Contraction Hierarchy' se používá binární formát, který je stručně popsaný na konci souboru `DDSGLoader.h`. Formát pro floating point 'Contraction Hierarchy' je identický, liší se pouze tím, že váhy jsou v něm ukládány jako doubly a ne jako unsigned inty. Na běžných architekturách to momentálně znamená, že každá hrana bude zabírat o 4 byty víc. Formát je stručně popsaný na konci souboru `DDSGLoader.h`.


Kde co hledat
-------------
Zde ještě stručně popíšu, kde v C++ kódu zhruba hledat jednotlivé komponenty 'Contraction Hierarchies' pro snažší orientaci v případě, že by v budoucnu bylo potřeba do kódu zasahovat.

Předně bych rád poznamenal, že aby aplikace mohla podporovat jak celočíselné tak floating point 'Contraction Hierarchies', existuje většina struktur dvakrát, tedy většina zdrojového kódu je rozdělena do podsložek 'Integer' a 'FloatingPoint' (nebo jen krátce 'FPoint'). Já tedy popíšu spíše celočíselnou variantu, ale ekvivalentní třídy existují i pro floating point variantu.


### Předzpracování
- V podstatě za veškerý preprocessing grafu na 'Contraction Hierarchy' je zodpovědná třída `IntegerCHPreprocessor` s malou pomocí třídy `EdgeDifferenceManager` (ta je generická a používá se ta samá jak v integer tak ve floating point variantě). Při předzpracování se používá `IntegerUpdateableGraph` pro reprezentaci grafu, což je třída, která umožňuje snadné přidávání a odebírání hran. Fungování jednotlivých metod je stručně popsáno v kódu vždy nad každou metodou.

### Dotazy
- Vyhodnocování 'Contraction Hierarchy' dotazů řeší třída `IntegerCHDistanceQueryManager`. Ten dostane reprezentaci 'Contraction Hierarchy' jakožto instanci třídy `IntegerFlagsGraph` a následně se na něj dají volat dotazy pomocí funkce `findDistance`. V rámci `IntegerFlagsGraph` je každá hrana uložena pouze jednou a sice u vrcholu s menším rankem v rámci 'Contraction Hierarchies' společně s flagy v kterém směru tato hrana existuje. Navíc tato reprezentace grafu obsahuje různé pomocné informace o jednotlivých vrcholech potřebné pro vyhodnocování dotazů.
- Třída `IntegerCHDistanceQueryManagerWithMapping` umožňuje dotazování stejně jako `IntegerCHDistanceQueryManager`, avšak zatímco v `IntegerCHDistanceQueryManager` je potřeba používat indexy z rozsahu 0 až 'n' - 1, třídě `IntegerCHDistanceQueryManagerWithMapping` je možné předat libovolné mapování indexů vrcholů a poté se dotazovat těmito indexy.
- Třídy `IntegerCHDistanceQueryManager` a `IntegerCHDistanceQueryManagerWithMapping` řeší pouze dotazy na vzdálenost, tedy dotaz typu 'vrať nejkratší vzdálenost z bodu a do bodu b'. V budoucnu by třída `IntegerCHPathQueryManager` mohla vracet i posloupnost vrcholů na nalezené nejkratší cestě. Momentálně `IntegerCHPathQueryManager` nefunguje správně (nevrací správné cesty).

### Načítání
- Načítání grafů řeší jednotlivé 'Loadery'. Při vytváření hierarchií se používají `IntegerXenGraphLoader`, `FloatingPointXenGraphLoader` a `DIMACSLoader`. Při dotazech se využívá `DDSGLoader` a `DDSGFLoader` pro načtení hierarchie, nad kterou se následně dají dělat dotazy.

### API - kód pro propojení s Javou
- Trochu samostatnou kapitolou je zajištění toho, že se bude dát C++ kód v podstatě volat z Javy. K tomuto jsem nejprve vytvořil s podsložce `src/API` třídy, které slouží jako jakési rozhraní, vystavují pouze takové funkce, které bude potřeba volat z Javy a nic navíc.
- Pro tyto funkce je pak potřeba vygenerovat jakýsi 'glue' kód, k tomu využíván nástroj `SWIG`. Ten pracuje s vlastními soubory s příponou `.i`, v rámci kterých se dá specifikovat, jak má vypadat dané rozhraní. V mém případě jsou tyto soubory velice jednoduché, vše pak do jednoho zabaluje soubor `CHinterface.i`.
- Pokud by bylo potřeba do API volaného z Javy něco přidat, případně by se třeba změnily signatury funkcí ve stávajících QueryManagerech, bude potřeba znovu vygenerovat 'glue' kód pomocí SWIGu. Na Linuxu je pro toto připraven skript `generateSWIGcode.sh`. Na jiných systémech doporučuji inspirovat se tímto skriptem v kombinaci s dokumentací SWIGu.
- SWIG vygeneruje jeden C++ soubor s příponou `.cxx` který je potřeba zahrnout do kompilace knihovny, a taky několik `.java` souborů, které je nutné mít zahrnuté v Java aplikaci, která bude knihovnu volat.
- Ve složce `src/API` je ještě dodatečné `Readme.txt`, popisující generování SWIG kódu pomocí přiložených skriptů. Tato varianta by se dala použít v Linuxu, pokud by nebyl k dispozici `cmake`, jinak ji nedoporučuji.

