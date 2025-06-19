#include <iostream>
#include <tuple>
#include <utility>
#include <vector>


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
    NizMapa &operator=(const NizMapa &n) override;
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


template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
    struct Cvor{
        Cvor *lijevi;
        Cvor *desni;
        Cvor *roditelj;
        TipKljuca kljuc;
        TipVrijednosti info;
        Cvor(Cvor *l, Cvor *d, Cvor *r, const TipVrijednosti &info) : lijevi(l), desni(d), roditelj(r), info(info) {}
    };
    int brojEl;
    Cvor *korijen;
    Cvor *pronadjicvor(Cvor *korijenn, const TipKljuca &t) const {
        if(korijenn==nullptr || t==korijenn->kljuc)
            return korijenn;
        if(t<korijenn->kljuc)
            return pronadjicvor(korijenn->lijevi, t);
        else
            return pronadjicvor(korijenn->desni, t);
    };
    void obrisicvor(Cvor *p){
        if(!p) return;
        obrisicvor(p->lijevi);
        obrisicvor(p->desni);
        delete p;
        p=nullptr;
    }
    void prekopiraj(Cvor *postojeci){
        Cvor *novi=new Cvor(0, 0, 0, postojeci->info);
        novi->kljuc=postojeci->kljuc;
        if(!korijen)
            korijen=novi;
        else{
            Cvor *p=korijen;
            Cvor *r=nullptr;
            while(p){
                r=p;
                if(postojeci->kljuc<p->kljuc)
                    p=p->lijevi;
                else
                    p=p->desni;
            }   
            novi->roditelj=r;
            if(postojeci->kljuc<r->kljuc)
                r->lijevi=novi;
            else 
                r->desni=novi;
        }
    }
    void preorder(Cvor *postojeci){
        if(postojeci){
            prekopiraj(postojeci);
            preorder(postojeci->lijevi);
            preorder(postojeci->desni);
        }
    }
    public:
    BinStabloMapa();
    ~BinStabloMapa();
    BinStabloMapa(const BinStabloMapa &b);
    BinStabloMapa &operator=(const BinStabloMapa &b);
    TipVrijednosti &operator[](const TipKljuca &t) override;
    TipVrijednosti operator[](const TipKljuca &t) const override;
    int brojElemenata() const override {return brojEl;};
    void obrisi() override ;
    void obrisi(const TipKljuca &kljuc) override;
};

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(){
    korijen=nullptr;
    brojEl=0;
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::~BinStabloMapa(){
    obrisi();
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(const BinStabloMapa &b){
    korijen=0;
    preorder(b.korijen);
    brojEl = b.brojEl;
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti> &BinStabloMapa<TipKljuca, TipVrijednosti>::operator=(const BinStabloMapa &b){
    if(&b==this)
        return *this;
    obrisi();
    preorder(b.korijen);
    brojEl = b.brojEl;
    return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t){
    Cvor *pronadjen=pronadjicvor(korijen, t);
    if(pronadjen)
        return pronadjen->info;
    Cvor *novi=new Cvor(0, 0, 0, TipVrijednosti());
    novi->kljuc=t;
    if(!korijen)
        korijen=novi;
    else{
        Cvor *p=korijen;
        Cvor *r=nullptr;
        while(p){
            r=p;
            if(t<p->kljuc)
                p=p->lijevi;
            else
                p=p->desni;
        }
        novi->roditelj=r;
        if(t<r->kljuc)
            r->lijevi=novi;
        else 
            r->desni=novi;
    }
    brojEl++;
    return novi->info;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t) const{
    Cvor *pronadjen=pronadjicvor(korijen, t);
    if(pronadjen)
        return pronadjen->info;
    return TipVrijednosti();
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &t){
    Cvor *p=korijen;
    Cvor *r=nullptr;
    while(p && t!=p->kljuc){
        r=p;
        if(t<p->kljuc)
            p=p->lijevi;
        else
            p=p->desni;
    }
    if(!p)
        throw std::domain_error("Nema kljuca majmune");
    Cvor *m=nullptr;
    if(!p->lijevi)
        m=p->desni;
    else{
        Cvor *pm=p;
        m=p->lijevi;
        Cvor *tmp=m->desni;
        while(tmp){
            pm=m;
            m=tmp;
            tmp=m->desni;
        }
        if(pm!=p){
            pm->desni=m->lijevi;
            m->lijevi=p->lijevi;
        }
        m->desni=p->desni;
    }
    if(!r)
        korijen=m;
    else{
        if(p==r->lijevi)
            r->lijevi=m;
        else
            r->desni=m;
    }
    delete p;
    brojEl--;
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi(){
    obrisicvor(korijen);
    korijen=nullptr;
    brojEl=0;
}

int main(){
}