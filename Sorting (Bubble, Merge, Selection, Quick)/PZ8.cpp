#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

template <typename Tip>
void bubble_sort(Tip *niz, int vel){
    for(int i=vel-1; i>=1; i--)
        for(int j=1; j<=i; j++)
            if(niz[j-1]>niz[j])
                std::swap(niz[j-1], niz[j]);
}

template <typename Tip>
void selection_sort(Tip *niz, int vel){
    for(int i=0; i<vel-1; i++)
        for(int j=i+1; j<vel; j++)
            if(niz[j]<niz[i])
                std::swap(niz[j], niz[i]);
}

template <typename Tip>
void merge(Tip *niz, int l, int p, int q, int u, int vel){
    Tip B[vel];
    int i=0, j=q-l, k=l;
    for(int m=0; m<=u-l; m++)
        B[m]=niz[l+m];
    while(i<=p-l && j<=u-l){
        if(B[i]<B[j])
            niz[k]=B[i++];
        else
            niz[k]=B[j++];
        k++;
    }
    while(i<=p-l)
        niz[k++]=B[i++];
    while(j<=u-l)
        niz[k++]=B[j++];
}

template <typename Tip>
void merge_rekurzija(Tip *niz, int l, int u, int vel){
    if(u>l){
        int p=(l+u-1)/2;
        int q=p+1;
        merge_rekurzija(niz, l, p, vel);
        merge_rekurzija(niz, q, u, vel);
        merge(niz, l, p, q, u, vel);
    }
}

template <typename Tip>
void merge_sort(Tip *niz, int vel){
    merge_rekurzija(niz, 0, vel-1, vel);
}

template <typename Tip>
int particija(Tip *niz, int prvi, int zadnji){
    Tip pivot=niz[prvi];
    int p=prvi+1;
    while(p<=zadnji){
        if(niz[p]<=pivot)
            p++;
        else
            break;
    }
    for(int i=p+1; i<=zadnji; i++){
        if(niz[i]<=pivot){
            std::swap(niz[p], niz[i]);
            p++;
        }
    }
    std::swap(niz[prvi], niz[p-1]);
    return p-1;
}

template <typename Tip>
void quick_rekurzija(Tip *niz, int vel, int prvi, int zadnji){
    if(prvi<zadnji){
        int j=particija(niz, prvi, zadnji);
        quick_rekurzija(niz, vel, prvi, j-1);
        quick_rekurzija(niz, vel, j+1, zadnji);
    }
}

template <typename Tip>
void quick_sort(Tip *niz, int vel){
    quick_rekurzija(niz, vel, 0, vel-1);
}

int main(){
}