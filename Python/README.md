Contraction hierarchies - auxiliary Python scripts
==================================================

Tato složka obsahuje tři pomocné Python scripty (testováno s Pythonem 3.7), které umožňují přetransformovat dvojici souborů `edges.geojson` a `nodes.geojson` na vstup pro aplikaci `contractionHierarchiesPreprocessor`, která může pro daný graf následně vytvořit 'Contraction Hierarchy'. Scripty využívají balíček `roadmaptools` pro načítání GeoJSON souborů.

### transformGeoJSONtoIntegerXenGraph.py
Umožňuje přetransformovat dvojici souborů `edges.geojson` a `nodes.geojson` na celočíselný XenGraph soubor s příponou `.xeng`. Navíc vygeneruje i soubor s mapováním vrcholů `.xeni`, který se dá využít, pokud se nad výslednou hierarchií bude potřeba dotazovat původnímy indexy vrcholů z GeoJSON souborů. Pokud není soubor s mapováním potřeba, stačí zakomentovat příslušnou řádku ve scriptu. Stejně tak vstupní a výstupní cesty je možné změnit ve scriptu. Funkce `create_xengraph_file()` má volitelný parametr precision, kterým se dá určit, v jakých jednotkách v podstatě mají být váhy hran. Deaultně odpovídají váhy hran sekundám (travel time v sekundách). V případě potřeby je možné za pomocí tohoto parametru například mít místo toho travel time v milisekundách. Více popsáno v komentáři u dané funkce.

### transformGeoJSONtoFloatingPointXenGraph.py
Dělá v podstatě to samé, jako předchozí zmiňovaný, akorát vytvoří floating point XenGraph soubor. Tedy váhy hran budou desetinná čísla a ne celá čísla.

### transformGeoJSONtoDIMACS.py
Umožňuje převést dvojici souborů `edges.geojson` a `nodes.geojson` na soubor ve formátu DIMACS. Negeneruje žádný soubor s mapováním, což je momentálně pro většinu použití potřeba. Nedoporučuji moc používat, lepší je použít XenGraph jako formát, pokud se generují grafy z GeoJSON souborů.

