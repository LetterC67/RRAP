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

tuple<double, int, int, int> Ant::relocate(_tour &a, _tour &b, int idx_a, int idx_b){
    if(a.cost < b.cost ) return make_tuple(1e9, -1, -1, 0);

    bool relocated = false;
    double min_cost = 1e9;
    int _i = -1, _j = -1, type = -1;

    for(int i = 1; i < a.size() - 1; i++){
        double delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            // assert(j != 0);
            double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];
            
            double new_cost_a = a.cost + delta_decrease_a;
            double new_cost_b = b.cost + delta_increase_b;
            if(max(new_cost_a, new_cost_b) + 1e-4 < max(a.cost, b.cost)){
                if(min_cost > max(new_cost_a, new_cost_b)){
                    min_cost = max(new_cost_a, new_cost_b);
                    _i = i;
                    _j = j;
                    type = 0;
                }
            }
            
            if(i < a.size() - 2){   
                double delta_decrease_a_2 = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] - (*distance)[a[i + 1]][a[i + 2]]  + (*distance)[a[i - 1]][a[i + 2]];
                double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i + 1]][b[j]];
                double _delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i + 1]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i]][b[j]];
                int t = 1;

                if(delta_increase_b > _delta_increase_b || fabs(delta_increase_b - _delta_increase_b) < 1E-4){
                    t = 2;
                    delta_increase_b = _delta_increase_b; 
                }

                double new_cost_a_2 = a.cost + delta_decrease_a_2;
                double new_cost_b_2 = b.cost + delta_increase_b;
                if(max(new_cost_a_2, new_cost_b_2) + 1e-4 < max(a.cost, b.cost)){
                    if(min_cost > max(new_cost_a_2, new_cost_b_2)){
                        min_cost = max(new_cost_a_2, new_cost_b_2);
                        _i = i;
                        _j = j;
                        type = t;
                    }
                }
            }
        }
    }

    if(_i == -1) return make_tuple(1e9, -1, -1, 0);
    return make_tuple(min_cost, _i, _j, type);
}

void Ant::update_relocate(_tour& a, _tour& b,
                          int idx_a, int idx_b,
                          int i, int j,
                          int type) {
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
            // cerr << "M2.1 is good\n";
        }else{
            delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i + 1]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i]][b[j]];
            // cerr << "M2.2 is good\n";
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
}

tuple<double, int, int, bool> Ant::two_opt_inter_tour(_tour &a, _tour &b, int idx_a, int idx_b){
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
            // assert(j != 0);
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

    if(type == -1) return make_tuple(1e9, -1, -1, false);

    return make_tuple(min_cost, _i, _j, type);
}

void Ant::update_two_opt_inter_tour(_tour& a, _tour& b,
                                    int idx_a, int idx_b,
                                    int i, int j,
                                    bool type) {
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

    if(type == 0){
        double _new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
        double _new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];

        a.cost = _new_cost_a;
        b.cost = _new_cost_b;

        vector<int> c = vector<int>(a.tour.begin() + i, a.tour.end()), d = vector<int>(b.tour.begin() + j, b.tour.end());

        a.tour = vector<int>(a.begin(), a.begin() + i);
        b.tour = vector<int>(b.begin(), b.begin() + j);

        a.tour.insert(a.end(), d.begin(), d.end());
        b.tour.insert(b.end(), c.begin(), c.end());
    }else{
        double _new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
        double _new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];

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

//  bool Ant::two_opt_sweepline(_tour &tour, int idx){

//     double min_cost = 1e9;
//     int l = -1, r = -1;

//         for(int i = 1; i < tour.size() - 2; i++){
//             for(int j = i + 1; j < tour.size() - 1; j++){
//                     double delta = get_delta(i - 1, i, j , j + 1, tour, distance);
//                     if(delta < -1e-4){
//                         if(delta < min_cost){
//                             min_cost = delta;
//                             l = i, r = j;
//                         }
//                     }
//             }
//     }
//     if(l == -1) return false;

//     double delta = get_delta(l - 1, l, r, r + 1, tour, distance);
//     reverse(tour.begin() + l, tour.begin() + r + 1);
//     tour.cost += delta;
//     retag(idx);

//     // assert(abs(tour.cost - tour_length(tour)) < 1e-3);

//     return true;
// }

