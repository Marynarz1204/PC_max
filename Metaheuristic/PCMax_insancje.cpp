#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>


using namespace std;

int main()
{
    ofstream plik;
    plik.open("test.txt");
    srand(time(NULL));
    int ilosc_procesorow;
    int ilosc_zadan;
    int n,m;
    cout<<"Przedzial liczby procesorow:";
    cin>>n;
    cin>>m;
    if(m==n)
        ilosc_procesorow=n;
    else
        ilosc_procesorow=n+rand()%(m-n);
    plik<<ilosc_procesorow<<endl;
    cout<<endl<<"Przedzial liczby zadan:";
    cin>>n;
    cin>>m;
    if(m!=n)
        ilosc_zadan=n+rand()%(m-n);
    else
        ilosc_zadan=n;
    plik<<ilosc_zadan;
    for(int i=0;i<ilosc_zadan;i++)
    {
        plik<<endl<<1+rand()%999;
    }
    plik.close();
}