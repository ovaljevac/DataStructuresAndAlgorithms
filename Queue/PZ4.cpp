#include <iostream>

using std::cout;

template <typename TipEl>
class Red{
    struct Cvor{
        TipEl el;
        Cvor *sljedeci;
        Cvor(const TipEl &el, Cvor *sljedeci) : el(el), sljedeci(sljedeci){}
    };
    Cvor *pocetak;
    Cvor *kraj;
    int brojEl;
    public:
    Red() : pocetak(0), kraj(0), brojEl(0){}
    Red(const Red &r);
    Red &operator=(const Red &r);
    ~Red();
    void brisi();
    void stavi(const TipEl &el);
    TipEl skini();
    TipEl &celo();
    int brojElemenata() {return brojEl;}
};

template<typename TipEl>
Red<TipEl>::Red(const Red &r){
pocetak=kraj=nullptr;
brojEl=0;
    if(r.brojEl!=0){
        Cvor *p=r.pocetak;
        while(p){
            stavi(p->el);
            p=p->sljedeci;
        }
    }
}

template <typename TipEl>
Red<TipEl> &Red<TipEl>::operator=(const Red &r){
    if(this==&r)
        return *this;
    brisi();
    if(r.brojEl!=0){
        Cvor *p=r.pocetak;
        while(p){
            stavi(p->el);
            p=p->sljedeci;
        }
    }
    return *this;
}

template <typename TipEl>
Red<TipEl>::~Red(){
    brisi();
}

template <typename TipEl>
void Red<TipEl>::brisi(){
    while(pocetak){
        Cvor *p=pocetak;
        pocetak=pocetak->sljedeci;
        delete p;
    }
    kraj=nullptr;
    pocetak=nullptr;
    brojEl=0;
}

template <typename TipEl>
void Red<TipEl>::stavi(const TipEl &el){
    Cvor *p=new Cvor(el, nullptr);
    if(!kraj){
        pocetak=kraj=p;
    }
    else{
        kraj->sljedeci=p;
        kraj=kraj->sljedeci;
    }
    brojEl++;
}

template <typename TipEl>
TipEl Red<TipEl>::skini(){
    if(brojEl==0)
        throw std::domain_error("Red je prazan");
    TipEl el=pocetak->el;
    Cvor *p=pocetak->sljedeci;
    delete pocetak;
    pocetak=p;
    brojEl--;
    if(brojEl==0){
        kraj=nullptr;
    }
    return el;
}

template <typename TipEl>
TipEl &Red<TipEl>::celo(){
    if(brojEl==0)
        throw std::domain_error("Red je prazan");
    return pocetak->el;
}

int main(){
    
}