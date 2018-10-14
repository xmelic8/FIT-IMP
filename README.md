Projekt do předmětu IMP na VUT FIT ve školním roce 2013/2014

Hodnocení 14/14b

Název: světelné noviny

V jazyce C nebo asembleru napište pro True-Time Simulator prostředí Freescale Codewarrior verze 6.x aplikaci Světelné noviny určenou pro zobrazování textu na displeji složeném z LED uspořádaných do matice o 8 řádcích a 32 sloupcích. Předpokládejte implementaci na platformě HC(S)08. Klíče k licencím ( license.dat ) pro bezproblémové ladění projektu Vám budou vhodně distribuovány vedoucím po ukončení Vašeho přihlašování se na projekty; cca od 13. 10. 2014 bude také možné využít instalací v učebnách v rámci CVT na FIT VUT v Brně.

Předpokládejte, že uživatel (tedy hodnotitel Vašeho projektu) bude ovládat aplikaci pomocí prvků nástroje Visualization Tool. Rozložení prvků nástroje Visualization Tool máte k dipozici ke stažení ve *.vlt souboru (viz Soubory / Projekty / Noviny), jejich konfigurace je ponechána na Vás.

Detaily k zadání

Vaše aplikace nechť je schopna zobrazit na displeji část Vašeho přihlašovacího jména (login) uloženého v paměti mikrokontroléru. Každý ze znaků Vašeho login reprezentujte bitmapou v rastru o velikosti 8 řádků a 8 sloupců, přičemž nejlevější a nejpravější sloupec budou (pro jednoduchost) plnit funci levé resp. pravé mezery znaku. Uvažujte pouze malá písmena abecedy.
Tlačítko init: Při spuštění aplikace musí být na displeji viditelné 4 nejlevější znaky Vašeho login. Do tohoto stavu se aplikace dostane i kdykoliv později, a to po stisku tlačítka init.
Tlačítko horiz: Po stisku tlačítka horiz bude na světelných novinách rotovat Váš login ve směru zprava doleva pokud Váš login končí dvojčíslím 00; pokud Váš login končí jiným dvojčíslím , bude rotovat zleva doprava.
Tlačítko vert: Po stisku tlačítka vert bude na světelných novinách rotovat Váš login ve směru zhora dolů pokud Váš login končí dvojčíslím 00 nebo 01; pokud Váš login končí jiným dvojčíslím, bude rotovat zdola nahoru.
Rychlost rotace: Rychlost rotace bude měnitelná pomocí táhla (baru) rychlost.