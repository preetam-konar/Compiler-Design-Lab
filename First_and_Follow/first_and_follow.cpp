#include <bits/stdc++.h>
using namespace std;

vector<string> findNonTerminals(vector<string> prods)
{
    set<string> s;
    for (auto i : prods)
    {
        int j = 0, k = 0;
        while (i[++j] != '-')
            ;
        s.insert(i.substr(k, j));
    }
    vector<string> nt(s.begin(), s.end());
    return nt;
}

vector<string> findTerminals(vector<string> prods)
{
    vector<string> nt = findNonTerminals(prods);
    set<string> s;
    for (auto i : prods)
    {
        int j = 0;
        string x = "";
        while (i[j++] != '>')
            ;

        while (i[j] != '\0')
        {
            if (i[j] == '.')
            {
                if (find(nt.begin(), nt.end(), x) == nt.end())
                    s.insert(x);
                x = "";
                j++;
                continue;
            }
            x += i[j];
            j++;
        }
        if (x != "" && find(nt.begin(), nt.end(), x) == nt.end())
            s.insert(x);
    }

    vector<string> t(s.begin(), s.end());
    return t;
}

vector<string> sepProds(vector<string> prods)
{
    vector<string> res;
    for (auto i : prods)
    {
        int j = 0;
        while (i[j++] == '-')
            ;
        string lhs = i.substr(0, j);
        j += 2;
        string temp = i.substr(j);
        j = 0;
        while (temp.find("/") != string::npos)
        {
            string x = temp.substr(j, temp.find("/") - j);
            res.push_back(lhs + "->" + x);
            j = temp.find("/") + 1;
            temp = temp.substr(j);
            j = 0;
        }
        res.push_back(lhs + "->" + temp);
    }
    return res;
}

map<string, vector<vector<string>>> findTokProd(vector<string> prods)
{
    map<string, vector<vector<string>>> res;
    for (auto i : prods)
    {
        int j = 0;
        while (i[++j] != '-')
            ;
        string lhs = i.substr(0, j);
        string temp = i.substr(j + 2, i.length() - (j + 1));
        vector<string> toks;
        while (temp.find('.') != string::npos)
        {
            j = 0;
            toks.push_back(temp.substr(j, temp.find('.') - j));
            temp = temp.substr(temp.find('.') + 1, temp.length() - temp.find('.') - 1);
        }
        toks.push_back(temp);
        res[lhs].push_back(toks);
    }
    return res;
}

vector<string> first(string x, map<string, vector<vector<string>>> prods, vector<string> t)
{
    vector<string> vec;
    vector<vector<string>> y = prods[x];
    for (auto i : y)
    {
        if (find(t.begin(), t.end(), i.front()) != t.end())
        {
            if (find(vec.begin(), vec.end(), i.front()) == vec.end())
                vec.push_back(i.front());
            continue;
        }
        vector<string> z = first(i.front(), prods, t);

        for (int p = 1; p < i.size() && find(z.begin(), z.end(), "eps") != z.end(); p++)
        {
            z.erase(find(z.begin(), z.end(), "eps"));
            if (find(t.begin(), t.end(), i.at(p)) != t.end())
            {
                z.push_back(i.at(p));
            }
            else
            {
                vector<string> m;
                m = first(i.at(p), prods, t);
                // cout << 1;
                for (auto k : m)
                {
                    if (find(z.begin(), z.end(), k) == z.end())
                        z.push_back(k);
                }
            }
        }
        for (auto j : z)
        {
            if (find(vec.begin(), vec.end(), j) == vec.end())
                vec.push_back(j);
        }
    }
    return vec;
}

map<string, vector<string>> findFirst(map<string, vector<vector<string>>> prods, vector<string> t)
{
    map<string, vector<string>> mp;
    for (auto i : prods)
    {
        mp[i.first] = first(i.first, prods, t);
    }
    return mp;
}

