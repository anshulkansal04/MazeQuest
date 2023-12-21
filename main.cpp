#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <ctime>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <cstdlib>
using namespace std;

int score = 0;
static int lives = 3;
vector<vector<char>> maze;

char u[15], ps[11], r1 = 201, r2 = 200, r3 = 188, r4 = 187, r5 = 205, r6 = 186, r7 = 220,
                    r8 = 202, r9 = 204, r10 = 203, r11 = 206, r12 = 185;

void printHeader()
{
    char u[15] = {r5, r9, r5, r5, r5, r5, r5, r5, r5, r5, r5, r5, r5, r5, r5}; // 'U' in ASCII art
    char ps[11] = {r1, r6, r6, r6, r10, r5, r5, r5, r5, r5, r5};               // 'PS' in ASCII art
    for (int i = 0; i < 89; i++)
        cout << r7;

    cout << "\n\n\t\t " << r1;
    for (int i = 1; i < 50; i++)
        cout << r5;
    cout << r4 << endl;
    cout << "\t\t " << r6 << "  WELCOME TO MazeQuest: A Graph-Based Adventure  " << r6 << endl;
    cout << "\t\t " << r2;
    for (int i = 1; i < 50; i++)
        cout << r5;
    cout << r3 << endl;

    for (int i = 0; i < 89; i++)
        cout << r7;
}

class Graph
{
public:
    int nodes;
    vector<vector<int>> adj;
    int sp;
    int lp;

    Graph(int n)
    {
        sp = 0;
        this->nodes = n;
        adj = vector<vector<int>>(n * n);
    }

    void redef_graph(int n1)
    {
        sp = 0;
        nodes = n1;
        adj = vector<vector<int>>(nodes * nodes);
    }

    void init()
    {
        adj = vector<vector<int>>(nodes * nodes);
    }

    void checkEdge()
    {
        for (int i = 0; i < nodes; i++)
        {
            for (int j = 0; j < nodes; j++)
            {
                edge(i, j);
            }
        }
    }

    void add_edge(int org, int dest)
    {
        adj[org].push_back(dest);
    }

    void edge(int i, int j)
    {
        if (maze[i][j] != 'X')
        {
            if (i > 0 && maze[i - 1][j] != 'X')
            {
                add_edge((i * nodes) + j, (i - 1) * nodes + j);
            }
            if (i < nodes - 1 && maze[i + 1][j] != 'X')
            {
                add_edge((i * nodes) + j, (i + 1) * nodes + j);
            }
            if (j > 0 && maze[i][j - 1] != 'X')
            {
                add_edge((i * nodes) + j, (i * nodes) + (j - 1));
            }
            if (j < nodes - 1 && maze[i][j + 1] != 'X')
            {
                add_edge((i * nodes) + j, (i * nodes) + (j + 1));
            }
        }
    }

    int minEdgeBFS(int u, int v)
    {
        vector<bool> visited(nodes * nodes, false);
        vector<int> distance(nodes * nodes, 0);
        queue<int> Q;
        distance[u] = 0;
        Q.push(u);
        visited[u] = true;
        while (!Q.empty())
        {
            int x = Q.front();
            Q.pop();
            for (int i = 0; i < adj[x].size(); i++)
            {
                if (visited[adj[x][i]])
                    continue;
                distance[adj[x][i]] = distance[x] + 1;
                Q.push(adj[x][i]);
                visited[adj[x][i]] = true;
            }
        }
        return distance[v];
    }
};

class MazeGame
{
public:
    string player;
    stack<pair<int, int>> S;
    int n, steps;

    void randomObstruction(double percent)
    {
        int c = (percent * n * n);
        while (c > 0)
        {
            int i = rand() % n;
            int j = rand() % n;
            if (maze[i][j] != 'X' && maze[i][j] != '#' && (i != 0 || j != 0))
            {
                maze[i][j] = 'X';
                c--;
            }
        }
    }

    MazeGame(int n, string name)
    {
        this->n = n;
        player = name;
        steps = 0;
        maze = vector<vector<char>>(n, vector<char>(n, ' '));
        maze[0][0] = '@';
        maze[n - 1][n - 1] = '#';
    }

    void redefine(int n1)
    {
        n = n1;
        steps = 0;
        maze = vector<vector<char>>(n, vector<char>(n, ' '));
        maze[0][0] = '@';
        maze[n - 1][n - 1] = '#';
    }

    void drawMaze()
    {
        int i, j = 0;
        cout << "\n\n\n";
        for (i = 0; i < n; i++)
        {
            cout << "\t   " << (i + 1) << "  ";
        }
        cout << "\n";
        for (i = 0; i < n; i++)
        {
            cout << "\t  ___  ";
        }
        cout << "\n";
        while (j < n)
        {
            for (i = 0; i < n; i++)
            {
                if (maze[i][j] != ' ')
                    printf("\t| _%c_ |", maze[i][j]);
                else
                    cout << "\t| ___ |";
            }
            cout << "\n";
            j++;
        }
        cout << "\n\n\n\n\n";
    }

