/*
Guillaume Samson 1947104
Etienne Lescarbeault 1948656
Une lumiere rouge s'allume pendant une seconde apres que l'utilisateur aie appuye cinq fois sur le bouton.

Connexions:
    Le bouton est connecte au port D2 avec une soudure.
    La lumiere LED est liee au ports B1 et B2. Le - de la LED est liee a B1 et le + est liee a B2.

Table d'etats :
    etat            in      prochain        out
    ----------------------------------------------------------------
    initial         0       initial         lumiere eteinte (PORT=0)
    initial         1       initialAppuye   lumiere eteinte (PORT=0)
    initialAppuye   0       etat1           lumiere eteinte (PORT=0)
    initialAppuye   1       initialAppuye   lumiere eteinte (PORT=0)
    etat1           0       etat1           lumiere eteinte (PORT=0)
    etat1           1       etat1Appuye     lumiere eteinte (PORT=0)
    etat1Appuyee    0       etat2           lumiere eteinte (PORT=0)
    etat1Appuyee    1       etat1Appuye     lumiere eteinte (PORT=0)
    etat2           0       etat2           lumiere eteinte (PORT=0)
    etat2           1       etat2Appuye     lumiere eteinte (PORT=0)
    etat2Appuyee    0       etat3           lumiere eteinte (PORT=0)
    etat2Appuyee    1       etat2Appuye     lumiere eteinte (PORT=0)
    etat3           0       etat3           lumiere eteinte (PORT=0)
    etat3           1       etat3Appuye     lumiere eteinte (PORT=0)
    etat3Appuyee    0       etat4           lumiere eteinte (PORT=0)
    etat3Appuyee    1       etat3Appuye     lumiere eteinte (PORT=0)
    etat4           0       etat4           lumiere eteinte (PORT=0)
    etat4           1       etat4Appuye     lumiere eteinte (PORT=0)
    etat4Appuyee    0       etat5           lumiere eteinte (PORT=0)
    etat4Appuyee    1       etat4Appuye     lumiere eteinte (PORT=0)
    etat5           0       etat5           lumiere eteinte (PORT=0)
    etat5           1       initial         lumiere rouge (PORT=1)
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
    const double delaiUneSec = 1000.0;

    enum Etats{initial, initialAppuye, etatUn, etatUnAppuye, etatDeux, etatDeuxAppuye, etatTrois, etatTroisAppuye, etatQuatre, etatQuatreAppuye, etatCinq }; 
    Etats etatPresent = initial;

    for(;;){
        switch(etatPresent){
            case initial : 
                PORTB = eteinte;
                if(estPresseDebounce()==true){ //L'utilisateur appuie sur le bouton
                    etatPresent=initialAppuye;
                }
                break;

            case initialAppuye :
                if(estPresseDebounce()==false){ //L'utilisateur relache le bouton
                    etatPresent = etatUn;
                } 
                break;

            case etatUn : 
                if(estPresseDebounce()==true){
                    etatPresent = etatUnAppuye;
                } 
                break;

            case etatUnAppuye : 
                if(estPresseDebounce()==false){
                    etatPresent = etatDeux;
                } 
                break;

            case etatDeux : 
                if(estPresseDebounce()==true){
                    etatPresent = etatDeuxAppuye;
                } 
                break;

            case etatDeuxAppuye : 
                if(estPresseDebounce()==false){
                    etatPresent = etatTrois;
                } 
                break;

            case etatTrois : 
                if(estPresseDebounce()==true){
                    etatPresent = etatTroisAppuye;
                } 
                break;

            case etatTroisAppuye : 
                if(estPresseDebounce()==false){
                    etatPresent = etatQuatre;
                } 
                break;

            case etatQuatre : 
                if(estPresseDebounce()==true){
                    etatPresent = etatQuatreAppuye;
                } 
                break;

            case etatQuatreAppuye : 
                if(estPresseDebounce()==false){
                    etatPresent = etatCinq;
                } 
                break;

            case etatCinq : 
                if(estPresseDebounce()==false){
                    etatPresent=initial;
                    PORTB = rouge;
                    _delay_ms(delaiUneSec);
                }
                break;
        }
    }
    return 0;
}