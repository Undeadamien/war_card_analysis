#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

// Initialize random engine
int seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine rng(seed);

void printResults(std::vector<int> results)
{
    for (int result : results)
    {
        std::cout << result << " ";
    }
}

void fill_decks(std::vector<int> &cards, std::vector<int> &b_deck, std::vector<int> &r_deck)
{
    for (int i = 0; i < 52; i++)
    {
        cards.push_back(i % 14 + 2);
    }
    std::shuffle(cards.begin(), cards.end(), rng);
    b_deck = std::vector<int>(cards.begin(), cards.begin() + 26);
    r_deck = std::vector<int>(cards.begin() + 26, cards.end());
}

int simulateGame()
{

    int turn = 1;
    std::vector<int> cards, r_deck, b_deck, r_pile, b_pile;
    fill_decks(cards, r_deck, b_deck);

    while (true)
    {
        if (r_pile.size() % 2 == 0 && b_pile.size() % 2 == 0)
        {
            if (r_deck.empty() || b_deck.empty())
            {
                r_deck.insert(r_deck.begin(), r_pile.begin(), r_pile.end());
                b_deck.insert(b_deck.begin(), b_pile.begin(), b_pile.end());
                r_pile.clear();
                b_pile.clear();
            }
            else
            {
                r_pile.push_back(r_deck.back());
                b_pile.push_back(b_deck.back());
                r_deck.pop_back();
                b_deck.pop_back();
            }
            continue;
        }

        turn++;
        // equality
        if (r_pile.back() == b_pile.back())
        {
            if (r_deck.empty() || b_deck.empty())
            {
                r_deck.insert(r_deck.begin(), r_pile.begin(), r_pile.end());
                b_deck.insert(b_deck.begin(), b_pile.begin(), b_pile.end());
                r_pile.clear();
                b_pile.clear();
            }
            else
            {
                r_pile.push_back(r_deck.back());
                b_pile.push_back(b_deck.back());
                r_deck.pop_back();
                b_deck.pop_back();
            }
        }
        // red won
        else if (r_pile.back() > b_pile.back())
        {
            r_pile.insert(r_pile.end(), b_pile.begin(), b_pile.end());
            std::shuffle(r_pile.begin(), r_pile.end(), rng);
            r_deck.insert(r_deck.begin(), r_pile.begin(), r_pile.end());
            b_pile.clear();
            r_pile.clear();
        }
        // black won
        else if (r_pile.back() < b_pile.back())
        {
            b_pile.insert(b_pile.end(), r_pile.begin(), r_pile.end());
            std::shuffle(b_pile.begin(), b_pile.end(), rng);
            b_deck.insert(b_deck.begin(), b_pile.begin(), b_pile.end());
            r_pile.clear();
            b_pile.clear();
        }
        // check for victory
        if (r_deck.size() == 52 || b_deck.size() == 52)
        {
            return turn;
        }
    }
}

void saveResults(std::vector<int> &results)
{
    std::ofstream myfile;
    myfile.open("results.csv");

    myfile << "turn\n";
    for (int result : results)
    {
        myfile << result << "\n";
    };
    myfile.close();
}

int askSampleSize()
{
    int number;

    std::cout << "Please enter the number of game to simulate: ";
    std::cin >> number;

    return number;
}

int main()
{
    std::vector<int> results;

    int currentGame = 0;
    int numberGame = askSampleSize();

    std::cout << "Simulating...\n";
    while (currentGame < numberGame)
    {
        int result = simulateGame();

        results.push_back(result);
        currentGame++;
    };

    saveResults(results);
    std::cout << "Done";
    return 0;
};