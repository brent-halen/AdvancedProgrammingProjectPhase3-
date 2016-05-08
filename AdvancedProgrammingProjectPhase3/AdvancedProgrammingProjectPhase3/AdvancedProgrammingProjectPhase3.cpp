// AdvancedProgrammingProjectPhase3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <Windows.h>
#include "Round.h"

using namespace std;

int BEEP_DURATION = 50;						// Beep duration in milliseconds. Shorter than 50 tends to be inaudible.
int SIGNALBEEP_DURATION = 250;				// Total length of the "signal Beep", signaling when a rest round or active round begins.
bool anyleft(vector<Round> & v);			// Prototype of the "anyleft" function, which counts the # of remaining rounds for each Round and determines if they're > 0. 
void displayworkout(vector<Round> & v);		// Prototype of the "Displayworkout" function, which pushes a chart of the current # of rounds to the console. 
void startworkout(vector<Round> & v);		// Prototype of the "startworkout" function, which actually carries out the workout. The 'meat' of the program. 
void signalrest();							// Prototype of the "signalrest" function, which plays the rest beep. 
void signalwork();							// Prototype of the "signalwork" function, which plays the round start beep.
void somemenu(vector<Round> & v);

int main()
{
	char again = 'y';
	vector<Round> workout = {
		Round(2, 10000, 30, 10000, 350),
		Round(3, 10000, 201, 10000, 450),
		Round(1, 10000, 60, 10000, 550) };
	do {
		cout << "\nThis program is a prototype for a metronome/round timer hybrid program. A vector of 'Round' objects";
		cout << "\nis passed as an argument to the workout program. Vectors are used to facilitate the ease of adding and removing";
		cout << "\nrounds from the workout cycle.";
		cout << "\nWhen you press 'Enter' to continue, a chart with the rounds in the system will display." << endl;
		system("pause");
		system("cls");
		somemenu(workout);
		/*
		displayworkout(workout);
		cout << "\n";
		cout << "The ability to modify the workout cycle is forthcoming.\nUntil then, please test the program by pressing 'Enter' and initiating the workout.\n";
		system("pause");


		startworkout(workout);*/
		
		cout << "\nWell, that was fun. Care to go again? \n (enter 'y' for 'yes' or any other character to end the program.) \n>>>>>>>    ";
		cin >> again;
		if (tolower(again) == 'y')
			system("cls");
	} while (tolower(again) == 'y');
	return 0;
}

// Boolean "anyleft" function determines whether any rounds will be initiated on the next cycle. 
// The workout will end when this function returns 'false'. 

bool anyleft(vector<Round> &workout)
{
	int count = 0;
	int workoutsize;
	long int totalcount = 0;
	long int added_round = 0;
	workoutsize = workout.size();
	for (int count = 0; count<workoutsize; count++)
	{
		added_round = workout[count].getRoundsremain();
		totalcount = totalcount + added_round;
		added_round = 0;
	}
	if (totalcount > 0)
		return(true);
	else
		return(false);
};

// "Displayworkout" function. Displays a chart of the current workout plan. Adaptable to any # of rounds, no modification necessary once user input is added.
// Time values, such as Length and Rest, are represented in seconds. 
void displayworkout(vector<Round> &workout)
{
	int counter;
	int workoutsize;
	workoutsize = workout.size();
	cout << '\n' << " | " << setw(7) << "Round #" << " | " << setw(6) << "Rounds" << " | " << setw(6) << "Length" << " | " << setw(5) << "Rest" << " | " << setw(5) << "BPM" << " | " << setw(5) << "Freq" << " | " << endl;
	cout << setw(47) << "-----------------------------------------------" << endl;
	for (counter = 0; counter < workoutsize; counter++)
	{
		cout << '\n' << " | " << setw(7) << (counter + 1) << " | " << setw(6) << workout[counter].getRoundnum() << " | " << setw(5) << (workout[counter].getRlength() / 1000) << " | " << setw(5) << (workout[counter].getRestlength() / 1000) << setw(5) << " | " << setw(5) << workout[counter].getRmet() << " | " << setw(5) << workout[counter].getRoundfreq() << " | " << endl;
		cout << setw(47) << "-----------------------------------------------" << endl;
	}
}

// "signalrest" function, plays a two-toned beep indicating that a rest phase has begun. 
// I chose a high-low tone to imply mentally that the user should be slowing down their activity. 
void signalrest()
{
	system("cls");
	cout << "\n\n\nROUND OVER! REST NOW!\n\n";
	Beep(785, (SIGNALBEEP_DURATION / 2));
	Beep(185, (SIGNALBEEP_DURATION / 2));
}

// "signalwork" function plays a two-toned beep indicating that the active phase has begun. 
void signalwork()
{
	system("cls");
	cout << "\n\n\nROUND BEGINS! GET TO WORK!\n\n";
	Beep(185, (SIGNALBEEP_DURATION / 2));
	Beep(785, (SIGNALBEEP_DURATION / 2));
}

