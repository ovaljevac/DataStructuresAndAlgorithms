#include <iostream>
#include <new>

using std::cout;

template <typename TipEl>
class Lista{
    public:
    virtual ~Lista() {}
    Lista() {};
    virtual int brojElemenata() const=0;
    virtual TipEl trenutni() const=0;
    virtual TipEl &trenutni()=0;
    virtual bool prethodni() =0;
    virtual bool sljedeci() =0;
    virtual void pocetak() =0;
    virtual void kraj() =0;
    virtual void obrisi() =0;
    virtual TipEl dajMaks() const =0;
    virtual void dodajIspred(const TipEl &el)=0;
    virtual void dodajIza(const TipEl &el)=0;
    virtual TipEl &operator[](int d)=0;
    virtual TipEl const &operator[](int d) const=0;
    template <typename TipEl2>
    friend TipEl2 dajMaksimum(const Lista<TipEl2> &l);
};

template <typename TipEl>
class DvostrukaLista : public Lista<TipEl>{
    struct Cvor{
        TipEl el;
        Cvor *sljedeci;
        Cvor *prethodni;
        Cvor (const TipEl &el, Cvor *sljedeci, Cvor *prethodni): el(el), sljedeci(sljedeci), prethodni(prethodni){}
    };
    Cvor *pocetakp, *krajp, *trenutnip;
    int brojEl;
    TipEl maksel;
    public:
    DvostrukaLista() : pocetakp(0), krajp(0), trenutnip(0), brojEl(0) {}
    DvostrukaLista(const DvostrukaLista &l);
    DvostrukaLista &operator=(const DvostrukaLista &l);
    ~DvostrukaLista();
    TipEl dajMaks() const override{
        return maksel;
    }
    int brojElemenata() const override{
        return brojEl;
    };
    TipEl trenutni() const override{
        return trenutnip->el;
    };
    TipEl &trenutni() override{
        return trenutnip->el;
    };
    bool prethodni() override{
        if(!trenutnip || trenutnip==pocetakp)
            return false;
        trenutnip=trenutnip->prethodni;
        return true;
    };
    bool sljedeci() override{
        if(!trenutnip || trenutnip==krajp)
            return false;
        trenutnip=trenutnip->sljedeci;
        return true;
    };
    void pocetak() override {
        trenutnip=pocetakp;
    }
    void kraj() override {
        trenutnip=krajp;
    }
    void obrisi() override;
    void dodajIspred(const TipEl &el) override;
    void dodajIza(const TipEl &el) override;
    TipEl &operator[](int d) override;
    TipEl const &operator[](int d) const override;
    template<typename TipEl2>
    friend TipEl2 dajMaksimum(const Lista<TipEl2> &l);
};

template <typename TipEl2>
TipEl2 dajMaksimum(const Lista<TipEl2> &l){
    return l.dajMaks();
}

template <typename TipEl>
DvostrukaLista<TipEl>::DvostrukaLista(const DvostrukaLista &l){
    if(l.brojEl!=0){brojEl=l.brojEl;
    Cvor *pomoc=l.pocetakp;
    pocetakp=new Cvor(l.pocetakp->el, 0, 0);
    trenutnip=pocetakp;
    int br=0;
    while(pomoc->sljedeci){
        trenutnip->sljedeci=new Cvor(pomoc->sljedeci->el, 0, trenutnip);
        if(br==0){
            pocetakp->sljedeci=trenutnip->sljedeci;
            trenutnip->sljedeci->prethodni=pocetakp;
            br++;
        }
        pomoc=pomoc->sljedeci;
        trenutnip=trenutnip->sljedeci;
    }
    krajp=trenutnip;
    trenutnip=pocetakp;
    }
}

