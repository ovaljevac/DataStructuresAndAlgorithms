#include <iostream>


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
    virtual void dodajIspred(const TipEl &el)=0;
    virtual void dodajIza(const TipEl &el)=0;
    virtual TipEl &operator[](int d)=0;
    virtual TipEl const &operator[](int d) const=0;
};

template <typename TipEl>
class NizLista : public Lista<TipEl>{
    int brojEl;
    int trenutniEl;
    int makskapacitet;
    TipEl **p=nullptr;
    void povecajkapacitet(){
        TipEl **novi=new TipEl*[makskapacitet*2]{};
        for(int i=0; i<brojEl; i++){
            novi[i]=p[i];
        }
        delete[] p;
        p=novi;
        makskapacitet*=2;
    };
    public:
    NizLista();
    NizLista(int brojEl);
    NizLista(const NizLista &n);
    NizLista<TipEl> &operator=(const NizLista<TipEl> &l);
    ~NizLista(){
        if(!p) return;
        for(int i=0; i<makskapacitet; i++)
            delete p[i];
        delete[] p;
    }
    int brojElemenata() const override {return brojEl;}
    TipEl trenutni() const override{return *p[trenutniEl];}
    TipEl &trenutni() override{return *p[trenutniEl];}
    bool prethodni() override {
        if(trenutniEl==0) return false;
        trenutniEl--;
        return true;
    };
    bool sljedeci() override{
        trenutniEl++;
        if(trenutniEl>=brojEl) {trenutniEl--; return false;};
       // trenutniEl++;
        return true;
    };
    void pocetak() override {trenutniEl=0;};
    void kraj() override{trenutniEl=brojEl-1;};
    void dodajIspred(const TipEl &el) override;
    void dodajIza(const TipEl &el) override;
    void obrisi() override;
    TipEl &operator[](int d) override;
    TipEl const &operator[](int d) const override;
};

template <typename TipEl>
NizLista<TipEl>::NizLista(const NizLista<TipEl> &n){
    brojEl=n.brojEl; trenutniEl=n.trenutniEl;
    makskapacitet=n.makskapacitet;
    p=new TipEl*[makskapacitet]{};
    for(int i=0; i<brojEl; i++){
        p[i]=new TipEl(*n.p[i]);
    }
}

template <typename TipEl>
NizLista<TipEl> &NizLista<TipEl>::operator=(const NizLista<TipEl> &l){
    if(&l==this) return *this;
    if(makskapacitet>=l.brojEl && brojEl!=0){
        for(int i=0; i<l.brojEl; i++)
            *p[i]=*l.p[i];
        brojEl=l.brojEl; trenutniEl=l.trenutniEl;
    }
    else{
        for(int i=0; i<brojEl; i++)
            delete p[i];
        delete[] p;
        brojEl=l.brojEl; trenutniEl=l.trenutniEl;
        makskapacitet=l.makskapacitet;
        p=new TipEl*[makskapacitet]{};
        for(int i=0; i<brojEl; i++){
            p[i]=new TipEl(*l.p[i]);
        }
    }
    return *this;   
}

template <typename TipEl>
NizLista<TipEl>::NizLista(){
    brojEl=0;
    trenutniEl=-1;
    makskapacitet=50;
    p=new TipEl*[makskapacitet]{};
}

template <typename TipEl>
NizLista<TipEl>::NizLista(int brojEl) : brojEl(brojEl){
    trenutniEl=0;
    makskapacitet=2*brojEl;
    p=new TipEl*[makskapacitet]{};
}

template <typename TipEl>
void NizLista<TipEl>::dodajIspred(const TipEl &el){
    if(trenutniEl==-1){
        p[0]=new TipEl(el);
        trenutniEl=0;
        brojEl++;
        return;
    }
    if(brojEl+1==makskapacitet){
        povecajkapacitet();   
    }
    for(int i=brojEl; i>trenutniEl-1; i--){
       if(i-1>=0) p[i]=p[i-1];
    }
    p[trenutniEl]=new TipEl(el);
    trenutniEl++;
    brojEl++;
}

