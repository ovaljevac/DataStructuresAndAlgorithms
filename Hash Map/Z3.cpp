#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
#include <functional>


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
class HashMapa : public Mapa<TipKljuca, TipVrijednosti>{
    int brojEl;
    int makskapacitet;
    std::pair<TipKljuca, TipVrijednosti> **p;
    std::function<unsigned int (TipKljuca, unsigned int)> hashfunkcija;
    void povecajkapacitet(){
        std::pair<TipKljuca, TipVrijednosti> **novi=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet*2]{};
        for(int i=0; i<makskapacitet; i++){
            if(p[i]){
                unsigned int indeks=hashfunkcija(p[i]->first, makskapacitet*2);
                while(novi[indeks])
                    indeks=(indeks+1)%(makskapacitet*2);
                novi[indeks]=p[i];
            };
            p[i]=nullptr;
        }
        delete [] p;
        p=novi;
        makskapacitet*=2;
    };
    public:
    HashMapa();
    int brojElemenata() const override {return brojEl;}
    ~HashMapa(){
        if(!p) return;
        for(int i=0; i<makskapacitet; i++) 
            delete p[i];
        delete[] p;
    }
    HashMapa(const HashMapa &n);
    HashMapa &operator=(const HashMapa &n);
    TipVrijednosti &operator[](const TipKljuca &t) override;
    TipVrijednosti operator[](const TipKljuca &t) const override;
    void obrisi() override ;
    void obrisi(const TipKljuca &kljuc) override;
    void definisiHashFunkciju(std::function<unsigned int (TipKljuca, unsigned int)> f){
        hashfunkcija=f;
    };
};

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(const HashMapa &n){
    brojEl=n.brojEl;
    makskapacitet=n.makskapacitet;
    p=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet]{};
    for(int i=0; i<makskapacitet; i++)
        if(n.p[i]) p[i]=new std::pair<TipKljuca, TipVrijednosti>(*n.p[i]);
    hashfunkcija=n.hashfunkcija;
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti> &HashMapa<TipKljuca, TipVrijednosti>::operator=(const HashMapa &n){
    if(&n==this) return *this;
    if(p){
        for(int i=0; i<makskapacitet; i++)
            delete p[i];
        delete[] p;
    }
    brojEl=n.brojEl;
    makskapacitet=n.makskapacitet;
    p=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet]{};
    for(int i=0; i<makskapacitet; i++)
        if(n.p[i]) p[i]=new std::pair<TipKljuca, TipVrijednosti>(*n.p[i]);
    hashfunkcija=n.hashfunkcija;
    return *this;
}


template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(){
    brojEl=0;
    makskapacitet=10;
    p=new std::pair<TipKljuca, TipVrijednosti>*[makskapacitet]{}; 
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &HashMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t){
    unsigned int indeks=hashfunkcija(t, makskapacitet);
    int br=0;
    while(p[indeks] && p[indeks]->first!=t){
        indeks=(indeks+1)%makskapacitet;
        br++;
        if(br==makskapacitet){
            povecajkapacitet();
            indeks=hashfunkcija(t, makskapacitet);
        }
    }
    if(p[indeks] && p[indeks]->first==t)
        return p[indeks]->second;
    brojEl++;
    p[indeks]=new std::pair<TipKljuca, TipVrijednosti>(std::make_pair(t, TipVrijednosti()));
    return p[indeks]->second;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti HashMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t) const{
    unsigned int indeks=hashfunkcija(t, makskapacitet);
    while(p[indeks] && p[indeks]->first!=t)
        indeks=(indeks+1)%makskapacitet;
    if(p[indeks] && p[indeks]->first==t)
        return p[indeks]->second;
    return TipVrijednosti();
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc){
    unsigned int indeks=hashfunkcija(kljuc, makskapacitet);
    while(p[indeks]){
        if(p[indeks]->first==kljuc){
            delete p[indeks];
            p[indeks]=nullptr;
            brojEl--;
            indeks=(indeks+1)%makskapacitet;
            while(p[indeks]){
                TipKljuca temp=p[indeks]->first;
                TipVrijednosti temp2=p[indeks]->second;
                delete p[indeks];
                p[indeks]=nullptr;
                brojEl--;
                (*this)[temp]=temp2;
                indeks=(indeks+1)%makskapacitet;
            }
            return;
        }
        indeks=(indeks+1)%makskapacitet;
    }
    throw std::domain_error("nema kljuca debilu");
}


template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi(){
    if(!p) return;
    for(int i=0; i<makskapacitet; i++){
        delete p[i];
        p[i]=nullptr;
    }
    brojEl=0;
}

int main(){
}