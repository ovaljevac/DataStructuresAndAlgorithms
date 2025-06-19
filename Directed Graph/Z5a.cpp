#include <iostream>
#include <vector>
#include <queue>

template <typename TipOznake>
class Grana;
template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class GranaIterator;
template <typename TipOznake>
class UsmjereniGraf;

using std::cout;



template <typename TipOznake>
class Grana {
        std::pair<Cvor<TipOznake>*, Cvor<TipOznake>*> cvorovi;
        float tezina;
        TipOznake oznaka;
    public:
        friend class GranaIterator<TipOznake>;
        Grana(Cvor<TipOznake>* polazniCvor, Cvor<TipOznake>* dolazniCvor, float tezina = 0);
        void postaviTezinu(float tezina = 0) {
            UsmjereniGraf<TipOznake>* graf = cvorovi.first->dajGraf();
            graf->dajTuGranu(cvorovi.first->dajRedniBroj(), cvorovi.second->dajRedniBroj())->tezina = tezina;
        };
        void postaviOznaku(TipOznake oznaka) {
            UsmjereniGraf<TipOznake>* graf = cvorovi.first->dajGraf();
            graf->dajTuGranu(cvorovi.first->dajRedniBroj(), cvorovi.second->dajRedniBroj())->oznaka = oznaka;
        };
        Cvor<TipOznake> dajPolazniCvor() { return *cvorovi.first; }
        Cvor<TipOznake> dajDolazniCvor() { return *cvorovi.second; }
        float dajTezinu() { return tezina; };
        TipOznake dajOznaku() { return oznaka; };
        bool operator==(const Grana &grana) const;
};

template <typename TipOznake>
bool Grana<TipOznake>::operator==(const Grana &grana) const {
    if(cvorovi.first->dajRedniBroj() == grana.first->dajRedniBroj()
    &&cvorovi.second->dajRedniBroj() == grana.second->dajRedniBroj())
        return true;
    return false;
}

template <typename TipOznake>
Grana<TipOznake>::Grana(Cvor<TipOznake>* polazniCvor, Cvor<TipOznake>* dolazniCvor, float tezina){
    cvorovi.first=polazniCvor;
        cvorovi.second=dolazniCvor;
        this->tezina = tezina;
        oznaka = TipOznake();
}
        

template <typename TipOznake>
class Cvor {
        int redniBrojCvora;
        UsmjereniGraf<TipOznake> *graf;
        TipOznake oznaka;
    public:
        Cvor(UsmjereniGraf<TipOznake> *ug, int redniBrojCvora);
        TipOznake dajOznaku() { return oznaka; }
        void postaviOznaku(TipOznake oznaka) { graf->dajTajCvor(redniBrojCvora)->oznaka = oznaka;};
        int dajRedniBroj() { return redniBrojCvora; };
        UsmjereniGraf<TipOznake>* dajGraf() { return graf; }
};

template <typename TipOznake>
Cvor<TipOznake>::Cvor(UsmjereniGraf<TipOznake> *ug, int redniBrojCvora){
    this->redniBrojCvora = redniBrojCvora;
    graf = ug;
    oznaka = TipOznake();
}




