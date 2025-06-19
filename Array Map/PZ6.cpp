#include <iostream>
#include <tuple>
#include <utility>


using std::cout;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
    public:
    virtual ~Mapa();
    Mapa();
    virtual TipVrijednosti &operator[](const TipKljuca &t) =0;
    virtual TipVrijednosti operator[](const TipKljuca &t) const=0;
    virtual int brojElemenata() const=0;
    virtual void obrisi()=0;
    virtual void obrisi(const TipKljuca &kljuc)=0;
};

template <typename TipKljuca, typename TipVrijednosti>
Mapa<TipKljuca, TipVrijednosti>::Mapa(){
    
}

template <typename TipKljuca, typename TipVrijednosti>
Mapa<TipKljuca, TipVrijednosti>::~Mapa(){
    
}

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>{
    int brojEl;
    int makskapacitet;
    std::pair<TipKljuca, TipVrijednosti> **p;
    void inicijaliziraj(int pocetni){
        for(int i=pocetni; i<makskapacitet; i++)
        p[i]=new std::pair<TipKljuca, TipVrijednosti>(); 
    }
    void povecajkapacitet(){
        std::pair<TipKljuca, TipVrijednosti> **novi=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet*2]{};
        for(int i=0; i<brojEl; i++){
            novi[i]=p[i];
        }
        delete[] p;
        p=novi;
        makskapacitet*=2;
        inicijaliziraj(brojEl);
    };
    public:
    NizMapa();
    int brojElemenata() const override {return brojEl;}
    ~NizMapa(){
        if(!p) return;
        for(int i=0; i<makskapacitet; i++)
            delete p[i];
        delete[] p;
    }
    NizMapa(const NizMapa &n);
    NizMapa &operator=(const NizMapa &n);
    TipVrijednosti &operator[](const TipKljuca &t) override;
    TipVrijednosti operator[](const TipKljuca &t) const override;
    void obrisi() override ;
    void obrisi(const TipKljuca &kljuc) override;
};

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa &n){
    brojEl=n.brojEl;
    makskapacitet=n.makskapacitet;
    p=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet]{};
    for(int i=0; i<brojEl; i++){
        p[i]=new std::pair<TipKljuca, TipVrijednosti>(*n.p[i]);
    }
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &NizMapa<TipKljuca, TipVrijednosti>::operator=(const NizMapa &n){
    if(&n==this){
        return *this;
    }
    brojEl=n.brojEl;
    if(makskapacitet>=n.brojEl && brojEl!=0){
        for(int i=0; i<brojEl; i++)
            *p[i]=*n.p[i];
    }
    else if(makskapacitet<brojEl){
        obrisi();
        makskapacitet=n.makskapacitet;
        delete[] p;
        p=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet]{};
        for(int i=0; i<makskapacitet; i++){
            p[i]=new std::pair<TipKljuca, TipVrijednosti>(*n.p[i]);
        }
    }
    return *this;
}


template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(){
    brojEl=0;
    makskapacitet=10;
    p=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet]{};
    inicijaliziraj(0);  
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &NizMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t){
    for(int i=0; i<brojEl; i++){
        if(p[i]){
        if(p[i]->first==t){
            return p[i]->second;
        }
        }
    }
    if(brojEl==makskapacitet)
        povecajkapacitet();
    brojEl++;
    *p[brojEl-1]=std::make_pair(t, TipVrijednosti());
    return p[brojEl-1]->second;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti NizMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t) const{
    for(int i=0; i<brojEl; i++){
        if(p[i]){
        if(p[i]->first==t){
            return p[i]->second;
        }
        }
    }
    return TipVrijednosti();
}

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc){
    for(int i=0; i<brojEl; i++){
        if(p[i]->first==kljuc){
            delete p[i];
            p[i]=nullptr;
            brojEl--;
            return;
        }
    }
    throw std::domain_error("nema kljuca debilu");
}


template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(){
    if(!p) return;
    for(int i=0; i<makskapacitet; i++){
        delete p[i];
        p[i]=nullptr;
    }
    inicijaliziraj(0);
    brojEl=0;
}


int main(){
}