template <typename TipEl>
DvostrukaLista<TipEl> &DvostrukaLista<TipEl>::operator=(const DvostrukaLista &l){
    if(this==&l) return *this;
    while(brojEl) obrisi();
    if(l.brojEl!=0){brojEl=l.brojEl;
    Cvor *pomoc=l.pocetakp;
    pocetakp=new Cvor(l.pocetakp->el, 0, 0);
    trenutnip=pocetakp;
    int br=0;
    while(pomoc->sljedeci){
        trenutnip->sljedeci=new Cvor(pomoc->sljedeci->el, 0, trenutnip);
        if(br==0){
            pocetakp->sljedeci=trenutnip->sljedeci;
            trenutnip->sljedeci->prethodni=pocetakp;
            br++;
        }
        pomoc=pomoc->sljedeci;
        trenutnip=trenutnip->sljedeci;
    }
    krajp=trenutnip;
    trenutnip=pocetakp;
    }
    return *this;
}

template <typename TipEl>
DvostrukaLista<TipEl>::~DvostrukaLista(){
    trenutnip=pocetakp;
    while(brojEl!=0) 
        obrisi();
}

template <typename TipEl>
void DvostrukaLista<TipEl>::obrisi(){
    if(brojEl==0) return;
    if(trenutnip==pocetakp){
        if(pocetakp==krajp){
            delete trenutnip;
            trenutnip=krajp=pocetakp=nullptr;
            brojEl--;
            return;
        }
        Cvor *p=pocetakp->sljedeci;
        delete pocetakp;
        trenutnip=pocetakp=p;
        pocetakp->prethodni=nullptr;
    }
    else if(trenutnip==krajp){
        Cvor *p=krajp->prethodni;
        delete krajp;
        trenutnip=krajp=p;
        krajp->sljedeci=nullptr;
    }
    else{
        Cvor *p=trenutnip->sljedeci;
        trenutnip->prethodni->sljedeci=trenutnip->sljedeci;
        trenutnip->sljedeci->prethodni=trenutnip->prethodni;
        delete trenutnip;
        trenutnip=p;
    }
    brojEl--;
}

template <typename TipEl>
void DvostrukaLista<TipEl>::dodajIspred(const TipEl &el){
    Cvor *p=new Cvor(el, 0, 0);
    if(brojEl==0){
        pocetakp=trenutnip=krajp=p;
        brojEl++;
        maksel=p->el;
        return;
    }
    if(trenutnip==pocetakp){
        pocetakp=p;
        pocetakp->sljedeci=trenutnip;
        trenutnip->prethodni=p;
        brojEl++;
        if(p->el>maksel) maksel=p->el;
        return;
    }
    if(brojEl==2){
        pocetakp->sljedeci=trenutnip->prethodni=p;
        p->sljedeci=trenutnip;
        p->prethodni=pocetakp;
        brojEl++;
        if(p->el>maksel) maksel=p->el;
        return;
    }
    Cvor *pomoc=trenutnip->prethodni;
    pomoc->sljedeci=p;
    p->sljedeci=trenutnip;
    p->prethodni=pomoc;
    trenutnip->prethodni=p;
    if(p->el>maksel) maksel=p->el;
    brojEl++;
}

template<typename TipEl>
void DvostrukaLista<TipEl>::dodajIza(const TipEl &el){
    Cvor *p=new Cvor(el, 0, 0);
    if(brojEl==0){
        pocetakp=trenutnip=krajp=p;
        brojEl++;
        maksel=p->el;
        return;
    }
    if(trenutnip==krajp){
        trenutnip->sljedeci=p;
        if (brojEl==1){
            pocetakp->sljedeci=p;
            p->prethodni=pocetakp;
        }
        p->prethodni=trenutnip;
        krajp=p;
    }
    else{
        Cvor *pomoc=trenutnip->sljedeci;
        trenutnip->sljedeci=p;
        p->sljedeci=pomoc;
        pomoc->prethodni=p;
        p->prethodni=trenutnip;
    }
    if(p->el>maksel) maksel=p->el;
    brojEl++;
}

template<typename TipEl>
TipEl &DvostrukaLista<TipEl>::operator[](int d){
    Cvor *pomoc=pocetakp;
    for(int i=0; i<d; i++) pomoc=pomoc->sljedeci;
    return pomoc->el;
}

template<typename TipEl>
const TipEl &DvostrukaLista<TipEl>::operator[](int d) const {
    Cvor *pomoc=pocetakp;
    for(int i=0; i<d; i++) pomoc=pomoc->sljedeci;
    return pomoc->el;
}


int main(){
}