template <typename TipOznake> class GranaIterator {
    Grana<TipOznake> *trenutna;
    UsmjereniGraf<TipOznake> *graf;
public:
    Grana<TipOznake>* dajTrenutnu(){
        return trenutna;
    }
    
    GranaIterator(Grana<TipOznake> *t){
        trenutna = t;
        graf = t->dajPolazniCvor().dajGraf();
    }
    Grana<TipOznake> operator*(){
        return *trenutna;
    }
    bool operator==(const GranaIterator &iter) const {
        if (this->trenutna->dajPolazniCvor().dajRedniBroj() == iter.trenutna->dajPolazniCvor().dajRedniBroj()
        && this->trenutna->dajDolazniCvor().dajRedniBroj() == iter.trenutna->dajDolazniCvor().dajRedniBroj()
        && this->trenutna->dajTezinu() == iter.trenutna->dajTezinu())
            return true;
        return false;    
    }
    
    bool operator!=(const GranaIterator &iter) const {
        if( !(this->trenutna->dajPolazniCvor().dajRedniBroj() == iter.trenutna->dajPolazniCvor().dajRedniBroj() 
        && this->trenutna->dajDolazniCvor().dajRedniBroj() == iter.trenutna->dajDolazniCvor().dajRedniBroj()
        && this->trenutna->dajTezinu() == iter.trenutna->dajTezinu()))
            return true;
        return false;
    }
    
GranaIterator& operator++(){
    GranaIterator<TipOznake> povratni(trenutna);
    int redniPocetnog = trenutna->dajPolazniCvor().dajRedniBroj();
    int redniKrajnjeg = trenutna->dajDolazniCvor().dajRedniBroj();
    std::vector<Grana<TipOznake>*> grancice =  graf->dajGrane();
    std::vector <std::vector <bool>> matrica = graf-> dajMatricu();
    bool dosoDoKraja=false;
    redniKrajnjeg++;
    if(redniPocetnog == matrica[0].size() && redniKrajnjeg == matrica[0].size())
        throw ("DOSO DO KRAJA");
    if(redniKrajnjeg == matrica[0].size()){
        redniKrajnjeg=0;
        redniPocetnog++;
    }
    int polazni=-1;
    int dolazni=-1;
    int j=redniKrajnjeg;
    bool nadjen = false;
    for(int i=redniPocetnog; i<matrica[0].size(); i++){
        for(j; j<matrica[0].size(); j++){
            if(matrica[i][j]==true){
                polazni=i;
                dolazni=j;
                nadjen = true;
                break;
            }
            
        }
        if(nadjen)
            break;
        j=0;
    }

    if(polazni==-1){
        trenutna=nullptr;
        return *this;
    }
    
    if(polazni==-1)
        throw ("NEMA DALJE");
    
    if(dosoDoKraja)
        throw("Naiso do kraja");
    for(int i=0; i<grancice.size(); i++){
        if(grancice[i]->dajPolazniCvor().dajRedniBroj() == polazni &&
        grancice[i]->dajDolazniCvor().dajRedniBroj() == dolazni){
            trenutna = grancice[i];
            break;
        }
    }
    
    return povratni;    
}

GranaIterator operator++(int){
    int redniPocetnog = trenutna->dajPolazniCvor().dajRedniBroj();
    int redniKrajnjeg = trenutna->dajDolazniCvor().dajRedniBroj();
    redniPocetnog++;
    redniKrajnjeg++;
    if(graf->postojiGrana(redniPocetnog, redniKrajnjeg))
        trenutna = graf->dajTuGranu(redniPocetnog, redniKrajnjeg);
    return *this;
}
};

template<typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* graf, std::vector<Cvor<TipOznake>> &obilazni, Cvor<TipOznake> trenutni, std::vector<bool> vektor={} ){
    bool imaLiGa=false;
    if(vektor.size()==0)
        vektor.resize(graf->dajBrojCvorova(),false);
    for(int i=0; i<obilazni.size(); i++){
    if(obilazni[i].dajRedniBroj()==trenutni.dajRedniBroj())
        imaLiGa=true;
    }
    if(!imaLiGa){
        obilazni.push_back(trenutni);
        vektor[trenutni.dajRedniBroj()]=true;
    }
    if(obilazni.size()==graf->dajBrojCvorova())
        return;
    int novi = -1;
    bool nijeNadjena=false;
    for(int i=trenutni.dajRedniBroj(); i<graf->dajBrojCvorova(); i++){
        if(graf->postojiGrana(trenutni.dajRedniBroj(), i)){
            bool dodan = vektor[i];

            if(!dodan){
                novi=i;
                break;
            }
        }
        if(i==graf->dajBrojCvorova()-1)
            nijeNadjena=true;
        
    }
    if(nijeNadjena && obilazni.size()!=graf->dajBrojCvorova())
        dfs(graf, obilazni, graf->dajCvor(trenutni.dajRedniBroj()-1), vektor);
        
    if(novi!=-1)
    dfs(graf, obilazni, graf->dajCvor(novi), vektor);
}

 template<typename TipOznake>
 void bfs(UsmjereniGraf<TipOznake>* graf, std::vector<Cvor<TipOznake>> &obilazni, Cvor<TipOznake> trenutni ){
    std::vector<bool> vektor (graf->dajBrojCvorova(), false);
    std::queue<Cvor<TipOznake>> red;
    vektor[trenutni.dajRedniBroj()]=true;
    red.push(trenutni);
    while(!red.empty()){
        Cvor<TipOznake> izvadjeni = red.front();
        red.pop();
        obilazni.push_back(izvadjeni);
        for(int i=0; i<graf->dajBrojCvorova(); i++){
            if(graf->postojiGrana(izvadjeni.dajRedniBroj(), i)){
                if(vektor[i]==false){
                    vektor[i]=true;
                    red.push(graf->dajCvor(i));
                }
            }
        }
    }
        
 }