template <typename TipEl>
void NizLista<TipEl>::dodajIza(const TipEl &el){
    if(trenutniEl==-1){
        p[0]=new TipEl(el);
        trenutniEl=0;
        brojEl++;
        return;
    }
    if(brojEl+1==makskapacitet){
        povecajkapacitet();   
    }
    for(int i=brojEl; i>trenutniEl; i--){
        if(i-1>=0) p[i]=p[i-1];
    }
    p[trenutniEl+1]=new TipEl(el);
    //trenutniEl++;
    brojEl++;
}

template <typename TipEl>
void NizLista<TipEl>::obrisi(){
    if(brojEl==0) throw std::domain_error("praznoui");
    delete p[trenutniEl];
    p[trenutniEl]=nullptr;
    for(int i=trenutniEl; i<brojEl-1; i++){
        p[i]=p[i+1];
    }
    p[brojEl-1]=nullptr;
    brojEl--;
    if(trenutniEl>=brojEl) trenutniEl=brojEl-1;
}

template <typename TipEl>
TipEl &NizLista<TipEl>::operator[](int d){
    if(d>=brojEl || d<0) throw std::range_error("Izvan opsega!");
    return *p[d];
}

template<typename TipEl>
TipEl const &NizLista<TipEl>::operator[](int d) const {
    if(d>=brojEl || d<0) throw std::range_error("Izvan opsega!");
    return *p[d];
}

template <typename TipEl>
class JednostrukaLista : public Lista<TipEl>{
    struct Cvor{
        TipEl el;
        Cvor *sljedeci;
        Cvor (const TipEl &el, Cvor *sljedeci): el(el), sljedeci(sljedeci){}
    };
    Cvor *trenutnip;
    Cvor *pocetakp;
    Cvor *krajp;
    int brojEl;
    void postavikraj();
    public:
    JednostrukaLista() : pocetakp(0), krajp(0), trenutnip(0), brojEl(0) {}
    ~JednostrukaLista();
    JednostrukaLista(const JednostrukaLista &l);
    JednostrukaLista &operator=(const JednostrukaLista &l);
    bool Prazna(){if(brojEl==0) return true;
    return false;}
    int brojElemenata() const override;
    TipEl trenutni() const override;
    TipEl &trenutni() override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override {trenutnip=pocetakp;};
    void kraj() override {trenutnip=krajp;};
    void obrisi() override;
    void dodajIspred(const TipEl &el) override;
    void dodajIza(const TipEl &el) override;
    TipEl &operator[](int d) override;
    TipEl const &operator[](int d) const override;
};

