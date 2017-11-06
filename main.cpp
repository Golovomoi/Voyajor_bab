#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

int n = 100;
int H;
int S_H;
int S_WE_H;

//int priv_max_sum;
//int priv_max_i;
//int priv_max_j;
int way_length;

struct bab_S {
    int priv_max_sum;
    int priv_max_i;
    int priv_max_j;
    int way_length;
    int H;
    int S_WO_E;
    int S_W_E;
    vector<pair<int, int>> ans;
    vector<vector<int>> cost;
    vector<vector<int>> work_cost;
//    vector<pair<int, int>> ans;
};

vector<vector<int>> cost;
vector<vector<int>> work_cost;
vector<vector<int>> ans_cost;
vector<pair<int, int>> ans;
vector<bool> deleted_str;
vector<bool> deleted_col;

int infs_to_zero( vector<vector<int>> & ar ) {
    for (int i = 0; i < ar.size() - 1; i++) {
        if (ar[i][ar.size() - 1] == INT_MAX) ar[i][ar.size() - 1] = 0;
        if (ar[ar.size() - 1][i] == INT_MAX) ar[ar.size() - 1][i] = 0;
    }
    return 0;
}

int set_infs( vector<vector<int>> & ar ) {
    for (int i = 0; i < ar.size() - 1; i++) {
        ar[i][ar.size() - 1] = INT_MAX;
        ar[ar.size() - 1][i] = INT_MAX;
    }
    return 0;
}

int prepare_matrix( vector<vector<int>> & ar, bool str_col ) {

    for (int i = 0; i < ar.size() - 1; i++)
        for (int j = 0; j < ar[i].size() - 1; j++) {
            if (ar[i][j] == -1) continue;
            str_col ? ar[i][ar[i].size() - 1] = min( ar[i][ar[i].size() - 1], ar[i][j] ) :
                ar[ar[j].size() - 1][j] = min( ar[ar[j].size() - 1][j], ar[i][j] );
        }
    for (int i = 0; i < ar.size() - 1; i++)
        for (int j = 0; j < ar[i].size() - 1; j++) {
            if (ar[i][j] == -1) continue;
            str_col ? ar[i][j] -= ar[i][ar[i].size() - 1] :
                ar[i][j] -= ar[ar[j].size() - 1][j];
        }
    return 0;

}

int find_h( vector<vector<int>> ar ) {
    int result = 0;
    for (int i = 0; i < ar.size() - 1; i++)
        result += ar[i][ar.size() - 1] + ar[ar.size() - 1][i];
    return result;

}

int find_max_zero( bab_S & babr ) {
    babr.priv_max_sum = 0;
    for (int i = 0; i < babr.cost.size() - 1; i++)
        for (int j = 0; j < babr.cost.size() - 1; j++) {
            if (babr.cost[i][j] == -1) continue;
            if (babr.cost[i][j] == 0) {
                babr.cost[i][babr.cost.size() - 1] = INT_MAX;
                babr.cost[babr.cost.size() - 1][j] = INT_MAX;
                for (int k = 0; k < babr.cost.size() - 1; k++) {
                    if (k == j || babr.cost[i][k] == -1) continue;
                    babr.cost[i][babr.cost.size() - 1] = min( babr.cost[i][babr.cost.size() - 1], babr.cost[i][k] );
                }
                for (int k = 0; k < babr.cost.size() - 1; k++) {
                    if (k == i || babr.cost[k][j] == -1) continue;
                    babr.cost[babr.cost.size() - 1][j] = min( babr.cost[babr.cost.size() - 1][j], babr.cost[k][j] );
                }
                if ( (babr.cost[babr.cost.size() - 1][j] + babr.cost[i][babr.cost.size() - 1]) > babr.priv_max_sum) {
                    if (babr.cost[i][babr.cost.size() - 1] == INT_MAX) babr.cost[i][babr.cost.size() - 1] = 0;
                    if (babr.cost[babr.cost.size() - 1][j] == INT_MAX) babr.cost[babr.cost.size() - 1][j] = 0;
                    babr.priv_max_sum = babr.cost[babr.cost.size() - 1][j] + babr.cost[i][babr.cost.size() - 1];
                    babr.priv_max_i = i;
                    babr.priv_max_j = j;
                }
            }
        }

    return 0;
}