template <typename TipOznake>
class UsmjereniGraf {
    public:
        UsmjereniGraf(int brojCvorova) {} 
        virtual ~UsmjereniGraf() {} ;
        virtual int dajBrojCvorova() const = 0;
        virtual void postaviBrojCvorova(int noviBroj) = 0;
        virtual void dodajGranu(int polazniCvor, int dolazniCvor, float tezina = 0) = 0;
        virtual void obrisiGranu(int polazniCvor, int dolazniCvor) = 0;
        virtual void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina = 0) = 0;
        virtual float dajTezinuGrane(int polazniCvor, int dolazniCvor) = 0;
        virtual bool postojiGrana(int polazniCvor, int dolazniCvor) = 0;
        virtual void postaviOznakuCvora(int cvor, TipOznake oznaka) = 0;
        virtual TipOznake dajOznakuCvora(int cvor) = 0;
        virtual void postaviOznakuGrane(int polazniCvor, int dolazniCvor, TipOznake oznaka) = 0;
        virtual TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) = 0;
        virtual Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor) = 0;
        virtual  Cvor<TipOznake> *dajTajCvor(int r_broj) = 0;
        virtual Grana<TipOznake> *dajTuGranu(int polazniCvor, int dolazniCvor) = 0;
        virtual Cvor<TipOznake> dajCvor(int redniBroj) = 0;
        virtual GranaIterator<TipOznake> dajGranePocetak() = 0; 
        virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
        virtual std::vector<Grana<TipOznake>*> dajGrane() = 0;
        virtual std::vector<std::vector<bool>> dajMatricu() = 0; 
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
        std::vector<std::vector <bool>> matrica;
        std::vector<Grana<TipOznake>*> grane;
        std::vector<Cvor<TipOznake>*> cvorovi;
    public:
        std::vector<Grana<TipOznake>*> dajGrane(){ return grane; }
        std::vector <std::vector <bool>> dajMatricu() { return matrica; }
        MatricaGraf(int brojCvora) : 
        UsmjereniGraf<TipOznake>(brojCvora){
      matrica.resize(brojCvora);
      for(int i=0; i<brojCvora; i++)
        matrica[i].resize(brojCvora, false);
      for(int i=0; i<brojCvora; i++){
          Cvor<TipOznake> *cvor = new Cvor<TipOznake>(this, i);
          cvorovi.push_back(cvor);
      }
    }
    
    ~MatricaGraf(){
        int brojCvorova = cvorovi.size();
        for(int i=0; i<brojCvorova; i++)
            delete cvorovi[i];
        int brojGrana = grane.size();
        for(int i=0; i<brojGrana; i++)
            delete grane[i];
    }
    
    int dajBrojCvorova() const{
        return cvorovi.size();
    }
    
    void postaviBrojCvorova(int brojCvorova){
        cvorovi.resize(brojCvorova);
    }
    
    void dodajGranu(int prvi, int drugi, float tezina = 0) {
        matrica[prvi][drugi] = true;
        Grana<TipOznake>* za_ubaciti = new Grana<TipOznake>(cvorovi[prvi],cvorovi[drugi],tezina);
        grane.push_back(za_ubaciti);
    }
    
    bool postojiGrana(int polazni, int dolazni){
        if(matrica[polazni][dolazni]==true)
            return true;
        return false;    
    }
    
     void obrisiGranu(int polazni, int dolazni) {
         matrica[polazni][dolazni] = false;
     }
     
     void postaviTezinuGrane(int polazni, int dolazni, float tezina=0){
        for(int i=0; i<grane.size(); i++){
            if(grane[i]->dajPolazniCvor().dajRedniBroj() == polazni &&
            grane[i]->dajDolazniCvor().dajRedniBroj() == dolazni){
                    grane[i]->postaviTezinu(tezina);
                    break;}
        }
     }
     
     float dajTezinuGrane(int polazni, int dolazni){
         for(int i=0; i<grane.size(); i++){
             if(grane[i]->dajPolazniCvor().dajRedniBroj()==polazni &&
                grane[i]->dajDolazniCvor().dajRedniBroj()==dolazni)
                    return grane[i]->dajTezinu();
         }
         return 0;
     }
     
     void postaviOznakuCvora(int brojCvora, TipOznake oznaka){
         cvorovi[brojCvora]->postaviOznaku(oznaka);
     }
     TipOznake dajOznakuCvora(int cvor){
         return cvorovi[cvor]->dajOznaku();
     }
     void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) {
         for(int i=0; i<grane.size(); i++){
            if(grane[i]->dajPolazniCvor().dajRedniBroj() == polazni &&
            grane[i]->dajDolazniCvor().dajRedniBroj() == dolazni){
                    grane[i]->postaviOznaku(oznaka);
                    break;}
        }
     }
     Cvor<TipOznake>* dajTajCvor(int r_broj) {
         return cvorovi[r_broj];
     }
     TipOznake dajOznakuGrane(int polazni, int dolazni){
         for(int i=0; i<grane.size(); i++){
             if(grane[i]->dajPolazniCvor().dajRedniBroj()==polazni &&
             grane[i]->dajDolazniCvor().dajRedniBroj()==dolazni)
             return grane[i]->dajOznaku();
         }
         return 0;
     }
     Cvor<TipOznake> dajCvor(int redniBroj) {
         return *cvorovi[redniBroj];
     }
     Grana<TipOznake> dajGranu(int polazni, int dolazni) {
         for(int i=0; i<grane.size(); i++){
             if(grane[i]->dajPolazniCvor().dajRedniBroj() == polazni &&
             grane[i]->dajDolazniCvor().dajRedniBroj() == dolazni){
                 return *grane[i];
             }
         }
         return Grana<TipOznake>(nullptr, 0);
     }
     Grana <TipOznake>* dajTuGranu(int prvi, int drugi) {
        for(int i=0;i<grane.size();i++) {
            if(grane[i]->dajPolazniCvor().dajRedniBroj() == prvi && grane[i]->dajDolazniCvor().dajRedniBroj() == drugi) {
                return grane[i];
            }
        }
        return 0;
    }
    GranaIterator<TipOznake> dajGranePocetak()  {
        int polazni = 0;
        int dolazni=0;
        bool nadjen =false;
        for(int i = 0; i < matrica.size(); i++) {
            for(int j = 0; j < matrica.size(); j++) {
                if(matrica[i][j] ==true){
                    polazni=i;
                    dolazni=j;
                    nadjen=true;
                    break;
                } 
            }
            if(nadjen)
                break;
        }
        for(int i=0; i<grane.size(); i++){
            if(grane[i]->dajPolazniCvor().dajRedniBroj()==polazni &&
            grane[i]->dajDolazniCvor().dajRedniBroj()==dolazni)
                return GranaIterator<TipOznake>(grane[i]);
        }
        return 0;
    }
    virtual GranaIterator<TipOznake> dajGraneKraj() {
        this->dodajGranu(matrica[0].size()-1,matrica[0].size()-1,0);
        return grane[grane.size()-1];

    }
    

    
};


int main(){
}