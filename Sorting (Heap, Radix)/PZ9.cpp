#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int izdvojicifru(int x, int djelitelj){
    return(x/(int)pow(10, djelitelj))%10;
}

void sortiranjepocifri(vector<int> &a, int djelitelj){
    vector<int> pomocni(10, 0);
    vector<int> novi(a.size());
    for(int i=0; i<a.size(); i++){
        pomocni[izdvojicifru(a[i], djelitelj)]++;
    }
    for(int i=1; i<10; i++)
        pomocni[i]+=pomocni[i-1];
    for(int i=a.size()-1; i>=0; i--){
        novi[pomocni[izdvojicifru(a[i], djelitelj)]-1]=a[i];
        pomocni[izdvojicifru(a[i], djelitelj)]--;
    }
    for(int i=0; i<a.size(); i++)
    a[i]=novi[i];
}

void radixSort(vector<int> &a){
    int max=*max_element(a.begin(), a.end());
    int brc=0;
    while(max){
        max/=10;
        brc++;
    }
    for(int i=1; i<=brc; i++)
        sortiranjepocifri(a, i-1);
}

int roditelj(int i){
    return (i-1)/2;
}

void popraviGore(vector<int> &a, int i, bool maxheap){
    while(i!=0 && a[i]>a[roditelj(i)]){
        swap(a[i], a[roditelj(i)]);
        i=roditelj(i);
    }
}

int lijevoDijete(int i){
    return 2*i+1;
}

int desnoDijete(int i){
    return 2*i+2;
}

void popraviDolje(vector<int> &a, int i, int vel){
    int roditelj=i;
    int ld=lijevoDijete(i);
    int dd=desnoDijete(i);
    if(ld<vel && a[ld]>a[roditelj])
        roditelj=ld;
    if(dd<vel && a[dd]>a[roditelj])
        roditelj=dd;
    if(roditelj!=i){
       swap(a[i], a[roditelj]);
        popraviDolje(a, roditelj, vel);
    }
}

void stvoriGomilu(vector<int> &a){
    for(int i=a.size()/2-1; i>=0; i--)
        popraviDolje(a, i, a.size());
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina){
    a.push_back(umetnuti);
    popraviGore(a, velicina++, true);
}

int izbaciPrvi(vector<int> &a, int &velicina){
    if(velicina==0)
        return -1;
    swap(a[0], a[--velicina]);
    if(velicina!=0)
        popraviDolje(a, 0, velicina);
    return a[velicina];
}


void gomilaSort(vector<int> &a){
    stvoriGomilu(a);
    int vel=a.size();
    for(int i=vel-1; i>=0;i--){
        swap(a[0], a[i]);
        popraviDolje(a, 0, i);
    }
}

int main(){
}