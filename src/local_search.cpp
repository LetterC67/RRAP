#include <bits/stdc++.h>
#include "ant.h"
#include "parameters.h"
#include "utils.h"
using namespace std;


template<typename T>
void move_segment(vector<T>& vec, int l, int r, int i, bool type = false) {
    if (l < 0 || r >= vec.size() || i < 0 || i >= vec.size()) {
        cout << l << ' ' << r <<  ' ' << i << ' ' << vec.size() << endl;
        cout << "Invalid indices\n";
        return;
    }

    if (l > r || i >= l && i <= r) {
        cout << "Invalid segment or destination position " << l << ' ' << r << ' ' << i << endl;;
        return;
    }

    vector<T> segment(vec.begin() + l, vec.begin() + r + 1);
    vec.erase(vec.begin() + l, vec.begin() + r + 1);

    if (i > r) {
        i -= (r - l + 1);
    }

    if(type)
        reverse(segment.begin(), segment.end());
    vec.insert(vec.begin() + i + 1, segment.begin(), segment.end());
}

bool Ant::relocate(_tour &a, _tour &b, int idx_a, int idx_b){
    if(a.cost < b.cost ) return false;

    bool relocated = false;
    double min_cost = 1e9;
    int _i = -1, _j = -1, type = -1;

    for(int i = 1; i < a.size() - 1; i++){
        double delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
        double delta_decrease_a_2 = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] - (*distance)[a[i + 1]][a[i + 2]]  + (*distance)[a[i - 1]][a[i + 2]];
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];

            if(max(a.cost + delta_decrease_a, b.cost + delta_increase_b) + 1e-4 < max(a.cost, b.cost)){
                if(min_cost > delta_increase_b + delta_decrease_a){
                    min_cost = delta_increase_b + delta_decrease_a;
                    _i = i;
                    _j = j;
                    type = 0;
                }
            }

            if(i < a.size() - 2){   
                double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i + 1]][b[j]];
                double _delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i + 1]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i]][b[j]];
                int t = 1;

                if(delta_increase_b > _delta_increase_b){
                    t = 2;
                    delta_increase_b = _delta_increase_b; 
                }

                if(max(a.cost + delta_decrease_a_2, b.cost + delta_increase_b) + 1e-4 < max(a.cost, b.cost)){
                    if(min_cost > delta_increase_b + delta_decrease_a_2){
                        min_cost = delta_increase_b + delta_decrease_a_2;
                        _i = i;
                        _j = j;
                        type = t;
                    }
                }
            }
        }
    }

    if(_i == -1) return false;

    int i = _i, j = _j;

    if(type == 0){
        double delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
        double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];

        a.cost += delta_decrease_a;
        b.cost += delta_increase_b;

        b.tour.insert(b.begin() + j, a[i]);
        a.tour.erase(a.begin() + i);
    }else{
        double delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] - (*distance)[a[i + 1]][a[i + 2]]  + (*distance)[a[i - 1]][a[i + 2]];
        double delta_increase_b;

        if(type == 1){
            delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i + 1]][b[j]];
        }else{
            delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i + 1]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i]][b[j]];
        }

        a.cost += delta_decrease_a;
        b.cost += delta_increase_b;

        if(type == 1){
            b.tour.insert(b.begin() + j, a[i + 1]);
            b.tour.insert(b.begin() + j, a[i]);
        }else{
            b.tour.insert(b.begin() + j, a[i]);
            b.tour.insert(b.begin() + j, a[i + 1]);
        }
        a.tour.erase(a.begin() + i);
        a.tour.erase(a.begin() + i);
    }

    // assert(abs(a.cost - tour_length(a)) < 1e-3);
    // assert(abs(b.cost - tour_length(b)) < 1e-3);

    retag(idx_a);
    retag(idx_b);

    return true;
}

