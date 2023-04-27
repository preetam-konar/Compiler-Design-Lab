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
            int h = 0;
            while (h < j.size() && find(j.begin() + h, j.end(), x) != j.end())
            {
                int m = find(j.begin() + h, j.end(), x) - j.begin();
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

map<string, vector<vector<string>>> makeAugmented(string start, map<string, vector<vector<string>>> prods)
{
    map<string, vector<vector<string>>> mp;
    string s = "S\'";
    mp[s] = {{".", start}};
    for (auto i : prods)
    {
        for (auto j : i.second)
        {
            vector<string> temp(j.begin(), j.end());
            temp.insert(temp.begin(), ".");
            mp[i.first].push_back(temp);
        }
    }
    return mp;
}

map<string, vector<vector<string>>> findClosure(map<string, vector<vector<string>>> prod, map<string, vector<vector<string>>> prods, vector<string> nt)
{
    map<string, vector<vector<string>>> mp(prod.begin(), prod.end());
    for (auto i : prod)
    {
        for (auto j : i.second)
        {
            auto m = find(j.begin(), j.end(), ".");
            if (m != j.end() || m - j.begin() != j.size() - 1)
            {
                int n = m - j.begin() + 1;
                if (find(nt.begin(), nt.end(), j[n]) != nt.end())
                {
                    vector<vector<string>> temp = prods[j[n]];
                    // for(auto l: temp) {
                    //     for(auto te: l) cout<<te<<" ";
                    //     cout<<"\n";
                    // }
                    map<string, vector<vector<string>>> temp2 = findClosure({{j[n], temp}}, prods, nt);
                    /*
                    for (auto l : temp2)
                    {
                        for (auto te : l.second)
                        {
                            for (auto tx : te)
                                cout << tx << " ";
                            cout << endl;
                        }
                    }
                    */
                    map<string, vector<vector<string>>> mp2(mp.begin(), mp.end());
                    for (auto k : temp2)
                    {
                        vector<vector<string>> z = k.second;
                        vector<vector<string>> y = mp[k.first];
                        for (auto l : z)
                        {
                            int flag = 0;
                            for (auto h : y)
                            {
                                if (l == h)
                                {
                                    flag = 1;
                                    break;
                                }
                            }
                            if (!flag)
                                mp2[k.first].push_back(l);
                        }
                    }
                    mp = mp2;
                }
            }
        }
    }
    return mp;
}

set<string> lookAhead(string x, map<string, vector<pair<set<string>, vector<string>>>> prod, map<string, vector<string>> mpf, vector<string> t)
{
    set<string> s;
    vector<string> vec;
    for (auto i : prod)
    {
        for (auto j : i.second)
        {
            if (x == i.first && !j.first.empty())
                return j.first;
            if (find(j.second.begin(), j.second.end(), x) == j.second.end())
                continue;
            int m = find(j.second.begin(), j.second.end(), ".") - j.second.begin() + 1;
            if ((j.second)[m] != x)
                continue;
            if (m == j.second.size() - 1)
            {
                set<string> a = j.first;
                if (a.empty())
                    a = lookAhead(i.first, prod, mpf, t);
                s.insert(a.begin(), a.end());
                continue;
            }
            if (find(t.begin(), t.end(), j.second[m + 1]) != t.end())
            {
                s.insert(j.second[m + 1]);
                continue;
            }
            vec = mpf[j.second[m + 1]];
            s.insert(vec.begin(), vec.end());
            return s;
        }
    }
    return s;
}

map<string, vector<pair<set<string>, vector<string>>>> lrOneItem(map<string, vector<pair<set<string>, vector<string>>>> prod, map<string, vector<vector<string>>> aug, map<string, vector<string>> mpf, vector<string> t, vector<string> nt)
{
    map<string, vector<pair<set<string>, vector<string>>>> res;
    map<string, vector<vector<string>>> arg;
    for (auto x : prod.begin()->second)
    {
        arg[prod.begin()->first].push_back(x.second);
    }

    map<string, vector<vector<string>>> cl = findClosure(arg, aug, nt);
    map<string, vector<pair<set<string>, vector<string>>>> arg2;
    for (auto i : cl)
    {
        for (auto j : i.second)
        {
            if (i.first == prod.begin()->first)
            {
                for (auto x : prod.begin()->second)
                {
                    if (j == x.second)
                    {
                        arg2[i.first] = prod.begin()->second;
                        continue;
                    }
                }
            }
            pair<set<string>, vector<string>> p = {{}, j};
            arg2[i.first].push_back(p);
        }
    }
    for (auto i : cl)
    {
        for (auto j : i.second)
        {
            pair<set<string>, vector<string>> p;
            p = {lookAhead(i.first, arg2, mpf, t), j};
            res[i.first].push_back(p);
        }
    }
    return res;
}

bool checkEqualLrItems(map<string, vector<pair<set<string>, vector<string>>>> a, map<string, vector<pair<set<string>, vector<string>>>> b)
{
    for (auto i : a)
    {
        if (a[i.first] != b[i.first])
            return false;
    }
    return true;
}

pair<string, map<string, vector<pair<set<string>, vector<string>>>>> utilClr(map<string, pair<set<string>, vector<string>>> x, map<string, vector<vector<string>>> aug, map<string, vector<string>> mpf, vector<string> t, vector<string> nt)
{
    string change;
    vector<string> p = (x.begin()->second).second;
    int m = find(p.begin(), p.end(), ".") - p.begin();
    if (m == p.size() - 1)
    {
        map<string, vector<pair<set<string>, vector<string>>>> ret = {{x.begin()->first, {{(x.begin()->second).first, p}}}};
        return {"r", ret};
    }
    change = p[m + 1];
    swap(p[m], p[m + 1]);
    map<string, vector<pair<set<string>, vector<string>>>> n = {{x.begin()->first, {{(x.begin()->second).first, p}}}};
    map<string, vector<pair<set<string>, vector<string>>>> ret = lrOneItem(n, aug, mpf, t, nt);
    return {change, ret};
}

map<pair<int, string>, string> clr(string start, map<string, vector<vector<string>>> aug, map<string, vector<string>> mpf, vector<string> t, vector<string> nt)
{
    map<int, map<string, vector<pair<set<string>, vector<string>>>>> mp;
    map<pair<int, string>, string> res;
    map<string, vector<pair<set<string>, vector<string>>>> temp;
    temp[start] = {{{"$"}, aug[start].front()}};
    map<string, vector<pair<set<string>, vector<string>>>> temp2 = lrOneItem(temp, aug, mpf, t, nt);
    queue<pair<int, map<string, pair<set<string>, vector<string>>>>> q;
    for (auto i : temp2)
    {
        for (auto j : i.second)
        {
            q.push({0, {{i.first, j}}});
        }
    }
    mp[0] = temp2;
    int count = 0;
    while (!q.empty())
    {
        // cout << q.front().first << " ";
        map<string, pair<set<string>, vector<string>>> x = q.front().second;
        pair<string, map<string, vector<pair<set<string>, vector<string>>>>> ut = utilClr(x, aug, mpf, t, nt);
        int flag = 0;
        // cout << ut.first;
        if (ut.first == "r")
        {
            string c = "";
            string r = x.begin()->first + "->";
            for (auto i : (x.begin()->second).second)
            {
                if(i==".") continue;
                r += i;
            }
            for (auto i : ((ut.second).begin()->second).begin()->first)
            {
                res[{q.front().first, i}] = r;
            }
            q.pop();
            // cout << "\n";
            continue;
        }
        for (auto i : mp)
        {

            if (checkEqualLrItems(i.second, ut.second))
            {
                if (find(nt.begin(), nt.end(), ut.first) == nt.end())
                    res[{q.front().first, ut.first}] = {"s" + to_string(i.first)};
                else
                    res[{q.front().first, ut.first}] = {to_string(i.first)};
                flag = 1;
                break;
            }
        }

        if (!flag)
        {
            mp[++count] = ut.second;
            if (find(nt.begin(), nt.end(), ut.first) == nt.end())
                res[{q.front().first, ut.first}] = "s" + to_string(count);
            else
            {
                // cout << " yes";
                res[{q.front().first, ut.first}] = to_string(count);
            }
            for (auto i : ut.second)
            {
                for (auto j : i.second)
                {
                    q.push({count, {{i.first, j}}});
                }
            }
            q.pop();
            // cout<<"\n";
            continue;
        }
        q.pop();
        // cout << "\n";
    }
    return res;
}

int main()
{
    vector<string> prod;
    int n;

    cout << "Enter number of productions: ";
    cin >> n;
    cout<<"Enter productions:\n";
    for (int i = 0; i < n; i++)
    {
        string x;
        cin >> x;
        prod.push_back(x);
    }
    string startSymbol = "S";
    cout << "Enter start symbol: ";
    cin >> startSymbol;
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

    vector<string> terminals = findTerminals(separated_prods);
    vector<string> non_terminals = findNonTerminals(separated_prods);
    map<string, vector<string>> mp_first = findFirst(mp, terminals);
    // vector<string> te = follow("B", mp, terminals);
    map<string, vector<string>> mp_follow = findFollow(startSymbol, mp, terminals);
    map<string, vector<vector<string>>> aug = makeAugmented(startSymbol, mp);
    // map<string, vector<vector<string>>> cl = findClosure({{"S'", {{".", "S"}}}}, aug, non_terminals);
    // pair<set<string>, vector<string>> p;
    // p.first.insert("$");
    // p.second.push_back(".");
    // p.second.push_back("A");
    // p.second.push_back("A");
    // vector<pair<set<string>, vector<string>>> vp;
    // vp.push_back(p);
    // map<string, vector<pair<set<string>, vector<string>>>> m;
    // m["S"] = vp;
    // set<string> la = lookAhead("A", {{"S", {p}}}, mp_first, terminals);
    // map<string, vector<pair<set<string>, vector<string>>>> lroi = lrOneItem({{"S", {{{"$"}, {".", "A", "A"}}}}}, aug, mp_first, terminals, non_terminals);

    // for(auto i: cl.first) {
    //     for(auto j: i.second) {
    //         cout<<i.first<<"->";
    //         for(auto k: j) {
    //             cout<<k;
    //         }
    //         cout<<"\n";
    //     }
    // }
    // cout<<"\n";
    // for (auto i : cl)
    // {
    //     for (auto j : i.second)
    //     {
    //         cout << i.first << "->";
    //         for (auto k : j)
    //             cout << k;
    //         cout << "\n";
    //     }
    // }

    // for (auto i : lroi)
    // {
    //     for (auto j : i.second)
    //     {
    //         cout << i.first << "->";
    //         for (auto k : j.second)
    //         {
    //             cout << k;
    //         }
    //         cout << "\tLA: ";
    //         for (auto k : j.first)
    //             cout << k;
    //         cout << "\n";
    //     }
    // }
    // for (auto x : la)
    // {
    //     cout << x << "\n";
    // }

    map<pair<int, string>, string> res = clr("S'", aug, mp_first, terminals, non_terminals);
    // for (auto i : res)
    // {
    //     cout << "(" << i.first.first << "," << i.first.second << ")"
    //          << " : " << i.second << "\n";
    // }

    sort(terminals.begin(), terminals.end());
    sort(non_terminals.begin(), non_terminals.end());
    terminals.push_back("$");
    int m = -1;
    for (auto x : res)
    {
        if (m < x.first.first)
            m = x.first.first;
    }

    cout << "\t\t";
    for (auto i : terminals)
    {
        cout << i << "\t\t";
    }
    for (auto i : non_terminals)
    {
        cout << i << "\t\t";
    }
    cout << "\n";
    for (int i = 0; i <= m; i++)
    {
        cout << "I" << i << "\t\t";
        for (auto j : terminals)
        {
            if (res[{i, j}] == "S'->" + startSymbol)
                cout << "accept"
                     << "\t\t";
            else
                cout << res[{i, j}] << "\t\t";
        }
        for (auto j : non_terminals)
        {
            cout << res[{i, j}] << "\t\t";
        }
        cout << "\n";
    }

    return 0;
}