// The startworkout function. Accepts the Rounds vector as a pointer and initiates the workout. This is the meat of the program. 
void startworkout(vector<Round> &workout)
{
	int counter;					// A counter to increment through the index of the vector. 
	int counter2;					// A counter to increment through the # of metronome ticks in a given round. 
	int workoutsize;				// An integer to carry the size of the workout vector. 
									// Assign the workout size variable.
	workoutsize = workout.size();
	// Set up the round counter at the beginning of the workout.
	for (counter = 0; counter < workoutsize; counter++)
	{
		workout[counter].setupround();
	}
	// Signal the beginning of the workout.
	signalwork();
	// Set a do-while loop to keep the workout going until the # of rounds has completed. 
	do {
		// Set a for loop to increment through the workout vector. 
		for (counter = 0; counter < workoutsize; counter++)
		{
			// An if conditional that will only trigger the round if it has any instances left for this cycle. 
			if (workout[counter].getRoundsremain() != 0) {
				// Signal the workout has begun. 
				// Increment through the # of metronome ticks.
				signalwork();
				for (counter2 = 0; counter2 < (workout[counter].Rmetcount() - 1); counter2++)
				{
					// Alternate between the "Beep()" and "Sleep()" functions for the course of the round.
					Beep(workout[counter].getRoundfreq(), BEEP_DURATION);
					Sleep((workout[counter].Rmetint()) - BEEP_DURATION);
				}
				// Set a conditional that will account for an imperfectly divided round
				if (workout[counter].Rrem() >= BEEP_DURATION)
				{
					Beep(workout[counter].getRoundfreq(), BEEP_DURATION);
					Sleep((workout[counter].Rrem() - BEEP_DURATION));
				}
				// Indicate the beginning of the rest cycle. 
				signalrest();
				// Set the rest cycle to process for the total rest length minus the duration of the rest signal.
				Sleep(workout[counter].getRestlength() - (SIGNALBEEP_DURATION));
				// Decrement the # of cycles left for this particular round. 
				workout[counter].decrementroundsremain();
			}
		}
	} while (anyleft(workout) == true);
	cout << "\nWorkout complete.\nI hope you enjoyed it.\n";
	return;

}

// user_menu function
// this function will present the user with a menu of choices, up to and including 'Quit'. 
// They will be able to modify and change the workout rounds 

