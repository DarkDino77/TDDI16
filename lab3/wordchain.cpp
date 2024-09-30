#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>


using std::cout;
using std::endl;
using std::queue;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// Typ som används för ordlistan. Den definieras med en typedef här så att du enkelt kan ändra
// representationen av en ordlista utefter vad din implementation behöver. Funktionen
// "read_questions" skickar ordlistan till "find_shortest" och "find_longest" med hjälp av denna
// typen.
typedef unordered_set<string> Dictionary;

/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */

vector<string> find_neigbohrs(const Dictionary &dict, const string &word, unordered_set<string> &visited)
{
    vector<string> neigbohrs;
    for (size_t i = 0; i < word.size(); i++)
    {
        for (char c = 'a'; c <= 'z'; c++)
        {
            string new_word = word;
            new_word[i] = c;
            if (dict.find(new_word) != dict.end() && new_word != word && !visited.count(new_word))
            {
                neigbohrs.push_back(new_word);
            }
        }
    }
    return neigbohrs;
}

vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to)
{
    vector<string> result;
    if (from == to)
    {
        result.push_back(from);
        return result;
    }
    if (dict.find(to) == dict.end())
    {
        return result;
    }
    unordered_set<string> visited;
    queue<vector<string>> to_visit;
    visited.insert(from);

    to_visit.push({from});

    while (!to_visit.empty())
    {
        const vector<string> current  = to_visit.front();
        to_visit.pop();
        if (current.back() == to)
        {
            return current;
        }
        for(string next : find_neigbohrs(dict, current.back(), visited))
        {
                visited.insert(next);
                vector<string> newpath = current;
                newpath.push_back(next);
                to_visit.push(newpath);
        }

    }

    return result;
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'. Returvärdet är den
 * ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(const Dictionary &dict, const string &word)
{
    vector<string> result(1, word);
      
    if (dict.find(word) == dict.end())
    {
        return result;
    }

    unordered_set<string> visited;
    queue<vector<string>> to_visit;

    visited.insert(word);
    to_visit.push({word});

    while (!to_visit.empty()) {
        vector<string> current = to_visit.front();
        const vector<string> neighbours = find_neigbohrs(dict, current.back(), visited);
        to_visit.pop();

        for (const string &next : neighbours) {
                visited.insert(next);
                vector<string> new_path = current;

                new_path.push_back(next);
                to_visit.push(new_path);
        }

        if (to_visit.empty())
        {
            std::reverse(current.begin(), current.end());
            return current;
        }
    }
    
    
    return result;
}

/**
 * Läs in ordlistan och returnera den som en vector av orden. Funktionen läser även bort raden med
 * #-tecknet så att resterande kod inte behöver hantera det.
 */
Dictionary read_dictionary()
{
    string line;
    unordered_set<string> result;
    while (std::getline(std::cin, line))
    {
        if (line == "#")
            break;

        result.insert(line);
    }

    return result;
}

/**
 * Skriv ut en ordkedja på en rad.
 */
void print_chain(const vector<string> &chain)
{
    if (chain.empty())
        return;

    vector<string>::const_iterator i = chain.begin();
    cout << *i;

    for (++i; i != chain.end(); ++i)
        cout << " -> " << *i;

    cout << endl;
}

/**
 * Skriv ut ": X ord" och sedan en ordkedja om det behövs. Om ordkedjan är tom, skriv "ingen lösning".
 */
void print_answer(const vector<string> &chain)
{
    if (chain.empty())
    {
        cout << "ingen lösning" << endl;
    }
    else
    {
        cout << chain.size() << " ord" << endl;
        print_chain(chain);
    }
}

/**
 * Läs in alla frågor. Anropar funktionerna "find_shortest" eller "find_longest" ovan när en fråga hittas.
 */
void read_questions(const Dictionary &dict)
{
    string line;
    while (std::getline(std::cin, line))
    {
        size_t space = line.find(' ');
        if (space != string::npos)
        {
            string first = line.substr(0, space);
            string second = line.substr(space + 1);
            vector<string> chain = find_shortest(dict, first, second);

            cout << first << " " << second << ": ";
            print_answer(chain);
        }
        else
        {
            vector<string> chain = find_longest(dict, line);

            cout << line << ": ";
            print_answer(chain);
        }
    }
}

int main()
{
    Dictionary dict = read_dictionary();
    read_questions(dict);

    return 0;
}
