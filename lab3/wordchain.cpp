#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <memory>


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

struct Node {
    Node(const std::string &w, std::shared_ptr<Node> prev = nullptr)
        : word{w},
          previous{prev}
    {}
    
    std::string              word;
    std::shared_ptr<Node>    previous;
};

/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */
vector<string> find_neighbours(const Dictionary &dict, const string &word, const unordered_set<string> &visited)
{
    vector<string> result;
    
    for (size_t i = 0; i < word.size(); i++)
    {
        for (char c = 'a'; c <= 'z'; c++)
        {
            string new_word = word;
            new_word[i] = c;
            
            if (dict.count(new_word) && new_word != word && !visited.count(new_word))
            {
                result.push_back(new_word);
            }
        }
    }

    return result;
}

vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to)
{
    if (from == to || !dict.count(to))
    {
        return {from};
    }
    
    unordered_set<string> visited;
    queue<std::shared_ptr<Node>> to_visit;
    
    visited.insert(from);
    to_visit.push(std::make_shared<Node>(from));

    std::shared_ptr<Node> current = to_visit.front();
    
    while (current->word != to && !to_visit.empty())
    {
        current = to_visit.front();
        to_visit.pop();

        vector<string> neighbours = find_neighbours(dict, current->word, visited);
        
        for(const std::string &neighbour : neighbours)
        {
            visited.insert(neighbour);
            to_visit.emplace(std::make_shared<Node>(neighbour, current));
        }
    }

    // If search terminated before reaching the word, there is no path between the words
    if (current->word != to) {
        return {};
    }
    
    // Destination found, now traverse path backwards
    vector<string> result;
    const Node *curr_node = current.get();
           
    while (curr_node->word != from) {
        result.push_back(curr_node->word);
        curr_node = curr_node->previous.get();
    }

    result.push_back(from);
    std::reverse(result.begin(), result.end());

    return result;
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'. Returvärdet är den
 * ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(const Dictionary &dict, const string &word)
{

    unordered_set<string> visited;
    queue<std::shared_ptr<Node>> to_visit;

    visited.insert(word);
    to_visit.push(std::make_shared<Node>(word));
    std::shared_ptr<Node> current = to_visit.front();

    while (!to_visit.empty()) {
        current = to_visit.front();
        to_visit.pop();

        vector<string> neighbours = find_neighbours(dict, current->word, visited);
        
        for (const std::string &neighbour : neighbours) {
            visited.insert(neighbour);
            to_visit.emplace(std::make_shared<Node>(neighbour, current));
        }
    }

    vector<string> result;
    const Node *current_node = current.get();
    
    while (current_node->word != word) {
        result.push_back(current_node->word);
        current_node = current_node->previous.get();
    }

    result.push_back(word);
    
    return result;
}

// GAMLA LÖSNINGEN
#if 0
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
        result = to_visit.front();
        const vector<string> neighbours = find_neigbohrs(dict, result.back(), visited);
        to_visit.pop();

        for (const string &next : neighbours) {
                visited.insert(next);
                vector<string> new_path = result;

                new_path.push_back(next);
                to_visit.push(new_path);
        }
    }
    
    
    std::reverse(result.begin(), result.end());
    return result;
}
#endif

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
