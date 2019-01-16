/*
Guillaume Samson 1947104
Etienne Lescarbeault 1948656

La lumiere alterne entre 6 etats a l'aide de l'interrupteur.

Connexions:
    Le bouton est connecte au port D2 avec une soudure. Un jumper est place sur les pins.
    La lumiere LED est liee au ports B1 et B2. Le - de la LED est liee a B1 et le + est liee a B2.

Table d'etats :
etat            in      prochain        out
-------------------------------------------
initial         0       initial         lumiere rouge (PORT=1)
initial         1       etat1           lumiere rouge (PORT=1)
etat1           0       etat2           lumiere ambre (PORT alterne entre 1 et 2)
etat1           1       etat1           lumiere ambre (PORT alterne entre 1 et 2)
etat2           0       etat2           lumiere verte (PORT=2)
etat2           1       etat3           lumiere verte (PORT=2)
etat3           0       etat4           lumiere rouge (PORT=1)
etat3           1       etat3           lumiere rouge (PORT=1)
etat4           0       etat4           lumiere eteinte (PORT=0)
etat4           1       etat5           lumiere eteinte (PORT=0)
etat5           0       initial         lumiere verte (PORT=2)
etat5           1       etat5           lumiere verte (PORT=2)
*/

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

const double delai = 10.0;
/* Nom: estPresseDebounce
   IN: Cette fonction ne prend aucun 
       parametre.
   OUT: Cette fonction retourne vrai si        
        l'interrupteur est presse avec
        le filtre anti-rebond.
*/
bool estPresseDebounce(){    //Cette fonction assure que l'interrupteur ne rebondit plus avant de changer d'etat. Sans quoi le programme pourrait considerer que le bouton a ete presse plus d'une fois.
  DDRD = 0x00; // PORT D est en mode entree
  bool estPresseBool = false;
    if(PIND & 0x04){
        _delay_ms(delai);
        if(PIND & 0x04){
            estPresseBool = true;
        }
    }
  return (estPresseBool);
}

int main()
{
    DDRB = 0xff; // PORT B est en mode sortie
    uint8_t eteinte = 0;
    uint8_t rouge = 2;
    uint8_t vert = 1;
    const double tempsMsAmbreRouge = 1.0;
    const double tempsMsAmbreVert = 12.0;

    enum Etats{initial,  etatUn,  etatDeux,  etatTrois,  etatQuatre,  etatCinq }; 
    Etats etatPresent = initial;

        for(;;){
            switch(etatPresent){
                case initial : 
                    PORTB = rouge; 
                    if(estPresseDebounce()==true){
                        etatPresent=etatUn;
                    } 
                    break;

                case etatUn : 
                    while(estPresseDebounce()==true){
                        PORTB = vert;
                        _delay_ms(tempsMsAmbreVert);
                        PORTB = rouge;
                        _delay_ms(tempsMsAmbreRouge);
                    }
                    etatPresent = etatDeux;
                    break;

                case etatDeux : 
                    PORTB = vert; 
                    if(estPresseDebounce()==true){
                        etatPresent=etatTrois;
                    } 
                    break;

                case etatTrois : 
                    PORTB = rouge; 
                    if(estPresseDebounce()==false){
                        etatPresent=etatQuatre;
                    } 
                    break;

                case etatQuatre : 
                    PORTB = eteinte; 
                    if(estPresseDebounce()==true){
                        etatPresent=etatCinq;
                    } 
                    break;

                case etatCinq : 
                    PORTB = vert; 
                    if(estPresseDebounce()==false){
                        etatPresent=initial;
                    } 
                    break;
            }
        } 
}  