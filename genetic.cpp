#include <vector>
#include <set>
#include <random>
#include <iostream>
#include <cstdlib>
#include <list>
#include <algorithm>

using namespace std;

int death = 5;

double frand(double min, double max){
    return min + (double)(rand()) / ((double)(RAND_MAX / (max - min)));
}

class tier{
public:
    double x, y;
    tier(){}
    int lifespan;
    tier(double x, double y, size_t l) : x(x), y(y), lifespan(l){}
    static vector<tier> reproduce(const tier& papa, const tier& mama){
        vector<tier> children;
        tier child;
        bernoulli_distribution d(0.5);
        knuth_b rand_engine;
        for (size_t i = 0; i < 10; ++i){
            if (d(rand_engine)) child.x = mama.x;
            else child.x = papa.x;
            if(d(rand_engine)) child.y = mama.y;
            else child.y = papa.y;
            child.lifespan = death;
            child.mutate();
            children.push_back(child);
        }
        return children;
    } 
    void mutate(){
        bernoulli_distribution d(0.66);
        knuth_b rand_engine;
        if(d(rand_engine)){
            x += frand(-fabs(x/4) - 1, fabs(x/4) + 1);
            y += frand(-fabs(y/4) - 1, fabs(y/4) + 1);
        }
    }
};

double func(const tier& t){
    return 100 - t.x*t.x - t.y*t.y;
}

bool operator<(const tier &left, const tier &right) { return func(left) < func(right); }
bool operator==(const tier &left, const tier &right) { return left.x == right.x && left.y == right.y; }

vector<tier> populate(size_t n){
    vector<tier> population;
    for (auto i = 0; i < n; ++i) population.push_back(tier(frand(-20, 20), frand(-20, 20), death));
    sort(population.begin(), population.end());
    return population;
}

void selection(vector<tier>& st, size_t n){
    knuth_b rand_engine;
    long double sum = 0;
    set<int> dead;
    for (auto i = 0; i < st.size(); ++i) if((--st[i].lifespan) == 0) dead.insert(i);
    for (auto index : dead) st.erase(st.begin() + index);
    while(st.size() > n) st.erase(st.begin());
}

void printGen(const vector<tier>& st, size_t gen){
    cout << "Generation " << gen << ": {" << endl;
    for(auto el : st) cout << "[" << el.x << ", " << el.y << ", " << el.lifespan << "]" << endl;
    cout << "} size " << st.size() << endl;
}

int main(){
    size_t n = 40, gen;
    bool male = true;
    vector<tier> homyaki = populate(n);
    sort(homyaki.begin(), homyaki.end());
    printGen(homyaki, 0);
    for (gen = 1; /*fabs(func(*homyaki.rbegin()) - 100) >= 0.01*/ gen < 400; ++gen){
        vector<tier> babies;
        for (size_t i = 0; i + 1 < homyaki.size(); ++i) homyaki[i].mutate(); // mutation
        for (size_t i = 0; i + 1 < homyaki.size(); i += 2)
        { // reproduction
            auto tmp = tier::reproduce(homyaki[i], homyaki[i+1]);
            babies.insert(babies.end(), tmp.begin(), tmp.end());
        }
        homyaki.insert(homyaki.end(), babies.begin(), babies.end());
        sort(homyaki.begin(), homyaki.end());
        selection(homyaki, n);// time to die!
        //printGen(homyaki, gen);
    }
    printGen(homyaki, gen);
    printf("Top homyak: %f %f\n", homyaki.rbegin()->x, homyaki.rbegin()->y);
    printf("func(homyak) - R^2 = %f\n", fabs(func(*(homyaki.rbegin())) - 100));
    system("pause");
}