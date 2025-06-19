#include <iostream>

using std::cout;


int fib2_0(int n, int pretprosli=0, int prosli=1, int br=2){
    if(n<=1) return n;
    if(br>n) return prosli;
    return fib2_0(n, prosli, pretprosli+prosli, ++br);
}

int nzd(int x, int y){
    if(y==0) return x;
    return nzd(y, x%y);
}

int main(){
}