template <typename TipEl>
JednostrukaLista<TipEl>::JednostrukaLista(const JednostrukaLista<TipEl> &l){
    if(l.brojEl!=0){brojEl=l.brojEl;
    Cvor *pomoc=l.pocetakp;
    pocetakp=new Cvor(l.pocetakp->el, nullptr);
    trenutnip=pocetakp;
    int br=0;
    while(pomoc->sljedeci){
        trenutnip->sljedeci=new Cvor(pomoc->sljedeci->el, nullptr);
        if(br==0){
            pocetakp->sljedeci=trenutnip->sljedeci;
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
JednostrukaLista<TipEl> &JednostrukaLista<TipEl>::operator=(const JednostrukaLista &l){
    if(this==&l) return *this;
    while(!Prazna()) obrisi();
    brojEl=l.brojEl;
    Cvor *pomoc=l.pocetakp;
    pocetakp=new Cvor(l.pocetakp->el, nullptr);
    trenutnip=pocetakp;
    int br=0;
    while(pomoc->sljedeci){
        trenutnip->sljedeci=new Cvor(pomoc->sljedeci->el, nullptr);
        if(br==0){
            pocetakp->sljedeci=trenutnip->sljedeci;
            br++;
        }
        pomoc=pomoc->sljedeci;
        trenutnip=trenutnip->sljedeci;
    }
    krajp=trenutnip;
    trenutnip=pocetakp;
    return *this;
};

template <typename TipEl>
JednostrukaLista<TipEl>::~JednostrukaLista(){
    trenutnip=pocetakp;
    while(!Prazna()) obrisi();
}

template <typename TipEl>
int JednostrukaLista<TipEl>::brojElemenata() const{
    return brojEl;
}

template <typename TipEl>
TipEl JednostrukaLista<TipEl>::trenutni() const{
    return trenutnip->el;
}

template <typename TipEl>
TipEl &JednostrukaLista<TipEl>::trenutni(){
    return trenutnip->el;
}


template <typename TipEl>
bool JednostrukaLista<TipEl>::prethodni(){
    if(!trenutnip) return false;
    Cvor *pomoc=pocetakp;
    while(pomoc->sljedeci!=trenutnip){
        pomoc=pomoc->sljedeci;
    }
    trenutnip=pomoc;
    return true;
}

template <typename TipEl> 
bool JednostrukaLista<TipEl>::sljedeci(){
    if(!trenutnip || trenutnip==krajp) return false;
    trenutnip=trenutnip->sljedeci;
    return true;
}

template <typename TipEl>
void JednostrukaLista<TipEl>::obrisi(){
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
    }
    else if(trenutnip==krajp){
        Cvor *p=pocetakp;
        while(p->sljedeci!=krajp){
            p=p->sljedeci;
        }
        delete krajp;
        trenutnip=krajp=p;
    }
    else{
        Cvor *p=pocetakp;
        while(p->sljedeci!=trenutnip){
            p=p->sljedeci;
        }
        p->sljedeci=trenutnip->sljedeci;
        delete trenutnip;
        trenutnip=p->sljedeci;
    }
    brojEl--;
}

template<typename TipEl>
void JednostrukaLista<TipEl>::dodajIspred(const TipEl &el){
    Cvor *p=new Cvor(el, nullptr);
    if(Prazna()){
        pocetakp=krajp=trenutnip=p;
        trenutnip->sljedeci=nullptr;
        brojEl++;
        return;
    }
    if(brojEl==1){
        pocetakp=p;
        pocetakp->sljedeci=trenutnip;
        p->sljedeci=trenutnip;
        brojEl++;
        return;
    }
    if(brojEl==2){
        pocetakp->sljedeci=p;
        p->sljedeci=trenutnip;
        brojEl++;
        return;
    }
    Cvor *pomoc=pocetakp;
    while(pomoc->sljedeci!=trenutnip){
        pomoc=pomoc->sljedeci;
    }
    pomoc->sljedeci=p;
    p->sljedeci=trenutnip;
    brojEl++;
}

template <typename TipEl>
void JednostrukaLista<TipEl>::postavikraj(){
    Cvor *pomoc=pocetakp;
    while(!pomoc->sljedeci) pomoc=pomoc->sljedeci;
    krajp=pomoc;
}

template<typename TipEl>
void JednostrukaLista<TipEl>::dodajIza(const TipEl &el){
    Cvor *p=new Cvor(el, 0);
    if(Prazna()){
        pocetakp=krajp=trenutnip=p;
        trenutnip->sljedeci=nullptr;
        brojEl++;
        return;
    }
    if(trenutnip==krajp){
        trenutnip->sljedeci=p;
        if(brojEl==1) pocetakp->sljedeci=p;
        krajp=p;
    }
    else{
        Cvor *pomoc=trenutnip->sljedeci;
        trenutnip->sljedeci=p;
        p->sljedeci=pomoc;
    }
    brojEl++;
}

template<typename TipEl>
TipEl &JednostrukaLista<TipEl>::operator[](int d){
    Cvor *pomoc=pocetakp;
    for(int i=0; i<d; i++) pomoc=pomoc->sljedeci;
    return pomoc->el;
}

template<typename TipEl>
const TipEl &JednostrukaLista<TipEl>::operator[](int d) const {
    Cvor *pomoc=pocetakp;
    for(int i=0; i<d; i++) pomoc=pomoc->sljedeci;
    return pomoc->el;
}

int main(){
}