bool Ant::two_opt_inter_tour(_tour &a, _tour &b, int idx_a, int idx_b){
    vector<double> pref_a(a.size()), suf_a(a.size()), pref_b(b.size()), suf_b(b.size());

    for(int i = 1; i < a.size(); i++){
        pref_a[i] = pref_a[i - 1] + (*distance)[a[i - 1]][a[i]];
    }
    for(int i = 1; i < b.size(); i++){
        pref_b[i] = pref_b[i - 1] + (*distance)[b[i - 1]][b[i]];
    }
    for(int i = a.size() - 2; i >= 0; i--){
        suf_a[i] = suf_a[i + 1] + (*distance)[a[i + 1]][a[i]];   
    }
    for(int i = b.size() - 2; i >= 0; i--){
        suf_b[i] = suf_b[i + 1] + (*distance)[b[i + 1]][b[i]];   
    }

    double min_cost = 1e9, _new_cost_a, _new_cost_b;
    int _i = -1, _j = -1, type = -1;

    for(int i = 1; i < a.size() - 1; i++){
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            double new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
            double new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];

            if(max(new_cost_a, new_cost_b) + 1e-4 < max(a.cost, b.cost)){
                if(max(new_cost_a, new_cost_b) < min_cost){
                    min_cost = max(new_cost_a, new_cost_b);
                    _new_cost_a = new_cost_a;
                    _new_cost_b = new_cost_b;
                    _i = i;
                    _j = j;
                    type = 0;
                }
            }

            new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
            new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];

            if(max(new_cost_a, new_cost_b) + 1e-4 < max(a.cost, b.cost)){
                if(max(new_cost_a, new_cost_b) < min_cost){
                    min_cost = max(new_cost_a, new_cost_b);
                    _new_cost_a = new_cost_a;
                    _new_cost_b = new_cost_b;
                    _i = i;
                    _j = j;
                    type = 1;
                }
            }
        }
    }

    if(type == -1) return false;

    int &i = _i, &j = _j;

    if(type == 0){
        a.cost = _new_cost_a;
        b.cost = _new_cost_b;

        vector<int> c = vector<int>(a.tour.begin() + i, a.tour.end()), d = vector<int>(b.tour.begin() + j, b.tour.end());

        a.tour = vector<int>(a.begin(), a.begin() + i);
        b.tour = vector<int>(b.begin(), b.begin() + j);

        a.tour.insert(a.end(), d.begin(), d.end());
        b.tour.insert(b.end(), c.begin(), c.end());
    }else{
        a.cost = _new_cost_a;
        b.cost = _new_cost_b;

        vector<int> c = vector<int>(a.tour.begin() + i, a.tour.end()),
                    d = vector<int>(b.tour.begin() + j, b.tour.end());
        a.tour.erase(a.tour.begin() + i, a.tour.end());
        b.tour.erase(b.tour.begin() + j, b.tour.end());

        reverse(b.begin(), b.end());
        a.tour.insert(a.end(), b.begin(), b.end());
        
        reverse(c.begin(), c.end());
        b.tour = c;
        b.tour.insert(b.end(), d.begin(), d.end());
    }

    // assert(abs(a.cost - tour_length(a)) < 1e-3);
    // assert(abs(b.cost - tour_length(b)) < 1e-3);

    retag(idx_a);
    retag(idx_b);

    return true;
}

struct Point{
    double x, y;
};

struct Segment{
    int u, v;

    Segment(int u, int v, Graph *graph): u(u), v(v){
        if((*graph).x[this -> u] > (*graph).x[this -> v]){
            swap(this -> u, this -> v);
        }
    }
};

double get_delta(int a, int b, int c, int d, _tour &tour, vector<vector<double>> *distance){
    return (*distance)[tour[a]][tour[c]] - (*distance)[tour[a]][tour[b]] + (*distance)[tour[b]][tour[d]] - (*distance)[tour[c]][tour[d]];
    
}

struct sweepline_event{
    double x;
    int index;
    bool type;
};

bool Ant::two_opt_sweepline(_tour &tour, int idx){
    vector<Segment> segment;

    for(int i = 0; i < tour.size() - 2; i++){
        segment.push_back(Segment(tour[i], tour[i + 1], graph));
    }

    unordered_set<int> current;

    vector<sweepline_event> event;
    event.reserve(segment.size() * 2);


    for(int i = 0; i < segment.size(); i++){
        event.push_back({(*graph).x[segment[i].u], i, 1});
        event.push_back({(*graph).x[segment[i].v], i, 0});
    }

    sort(event.begin(), event.end(), [](const sweepline_event &a, const sweepline_event &b){
        if(a.x != b.x) return a.x < b.x;
        return a.type < b.type;
    });

    double min_cost = 1e9;
    int l = -1, r = -1;

    for(auto &e : event){
        if(e.type){
            int a = position[segment[e.index].u], b = position[segment[e.index].v];
            if(a > b) swap(a, b);
            
            for(auto &i : current){
                int c = position[segment[i].u], d = position[segment[i].v];
                if(c > d) swap(c, d);
                
                int _a = a, _b = b;
                if(_a > c) swap(_a, c), swap(_b, d);
                
                double delta = get_delta(_a, _b, c, d, tour, distance);
                if(delta < -1e-4){
                    if(delta < min_cost){
                        min_cost = delta;
                        l = _b, r = c;
                    }
                }
            }
            current.insert(e.index);
        }else{
            current.erase(e.index);
        }
    }

    if(l == -1) return false;

    double delta = get_delta(l - 1, l, r, r + 1, tour, distance);
    reverse(tour.begin() + l, tour.begin() + r + 1);
    tour.cost += delta;
    retag(idx);

    // assert(abs(tour.cost - tour_length(tour)) < 1e-3);
    
    return true;
}

