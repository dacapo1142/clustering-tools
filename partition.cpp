#include <iostream>
#include "containers/disjoint_sets.h"
using namespace std;
int main(){
    int n=5;
    DisjointSets ds(n);
    for(int i=0; i<n; i++){
        cout<<ds.c_id[i]<<endl;
    }
    cout<<endl;
    ds.assign(0,1);
    for(int i=0; i<n; i++){
        cout<<ds.c_id[i]<<endl;
    }
    cout<<endl;

    cout<<"num nonempty: "<<ds.num_sets()<<endl;
    cout<<endl;
    
    ds.init_disjoint_set();
    for(auto v:ds.stored_range){
        cout<<v<<endl;
    }

    cout<<endl;
    
    
    for(int i=0; i<n; i++){
        cout<<ds.disjoint_set[i]<<endl;
    }
    return 0;
}