bool Ant::two_opt_sweepline(_tour &tour, int idx){
    vector<Segment> segment;

    for(int i = 0; i < tour.size() - 1; i++){
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
            if(a + 1 != b){
                swap(a, b);
                b = a + 1;
            }
            
            for(auto &i : current){
                int c = position[segment[i].u], d = position[segment[i].v];
                if(c > d) swap(c, d);
                if(c + 1 != d){
                    swap(c, d);
                    d = c + 1;
                }
                
                int _a = a, _b = b;
                if(_a > c) swap(_a, c), swap(_b, d);
                
                double delta = get_delta(_a, _b, c, d, tour, distance);
                if(delta < -1e-4 && _a != c){
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
    for(int i = 0; i < tour.size() - 1; i++){
        for(auto &u : (*graph).closest[tour[i]]){
            if(assigned[u] != idx) continue;
            int j = position[u];
            if (u == DEPOT) continue;
            int _l, _r;
            if(i < j) _l = i + 1, _r = j;
            else _l = j, _r = i - 1;
            double delta = get_delta(_l - 1, _l, _r, _r + 1, tour, distance);
            if(delta < -1e-4 && _l - 1 != _r){
                if(delta < min_cost){
                    min_cost = delta;
                    l = _l, r = _r; 
                }
            }
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

// bool Ant::intra_tour_optimization(vector<bool> &not_improved){
//     int idx = 0;
//     bool improved = false;

//     for(auto &tour : tours){
//         if(not_improved[idx]){
//             idx++;
//             continue;
//         }
//         bool tour_improved = false;
//         if(two_opt_sweepline(tour, idx)) tour_improved = true;
//         if(or_opt(tour, idx)) tour_improved = true;
//         if(!tour_improved){
//             not_improved[idx] = true;
//         }
//         improved |= tour_improved;
//         // if(two_opt_sweepline(tour, idx)) improved = true;
//         // if(or_opt(tour, idx)) improved = true;
//         // if(!improved){
//         //     not_improved[idx] = true;
//         // }
//         idx++;
//     }

//     for(int i = 0; i < tours.size(); i++)
//         retag(i);

//     return improved;
// }

bool Ant::two_opt_intra_tour(vector<int> &not_improved){
    int idx = 0;
    bool improved = false;

    for(auto &tour : tours){
        if(not_improved[idx] >= 1){
            idx++;
            continue;
        }
        bool tour_improved = false;
        if(two_opt_sweepline(tour, idx)) tour_improved = true;
        if(!tour_improved){
            not_improved[idx] = 1;
        }
        improved |= tour_improved;
        idx++;
    }

    for(int i = 0; i < tours.size(); ++i)
        if (not_improved[idx] == 0) retag(i);

    return improved;
}

bool Ant::or_opt_intra_tour(vector<int> &not_improved){
    int idx = 0;
    bool improved = false;

    for(auto &tour : tours){
        if(not_improved[idx] >= 2){
            idx++;
            continue;
        }
        bool tour_improved = false;
        if(or_opt(tour, idx)) tour_improved = true;
        if(!tour_improved){
            not_improved[idx] = 2;
        } else {
            not_improved[idx] = 0;
        }
        improved |= tour_improved;
        idx++;
    }

    for(int i = 0; i < tours.size(); ++i)
        if (not_improved[idx] == 0) retag(i);

    return improved;
}

void Ant::local_search(){
    vector<int> ord;
    vector<int> single_not_improved(tours.size());
    vector<vector<int>> not_improved;
    
    for(int i = 0; i < tours.size(); i++){
        ord.push_back(i);
        not_improved.push_back(vector<int>(tours.size()));
    }
    
    vector<vector<tuple<double, int, int, int>>> insert_cache(tours.size(), vector<tuple<double, int, int, int>>(tours.size(), make_tuple(1e9, -1, -1, 0)));
    vector<vector<tuple<double, int, int, bool>>> two_opt_cache(tours.size(), vector<tuple<double, int, int, bool>>(tours.size(), make_tuple(1e9, -1, -1, false)));
    for(int neighborhood = 0; neighborhood < 4; neighborhood++){
        bool improved = false;
        shuffle(ord.begin(), ord.end(), rng);
        
        if(neighborhood == 0){
            tuple<double, int, int, int> best = make_tuple(1e9, -1, -1, 0);
            int _i, _j;
            for(int i = 0; i < tours.size(); i++)
                for(int j = 0; j < tours.size(); j++)
                    if(j != i && not_improved[ord[i]][ord[j]] < 1) {
                        // auto candidate = relocate(tours[ord[i]], tours[ord[j]], ord[i], ord[j]);
                        auto candidate = (get<1>(insert_cache[ord[i]][ord[j]]) == -1 ? relocate(tours[ord[i]], tours[ord[j]], ord[i], ord[j])
                                                                                     : insert_cache[ord[i]][ord[j]]);
                        // assert(fabs(get<0>(candidate) - get<0>(relocate(tours[ord[i]], tours[ord[j]], ord[i], ord[j]))) < 1e-4);
                        insert_cache[ord[i]][ord[j]] = candidate;
                        // [min_cost, i, j, type]
                        if (get<1>(candidate) == -1) {
                            not_improved[ord[i]][ord[j]] = 1;
                            continue;
                        }
                        if (get<0>(candidate) + 1e-4 < get<0>(best)) {
                            _i = i; _j = j;
                            best = std::move(candidate);
                        }
                    }
            const auto& [min_cost, pos_i, pos_j, type] = best;
            // assert(pos_i != 0);
            // assert(pos_j != 0);
            if(get<1>(best) != -1){
                improved = true;
                update_relocate(tours[ord[_i]], tours[ord[_j]], ord[_i], ord[_j], pos_i, pos_j, type);

                // assert(fabs(tour_length(tours[ord[_i]]) - tours[ord[_i]].cost) < 1e-4);
                // assert(fabs(tour_length(tours[ord[_j]]) - tours[ord[_j]].cost) < 1e-4);

                assert(fabs(max(tours[ord[_i]].cost, tours[ord[_j]].cost) - min_cost) < 1e-4);

                fill(not_improved[ord[_i]].begin(), not_improved[ord[_i]].end(), 0);
                fill(not_improved[ord[_j]].begin(), not_improved[ord[_j]].end(), 0);
                
                fill(insert_cache[ord[_i]].begin(), insert_cache[ord[_i]].end(), make_tuple(1e9, -1, -1, 0));
                fill(insert_cache[ord[_j]].begin(), insert_cache[ord[_j]].end(), make_tuple(1e9, -1, -1, 0));
                fill(two_opt_cache[ord[_i]].begin(), two_opt_cache[ord[_i]].end(), make_tuple(1e9, -1, -1, false));
                fill(two_opt_cache[ord[_j]].begin(), two_opt_cache[ord[_j]].end(), make_tuple(1e9, -1, -1, false));
                for (int k = 0; k < tours.size(); ++k) {
                    not_improved[k][ord[_i]] = not_improved[k][ord[_j]] = 0;

                    insert_cache[k][ord[_i]] = insert_cache[k][ord[_j]] = make_tuple(1e9, -1, -1, 0);
                    two_opt_cache[k][ord[_i]] = two_opt_cache[k][ord[_j]] = make_tuple(1e9, -1, -1, false);
                }
                single_not_improved[ord[_i]] = single_not_improved[ord[_j]] = 0;
            }
        }else if(neighborhood == 1){
            tuple<double, int, int, bool> best = make_tuple(1e9, -1, -1, false);
            int _i, _j;
            for(int i = 0; i < tours.size(); i++)
                for(int j = 0; j < tours.size(); j++)
                    if(j != i && not_improved[ord[i]][ord[j]] < 2) {
                        // auto candidate = two_opt_inter_tour(tours[ord[i]], tours[ord[j]], ord[i], ord[j]);
                        auto candidate = (get<1>(two_opt_cache[ord[i]][ord[j]]) == -1 ? two_opt_inter_tour(tours[ord[i]], tours[ord[j]], ord[i], ord[j])
                                                                                      : two_opt_cache[ord[i]][ord[j]]);
                        // assert(fabs(get<0>(candidate) - get<0>(two_opt_inter_tour(tours[ord[i]], tours[ord[j]], ord[i], ord[j]))) < 1e-4);
                        two_opt_cache[ord[i]][ord[j]] = candidate;
                        // [min_cost, i, j, type]
                        if (get<1>(candidate) == -1) {
                            not_improved[ord[i]][ord[j]] = not_improved[ord[j]][ord[i]] = 2; // Thật ra đây cũng là 1 bug, chưa chắc đã đối xứng
                            continue;
                        }
                        if (get<0>(candidate) + 1e-4 < get<0>(best)) {
                            _i = i; _j = j;
                            best = std::move(candidate);
                        }
                    }
            const auto& [min_cost, pos_i, pos_j, type] = best;
            // assert(pos_i != 0);
            // assert(pos_j != 0);
            if(get<1>(best) != -1){
                improved = true;
                update_two_opt_inter_tour(tours[ord[_i]], tours[ord[_j]], ord[_i], ord[_j], pos_i, pos_j, type);
                
                // assert(fabs(tour_length(tours[ord[_i]]) - tours[ord[_i]].cost) < 1e-4);
                // assert(fabs(tour_length(tours[ord[_j]]) - tours[ord[_j]].cost) < 1e-4);
                
                assert(fabs(max(tours[ord[_i]].cost, tours[ord[_j]].cost) - min_cost) < 1e-4);

                fill(not_improved[ord[_i]].begin(), not_improved[ord[_i]].end(), 0);
                fill(not_improved[ord[_j]].begin(), not_improved[ord[_j]].end(), 0);

                fill(insert_cache[ord[_i]].begin(), insert_cache[ord[_i]].end(), make_tuple(1e9, -1, -1, 0));
                fill(insert_cache[ord[_j]].begin(), insert_cache[ord[_j]].end(), make_tuple(1e9, -1, -1, 0));
                fill(two_opt_cache[ord[_i]].begin(), two_opt_cache[ord[_i]].end(), make_tuple(1e9, -1, -1, false));
                fill(two_opt_cache[ord[_j]].begin(), two_opt_cache[ord[_j]].end(), make_tuple(1e9, -1, -1, false));
                for (int k = 0; k < tours.size(); ++k) {
                    not_improved[k][ord[_i]] = not_improved[k][ord[_j]] = 0;

                    insert_cache[k][ord[_i]] = insert_cache[k][ord[_j]] = make_tuple(1e9, -1, -1, 0);
                    two_opt_cache[k][ord[_i]] = two_opt_cache[k][ord[_j]] = make_tuple(1e9, -1, -1, false);
                }
                single_not_improved[ord[_i]] = single_not_improved[ord[_j]] = 0;
            }
        }else if(neighborhood == 2){
            // improved |= intra_tour_optimization(single_not_improved);
            improved = two_opt_intra_tour(single_not_improved);
            for(int i = 0; i < single_not_improved.size(); i++){
                if(single_not_improved[i] == 0){ 
                    fill(not_improved[i].begin(), not_improved[i].end(), 0);
                    // assert(fabs(tour_length(tours[i]) - tours[i].cost) < 1e-4);

                    fill(insert_cache[i].begin(), insert_cache[i].end(), make_tuple(1e9, -1, -1, 0));
                    fill(two_opt_cache[i].begin(), two_opt_cache[i].end(), make_tuple(1e9, -1, -1, false));
                    for (int k = 0; k < tours.size(); ++k) {
                        not_improved[k][i] = 0;

                        insert_cache[k][i] = make_tuple(1e9, -1, -1, 0);
                        two_opt_cache[k][i] = make_tuple(1e9, -1, -1, false);
                    }
                }
            }
        }else{
            improved |= or_opt_intra_tour(single_not_improved);
            for(int i = 0; i < single_not_improved.size(); i++){
                if(single_not_improved[i] == 0){ 
                    fill(not_improved[i].begin(), not_improved[i].end(), 0);
                    // assert(fabs(tour_length(tours[i]) - tours[i].cost) < 1e-4);

                    fill(insert_cache[i].begin(), insert_cache[i].end(), make_tuple(1e9, -1, -1, 0));
                    fill(two_opt_cache[i].begin(), two_opt_cache[i].end(), make_tuple(1e9, -1, -1, false));
                    for (int k = 0; k < tours.size(); ++k) {
                        not_improved[k][i] = 0;
                        
                        insert_cache[k][i] = make_tuple(1e9, -1, -1, 0);
                        two_opt_cache[k][i] = make_tuple(1e9, -1, -1, false);
                    }
                }
            }
        }
        if(improved) neighborhood = -1;
    }
        
    for(auto &tour : tours){
        // if (!(fabs(tour.cost - tour_length(tour)) < 1e-4)) {
        //     cerr << fixed << setprecision(4) << tour.cost << ' ' << tour_length(tour) << '\n';
        // }
        assert(fabs(tour.cost - tour_length(tour)) < 1e-4);
        tour.cost = tour_length(tour);
    }
}