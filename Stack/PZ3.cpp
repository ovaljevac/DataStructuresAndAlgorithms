#include <iostream>

using std::cout;

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

int main(){
}