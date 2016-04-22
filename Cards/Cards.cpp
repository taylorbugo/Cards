// Cards.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

class Card {
public:
	enum _Suit { Nothing, Spades, Hearts, Clubs, Diamonds };

private:
	int key;
	int value;
	_Suit Suit;

public:
	Card()
	{
		key = 0;
		value = 0;
		Suit = Nothing;
	}

	Card(int seed)
	{
		key = seed;
		value = (key % 13) + 1;
		switch (key / 14) {
		case 0:
			Suit = Spades;
			break;
		case 1:
			Suit = Hearts;
			break;
		case 2:
			Suit = Clubs;
			break;
		case 3:
			Suit = Diamonds;
			break;
		}
	}

	string OutputStringCard()
	{
		string output = "";
		switch (value) {
		case 13:
			output = "King";
			break;
		case 12:
			output = "Queen";
			break;
		case 11:
			output = "Jack";
			break;
		case 1:
			output = "Ace";
			break;
		default:
			output = to_string(value);
			break;
		}
		switch (key / 14) {
		case 0:
			output += " of Spades";
			break;
		case 1:
			output += " of Hearts";
			break;
		case 2:
			output += " of Clubs";
			break;
		case 3:
			output += " of Diamonds";
			break;
		}
		return output;
	}

	_Suit GetSuit()
	{
		return Suit;
	}

	int GetValue()
	{
		return value;
	}
};

class Hand {
public:
	Card* Cards;
	enum Ranking { Nothing, HighCard, Flush, Pair};

private:
	Ranking Rank;

public:
	string OutputStringHand()
	{
		string output = "";
		output = Cards[0].OutputStringCard() + ", " + Cards[1].OutputStringCard();
		return output;
	}

	//default constructor
	Hand()
	{
		Rank = Nothing;
		Cards = new Card[2];
	}

	//destructor to destroy cards array
	~Hand()
	{
		delete[] Cards;
	}

	//would look at updating the hand rank at a different time, perhaps as part of a constructor that takes two card objects or an array to fill
	//the hand with. Currently would have to be called separately or during the comparison operators which seems awkward.
	Ranking GetRank()
	{
		if (Cards[0].GetValue() == Cards[1].GetValue())
		{
			Rank = Pair;
		}
		else if (Cards[0].GetSuit() == Cards[1].GetSuit())
		{
			Rank = Flush;
		}
		else if ((Cards[0].GetValue() != 0) && (Cards[1].GetValue() != 0) && (Cards[0].GetSuit() != Nothing) && (Cards[1].GetSuit() != Nothing))
		{
			Rank = HighCard;
		}
		else
		{
			Rank = Nothing;
		}
		return Rank;
	}

	//would look at updating the hand rank at a different time, perhaps as part of a constructor that takes two card objects or an array to fill
	//the hand with. Currently would have to be called separately or during the comparison operators which seems awkward.
	int GetTopCard()
	{
		if (Cards[0].GetValue() >= Cards[1].GetValue())
		{
			return Cards[0].GetValue();
		}
		else
		{
			return Cards[1].GetValue();
		}
	}
};

//overridden operator to handle comparision as far as this game is concerned, looks at rank first, and then if rank the same then highest card
//if tied for highest card in hand the game considers this a tie, the second lower card is not considered.
bool operator> (Hand &A, Hand &B)
{
	//member functions were called outside of return statement to avoid early exit from boolean comparisons
	Hand::Ranking rankA = A.GetRank();
	Hand::Ranking rankB = B.GetRank();
	int topA = A.GetTopCard();
	int topB = B.GetTopCard();
	return ((rankA > rankB) || ((rankA == rankB) && (topA > topB)));
}

int main()
{
	int input = 0;
	int *deck = nullptr;
	int *storage = nullptr;
	int remaining = 0;
	int location = 0;
	int offset = 0;
	Hand HandA;
	Hand HandB;
	cout << "Welcome to Cards" << endl << endl << "Please enter the number of hands you want to play or enter a number < 1 to exit" << endl << endl << ">";
	cin >> input;
	while (input > 0)
	{
		if (input >= 1)
		{
			//creates the deck for the game
			//the deck could also potentially be made into a class and reused in a few places with functions such as shuffle, draw, etc.
			cout << endl << "Shuffling Deck" << endl;
			deck = new int[52];
			for (int index = 0; index < 52; index++)
			{
				deck[index] = (index + 1);
			}
			remaining = 52;
			cout << "Deck Shuffled" << endl << endl;

			while ((input > 0) && (remaining >= 4))
			{
				for (int cardA = 0; cardA < 2; cardA++)
				{
					//only pseudo random again, reused the code from Shuffle but with some changes specific to this card game.
					location = (rand() % remaining);
					HandA.Cards[cardA] = Card(deck[location]);
					if (remaining > 1)
					{
						storage = new int[remaining - 1];
						for (int place = 0; place < (remaining - 1); place++)
						{
							if (place == location)
							{
								offset = 1;
							}
							storage[place] = deck[place + offset];
						}
					}
					delete[] deck;
					deck = storage;
					remaining--;
					offset = 0;
				}
				cout << "Hand A: " << HandA.OutputStringHand() << endl;
				for (int cardB = 0; cardB < 2; cardB++)
				{
					//only pseudo random again, reused the code from Shuffle but with some changes specific to this card game.
					location = (rand() % remaining);
					HandB.Cards[cardB] = Card(deck[location]);
					if (remaining > 1)
					{
						storage = new int[remaining - 1];
						for (int spot = 0; spot < (remaining - 1); spot++)
						{
							if (spot == location)
							{
								offset = 1;
							}
							storage[spot] = deck[spot + offset];
						}
					}
					delete[] deck;
					deck = storage;
					remaining--;
					offset = 0;
				}
				cout << "Hand B: " << HandB.OutputStringHand() << endl;
				input--;
				if (HandA > HandB)
				{
					cout << "Hand A is better, Hand A wins!" << endl << endl;
				}
				else if (HandB > HandA)
				{
					cout << "Hand B is better, Hand B wins!" << endl << endl;
				}
				else
				{
					cout << "Hands are equal, it's a tie!" << endl << endl;
				}
			}
		}
		if (input == 0)
		{
			cout << "Please enter the number of hands you want to play or enter a number < 1 to exit" << endl << endl << ">";
			cin >> input;
		}
	}
	cout << endl << "Goodbye" << endl;
	return 0;
}

