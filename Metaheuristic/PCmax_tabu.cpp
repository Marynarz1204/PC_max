#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono> 

using namespace std;
#define pb push_back

vector<int> czasy_zadan;
vector<vector<int>> lista_tabu;
vector<int> osobnik;
vector<int> wyniki;
int n; //liczba zadan
int m; //liczba procesorow

vector<int> greedy(bool LPT)
{
    vector<pair<int,int>> kopiec;
    vector<int> procesy;
    vector<int> wynik;
    int czas_zadania;
    kopiec.push_back(make_pair(-1,-1));
    for(int i=1;i<=m;i++)
        kopiec.push_back(make_pair(i,0));
    for(int i=0;i<n;i++)
    {
        procesy.push_back(czasy_zadan[i]);
    }
    if(LPT)
        sort(procesy.begin(),procesy.end(),greater<int>());
    else
    {
        random_shuffle(procesy.begin(),procesy.end());
    }
    for(int i=1;i<=n;i++)
    {
        czas_zadania = procesy[i-1];
        kopiec[1].second+=czas_zadania;
        wynik.pb(kopiec[1].first-1);
        int pomocnicza;
        int wskaznik=1;
        while(wskaznik*2+1<=m)
        {
            if(kopiec[wskaznik*2+1].second>=kopiec[wskaznik*2].second)
            {
                if(kopiec[wskaznik].second>kopiec[wskaznik*2].second)
                {
                    pomocnicza=kopiec[wskaznik].second;
                    kopiec[wskaznik].second=kopiec[wskaznik*2].second;
                    kopiec[wskaznik*2].second=pomocnicza;
                    pomocnicza=kopiec[wskaznik].first;
                    kopiec[wskaznik].first=kopiec[wskaznik*2].first;
                    kopiec[wskaznik*2].first=pomocnicza;
                    wskaznik=wskaznik*2;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(kopiec[wskaznik].second>kopiec[wskaznik*2+1].second)
                {
                    pomocnicza=kopiec[wskaznik].second;
                    kopiec[wskaznik].second=kopiec[wskaznik*2+1].second;
                    kopiec[wskaznik*2+1].second=pomocnicza;
                    pomocnicza=kopiec[wskaznik].first;
                    kopiec[wskaznik].first=kopiec[wskaznik*2+1].first;
                    kopiec[wskaznik*2+1].first=pomocnicza;
                    wskaznik=wskaznik*2+1;
                }
                else
                {
                    break;
                }
            }
        }
        if(m==2*wskaznik)
        {
            if(kopiec[wskaznik].second>kopiec[wskaznik*2].second)
                {
                    pomocnicza=kopiec[wskaznik].second;
                    kopiec[wskaznik].second=kopiec[wskaznik*2].second;
                    kopiec[wskaznik*2].second=pomocnicza;
                    pomocnicza=kopiec[wskaznik].first;
                    kopiec[wskaznik].first=kopiec[wskaznik*2].first;
                    kopiec[wskaznik*2].first=pomocnicza;
                    wskaznik=wskaznik*2;
                }
        }
    }
    return wynik;
}

void oblicz_osobnika()
{
    int maks=0;
    for(int i=0;i<m;i++)
    {
        wyniki[i]=0;
    }
    for(int i=0;i<n;i++)
    {
        wyniki[osobnik[i]]+=czasy_zadan[i];
        maks=max(maks,wyniki[osobnik[i]]);
    }
    wyniki[m]=maks;
}

int zamiana_wynik(int i, int j)
{
    int roznica=0;
    int pom;
    vector<int> pom_osobnik=osobnik;
    vector<int> pom_wyniki=wyniki;
    pom=osobnik[i];
    pom_wyniki[osobnik[i]]-=czasy_zadan[i];
    pom_wyniki[osobnik[j]]-=czasy_zadan[j];
    pom_wyniki[osobnik[i]]+=czasy_zadan[j];
    pom_wyniki[osobnik[j]]+=czasy_zadan[i];
    pom_osobnik[i]=osobnik[j];
    pom_osobnik[j]=pom;
    pom=wyniki[m];
    pom_wyniki[m]=0;
    for(int i=0;i<m;i++)
    {
        pom_wyniki[m]=max(pom_wyniki[m],pom_wyniki[i]);
    }
    roznica=pom-pom_wyniki[m];
    return roznica;
}

void zamien_para(int i, int j, int roznica)
{
    int pom=osobnik[i];
    wyniki[osobnik[i]]-=czasy_zadan[i];
    wyniki[osobnik[j]]-=czasy_zadan[j];
    wyniki[osobnik[i]]+=czasy_zadan[j];
    wyniki[osobnik[j]]+=czasy_zadan[i];
    osobnik[i]=osobnik[j];
    osobnik[j]=pom;
    wyniki[m]-=roznica;
}


void zamiana(int iteracja)
{
    int losowanie=rand()%n;
    int suma;
    int roznica;
    int maks=-100000;
    int ktore_i;
    int ktore_j;
    for(int i=0;i<n;i++)
    {   
        suma=(losowanie+i)%n;
        for(int j=suma+1;j<n;j++)
        {
            if(iteracja>=lista_tabu[suma][j])
            {
                roznica=zamiana_wynik(suma,j);
                if(roznica>=maks && roznica!=0)
                {
                    maks=roznica;
                    ktore_i=suma;
                    ktore_j=j;
                }
            }
        }
    }
    lista_tabu[ktore_i][ktore_j]=iteracja+(n*n/3);
    zamien_para(ktore_i,ktore_j,maks);
}

int main(){
    
    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed;
    auto finish = std::chrono::high_resolution_clock::now();
    srand (time(NULL));
    ifstream data;
    data.open("m10n200.txt");
    data>>m;
    data>>n;
    int placeholder;
    for(int i=0;i<n;i++)
    {
        data>>placeholder;
        czasy_zadan.push_back(placeholder);
        lista_tabu.pb({});
        for(int j=0;j<n;j++)
        {
            lista_tabu[i].pb(0);
        }
    }
    sort(czasy_zadan.begin(),czasy_zadan.end(),greater<int>());
    osobnik=greedy(0);
    for(int i=0;i<=m;i++)
    {
        wyniki.pb(0);
    }
    int drugi_wynik;
    oblicz_osobnika();
    int najlepszy_wynik=wyniki[m];
    int i=0; 
    while(elapsed.count()<=600)
    {   
        drugi_wynik=najlepszy_wynik;
        najlepszy_wynik=min(najlepszy_wynik,wyniki[m]);
        if(drugi_wynik!=najlepszy_wynik)
        {
            cout<<wyniki[m]<<endl;
        }
        zamiana(i);
        i++;
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
    }
    cout<<najlepszy_wynik<<endl;
}