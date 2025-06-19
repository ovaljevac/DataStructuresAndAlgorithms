#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

using std::endl;
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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
    struct Cvor{
        Cvor *lijevi;
        Cvor *desni;
        TipKljuca kljuc;
        TipVrijednosti info;
        int visina;
        Cvor(Cvor *l, Cvor *d, const TipKljuca &k, const TipVrijednosti &info, int visina=1) : lijevi(l), desni(d), kljuc(k), info(info), visina(visina){}
    };
    int brojEl;
    Cvor *korijen;
    int visina(Cvor *cvor) const{
        if(cvor)
            return cvor->visina;
        return 0;
    }
    int balans(Cvor *cvor) const{
        if(cvor)
            return visina(cvor->lijevi)-visina(cvor->desni);
        return 0;
    }
    void novavis(Cvor *cvor){
        if(cvor)
            cvor->visina=1+std::max(visina(cvor->lijevi), visina(cvor->desni));
    }
    Cvor *rotirajdesno(Cvor *y){
        Cvor *x=y->lijevi;
        Cvor *T2=x->desni;
        x->desni=y;
        y->lijevi=T2;
        novavis(y);
        novavis(x);
        return x;
    }

    Cvor *rotirajlijevo(Cvor *x){
        Cvor *y=x->desni;
        Cvor *T2=y->lijevi;
        y->lijevi=x;
        x->desni=T2;
        novavis(x);
        novavis(y);
        return y;
    }

Cvor *balansiraj(Cvor *cvor, const TipKljuca &kljuc, const TipVrijednosti &info){
            if(!cvor){
            brojEl++;
            return new Cvor(0, 0, kljuc, info);
        }
        if(kljuc<cvor->kljuc)
            cvor->lijevi=balansiraj(cvor->lijevi, kljuc, info);
        else if(kljuc>cvor->kljuc)
            cvor->desni=balansiraj(cvor->desni, kljuc, info);
        else
            return cvor;
        novavis(cvor);
        int b=balans(cvor);
        if(b>1 && kljuc<cvor->lijevi->kljuc)
            return rotirajdesno(cvor);
        if(b<-1 && kljuc>cvor->desni->kljuc)
            return rotirajlijevo(cvor);
        if(b>1 && kljuc>cvor->lijevi->kljuc){
            cvor->lijevi=rotirajlijevo(cvor->lijevi);
            return rotirajdesno(cvor);
        }
        if(b<-1 && kljuc<cvor->desni->kljuc){
            cvor->desni=rotirajdesno(cvor->desni);
            return rotirajlijevo(cvor);
        }
        return cvor;    
}


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
        Cvor *novi=new Cvor(0, 0, postojeci->kljuc, postojeci->info);
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
           // novi->roditelj=r;
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
    AVLStabloMapa();
    ~AVLStabloMapa();
    AVLStabloMapa(const AVLStabloMapa &b);
    AVLStabloMapa &operator=(const AVLStabloMapa &b);
    TipVrijednosti &operator[](const TipKljuca &t) override;
    TipVrijednosti operator[](const TipKljuca &t) const override;
    int brojElemenata() const override {return brojEl;};
    void obrisi() override ;
    void obrisi(const TipKljuca &kljuc) override;
    void Preorder();
    void Preorder2(Cvor *postojeci);
};

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::Preorder(){
    Preorder2(korijen);
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::Preorder2(Cvor *postojeci){
    if(postojeci){
        std::cout<<postojeci->info<<",";
        Preorder2(postojeci->lijevi);
        Preorder2(postojeci->desni);
    }
}

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::AVLStabloMapa(){
    korijen=nullptr;
    brojEl=0;
}

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::~AVLStabloMapa(){
    obrisi();
}

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::AVLStabloMapa(const AVLStabloMapa &b){
    korijen=0;
    preorder(b.korijen);
    brojEl = b.brojEl;
}

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti> &AVLStabloMapa<TipKljuca, TipVrijednosti>::operator=(const AVLStabloMapa &b){
    if(&b==this)
        return *this;
    obrisi();
    preorder(b.korijen);
    brojEl = b.brojEl;
    return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t){
    Cvor *pronadjen=pronadjicvor(korijen, t);
    if(pronadjen)
        return pronadjen->info;
    Cvor *novi=new Cvor(0, 0, t, TipVrijednosti());
    brojEl++;
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
       // novi->roditelj=r;
        if(t<r->kljuc)
            r->lijevi=novi;
        else 
            r->desni=novi;
    }
    korijen=balansiraj(korijen, t, novi->info);
    return novi->info;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](const TipKljuca &t) const{
    Cvor *pronadjen=pronadjicvor(korijen, t);
    if(pronadjen)
        return pronadjen->info;
    return TipVrijednosti();
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &t){
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
void AVLStabloMapa<TipKljuca, TipVrijednosti>::obrisi(){
    obrisicvor(korijen);
    korijen=nullptr;
    brojEl=0;
}

int main() {
}