bool Ant::or_opt(_tour &tour, int idx){
    bool improved = false;
    int _i = -1, _j = -1, _opt_size = -1, type = -1;
    double best = 1e9;

    for(int opt_size = 1; opt_size <= OPT_SIZE; opt_size++){
        for(int i = 1; i < (int)tour.size() - opt_size ; i++){
            double __delta = - (*distance)[tour[i - 1]][tour[i]] - (*distance)[tour[i + opt_size - 1]][tour[i + opt_size]] + (*distance)[tour[i - 1]][tour[i + opt_size]];

            for(auto &u : (*graph).closest[tour[i]]){
                if(assigned[u] != idx) continue;
                int j = position[u];
                if(i - 1 <= j && j <= i + opt_size - 1) continue;
                double delta = __delta + (*distance)[tour[j]][tour[i]] + (*distance)[tour[i + opt_size - 1]][tour[j + 1]] - (*distance)[tour[j]][tour[j + 1]];            
                double _delta = __delta + (*distance)[tour[j]][tour[i + opt_size - 1]] + (*distance)[tour[i]][tour[j + 1]] - (*distance)[tour[j]][tour[j + 1]];
                int t = 0;

                if(_delta < delta){
                    t = 1;
                    delta = _delta;
                }

                if(delta < -1e-4){
                    if(delta < best){
                        best = delta;
                        _i = i;
                        _j = j;
                        _opt_size = opt_size;
                        type = t;
                    }                 
                }
            }
        }
    }

    if(_i == -1) return false;
    move_segment(tour.tour, _i, _i + _opt_size - 1, _j, type);
    tour.cost += best;
    // assert(abs(tour.cost - tour_length(tour)) < 1e-3);

    return true;
}

bool Ant::intra_tour_optimization(vector<bool> &not_improved){
    int idx = 0;
    bool improved = false;

    for(auto &tour : tours){
        if(not_improved[idx]){
            idx++;
            continue;
        }
        if(two_opt_sweepline(tour, idx)) improved = true;
        if(or_opt(tour, idx)) improved = true;
        if(!improved){
            not_improved[idx] = true;
        }
        idx++;
    }

    for(int i = 0; i < tours.size(); i++)
        retag(i);

    return improved;
}

void Ant::local_search(){
    vector<int> ord;
    vector<bool> single_not_improved(tours.size());
    vector<vector<int>> not_improved;
    
    for(int i = 0; i < tours.size(); i++){
        ord.push_back(i);
        not_improved.push_back(vector<int>(tours.size()));
    }

    vector<int> _del(del.begin(), del.end());

    for(int neighborhood = 0; neighborhood < 3; neighborhood++){
        bool improved = false;
        shuffle(ord.begin(), ord.end(), rng);
        
        if(neighborhood == 0){
            for(int i = 0; i < tours.size(); i++)
                for(int j = 0; j < tours.size(); j++)
                    if(j != i && not_improved[ord[i]][ord[j]] < 1)
                        if(relocate(tours[ord[i]], tours[ord[j]], ord[i], ord[j])){
                            improved = true;             
                            fill(not_improved[ord[i]].begin(), not_improved[ord[i]].end(), 0);
                            fill(not_improved[ord[j]].begin(), not_improved[ord[j]].end(), 0);
                            single_not_improved[ord[i]] = single_not_improved[ord[j]] = false;
                        }else{
                            not_improved[ord[i]][ord[j]] = 1;
                        }
            
        }else if(neighborhood == 1){
            for(int i = 0; i < tours.size(); i++)
                for(int j = 0; j < tours.size(); j++)
                    if(j != i && not_improved[ord[i]][ord[j]] < 2)
                        if(two_opt_inter_tour(tours[ord[i]], tours[ord[j]], ord[i], ord[j])){
                            improved = true;
                            fill(not_improved[ord[i]].begin(), not_improved[ord[i]].end(), 0);
                            fill(not_improved[ord[j]].begin(), not_improved[ord[j]].end(), 0);
                            single_not_improved[ord[i]] = single_not_improved[ord[j]] = false;
                        }else{
                            not_improved[ord[i]][ord[j]] = not_improved[ord[j]][ord[i]] = 2;
                        }
        }else{
            improved |= intra_tour_optimization(single_not_improved);
            for(int i = 0; i < single_not_improved.size(); i++){
                if(!single_not_improved[i]){ 
                    fill(not_improved[i].begin(), not_improved[i].end(), 0);
                }
            }
        }

        if(improved) neighborhood = -1;
    }

        
    for(auto &tour : tours){
        tour.cost = tour_length(tour);
    }
}