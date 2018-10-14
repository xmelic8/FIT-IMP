/* Michal Melichar, xmelic17                                            *
 * cca 90% zmen (vygenerovan pouze soubor main.c, zbytek vlastni prace) *
 * posledni zmena: 18/12/2014                                           */

#include <hidef.h> 
#include "derivative.h" 


#define DELKA_RADKU 32
#define VYSKA_SLOUPCE 8
#define MAX_INDEX_LED 255
#define POCET_LED 256
#define RADKY_PISMENE 8
#define SLOUPCE_PISMENE 8
#define PISMENA_LOGINU 8 


// adresy obsahujici mapovani tlacitek a startu LCD 
unsigned char INIT_BTN @0x0200;
unsigned char HOR_BTN @0x0201;
unsigned char VERT_BTN @0x0202;
unsigned char SPD_BAR @0x0203;
unsigned char LCD_Start @0x0300;


/* Globalni promenne                               *
 * pohyb: 0 = init, 1 = horizontalni posuv,        *
                    2 = vertikalni posuv           *
 * offset: slouzi pro urceni mista, kde ma zacit   *
           text rolovat.                           */ 
short pohyb = 0;
short offset = 0;


// bitmapa mapovani znaku loginu 
const byte login[8][8][8] = { 
  {//pismeno x 
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,1,0,0,0,1,0,0},
    {0,0,1,0,1,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,1,0,1,0,0,0},
    {0,1,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0}
  },
  {// pismeno m 
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,0},
    {0,1,0,1,0,1,0,0},
    {0,0,0,0,0,0,0,0}  
  },
  {// pismeno e 
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,0},
    {0,0,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0} 
  },
  {// pismeno l 
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0}  
  },
  {// pismeno i 
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,0,0}  
  },
  {// pismeno c 
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0}  
  },
  {// cislo 1 
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,0,0},
    {0,0,1,1,1,1,0,0}, 
    {0,1,1,0,1,1,0,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,0,0}  
  },
  {// cislo 7 
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,1,1,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,1,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}  
  }
};



// Deklarace funkci
void tisk_znaku(const byte bitmapa_znaku[8][8], short start_x, short start_y);
void tisk_login(short start);
void clear_display(void);
void zpozdeni(unsigned int n);


// Funkce slouzi k nastaveni pocatecnich hodnot promennych.
void init(){
  pohyb = 0;
  INIT_BTN = 0;
  VERT_BTN = 0;
  HOR_BTN = 0;
  SPD_BAR = 0;
}


void main(void) {  
  init();
  EnableInterrupts; 
     
  for(;;) {
    __RESET_WATCHDOG(); 
    tisk_login(offset);

    /* Podminka nastavujici zpozdeni pouze v dobe, *
     * kdu se text pohybuje.                       */
    if(pohyb != 0){
      zpozdeni((MAX_INDEX_LED - SPD_BAR) * 10);      
    }

    // Horizontalni pohyb smerem zleva doprava   
    if(pohyb == 1){
      offset += 1;          
      if(offset == DELKA_RADKU){
        offset = -(2 * DELKA_RADKU); 
      }
    }
    // Vertikalni pohyb smerem zdola nahoru
    else if(pohyb == 2){
      offset -= 1;
      if(offset == -VYSKA_SLOUPCE){
        offset = VYSKA_SLOUPCE; 
      }      
    }

    
    // Zpracovani stisknuti tlacitek  
    if(INIT_BTN == 1){
      clear_display();
      offset = 0;
      pohyb = 0;
    }
    
    else if(HOR_BTN == 1){
      offset = 0; 
      pohyb = 1;      
    }
    
    else if(VERT_BTN == 1){
      offset = 0;
      pohyb = 2;
    }    
  } 
}



// Definice pomocnych funkci

/* Funkce slouzi pro vypis pisemene na maticovy display. Promenne *
 * start_x a start_y slouzi pro predani souradnic vypisu znaku.   */
void tisk_znaku(const byte bitmapa_znaku[8][8], short start_x, short start_y){

  /* Podminka osetrujici provadeni operaci, pouze pokud jsou sou- *
   * radnice tisknuteho znaku uvnitr maticoveho znaku.            */
  if( (pohyb < 2 && (start_x < DELKA_RADKU && start_x >= -7))  || pohyb == 2){    
    short radek;
    short sloupec;
    short tmp; // Pomocna promenna
    short tmp2; // Pomocna promenna
    
    // Cyklus na prochazeni radku a sloupcu.
    for(radek = 0; radek < RADKY_PISMENE; radek++){
      for(sloupec = 0; sloupec < RADKY_PISMENE; sloupec++){       
        if(pohyb < 2){            
          tmp2 = start_x + sloupec;
          
          if(tmp2 >= 0 && tmp2 < 32){          
            tmp = radek * DELKA_RADKU;
            *(&LCD_Start + tmp2 + tmp) = bitmapa_znaku[radek][sloupec]; 
          }
        }
        
        else{        
          tmp2 = start_y + radek;
          if(tmp2 >= 0 && tmp2 < RADKY_PISMENE){
            tmp = tmp2 * DELKA_RADKU;                        
            *(&LCD_Start + tmp + sloupec + start_x) = bitmapa_znaku[radek][sloupec];   
          }
        }
      }
    }
  }
}     


/* Funkce pro tisk celeho loginu. Funkce vyuziva *
 * funkci tisk_znaku().                          */
void tisk_login(short start){  
  short i;
  
  if(pohyb < 2){    
    for(i = 0; i < 8; i++){  
      tisk_znaku(login[i],start + i * SLOUPCE_PISMENE,0);
    }
  }
  
  else{
    short tmp;
    
    if(start < 0){
      tmp = start + 8;
    }  
    else{
      tmp = start - 8;        
    }
    
    for(i = 4; i < PISMENA_LOGINU; i++){
      tisk_znaku(login[i],(i - 4) * SLOUPCE_PISMENE,tmp);       
    }
    
    for(i = 0; i < PISMENA_LOGINU/2; i++){
       tisk_znaku(login[i],i * SLOUPCE_PISMENE,start);
    }
  }
}


// Funkce pro vycisteni displeje.
void clear_display(void){
  short i;
  for(i = 0; i < POCET_LED; i++){
    *(&LCD_Start + i) = 0;
  }
}

 
// Funkce vytvarejici zpozdeni.
void zpozdeni(unsigned int n){
	unsigned int i;
	unsigned int j;
	
	for (i = 0; i < n; i++){
	  for (j = 0; j < 1; j++){
      asm nop; 
	  }
	}
}