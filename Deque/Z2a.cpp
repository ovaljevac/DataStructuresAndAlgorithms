#include <iostream>
#include <vector>

using std::cout;
using std::vector;
template <typename TipEl>
class DvostraniRed{
    struct Cvor{
        TipEl el;
        Cvor *sljedeci;
        Cvor *prethodni;
        Cvor(const TipEl &el, Cvor *sljedeci, Cvor *prethodni) : el(el), sljedeci(sljedeci), prethodni(prethodni){}
    };
    Cvor *pocetak;
    Cvor *kraj;
    int brojEl;
    void Provjera(){
        if(brojEl==0)
            throw std::domain_error("Red je prazan!");
    }
    public:
    DvostraniRed() : pocetak(0), kraj(0), brojEl(0) {}
    ~DvostraniRed() { brisi(); }
    DvostraniRed(const DvostraniRed<TipEl> &d);
    DvostraniRed &operator=(const DvostraniRed<TipEl> &d);
    void staviNaVrh(const TipEl &el);
    TipEl skiniSaVrha();
    void staviNaCelo(const TipEl &el);
    TipEl skiniSaCela();
    TipEl &vrh();
    TipEl &celo();
    int brojElemenata() {return brojEl;}
    void brisi();
};

template <typename TipEl>
DvostraniRed<TipEl>::DvostraniRed(const DvostraniRed<TipEl> &d){
    pocetak=kraj=0;
    brojEl=0;
    if(d.brojEl!=0){
        Cvor *p=d.pocetak;
        while(p){
            staviNaVrh(p->el);
            p=p->sljedeci;
        }
    }
}

template <typename TipEl>
DvostraniRed<TipEl> &DvostraniRed<TipEl>::operator=(const DvostraniRed<TipEl> &d){
    if(this==&d)
        return *this;
    brisi();
    if(d.brojEl!=0){
        Cvor *p=d.pocetak;
        while(p){
            staviNaVrh(p->el);
            p=p->sljedeci;
        }
    }
    return *this;
}

template <typename TipEl>
void DvostraniRed<TipEl>::staviNaVrh(const TipEl &el){
    Cvor *p=new Cvor(el, 0, 0);
    if(!kraj)
        pocetak=kraj=p;
    else{
        Cvor *pomoc=kraj;
        kraj->sljedeci=p;
        p->prethodni=kraj;
        kraj=kraj->sljedeci;
        kraj->prethodni=pomoc;
        if(brojEl==0)
            pocetak=pomoc;
    }
    brojEl++;
}

template <typename TipEl>
TipEl DvostraniRed<TipEl>::skiniSaVrha(){
    Provjera();
    TipEl el=kraj->el;
    Cvor *p=kraj->prethodni;
    delete kraj;
    kraj=p;
    if(kraj)
        kraj->sljedeci=0;
    brojEl--;
    if(brojEl==0)
        pocetak=kraj=0;
    return el;
}

template <typename TipEl>
void DvostraniRed<TipEl>::staviNaCelo(const TipEl &el){
    Cvor *p=new Cvor(el, 0, 0);
    if(!pocetak)
        pocetak=kraj=p;
    else{
        Cvor *pomoc=pocetak;
        pocetak->prethodni=p;
        p->sljedeci=pocetak;
        pocetak=pocetak->prethodni;
        pocetak->sljedeci=pomoc;
        if(brojEl==0)
            kraj=pomoc;
    }
    brojEl++;
}

template <typename TipEl>
TipEl DvostraniRed<TipEl>::skiniSaCela(){
    Provjera();
    TipEl el=pocetak->el;
    Cvor *p=pocetak->sljedeci;
    delete pocetak;
    pocetak=p;
    if(pocetak)
        pocetak->prethodni=0;
    brojEl--;
    if(brojEl==0)
        pocetak=kraj=0;
    return el;
}

template <typename TipEl>
TipEl &DvostraniRed<TipEl>::vrh(){
    Provjera();
    return kraj->el;
}

template <typename TipEl>
TipEl &DvostraniRed<TipEl>::celo(){
    Provjera();
    return pocetak->el;
}

