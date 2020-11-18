#include<bits/stdc++.h>
using namespace std;
#define M 105
#define INF 1000000000
typedef long long ll;

char input_file[] = "csp-task/data/d-10-09.txt.txt";
//char input_file[] = "big.txt";

int n;
int grid[M][M];
int row_cnt[M], row_ald[M][M];
int col_cnt[M], col_ald[M][M];
int cant_place[M][M][M];
int dom_size[M][M];
int tot;

ll nodes_visited, fails;

void parse_input()
{
    string line;

    getline(cin, line);
    n = atoi(line.substr(2, line.size()-3).c_str());

    getline(cin, line);
    getline(cin, line);

    for(int i = 1; i <= n; i++)
    {
        getline(cin, line);

        vector<int> delimeters;
        delimeters.push_back(-1);

        for(int j = 0; j < line.size(); j++)
        {
            if(line[j] == ',' || line[j] == '|') delimeters.push_back(j);
        }

        for(int j = 0; j+1 < delimeters.size(); j++)
        {
            int st = delimeters[j], ed = delimeters[j+1];
            grid[i][j+1] = atoi(line.substr(st+1, ed-st-1).c_str());
        }
    }
}

void print_board()
{
    printf("%d\n\n", n);
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++) printf("%2d ", grid[i][j]);
        printf("\n");
    }
}

int is_valid()
{
    bool shob = true;

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(grid[i][j] < 0 || grid[i][j] > n) return 0;
            if(grid[i][j] == 0) shob = false;

            for(int k = 1; k <= n; k++)
            {
                for(int l = 1; l <= n; l++)
                {
                    if(i == k && j == l) continue;
                    if(i != k && j != l) continue;

                    if(grid[i][j] && grid[i][j] == grid[k][l]) return 0;
                }
            }
        }
    }

    return shob+1;
}

void place(int x, int y, int val)
{
    grid[x][y] = val;

    row_cnt[x]++, row_ald[x][val]++;
    col_cnt[y]++, col_ald[y][val]++;
    tot++;
}

void unplace(int x, int y, int val)
{
    grid[x][y] = 0;
    
    row_cnt[x]--, row_ald[x][val]--;
    col_cnt[y]--, col_ald[y][val]--;
    tot--;
}

void init_basic()
{
    nodes_visited++;

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(grid[i][j]) place(i, j, grid[i][j]);
            else dom_size[i][j] = n;
        }
    }
}

bool update(int x, int y, int val, bool invert)
{
    if(!invert)
    {
        cant_place[x][y][val]++;

        if(cant_place[x][y][val] == 1)
        {
            dom_size[x][y]--;
            if(dom_size[x][y] == 0) return false;
        }
    }
    else
    {
        cant_place[x][y][val]--;

        if(cant_place[x][y][val] == 0)
        {
            dom_size[x][y]++;
            if(dom_size[x][y] == 1) return false;
        }
    }

    return true;
}

bool propagate_bt(int x, int y)
{
    int val = grid[x][y];
    return (row_ald[x][val] == 1 && col_ald[y][val] == 1);
}

bool propagate_fc(int x, int y, bool invert = false)
{
    int val = grid[x][y];

    for(int j = 1; j <= n; j++)
    {
        if(grid[x][j]) continue;
        if(!update(x, j, val, invert)) return false;
    }
    for(int i = 1; i <= n; i++)
    {
        if(grid[i][y]) continue;
        if(!update(i, y, val, invert)) return false;
    }

    return true;
}

void init_bt() {}

void init_fc()
{
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(grid[i][j]) propagate_fc(i, j);
        }
    }
}

void sdf(int &x, int &y)
{
    int mn = INF;

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(grid[i][j]) continue;

            if(dom_size[i][j] < mn) mn = dom_size[i][j], x = i, y = j;
        }
    }
}

void brelaz(int &x, int &y)
{
    int mn_dom = INF, mx_for_deg = 0;

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(grid[i][j]) continue;

            if(dom_size[i][j] < mn_dom)
            {
                mn_dom = dom_size[i][j];
                mx_for_deg = n-row_cnt[i]+n-col_cnt[j];
                x = i, y = j;
            }
            else if(dom_size[i][j] == mn_dom && mx_for_deg < n-row_cnt[i]+n-col_cnt[j])
            {
                mx_for_deg = n-row_cnt[i]+n-col_cnt[j];
                x = i, y = j;
            }
        }
    }
}

bool solver_bt()
{
    assert(tot < n*n);

    int x, y;
    brelaz(x, y);

    for(int val = 1; val <= n; val++)
    {
        place(x, y, val);
        nodes_visited++;
        
        if(propagate_bt(x, y))
        {
            if(tot == n*n) return true;
            if(solver_bt()) return true;
        }
        else fails++;

        unplace(x, y, val);
    }

    return false;
}

bool solver_fc()
{
    assert(tot < n*n);

    int x, y;
    brelaz(x, y);

    for(int val = 1; val <= n; val++)
    {
        if(cant_place[x][y][val]) continue;

        place(x, y, val);
        nodes_visited++;
        
        if(propagate_fc(x, y))
        {
            if(tot == n*n) return true;
            if(solver_fc()) return true;
        }
        else fails++;

        propagate_fc(x, y, true);
        unplace(x, y, val);
    }

    return false;
}

int main()
{
    freopen(input_file, "r", stdin);

    parse_input();

    if(!is_valid())
    {
        printf("Invalid input\n");
        return 0;
    }

    init_basic();
    if(tot == n*n)
    {
        printf("Already completed\n");
        return 0;
    }

    init_fc();

    bool res = solver_fc();

    if(res)
    {
        assert(is_valid() == 2);

        print_board();
        printf("Number of nodes visited = %lld\n", nodes_visited);
        printf("Number of fails = %lld\n", fails);
    }
    else printf("Bhul\n");

    return 0;
}