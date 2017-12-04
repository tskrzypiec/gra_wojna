#include <iostream>
#include <time.h>
//#include <conio.h> // getch();
#include <stdlib.h>
#include <string.h>
using namespace std;
//#define DEBUG


const int ROZMIAR_TALII = 52;
char symbole[15] = {'_', ',', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K', 'A'};
static int taliaKart[ROZMIAR_TALII] = {0};
int reka1[ROZMIAR_TALII] = {0}, reka2[ROZMIAR_TALII] = {0};
int stol1[ROZMIAR_TALII] = {0}, stol2[ROZMIAR_TALII] = {0};
int iloscWylozen = 0;


int wyswietlKarty(int *talia)
{
    int silaKart = 0; // Sila kart obliczana ze wzoru: K = 1pkt, A = 2 pkt
    for (int i = 0; i < ROZMIAR_TALII; i++)
    {
        cout << symbole[talia[i]] << " ";
        if(talia[i] == 13) silaKart +=1;
        if(talia[i] == 14) silaKart +=2;
    }
    cout << "  " << silaKart << endl;
    return silaKart;
}

void wyswietlStanRozgrywki()
{
    cout << "iloscWylozen: " << iloscWylozen << endl;
    cout << "Stol1: ";
    wyswietlKarty(stol1);
    cout << "Stol2: ";
    wyswietlKarty(stol2);
    cout << "Reka1: ";
    wyswietlKarty(reka1);
    cout << "Reka2: ";
    wyswietlKarty(reka2);
    cout << endl;
}


void czyscStol()
{
    for (int i = 0; i < ROZMIAR_TALII; i++)
    {
        stol1[i] = 0;
        stol2[i] = 0;
    }
}

void czyscRece()
{
    for (int i = 0; i < ROZMIAR_TALII; i++)
    {
        reka1[i] = 0;
        reka2[i] = 0;
    }
}

void initTalia(int *talia)
{
    int najmnijeszaKarta = 15 - ROZMIAR_TALII / 4;
    for (int i = ROZMIAR_TALII - 1; i>=0; i--)
    {
        *(talia) = (najmnijeszaKarta + (i % (ROZMIAR_TALII / 4)));
        talia++;
    }
    czyscRece();
    czyscStol();
}

void swapNum(int *x, int *y)
{
    int z = *x;
    *x = *y;
    *y = z;
}

void tasuj (int *talia)
{
    int r = 0;
    for (int i=0; i<ROZMIAR_TALII; i++)
    {
        r = rand() % (ROZMIAR_TALII-i) + i;
        swapNum ((talia+i), (talia+r));
    }
}

void rozdaj(int *talia, int *reka1, int *reka2)
{
    for (int i = 0; i < ROZMIAR_TALII; i++)
    {
        if(i%2)
        {
            *reka1 = talia[i];
            reka1++;
        }
        else
        {
            *reka2 = talia[i];
            reka2++;
        }
    }
}

/*
Wyrzuca karte na stol oraz przesuwa karty w rece
Na stole bedzie 1 karta wiecej, w rece 1 mniej
*/
void kartyNaStol(int *a_reka, int *a_stol)
{
    *a_stol = *a_reka;
    // todo: tutaj powinno byc while(!0) lub po memmove jeszcze memset(0)
    memmove(a_reka, a_reka + 1, ROZMIAR_TALII * sizeof(int));
    *(a_reka + ROZMIAR_TALII - 1) = 0;
}

int wylozKarty(int *a_reka1, int *a_reka2, int *a_stol1, int *a_stol2)
{
    int zwycieskiGracz = 0;

    // Czy obaj gracze maja jeszcze karty?
    if(*a_reka1 != 0 && *a_reka1 !=0)
    {
        kartyNaStol(a_reka1, a_stol1);
        kartyNaStol(a_reka2, a_stol2);
        if(*a_stol1 == *a_stol2) // wojna
        {
            kartyNaStol(a_reka1, a_stol1 + 1);
            kartyNaStol(a_reka2, a_stol2 + 1);
            // w przypadku wojny rekurencja
            zwycieskiGracz = wylozKarty(a_reka1, a_reka2, a_stol1 + 2, a_stol2 + 2);
        }
        else if(*a_stol1 > *a_stol2)
        {
            zwycieskiGracz = 1;
        }
        else
        {
            zwycieskiGracz = 2;
        }
    }
    else
    {
        if(*a_reka1 == 0 && *a_reka1 ==0)
            zwycieskiGracz = 0; // remis
        else if(*a_reka1 == 0)
            zwycieskiGracz = 2;
        else if(*a_reka2 == 0)
            zwycieskiGracz = 1;
        else
            cout << "\nERROR!" << __LINE__ << endl; // Nigdy nie powinno sie zdarzyc
    }
    return zwycieskiGracz;
}

void zbierzKupkiZeStolu(int *a_reka, int *a_kupka1, int *a_kupka2)
{
    while((*a_kupka1) != 0)
    {
        *a_reka = *a_kupka1;
        a_reka++; a_kupka1++;
    }
    while((*a_kupka2) != 0)
    {
        *a_reka = *a_kupka2;
        a_reka++; a_kupka2++;
    }    
}

void zbierzKartyZeStolu(int *a_reka)
{
    // Przesun wskaznik na ostatnia karte
    while((*a_reka) != 0)
    {
        a_reka++;
    }
    if ( rand() % 2 == 0) // Kolejnosc zbierania kart ze stolu ma byc losowa
        zbierzKupkiZeStolu(a_reka, stol1, stol2);
    else
        zbierzKupkiZeStolu(a_reka, stol2, stol1);

    czyscStol();
}

void graj()
{
    int zwycieskiGracz = 0;
    zwycieskiGracz = wylozKarty(reka1, reka2, stol1, stol2);

#ifdef DEBUG
wyswietlStanRozgrywki();
getch();
#endif

    if (zwycieskiGracz == 1)
    {
        // karty ze stolu do reki gracza1
        zbierzKartyZeStolu(reka1);
    }
    else if (zwycieskiGracz == 2)
    {
        // karty ze stolu do reki gracza2
        zbierzKartyZeStolu(reka2);
    }
}


int main()
{
    int maxIloscWylozen = 1000;
#ifdef DEBUG
    int maxIloscGier = 10;
#else    
    int maxIloscGier = 1000;
#endif
    int wygraneG1 = 0, wygraneG2 = 0, remisy = 0;
    bool graRemisowa = true;

    srand(time(NULL));

    while (maxIloscGier--)
    {
        graRemisowa = true;
        initTalia(taliaKart);
        tasuj(taliaKart);

        rozdaj(taliaKart, reka1, reka2);
        
#ifdef DEBUG  
wyswietlStanRozgrywki();
#endif
        // W kazdej grze moze byc maxIloscWylozen kart, po tej ilosci remis
        for (iloscWylozen = 1; iloscWylozen <= maxIloscWylozen; iloscWylozen++)
        {
            graj();
            
            if(reka1[0] == 0 && reka2[0] == 0)
            {
#ifdef DEBUG                  
                cout << "Remis po " << iloscWylozen << " wylozeniach\n\n";
#endif
                remisy++;
                break;
            }
            if(reka1[0] == 0)
            {
#ifdef DEBUG                     
                cout << "Wygral gracz 2 po " << iloscWylozen << " wylozeniach\n\n";
#endif
                wygraneG2++;
                graRemisowa = false;
                break;
            }
            if(reka2[0] == 0)
            {
#ifdef DEBUG                     
                cout << "Wygral gracz 1 po " << iloscWylozen << " wylozeniach\n\n";
#endif
                wygraneG1++;
                graRemisowa = false;
                break;
            }
        } // for()

        if (graRemisowa == true)
        {
#ifdef DEBUG   
            cout << "Remis po " << iloscWylozen - 1 << " wylozeniach\n\n";
#endif
            remisy++;
        }


    } // while (maxIloscGier--)

    cout << "Ilosc wygranych gracza 1: " << wygraneG1 << endl;
    cout << "Ilosc wygranych gracza 2: " << wygraneG2 << endl;
    cout << "Ilosc remisow: " << remisy << endl;
    return 0;
}