void somemenu(vector<Round> &workout)
{
	int choice;
	int modchoice;
	int roundmodmenuchoice;
	long int roundmodmenuinput;
	long int Roundnumthree;
	long int Roundlengththree;
	long int Restlengththree;
	long int BPMthree;
	long int Roundfreqthree;

	do {
		displayworkout(workout);
		cout << "\n\n";
		//Display menu.
		cout << "\n************************WORKOUT MENU************************************\n\n";
		cout << "1. Start Workout.\n";
		cout << "2. Modify a Round.\n";
		cout << "3. Add a Round.\n";
		cout << "4. Remove a Round.\n";
		cout << "5. Quit.\n";
		cout << "\n\nPlease select from the menu above (1, 2, 3, 4, or 5).\n\n>>>>>>>>>> ";
		cin >> choice;
		cin.ignore();
		// validate the input.
		while (choice < 1 || choice > 5)
		{
			cout << "ERROR! Invalid input! \n";
			system("pause");
			system("cls");
			cout << "\n************************WORKOUT MENU************************************\n\n";
			cout << "1. Start Workout.\n";
			cout << "2. Modify a Round.\n";
			cout << "3. Add a Round.\n";
			cout << "4. Remove a Round.\n";
			cout << "5. Quit.\n";
			cout << "\n\nPlease select from the menu above (1, 2, 3, 4, or 5).\n\n>>>>>>>>>> ";
			cin >> choice;
			cin.ignore();
		}
		// Execute user command
		switch (choice)
		{
		case 1:
			startworkout(workout);
			break;
		case 2:
			displayworkout(workout);
			cout << "\nWhich round would you like to modify?\n\n>>>>>>>>> ";
			cin >> modchoice;
			while (modchoice < 1 || modchoice > workout.size())
			{
				cout << "\nERROR! Inavlid input!";
				system("pause");
				cout << "\nWhich round would you like to modify?\n\n>>>>>>>>> ";
				cin >> modchoice;
			}
			modchoice--;
			do {
				cout << '\n' << " | " << setw(6) << "Rounds" << " | " << setw(6) << "Length" << " | " << setw(5) << "Rest" << " | " << setw(5) << "BPM" << " | " << setw(5) << "Freq" << " | " << endl;
				cout << setw(47) << "-----------------------------------------------" << endl;
				cout << '\n' << " | " << setw(6) << workout[modchoice].getRoundnum() << " | " << setw(5) << (workout[modchoice].getRlength() / 1000) << " | " << setw(5) << (workout[modchoice].getRestlength() / 1000) << setw(5) << " | " << setw(5) << workout[modchoice].getRmet() << " | " << setw(5) << workout[modchoice].getRoundfreq() << " | " << endl;
				cout << setw(47) << "-----------------------------------------------" << endl;
				cout << "\n************************Round MENU************************************\n\n";
				cout << "1. Modify # of rounds.\n";
				cout << "2. Modify Round Length.\n";
				cout << "3. Modify rest length.\n";
				cout << "4. Modify BPM.\n";
				cout << "5. Modify Frequency.\n";
				cout << "6. Go back to main menu.\n";
				cout << "\n\nPlease select from the menu above (1, 2, 3, 4, 5, or 6).\n\n>>>>>>>>>> ";
				cin >> roundmodmenuchoice;
				cin.ignore();
				// validate the input.
				while (roundmodmenuchoice < 1 || roundmodmenuchoice > 6)
				{
					cout << "ERROR! Invalid input! \n";
					system("pause");
					system("cls");
					cout << "\n************************Round MENU************************************\n\n";
					cout << "1. Modify # of rounds.\n";
					cout << "2. Modify Round Length.\n";
					cout << "3. Modify rest length.\n";
					cout << "4. Modify BPM.\n";
					cout << "5. Modify Frequency.\n";
					cout << "6. Go back to main menu.\n";
					cout << "\n\nPlease select from the menu above (1, 2, 3, 4, 5, or 6).\n\n>>>>>>>>>> ";
					cin >> roundmodmenuchoice;
					cin.ignore();
				}
				switch (roundmodmenuchoice)
				{
				case 1:
					cout << "\nThe current # of rounds is: " << workout[modchoice].getRoundnum() << endl;
					cout << "What would you like to change this value to?\n>>>>>>>> ";
					cin >> roundmodmenuinput;
					cin.clear();
					workout[modchoice].setRoundnum(roundmodmenuinput);
					break;
				case 2:
					cout << "\nThe current # of seconds in the round is: " << (workout[modchoice].getRlength() / 1000) << endl;
					cout << "How many seconds per round would you like to set? \n>>>>>>>> ";
					cin >> roundmodmenuinput;
					cin.clear();
					workout[modchoice].setRlength((roundmodmenuinput * 1000));
					break;
				case 3:
					cout << "\nThe current # of seconds in the rest phase is: " << (workout[modchoice].getRestlength() / 1000) << endl;
					cout << "How many seconds per rest phase? \n>>>>>>>>> ";
					cin >> roundmodmenuinput;
					cin.clear();
					workout[modchoice].setRestlength((roundmodmenuinput * 1000));
					break;
				case 4:
					cout << "\nThe current BPM setting is: " << workout[modchoice].getRmet() << endl;
					cout << "How many beats per minute do you want to set? \n>>>>>>> ";
					cin >> roundmodmenuinput;
					cin.clear();
					workout[modchoice].setBPM(roundmodmenuinput);
					break;
				case 5:
					cout << "\nThe current beep frequency is: " << workout[modchoice].getRoundfreq() << endl;
					cout << "What frequency would you like to set? \n >>>>>>>>  ";
					cin >> roundmodmenuinput;
					cin.clear();
					workout[modchoice].setRoundFreq(roundmodmenuinput);
					break;
				case 6:
					cout << "\nReturning to Main Menu\n";
					system("pause");
					system("cls");
					break;
				}

			} while (roundmodmenuchoice != 6);
			break;
		case 3:
			cout << "\nHow many times should the workout cycle through this round?\n>>>>>>> ";
			cin >> Roundnumthree;
			cin.clear();
			cout << "\nHow many seconds per work phase?\n>>>>>>>> ";
			cin >> Roundlengththree;
			cin.clear();
			cout << "\nHow many seconds per rest phase?\n>>>>>>>> ";
			cin >> Restlengththree;
			cin.clear();
			cout << "\nHow many Beats per Minute?\n>>>>>> ";
			cin >> BPMthree;
			cin.clear();
			cout << "\nWhat Beep Frequency should be used?\n>>>>> ";
			cin >> Roundfreqthree;
			cin.clear();
			workout.push_back(Round(Roundnumthree, (Roundlengththree * 1000), BPMthree, (Restlengththree * 1000), Roundfreqthree));
			break;
		case 4:
			displayworkout(workout);
			cout << "Which round do you want to delete from the workout?\n>>>>>>> ";
			cin >> roundmodmenuinput;
			while (roundmodmenuinput < 1 || roundmodmenuinput > workout.size())
			{
				cout << "ERROR!! INVALID INPUT!";
				system("pause");
				cout << "Which round do you want to delete from the workout?\n>>>>>>> ";
				cin >> roundmodmenuinput;
			}
			workout.erase(workout.begin() + (roundmodmenuinput - 1));
			break;
		case 5:
			cout << "\nEnding program. Thank you for your time!\n";
			system("pause");
			break;
		}
	
	
	}while (choice != 5);
}

