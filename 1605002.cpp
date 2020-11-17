#include<bits/stdc++.h>
using namespace std;
#define M 105

char input_file[] = "csp-task/data/d-10-09.txt.txt";

int n;
int grid[M][M];
int row_cnt[M], row_ald[M][M];
int col_cnt[M], col_ald[M][M];
int dom_size[M][M];
int tot;

int nodes_visited, fails;

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
    printf("%d\n", n);
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++) printf("%d ", grid[i][j]);
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

void init_bt() {}

void sdf(int &x, int &y)
{
    int mn = n+1;

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(grid[i][j]) continue;

            if(dom_size[i][j] < mn) mn = dom_size[i][j], x = i, y = j;
        }
    }
}

bool propagate_bt(int x, int y)
{
    int val = grid[x][y];
    return (row_ald[x][val] == 1 && col_ald[y][val] == 1);
}

bool solver_bt()
{
    assert(tot < n*n);

    int x, y;
    sdf(x, y);

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

    init_bt();

    bool res = solver_bt();

    if(res)
    {
        assert(is_valid() == 2);

        print_board();
        printf("Number of nodes visited = %d\n", nodes_visited);
        printf("Number of fails = %d\n", fails);
    }
    else printf("Bhul\n");

    return 0;
}