vector<Point> PreProcess(vector<Point>&P,int range)
{
    vector<vector<int>> table(range+1,vector<int>(range+1));
    vector<Point> PCHP;
    for (Point &p : P)
        ++table[p.x][p.y];
    for (int i = 1; i <= range; i++)
        for (int j = 1; j <= range; j++)
            table[i][j] += table[i - 1][j] + table[i][j - 1] - table[i - 1][j - 1];
    auto Sum = [&](int x1, int y1, int x2, int y2) -> int {
        return table[x2][y2] - table[x2][y1 - 1] - table[x1 - 1][y2] + table[x1 - 1][y1 - 1];
    };
    for (Point &p : P)
    {
        int x = p.x, y = p.y;
        if (!Sum(x, y + 1, range, range) || !Sum(1, y, x - 1, range) || !Sum(x + 1, 1, range, y) || !Sum(1, 1, x, y-1))
            PCHP.push_back(p);
    }
    return PCHP;
}