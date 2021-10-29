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
#define liczba_procesorow m
#define liczba_zadan n

int n; // liczba zadan
int m; // liczba procesorow
vector<int> task_time; // czasy poszczegolnych zadan
vector<pair<pair<int,int>,vector<int>>> population; // populacja
int population_size; // rozmiar populacji wzgledem liczby zadan

pair<pair<int,int>,vector<int>> greedy(bool LPT)
{
    vector<pair<int,int>> kopiec;
    vector<int> procesy;
    vector<int> wynik;
    int czas_zadania;
    kopiec.push_back(make_pair(-1,-1));
    for(int i=1;i<=liczba_procesorow;i++)
        kopiec.push_back(make_pair(i,0));
    for(int i=0;i<liczba_zadan;i++)
    {
        procesy.push_back(task_time[i]);
    }
    if(LPT)
        sort(procesy.begin(),procesy.end(),greater<int>());
    else
    {
        random_shuffle(procesy.begin(),procesy.end());
    }
    for(int i=1;i<=liczba_zadan;i++)
    {
        czas_zadania = procesy[i-1];
        kopiec[1].second+=czas_zadania;
        wynik.pb(kopiec[1].first-1);
        int pomocnicza;
        int wskaznik=1;
        while(wskaznik*2+1<=liczba_procesorow)
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
        if(liczba_procesorow==2*wskaznik)
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
    int maksymalna_wartosc=0;
    int procesor;
    for(int i=1;i<=liczba_procesorow;i++)
    {
        if(maksymalna_wartosc<kopiec[i].second)
        {
            procesor=i-1;
            maksymalna_wartosc=kopiec[i].second;
        }
            
    }
    return make_pair(make_pair(maksymalna_wartosc,procesor),wynik);
}

// generacja populacji poczatkowej
void generate()
{
    vector<int> help;
    vector<int> sum;
    int max_time=0;
    int processor;
    for(int i=0;i<m;i++)
    {
        sum.pb(0);
    }
    population.pb(greedy(1));
    for(int i=1;i<population_size;i++)
    {
        population.pb(greedy(0));
        max_time=0;
        for(int j=0;j<m;j++)
            sum[j]=0;
        for(int j=0;j<n;j++)
        {
            sum[population[i].second[j]]+=task_time[j];
            if(max_time<sum[population[i].second[j]])
            {
                max_time=sum[population[i].second[j]];
                processor=population[i].second[j];
            }
        }
    }
    cout<<"Size of population: "<<population_size<<endl;
}

// losowa zamiana dwoch genow wewnatrz instancji
pair<pair<int,int>,vector<int>> mutate(pair<pair<int,int>,vector<int>> child)
{
    int processor;
    int gene1;
    int gene2;
    int number_of_swaps=(rand()%3);
    int gene_finder;
    vector<int> sum;
    int max_time=0;
    for(int i=0;i<m;i++)
        sum.pb(0);
    for(int i=0;i<number_of_swaps;i++)
    {
        gene1=rand()%n;
        if(i==0)
        {
            gene_finder=gene1;
            while(child.second[gene_finder]!=child.first.second)
            {
                if(child.second[gene_finder]==child.first.second)
                {
                    gene1=gene_finder;
                    break;
                }
                gene_finder++;
                gene_finder%=n;
            }
        }
        gene2=gene1;
        while(child.second[gene2]==child.second[gene1])
        {
            gene2=rand()%n;
        }
        swap(child.second[gene1],child.second[gene2]);
    }
    for(int j=0;j<n;j++)
    {
        sum[child.second[j]]+=task_time[j];
        if(max_time<sum[child.second[j]])
        {
            max_time=sum[child.second[j]];
            processor=child.second[j];
        }
    }
    child.first.first=max_time;
    child.first.second=processor;
    return child;
}

// tworzenie pokolenia potomnego i modyfikacja populacji
pair<pair<pair<int,int>,vector<int>>,pair<pair<int,int>,vector<int>>> cross(pair<pair<int,int>,vector<int>> parent1, pair<pair<int,int>,vector<int>> parent2)
{
    pair<pair<int,int>,vector<int>> child1;
    pair<pair<int,int>,vector<int>> child2;
    vector<int> sum1;
    vector<int> sum2;
    int processor1;
    int processor2;
    int max_time1=0;
    int max_time2=0;
    for(int i=0;i<m;i++)
    {
        sum1.pb(0);
        sum2.pb(0);
    }
    int sum_of_scores=parent1.first.first+parent2.first.first;
    for(int j=0;j<n;j++)
    {
        if(rand()%sum_of_scores>=parent1.first.first)
        {
            child1.second.pb(parent1.second[j]);
            sum1[child1.second[j]]+=task_time[j];
            child2.second.pb(parent2.second[j]);
            sum2[child2.second[j]]+=task_time[j];
            if(max_time1<sum1[child1.second[j]])
            {
                max_time1=sum1[child1.second[j]];
                processor1=child1.second[j];
            }
            if(max_time2<sum2[child2.second[j]])
            {
                max_time2=sum2[child2.second[j]];
                processor2=child2.second[j];
            }
        }
        else
        {
            child1.second.pb(parent2.second[j]);
            sum1[child1.second[j]]+=task_time[j];
            child2.second.pb(parent1.second[j]);
            sum2[child2.second[j]]+=task_time[j];
            if(max_time1<sum1[child1.second[j]])
            {
                max_time1=sum1[child1.second[j]];
                processor1=child1.second[j];
            }
            if(max_time2<sum2[child2.second[j]])
            {
                max_time2=sum2[child2.second[j]];
                processor2=child2.second[j];
            }
        }
    }
    child1.first.first=max_time1;
    child2.first.first=max_time2;
    child1.first.second=processor1;
    child2.first.second=processor2;
    child1=mutate(child1);
    child2=mutate(child2);
    return make_pair(child1,child2);
}

// wybor najlepszych instancji
vector<pair<pair<int,int>,vector<int>>> select(vector<int> trasition_value)
{
    vector<pair<pair<int,int>,vector<int>>> child;
    pair<pair<pair<int,int>,vector<int>>,pair<pair<int,int>,vector<int>>> help;
    int cross_part=population_size/5;
    float mutation_percent = 0.01f;
    int parent_number1;
    int parent_number2;
    int rand_size;
    int rand_size1;
    int rand_size2;
    int l,r,m,v;
    if(cross_part%2==1)
    {
        cross_part++;
    }
    for(int i=0;i<cross_part;i+=2)
    {
        rand_size1=rand()%10000;
        rand_size2=10000*(rand()%10000);
        rand_size=rand_size1+rand_size2;
        parent_number1=rand_size%(population_size*(population_size+1)/2);
        if(parent_number1==0)
            parent_number1=1;
        v=parent_number1;
        l=0;r=population_size;
        while(l<r)
        {
            m=(l+r)/2;
            if(trasition_value[m]>v)
                r=m;
            else
                l=m+1;
        }
        parent_number1=r;
        rand_size1=rand()%10000;
        rand_size2=10000*(rand()%10000);
        rand_size=rand_size1+rand_size2;
        parent_number2=rand_size%(population_size*(population_size+1)/2);
        if(parent_number2==0)
            parent_number2=1;
        v=parent_number2;
        l=0;r=population_size;
        while(l<r)
        {
            m=(l+r)/2;
            if(trasition_value[m]>v)
                r=m;
            else
                l=m+1;
        }
        parent_number2=r;
        help=cross(population[parent_number1-1],population[parent_number2-1]);
        child.pb(help.first);
        child.pb(help.second);
    }
    int who;
    for(int i=0;i<population_size*mutation_percent;i++)
    {
        who=1+rand()%(population_size-1);
        population[who]=mutate(population[who]);
    }
    return child;
}

// laczenie populacji i dzieci, wybieranie najlepszych
void merge(vector<pair<pair<int,int>,vector<int>>> child)
{
    vector<pair<pair<int,int>,vector<int>>> population_plus_child;
    population_plus_child=population;
    for(int i=0;i<child.size();i++)
    {
        population_plus_child.pb(child[i]);
    }
    sort(population_plus_child.begin(),population_plus_child.end());
    for(int i=0;i<population_size;i++)
    {
        population[i]=population_plus_child[i];
    }
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    srand (time(NULL));
    ifstream data;
    data.open("m10n200.txt");
    data>>m;
    data>>n;
    cout<<"Number of processors: "<<m<<endl<<"Number of tasks: "<<n<<endl;
    int placeholder;
    vector<int> trasition_value={};
    int sum2=0;
    population_size=200;
    trasition_value.pb(0);
    for(int i=0;i<population_size;i++)
    {
        sum2+=population_size-i;
        trasition_value.pb(sum2);
    }
    vector <int> range;
    for(int i=0;i<n;i++)
    {
        data>>placeholder;
        task_time.push_back(placeholder);
        range.pb(i);
    }
    data.close();
    std::chrono::duration<double> elapsed;
    generate();
    sort(population.begin(),population.end());

    int i=1;
    while(elapsed.count()<=120)
    {        
        merge(select(trasition_value));
        if((i%1000)==1)
        {
            printf("generacja %d: %d, %d\n",i,population[0].first.first,population[population_size-1].first.first);
        }    
        i++;
        auto finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
    }
}