template <typename TipEl>
void DvostraniRed<TipEl>::brisi(){
    while(pocetak){
        Cvor *p=pocetak;
        pocetak=pocetak->sljedeci;
        delete p;
    }
    pocetak=kraj=0;
    brojEl=0;
}

template <typename TipEl> 
class Stek {
    int makskapacitet;
    int brojEl;
    TipEl **p;
    void povecajkapacitet() {
        makskapacitet*=2;
        TipEl **novi=new TipEl*[makskapacitet]{};
        for(int i=0; i<brojEl; i++) 
            novi[i]=p[i];
        for(int i=brojEl; i<makskapacitet; i++)
            novi[i]=new TipEl();
        delete[] p;
        p=novi;
    };
    public:
    ~Stek();
    Stek();
    Stek(const Stek &s);
    Stek &operator=(const Stek &s);
    void brisi();
    void stavi(const TipEl &el);
    TipEl skini();
    TipEl &vrh();
    int brojElemenata() const { return brojEl; }
};

template <typename TipEl> 
Stek<TipEl>::Stek() : makskapacitet(10), brojEl(0){
    p=new TipEl*[makskapacitet]{};
    for(int i=0; i<makskapacitet; i++)
    p[i]=new TipEl();
}

template <typename TipEl> 
Stek<TipEl>::Stek(const Stek &s) {
    brojEl=s.brojEl;
    makskapacitet=s.makskapacitet;
    p=new TipEl*[makskapacitet]{};
    for(int i=0; i<makskapacitet; i++)
        p[i]=new TipEl(*s.p[i]);
}

template <typename TipEl> 
Stek<TipEl>::~Stek(){
    if(!p) return;
    for(int i=0; i<makskapacitet; i++)
        delete p[i];
    delete[] p;
}

template <typename TipEl> 
Stek<TipEl> &Stek<TipEl>::operator=(const Stek &s) {
    if(this==&s)
        return *this;
    if(makskapacitet>=s.brojEl && brojEl!=0) {
        for (int i=0; i<s.brojEl; i++)
            *p[i]=*s.p[i];
        brojEl=s.brojEl;
    } 
    else{
        for(int i=0; i<makskapacitet; i++)
            delete p[i];
        delete[] p;
        brojEl=s.brojEl;
        makskapacitet=s.makskapacitet;
        p=new TipEl*[makskapacitet]{};
        for(int i=0; i<makskapacitet; i++)
            p[i]=new TipEl(*s.p[i]);
    }
    return *this;
}

template <typename TipEl> 
void Stek<TipEl>::brisi() {brojEl = 0;}

template <typename TipEl> 
void Stek<TipEl>::stavi(const TipEl &el){
    if(brojEl>=makskapacitet)
        povecajkapacitet();
    *p[brojEl++]=el;
}

template <typename TipEl>
TipEl Stek<TipEl>::skini() {
    if(brojEl==0)
        throw std::domain_error("Stek je prazan");
    return *p[--brojEl];
}

template <typename TipEl> 
TipEl &Stek<TipEl>::vrh() {
    if(brojEl==0)
        throw std::domain_error("Stek je prazan");
    return *p[brojEl-1];
}

void pretraga(Stek<std::vector<int>> &s, int trazeni){
    std::vector<int> v;
    Stek<std::vector<int>> kopija=s;
    int i=kopija.brojElemenata()-1;
    while(kopija.brojElemenata()!=0){
        if(kopija.vrh().size()==0){
        }
        else if(kopija.vrh().at(0)<=trazeni && kopija.vrh().at(kopija.vrh().size()-1)>=trazeni){
            v=kopija.vrh();  
            break;
        }
        kopija.skini();
        i--;
    }
    if(kopija.brojElemenata()==0){
        std::cout<<"Nema elementa";
        return;
    }
    int dno=0;
    int vrh=v.size()-1;
    int index=-1;
    while(vrh>=dno){
        int srednji=(dno+vrh)/2;
        if(v.at(srednji)==trazeni){
            index=srednji;
            break;
        }
        else if(v.at(srednji)>trazeni)
            vrh=srednji-1;
        else
            dno=srednji+1;
    }
    if(index==-1){
        std::cout<<"Nema elementa";
        return;
    }
    std::cout<<index<<" "<<i;
}


int main() {
}