vector<string> follow(string x, string start, map<string, vector<vector<string>>> prods, vector<string> t)
{
    map<string, vector<string>> mpf = findFirst(prods, t);
    vector<string> vec;
    if (x == start)
    {
        vec.push_back("$");
    }

    for (auto i : prods)
    {
        for (auto j : i.second)
        {
            int h=0;
            while (h<j.size() && find(j.begin()+h, j.end(), x) != j.end())
            {
                int m = find(j.begin()+h, j.end(), x) - j.begin();
                if (m + 1 < j.size())
                {
                    m = m + 1;
                    if (find(t.begin(), t.end(), j[m]) != t.end() && j[m] != "eps")
                    {
                        vec.push_back(j[m]);
                    }
                    else
                    {
                        /*
                        "S->id.y.z/A.B/c"
                        "A->x/B.e/eps")
                        "B->z/eps")
                        */
                        vector<string> temp = mpf[j[m]];
                        while (m < j.size() && find(temp.begin(), temp.end(), "eps") != temp.end())
                        {
                            temp.erase(find(temp.begin(), temp.end(), "eps"));
                            if (find(t.begin(), t.end(), j[m]) != t.end())
                            {
                                temp.push_back(j[m]);
                                break;
                            }
                            for (auto l : mpf[j[m]])
                            {
                                if (find(temp.begin(), temp.end(), l) == temp.end())
                                    temp.push_back(l);
                            }
                            m++;
                            // cout<<k<<endl;
                        }
                        if (find(temp.begin(), temp.end(), "eps") != temp.end())
                        {
                            temp.erase(find(temp.begin(), temp.end(), "eps"));
                            vector<string> temp2 = follow(i.first, start, prods, t);
                            for (auto l : temp2)
                                temp.push_back(l);
                        }
                        for (auto q : temp)
                        {
                            if (find(vec.begin(), vec.end(), q) == vec.end())
                                vec.push_back(q);
                        }
                    }
                }
                else
                {
                    map<string, vector<vector<string>>> mp(prods.begin(), prods.end());
                    if (i.first == x)
                    {
                        mp.erase(mp.find(i.first));
                    }
                    vector<string> temp = follow(i.first, start, mp, t);
                    for (auto k : temp)
                    {
                        if (find(vec.begin(), vec.end(), k) == vec.end())
                            vec.push_back(k);
                    }
                }
                h++;
            }
        }
    }
    return vec;
}

map<string, vector<string>> findFollow(string start, map<string, vector<vector<string>>> prods, vector<string> t)
{
    map<string, vector<string>> mp;
    for (auto i : prods)
    {
        mp[i.first] = follow(i.first, start, prods, t);
    }
    return mp;
}

int main()
{
    vector<string> prod;
    int n;
    /*
    for (int i=0;i<n; i++) {
        string x;
        cin>>x;
        prod.push_back(x);
    }
    */

    cout<<"Enter number of productions: ";
    cin>>n;
    cout<<"Enter productions: \n";
    for(int i=0;i<n;i++) {
        string x;
        cin>>x;
        prod.push_back(x);
    }
    string startSymbol = "S";
    cout<<"Enter start symbol: ";
    cin>>startSymbol;

    // prod.push_back("S->A.A");
    // prod.push_back("A->a.A/b");
    // prod.push_back("B->z/eps");

    // prod.push_back("S->A.C.B/C.b.b/B.a");
    // prod.push_back("A->d.a/B.C");
    // prod.push_back("B->g/eps");
    // prod.push_back("C->h/eps");

    // prod.push_back("S->A.B.c");
    // prod.push_back("A->a/eps");
    // prod.push_back("B->eps");

    /*
    vector<string> t = findTerminals(prod);
    for (auto i : t)
        cout << i << "\n";
    */
    vector<string> separated_prods = sepProds(prod);
    // for (auto x : separated_prods)
    // {
    //     cout << x << "\n";
    // }
    map<string, vector<vector<string>>> mp = findTokProd(separated_prods);

    // for (auto i : mp)
    // {
    //     cout << i.first << "->";
    //     for (auto j : i.second)
    //     {
    //         for (auto k : j)
    //         {
    //             cout << k << " ";
    //         }
    //     }
    //     cout << endl;
    // }

    vector<string> terminals = findTerminals(sepProds(prod));
    map<string, vector<string>> mp_first = findFirst(mp, terminals);
    cout << "\n\nFIRST\n\n";
    for (auto i : mp_first)
    {
        cout << i.first << ": ";
        for (auto j : i.second)
        {
            cout << j << " ";
        }
        cout << "\n";
    }
    // vector<string> te = follow("B", mp, terminals);
    cout << "\n\nFOllOW\n\n";
    map<string, vector<string>> mp_follow = findFollow(startSymbol, mp, terminals);
    for (auto i : mp_follow)
    {
        cout << i.first << ": ";
        for (auto j : i.second)
        {
            cout << j << " ";
        }
        cout << endl;
    }

    return 0;
}