int delete_str_col( vector<vector<int>> & ar, int str, int col ) {

    for (int i = 0; i < ar.size() - 1; i++ )
        for (int j = 0; j < ar.size() - 1; j++) 
            if (i == str || j == col) {
                ar[i][j] = -1;
            }


    return 0;
}


bab_S find_way( bab_S & S ) {

    bab_S variety = S;

    if (S.ans.size() + 1 == variety.cost.size() - 1) {
        for (int i = 0; i < variety.cost.size() - 1; i++)
            for (int j = 0; j < variety.cost.size() - 1; j++)
                if (variety.cost[i][j] != -1) {
                    variety.ans.push_back( { i, j } );
                    return variety;
                }
    }

    set_infs( variety.cost );
    prepare_matrix( variety.cost, true );
    prepare_matrix( variety.cost, false );
    infs_to_zero( variety.cost );
    variety.H += find_h( variety.cost );

    find_max_zero( variety );

    variety.S_WO_E = variety.H + variety.priv_max_sum;
    bab_S variety_wo_e = variety;
    variety_wo_e.H = variety.S_WO_E;
    variety_wo_e.cost[variety_wo_e.priv_max_i][variety_wo_e.priv_max_j] = -1;
    
    bab_S variety_w_e = variety;
    delete_str_col( variety_w_e.cost, variety_w_e.priv_max_i, variety_w_e.priv_max_j );
    variety_w_e.cost[variety_w_e.priv_max_j][variety_w_e.priv_max_i] = -1;
    set_infs( variety_w_e.cost );
    prepare_matrix( variety_w_e.cost, true );
    prepare_matrix( variety_w_e.cost, false );
    infs_to_zero( variety_w_e.cost );
    variety_w_e.H += find_h( variety_w_e.cost );

    if (variety_w_e.H <= variety_wo_e.H) {
        variety_w_e.ans.push_back( { variety_w_e.priv_max_i, variety_w_e.priv_max_j } );
        return find_way( variety_w_e );
    }
    else {
        return find_way( variety_wo_e );
    }

    return variety;
}

int main( int argc, char *argv[] ) {

    //ofstream some;//fint("qwe");// ( inpt );
    if (argc > 1) {
        freopen( argv[1], "r", stdin );
        freopen( argv[1], "w", stdout );
    }
    else
    {
        freopen( "input.txt", "r", stdin );
        freopen( "output.txt", "w", stdout );
    }

    cin >> n;
    way_length = n;

    for (int i = 0; i < n; i++) {
        vector<int> tmp_cost;
        for (int j = 0; j < n; j++) {
            int value;
            cin >> value;
            //if (value == -1) value = INT_MAX;
            tmp_cost.push_back( value );
        }
        tmp_cost.push_back( INT_MAX );
        cost.push_back( tmp_cost );
    }
    vector<int> tmp_cost( n + 1, INT_MAX );
    cost.push_back( tmp_cost );
    
    bab_S initial;
    initial.cost = cost;
    initial.H = 0;
    initial.priv_max_sum = 0;
    initial.way_length = 0;
    initial.work_cost = cost;

    bab_S cycle;

    vector<int> for_ans (n, 0);

    cycle = find_way( initial );

    for (int i = 0; i < cycle.ans.size(); i++)
        for_ans[cycle.ans[i].first] = cycle.ans[i].second;

    
    int ans_c = for_ans.size();
    int first = 0, second;
    way_length = 0;
    while (ans_c > 0) {
        second = for_ans[first];
        ans.push_back( { first, second } );
        way_length += cost[first][second];
        first = second;
        ans_c--;
    }
    cout << way_length << endl;
    for (int i =0; i <ans.size(); i++)
    cout << "(" << ans[i].first + 1 << "->" << ans[i].second + 1<< ") ";

    return 0;

}