    void move(int x, int y)
    {
        pair<int, int> K;
        char ch;
        do
        {
            cout << "\nUP....'W'";
            cout << "\nLEFT....'A'";
            cout << "\nDOWN....'S'";
            cout << "\nRIGHT....'D'";
            cout << "\nUNDO....'U'";
            cout << "\nQUIT....'Q'";
            cout << "\n\t\t\t\t Your Lives:- " << lives;
            cout << "\nEnter Your Move:- ";
            cin >> ch;
            ch = toupper(ch);
            switch (ch)
            {
            case 'W':
            {
                if (y > 0 && maze[x][y - 1] != 'X' && maze[x][y - 1] != '#')
                {
                    maze[x][y] = '_';
                    if (!S.empty())
                    {
                        K = S.top();
                    }
                    S.push(make_pair(x, y));
                    y -= 1;
                    if ((K.first == x) && (K.second == y))
                    {
                        lives--;
                    }
                    maze[x][y] = '@';
                    steps++;
                }
                else if (maze[x][y - 1] == '#')
                {
                    cout << "\n\t\t\tHURRAY!! Level Cleared !!";
                    y -= 1;
                    break;
                }
                else
                    cout << "\nCan't Move up";

                if (lives == 0)
                {
                    cout << "\n"
                         << player << " Your Steps are:- " << steps;
                    cout << "\n"
                         << player << " Your score is:- " << score;
                         getch();
                    exit(0);
                }
                break;
            }
            case 'A':
            {
                if (x > 0 && maze[x - 1][y] != 'X' && maze[x - 1][y] != '#')
                {
                    maze[x][y] = '_';
                    if (!S.empty())
                    {
                        K = S.top();
                    }
                    S.push(make_pair(x, y));
                    x -= 1;
                    if ((K.first == x) && (K.second == y))
                    {
                        lives--;
                    }
                    maze[x][y] = '@';
                    steps++;
                }
                else if (maze[x - 1][y] == '#')
                {
                    cout << "\n\t\t\tHURRAY!! Level Cleared !!";
                    x -= 1;
                    break;
                }
                else
                    cout << "\nCan't Move left";

                if (lives == 0)
                {
                    cout << "\n"
                         << player << " Your Steps are:- " << steps;
                    cout << "\n"
                         << player << " Your score is:- " << score;
                         getch();
                    exit(0);
                }
                break;
            }
            case 'S':
            {
                if (y >= 0 && y + 1 < n && maze[x][y + 1] != 'X' && maze[x][y + 1] != '#')
                {
                    maze[x][y] = '_';
                    if (!S.empty())
                    {
                        K = S.top();
                    }
                    S.push(make_pair(x, y));
                    y += 1;
                    if ((K.first == x) && (K.second == y))
                    {
                        lives--;
                    }
                    maze[x][y] = '@';
                    steps++;
                }
                else if (maze[x][y + 1] == '#')
                {
                    cout << "\n\t\t\tHURRAY!! Level Cleared !!";
                    y += 1;
                    break;
                }
                else
                    cout << "\nCan't Move Down";

                if (lives == 0)
                {
                    cout << "\n"
                         << player << " Your Steps are:- " << steps + 1;
                    cout << "\n"
                         << player << " Your score is:- " << score;
                         getch();
                    exit(0);
                }
                break;
            }
            case 'D':
            {
                if (x >= 0 && x + 1 < n && maze[x + 1][y] != 'X' && maze[x + 1][y] != '#')
                {
                    maze[x][y] = '_';
                    if (!S.empty())
                    {
                        K = S.top();
                    }
                    S.push(make_pair(x, y));
                    x += 1;
                    if ((K.first == x) && (K.second == y))
                    {
                        lives--;
                    }
                    maze[x][y] = '@';
                    steps++;
                }
                else if (maze[x + 1][y] == '#')
                {
                    cout << "\n\t\t\tHURRAY!! Level Cleared !!";
                    x += 1;
                    break;
                }
                else
                    cout << "\nCan't Move Right";

                if (lives == 0)
                {
                    cout << "\n"
                         << player << " Your Steps are:- " << steps;
                    cout << "\n"
                         << player << " Your score is:- " << score;
                         getch();
                    exit(0);
                }
                break;
            }
            case 'U':
            {
                int i, j;
                if (S.empty() == true)
                {
                    cout << "\nUNDO NOT POSSIBLE";
                    break;
                }
                else
                {
                    pair<int, int> P = S.top();
                    i = P.first;
                    j = P.second;
                    S.pop();
                    maze[x][y] = '_';
                    maze[i][j] = '@';
                    x = i;
                    y = j;
                    steps--;
                }
                if (lives == 0)
                {
                    cout << "\n"
                         << player << " Your Steps are:- " << steps + 1;
                    cout << "\n"
                         << player << " Your score is:- " << score;
                         getch();
                    exit(0);
                }
                else
                    lives--;
                break;
            }
            case 'Q':
            {
                cout << "\n"
                     << player << " Your steps are:- " << steps + 1;
                cout << "\n"
                     << player << " Your score is:- " << score;
                     getch();
                exit(0);
            }
            default:
                cout << "\nInvalid Move. Please enter a valid move.\n";
                break;
            }
            drawMaze();

        } while (maze[x][y] != '#');
    }

