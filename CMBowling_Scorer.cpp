// CMBowling_Scorer.cpp : Defines the entry point for the application.
//

#include "CMBowling_Scorer.h"
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <forward_list>

#ifdef _WIN32
	#define CLEAR() system("cls");
#elif __linux__
	#define CLEAR() system("clear");
#endif



class Player
{
	private:
		std::string playerName;
		std::uint16_t totalScore;
		std::forward_list <uint8_t> ballScores;
		std::forward_list <uint8_t>::iterator prevBall;

		std::atomic_bool ball;


	public:
		
		Player(std::string playerName)
		{
			this->playerName = playerName;
			this->totalScore = 0;
			this->ball = 0;
		}

		uint16_t score() { return totalScore; }
		std::string name() { return playerName; }

		uint8_t bowl(uint8_t pins) {
			
			// set prevBall before we push_front, so it retains the previous front
			prevBall = ballScores.begin();
			// insert the new score at the front
			ballScores.push_front(pins);


			switch (pins) {
				case 0:
					return ' ';
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					if (ball == 1 && pins + *prevBall == 10) {
						ball = !ball;
						return '/';
					}
					else {
						ball = !ball;
						return pins + '0';
					}
				case 10:
					// do not increment ball here, we are moving directly to the next frame
					return 'X';

				default:
					return ' ';
			}

		}

		


	


};







int main()
{
	int nPlayerNum = 0;
	int nFrames = 10;
	int nBowls_Per_Frame = 3;
	int nNumBowls = 0;

	std::forward_list <Player*> players;
	std::forward_list <Player*>::iterator currentPlayer;

	printf ("Enter the name of player %d, if you are done entering names press enter again\n"
			"Note the name of the player can not exceed 10 characters.\n", nPlayerNum+1);
	

	std::string name;
	do {
			// getline grabs a string from cin, and discards any preexisting values in the variable
			std::getline(std::cin, name);

			if (!name.empty()) {
				Player* p = new Player(name);
				players.push_front(p);
				nPlayerNum += 1;
			}

	} while (!name.empty());

	// we pushed to the front, so player names need to be reversed.
	players.reverse();

	CLEAR();

	int n = 1;
	// for each value in the vector, print it to name. allow us to use n to count. 
	std::for_each(players.begin(), players.end(), [&n](Player* p) {
		printf("Player %d: %s\n", n, (*p).name().c_str());
		n++;
	});

	// extra newline for good luck :D
	putchar('\n');
/*
	// Total Number of Elements in 1D Array
	int nTotalElements = (nPlayerNum * nFrames * nBowls_Per_Frame);
	
	// Create a 1D Array
	int* scores = new int[nTotalElements];		// Dynamically Allocate Memory

	// Function used to get the index of 1D Array
	auto getIndex = [nFrames, nBowls_Per_Frame](int player, int frame, int bowl) {
		return (player * nFrames * nBowls_Per_Frame) + (frame * nBowls_Per_Frame) + bowl;
		};


	// Loop over player names and capture number of pins knocked over after each bowl
	for (int frame = 0; frame < nFrames; frame++) {
		for (int player = 0; player < nPlayerNum; player++) {
			for (int bowl = 0; bowl < nBowls_Per_Frame; bowl++) {
				int index = getIndex(player, frame, bowl); // Calculate the index
				nNumBowls += 1;
				if (bowl == 2 && !(frame == 9)) {
					scores[index] = 0;
				}
				if (bowl == 2 && frame == 9 && (scores[index - 2] + scores[index - 1]) >= 10) {
					std::cout <<  "Enter the number of pins knocked over for " << names[player]
						<< "\nFrame " << frame + 1
						<< "\nBowl " << bowl + 1 << ": ";
					std::cin >> scores[index]; // Store the score into the 1D array
					system("cls");
				} else if (bowl == 2) {
					scores[index] = 0;
				} else {
					std::cout << "Enter the number of pins knocked over for " << names[player]
						<< "\nFrame " << frame + 1
						<< "\nBowl " << bowl + 1 << ": ";
					std::cin >> scores[index]; // Store the score into the 1D array
					CLEAR();
				}

			}
		}
	}

	// Calculate Total Score

	std::vector<int> playertotals(nPlayerNum, 0);
	int nBowlsPerPlayer = nNumBowls / nPlayerNum;
	int nNextTurn = nBowls_Per_Frame;
	//int nArraySize = 5;
	// Output Bowling Scores
	for (int player = 0; player < nPlayerNum; player++) {
		for (int frame = 0; frame < nFrames; frame++) {
			int index = getIndex(player, frame, 0); // Start of the frame

			if (frame == 9) { // Last frame
				if (scores[index] == 10) { // Strike
					playertotals[player] += 10 + scores[index + 1] + scores[index + 2];
				}
				else if (scores[index] + scores[index + 1] == 10) { // Spare
					playertotals[player] += 10 + scores[index + 2];
				}
				else {
					playertotals[player] += scores[index] + scores[index + 1];
				}
			}
			else {
				if (scores[index] == 10) { // Strike
					int nextIndex1 = index + nNextTurn;
					int nextIndex2 = index + nNextTurn + 1;
					if (nextIndex1 < nTotalElements && nextIndex2 < nTotalElements) {
						playertotals[player] += 10 + scores[nextIndex1] + scores[nextIndex2];
					}
					else {
						playertotals[player] += 10;
					}
				}
				else if (scores[index] + scores[index + 1] == 10) { // Spare
					int nextIndex = index + nNextTurn;
					if (nextIndex < nTotalElements) {
						playertotals[player] += 10 + scores[nextIndex];
					}
					else {
						playertotals[player] += 10;
					}
				}
				else {
					playertotals[player] += scores[index] + scores[index + 1];
				}
			}
		}
	}


	std::cout << "\nBowling Scores:\n";
	for (int player = 0; player < nPlayerNum; player++) {
		for (int frame = 0; frame < nFrames; frame++) {
			std::cout << "Player " << names[player] << ", Frame " << frame + 1 << ": ";
			for (int bowl = 0; bowl < nBowls_Per_Frame; bowl++) {
				int index = getIndex(player, frame, bowl);
				if (bowl == 3 && frame != 9)
				std::cout << scores[index] << " ";
				
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
	for (int player = 0; player < nPlayerNum; player++) {
		std::cout << "player " << names[player] << " scored: " << playertotals[player];
		std::cout << std::endl;
	}

	// Deallocate the memory for 1D array
	delete[] scores;
	*/
	return 0;
}