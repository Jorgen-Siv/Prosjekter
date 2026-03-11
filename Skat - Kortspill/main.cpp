#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <iterator>

//List of suits and ranks for later use
const char SUITS[4] = { 'd','h','s', 'c' };
const char RANKS[8] = { '7','8','9','Q','K','T','A','J' };
//Enum classes for suits, ranks and gamemodes
enum class Suit {
	DIAMOND,
	HEART,
	SPADE,
	CLUB
};
enum class Rank {
	SEVEN,
	EIGHT,
	NINE,
	QUEEN,
	KING,
	TEN,
	ACE,
	JACK
};
enum class GameType {
	DIAMONDS,
	HEARTS,
	SPADES,
	CLUBS,
	GRAND,
	NONE
};


//Card class: Holds a rank and suit
//Prints suit-rank if a Card object is printed, e.g 'c7' which is seven of clubs
class Card {
private:
	Rank rank;
	Suit suit;
public:
	Card() {
		rank = Rank::SEVEN;
		suit = Suit::DIAMOND;
	}
	Card(Suit s, Rank r) {
		rank = r;
		suit = s;
	}
	friend std::ostream& operator<<(std::ostream& out, const Card& c) {
		out << SUITS[static_cast<int>(c.suit)];
		out << RANKS[static_cast<int>(c.rank)];
		return out;
	};

};
//Hand class: Holds a vector of cards
//Has functitons to add and remove cards from vector, get a specific card in the vector and get handsize
//Prints all cards in succesion if printed
//TODO: Add sort function for cards based on Gamemode
class Hand {
private:
	std::vector<Card> cards;
public:
	Hand() {};
	void addCard(Card c) {

		cards.push_back(c);
	}
	Card getCard(size_t number) {
		if (number < 0 || number >= cards.size()) {
			std::cerr << "Invalid Card number";
		}
		return cards[number];
	}
	void removeCard(size_t number) {
		cards.erase(cards.begin() + static_cast<int>(number));
	}
	size_t handSize() {
		return cards.size();
	}
	friend std::ostream& operator<<(std::ostream& out, const Hand& h) {
		for (size_t i = 0; i < h.cards.size(); i++) {
			out << h.cards[i] << " ";
		}
		return out;
	};

};
// Player class, class to hold control of hand, and points for a human player
// Functions to add points and cards, and to play a card from hand
//TODO: Add CPU class with logic to choose which cards to play
class Player {
private:
	int roundpoints;
	int gamepoints;
	Hand hand;
public:
	Player() {
		roundpoints = 0;
		gamepoints = 0;
	}
	void addCard(Card c) {
		hand.addCard(c);
	}
	void addroundPoints(int points) {
		roundpoints += points;
	}
	void addgamePoints(int points) {
		gamepoints += points;
		roundpoints = 0;
	}
	Card playcard() {
		std::cout << "\nYour hand: " << hand << '\n';
		std::cout << "Choose card to play (Card number, starts at 0): ";
		
		int cardnumber{ 0 };
		std::cin >> cardnumber;
		if (0 <= cardnumber && cardnumber < hand.handSize()) {
			Card c{ hand.getCard(static_cast<size_t>(cardnumber)) };
			hand.removeCard(static_cast<size_t>(cardnumber));
			return c;
		}
		else {
			std::cout << "Not valid number, try again!\n";
			return playcard();
		}

	}
};
//Deck class
//Generates a skat deck on construction, can be shuffled and contains a deal funciton to deal cards to players in game
class Deck {
private:
	std::vector<Card> cards;
public:
	Deck() {
		for (int i = 0; i <= static_cast<int>(Suit::CLUB); i++) {
			for (int j = 0; j <= static_cast<int>(Rank::JACK); j++) {
				Card c(static_cast<Suit>(i), static_cast<Rank>(j));
				cards.push_back(c);
			}
		}
	}
	void shuffle() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(cards.begin(), cards.end(), gen);
	}
	void deal(Player& p1, Player& p2, Player& p3) {
		for (size_t i = 0; i < cards.size()-2; i++) {
			if (i % 3 == 0) {
				p1.addCard(cards[i]);
			}
			else if (i % 3 == 1) {
				p2.addCard(cards[i]);
			}
			else {
				p3.addCard(cards[i]);
			}
		}
	}
	friend std::ostream& operator<<(std::ostream& out, const Deck& d) {
		for (size_t i = 0; i < d.cards.size(); i++) {
			out << d.cards[i] << " ";
		}
		return out;
	};
};
//A round is one card from every player
//TODO calculate winner
class Round {
private:
	Player playingPlayer;
	Player secondPlayer;
	Player finalPlayer;
	Player winner;
	GameType gametype;
	int points;
public:
	Round(Player& p1, Player& p2, Player& p3, GameType g) {
		playingPlayer = p1;
		secondPlayer = p2;
		finalPlayer = p3;
		points = 0;
		gametype = g;
	}
	void awardPoints() {
		winner.addgamePoints(points);
	}
	void play() {
		Card c1{ playingPlayer.playcard() };
		std::cout << c1;
		Card c2{ secondPlayer.playcard() };
		std::cout << c2;
		Card c3{ finalPlayer.playcard() };
		std::cout << c3;
	}
};
//Game: 10 rounds. 
//Starts with the betting round which decides who is playing alone and who is teaming up
class Game {
private:
	GameType g;
	std::vector<Round> rounds;
public:
	Game(Player& p1, Player& p2, Player& p3) {

	}

};
//Match: Collection of all games
//Starts the Games and has control over how many games and who is playing

class Match {
private:
	std::vector<Game> games;
public:
	void startGame() {
		size_t j;
		std::cout << "How many games would you like to play (3,6,9)? ";
		std::cin >> j;
		//Create players (and cpu).
		Player p1, p2, p3;
		//Create Deck
		for (size_t i = 0; i < j; i++) {
			Game game(p1, p2, p3);
		}



	}
};
int main() {
	Deck d;
	std::cout << d;
	d.shuffle();
	std::cout << '\n' << d;
	Player p1, p2, p3;
	d.deal(p1, p2, p3);
	Round r(p1, p2, p3, GameType::CLUBS);
	r.play();
	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