    void scoreBoard(int shortest, double percent)
    {
        int longest = (n * n) - (percent * n * n);
        int avg = (longest + shortest) / 2;
        int count = 0;
        if ((steps + 1) > avg)
        {
            score += 40;
        }
        else if ((steps + 1) == avg)
        {
            score += 50;
        }
        else if ((steps + 1) < avg)
        {
            if ((steps + 1) == shortest)
                score += 100;
            else
            {
                count = (steps + 1) - shortest;
                if (count == 1)
                    score += 95;
                else if (count == 2)
                    score += 90;
                else if (count == 3)
                    score += 85;
                else if (count == 4)
                    score += 80;
                else if (count == 5)
                    score += 75;
                else if (count == 6)
                    score += 70;
                else if (count == 7)
                    score += 65;
                else if (count == 8)
                    score += 60;
                else if (count == 9)
                    score += 55;
                else
                    score += 50;
            }
        }
    }
};

class Levels
{
public:
    double level(int size)
    {
        if (size == 8)
            return (0.15);
        if (size == 9)
            return (0.20);
        if (size == 10)
            return (0.25);
        if (size == 11)
            return (0.3);
        if (size == 12)
            return (0.35);
    }
};

void member()
{
    system("cls");
    char x = 2, d = 4;
    cout << "\n\n";
    cout << "\t\t     " << x << " THANK YOU FOR USING MazeQuest: A Graph-Based Adventure \a" << x;

    cout << "\n\n\n\t\t\tPREPARED BY:-";
    cout << "\n\t\t\t" << d << " VISHWAS MISHRA       22103122";
    cout << "\n\t\t\t" << d << " ANSHUL KANSAL        22103135";
    cout << "\n\t\t\t" << d << " LOV KUMAR KUMAWAT    22103150";
    cout << "\n\t\t\t" << d << " DAKSH JAIN           22103151\n\n\n";
    getch();
}

void login()
{
    int a = 0, i = 0;
    char username[10], c = ' ';
    char pword[10];
    char user[10] = "jiit";
    char pass[10] = "62128";
    do
    {
        system("cls");
        printHeader();
        cout << " \n\n\n\t\t-------------------  LOGIN PAGE  -------------------";
        cout << "\n\n                       ENTER USERNAME:-";
        cin >> username;
        cout << "\n\n                       ENTER PASSWORD:-";
        while (i < 10)
        {
            pword[i] = getch();
            c = pword[i];
            if (c == 13)
                break; // if press entered in password,password is till there
            else
                cout << "*"; // for any no. entered show "*"
            i++;
        }
        pword[i] = '\0';
        // at the end appended a null character
        i = 0;
        if (strcmp(username, "jiit") == 0 && strcmp(pword, "62128") == 0)
        {
            cout << "  \n\n\n       WELCOME TO MazeQuest    !!!! LOGIN IS SUCCESSFUL";
            cout << "\n\n\n\t\t\t\tPress any key to continue...";
            getch(); // holds the screen till any key is pressed
            system("cls");
            break;
        }
        else
        {
            cout << "\n        SORRY !!!!  LOGIN IS UNSUCESSFUL";
            if (a <= 1)
                cout << endl
                     << 2 - a << " Login Attempts left \n";
            a++;
            getch();
        }
    } while (a <= 2);
    if (a > 2)
    {
        cout << "\nSorry You Have Exceeded The Login Attempt Limit!!!";
        getch();
        system("cls");
        atexit(member);
        exit(0);
    }
}

int main()
{
    login();
    string name;
    int n = 8;
    double c;
    srand(time(NULL));
    cout << "\n\t Enter your Name:- ";
    cin >> name;

    int counter = 1;
    int sp = 0;
    MazeGame a(n, name);
    Levels l;
    Graph g(n);
    system("cls");
    while (counter <= 5)
    {
        int sp = 0;
        char ch;
        cout << "\n\n\t\t\t\t " << r1;
        for (int i = 1; i < 15; i++)
            cout << r5;
        cout << r4 << endl;
        cout << "\t\t\t\t " << r6 << "   Level " << counter << "    "<< r6 << endl;
        cout << "\t\t\t\t " << r2;
        for (int i = 1; i < 15; i++)
            cout << r5;
        cout << r3 << endl;
        c = l.level(n);

        while (sp == 0)
        {
            a.redefine(n);
            g.redef_graph(n);
            a.randomObstruction(c);
            g.init();
            g.checkEdge();
            sp = g.minEdgeBFS(0, (n * n) - 1);
            if (sp != 0)
            {
                a.drawMaze();
            }
        }
        a.move(0, 0);
        a.scoreBoard(sp, c);
        system("cls");
        cout << "\nYour Total Steps for Level " << counter << " were:- " << (a.steps + 1);
        cout << "\nYour Score:- " << score;
        cout << "\nMin Steps Were:- " << sp << endl
             << endl
             << endl;
        counter++;
        n++;
    }
    cout << "\nYou have won !!";

    return 0;
}