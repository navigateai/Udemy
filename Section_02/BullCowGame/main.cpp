/*------------------------------------------------------------------------------------*
 * Driver for a Console based text puzzle called Bulls and Cows, a kind of extended   *
 * Mastermind, much more difficult in that instead of a combination of 4 colours, we  *
 * are using 26 letters. In order to make the game slightly simpler, we are using     *
 * isograms, which are words that do not have repeating letters (ape, ale, four ...)  *
 *                                                                                    *
 * A player is given a certain number of attempts depending on the size of the word   *
 * presented; the larger the word, the greater the number of attempts. Error checks   *
 * are made on a players input, so that typos do not affect the number of tries that  *
 * a player has - the idea is to have fun, not worry about keyboard skills!           *
 *                                                                                    *
 * Wins and losses are acknowledged, and the game will repeat for as long as player   *
 * wishes to continue. At the beginning of each game, the player is given a chance    *
 * to increase the difficulty of the game by increasing the size of isogram to be     *
 * solved.                                                                            *
 *                                                                                    *
 * I have taken some liberties with the original requirements, as I have come from a  *
 * COBOL, C, 4GL and Java background and am learning C++ from that experience. This   *
 * can prove to a challenge as I try to "lose" bad habits and syntax from these       *
 * other environments.                                                                *
 *                                                                                    *
 * I am almost convinced that c++ is designed to be some awful mind bending device    *
 * which will drive a programmer insane with enough exposure, and if he/she were that *
 * way inclined already, then it would drive them beyond the pale after a very short  *
 * time indeed. The name of the inventor, Bjarne Stroustrup, and his general demeanour*
 * only serve to further my conviction.                                               *
 *                                                                                    *
 * My thanks to the people at http://www.chris.com for the ASCII bull/cow art.        *
 *------------------------------------------------------------------------------------*/

#pragma once

// 'Cut and Paste' definitions
#include <fstream>
#include <conio.h>

// User defined definitions - Unreal Engine Custom data types moved here
#include"FBullCowGame.h"

// Function declarations (function prototypes)
// (return type) (Function name) (Argument type, ...) 
bool AskToPlayAgain();
int32 AskToAdjustDifficulty();
void ClearScreen();
int32 DoMessagebox(int32);
FText GetAValidGuess();
FString GetFileContents(std::ifstream&); 
void PlayOneGame();
void PlayGameControl();
void PrintAnswer();
void PrintArt();
void PrintBye();
void PrintGameSummary();
void PrintGuess(FText);
void PrintScore(FBullCowCount);
void PrintWelcome();
FText SubmitValidGuess();

// Global variable - BCGame is reset each new game
FBullCowGame BCGame;

// std::wstring (wide string) version of Game name for MessageBox
// returned by GCGame.GetWideName()
std::wstring GameName;
//
// To display this on console, use { std::wcout << GameName }
// presented to MessageBox as:
// GCGame.GetWideName().c_str() which returns a (const wchar_t *)
//
// https://stackoverflow.com/questions/23665779/wstring-to-const-wchar-using-c-str
// '@user2002287 bear in mind that the result of c_str() is only valid until the
// next time you call a non-const member function of the string, or the string
// is destroyed.'
//
// This relates to MessageBox and the weird way it receives parameters. If you
// want to pass a variable (as opposed to a literal) value to MessageBox, it has
// to be presented as a (const wchar_t *), which is OK until you try and do it.
// That type has to be created as late as possible (ie in the function call) or
// you risk the contents of the reference being destroyed before they can be used.

// MessageBox message pointers
enum {
	difficulty,
	err_isogramtoosmall,
	err_isogramtoobig,
	err_isogramwronglength,
	err_dupchar,
	err_invchar,
	max_difficulty,
	min_difficulty,
	ask_playagain,
	bye_bye,
	game_won,
	game_lost,
	dumbo
};
std::wstring Messages[] =
{
	L"Increase Difficulty?\nYES: increase size of isogram.\nNO: Reduce size of isogram.\nCANCEL: Leave it as it is.",
	L"You need to type more letters for this isogram",
	L"You have typed enough letters for the current isogram.",
	L"Your isogram has the wrong number of letters!",
	L"You are trying to type a character already entered.",
	L"You are trying to type an invalid character.",
	L"You are at most difficult level.",
	L"You are at easiest level.",
	L"GAME OVER.\nWould you like to play again?",
	L"BYE!\nThanks for playing.",
	L"WELL DONE!\nYou have successfully deduced the answer, Doctor Watson.",
	// Note the missing comma for a multi line literal.
	L"HARD LUCK, Inspector Lestrade.\nMay I suggest you request assistance from Mr. Holmes?\n"
	L"OK: Reveal current hidden word\nCANCEL: Continue investigations.",
	L"It's official!\nThis programmer is amazing.\nMISSING MESSAGEBOX MESSAGE."
};
bool PlayerHasQuit;

//-----------------------------------------------------------------------------------------------------
// main
// C++ Start point is main()
//-----------------------------------------------------------------------------------------------------
int main()
{
	// High Level Function calls, keep it simple
	do {
		PlayerHasQuit = false;
		PlayGameControl();
	} while (AskToPlayAgain());
	PrintBye();
	// Good practise: always use return even if it is not needed
	return 0;
}
//-----------------------------------------------------------------------------------------------------
// AskToAdjustDifficulty
// Will scale as it uses word list to determine maximum isogram size
//-----------------------------------------------------------------------------------------------------
int32 AskToAdjustDifficulty()
{
	int32 reply = DoMessagebox(difficulty);
	switch (reply) {
	// YES = increase isogram size if possible
	case IDYES :
		if (BCGame.CanAdjustDifficulty(+1))
			BCGame.AdjustSizeOfIsogram(+1);
		else
			DoMessagebox(max_difficulty);
		break;
	// NO = reduce isogram size if possible
	case IDNO :
		if (BCGame.CanAdjustDifficulty(-1))
			BCGame.AdjustSizeOfIsogram(-1);
		else
			DoMessagebox(min_difficulty);
		break;
	// catch all
	case IDCANCEL :
	default :
		reply = IDCANCEL;
		break;
	}
	return reply;
}
//-----------------------------------------------------------------------------------------------------
// ClearScreen
//-----------------------------------------------------------------------------------------------------
// OS Dependant, this is for Windows
void ClearScreen() { std::system("CLS"); }
//-----------------------------------------------------------------------------------------------------
// DoMessageBox
//-----------------------------------------------------------------------------------------------------
int32 DoMessagebox(int32 option)
{
	int32 reply;
	int32 mOption;
	switch (option) {
	case difficulty :
		mOption = MB_YESNOCANCEL | MB_ICONQUESTION;
		break;
	case err_dupchar :
	case err_invchar :
	case err_isogramtoobig :
	case err_isogramtoosmall :
	case err_isogramwronglength :
	case max_difficulty :
	case min_difficulty :
	case game_won :
		mOption = MB_OK | MB_ICONEXCLAMATION;
		break;
	case game_lost:
		mOption = MB_OKCANCEL;
		break;
	case ask_playagain :
		mOption = MB_YESNO | MB_ICONQUESTION;
		break;
	case bye_bye :
		mOption = MB_OK | MB_ICONHAND;
		break;
	default :
		option = dumbo;
		mOption = MB_OK | MB_ICONINFORMATION;
		break;
	}
//	std::wstring wMessage = std::wstring(Messages[option].begin(), Messages[option].end());
	reply = MessageBox(nullptr,
		Messages[option].c_str(),
		BCGame.GetWideName().c_str(),
		mOption);
	return reply;
}
//-----------------------------------------------------------------------------------------------------
// AskToPlayAgain
//-----------------------------------------------------------------------------------------------------
bool AskToPlayAgain() {	return (DoMessagebox(ask_playagain) == IDYES); }
//-----------------------------------------------------------------------------------------------------
// GetAValidGuess
//
// PROTO from C days, a single character self validating routine which messages whenever
// the player makes an error while typing.
// Keeps all validation within this function, so only ever returns a valid (maybe short) guess
//-----------------------------------------------------------------------------------------------------
FText GetAValidGuess()
{
	char AChar = 'a';
	char BChar = 'a';
	FText Guess = "";
	TMap <char, bool> MyMap;
	bool Continue = true;
	while (Continue) {
		AChar = tolower(_getch());
		switch (AChar) {
		// RETURN key
		case '\r':
			// Allow early escape without regard for word size
			Continue = false;
			break;

		// BACKSPACE key
		case '\b':
			// Housekeeping for backspace key
			//	 1. turn off array entry for last letter
			//   2. remove last letter from current guess
			//   3. output 'BACKSPACE'/blank/'BACKSPACE'
			if (Guess.length() > 0) {
				BChar = Guess[Guess.length() - 1];
				MyMap[BChar] = false;
				Guess = Guess.substr(0, Guess.length() - 1);
				_putch('\b');
				_putch(' ');
				_putch('\b');
			}
			break;
		
		// Everything else
		default:
			// If hidden word length reached, refuse any more input
			// This forces Return or allows Backspace
			if (Guess.length() == BCGame.GetHiddenWordLength()) {
				DoMessagebox(err_isogramtoobig);
				break;
			}
			// Basic validation that current character is lower case a-z and not duplicated
			if (AChar >= 'a' && AChar <= 'z') {
				// Valid character, allow if not duplicated
				if (MyMap[AChar] == true) {
					DoMessagebox(err_dupchar);
				} else {
					Guess += AChar;
					MyMap[AChar] = true;
					_putch(AChar);
				}
			} else {
				DoMessagebox(err_invchar);
			}
			break;
		}
	}
	// Force newline
	std::cout << std::endl;
	return Guess;
}
//-----------------------------------------------------------------------------------------------------
// GetFileContents
//-----------------------------------------------------------------------------------------------------
// ASCII art is in a .txt file, which is read in and stored/returned as a single string variable
FString GetFileContents(std::ifstream& File)
{
	FString TempLine;
	FString Lines = "";
	if (File) {
		while (File.good()) {
			std::getline(File, TempLine);
			TempLine += "\n";
			Lines += TempLine;
		}
		return Lines;
	} else	{
		return "ERROR File does not exist.";
	}
}
//-----------------------------------------------------------------------------------------------------
// PlayGameControl
//-----------------------------------------------------------------------------------------------------
// Play a game - still higher level functions
void PlayGameControl()
{
	// This resets game variables to initial state
	BCGame.Reset();
	// Set game difficulty according to player choice
	do {
		PrintWelcome();
	} while (AskToAdjustDifficulty() != IDCANCEL);
	std::cout << "Type \'" << BCGame.GetQUIT() << "\' to cancel current game." << std::endl;
	PlayOneGame();
	if(!PlayerHasQuit)
		PrintGameSummary();
	return;
}
//-----------------------------------------------------------------------------------------------------
// PlayOneGame
//-----------------------------------------------------------------------------------------------------
// Single game function control loop
void PlayOneGame()
{
	FBullCowCount BullCowCount;
	// Sets a new hidden word according to word length selected
	BCGame.SetHiddenWord(BCGame.GetHiddenWordLength());
	// Control loop, keep going until game is won or player has run out of tries
	do
	{
		// A single try: get a valid guess and check against current word
		// Bulls = number of correct letters in correct place
		// Cows = number of correct letters in incorrect place
		BCGame.SetCurrrentTry(BCGame.GetCurrentTry() + 1);
		FString Guess = SubmitValidGuess();
		// Forced break for player request to QUIT
		if (PlayerHasQuit)
			break;
		// Submit valid guess to the game
		BullCowCount = BCGame.SubmitGuess(Guess);
		// This tells players how they are doing
		PrintScore(BullCowCount);
	} while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() < BCGame.GetMaxTries());
	return;
}
//-----------------------------------------------------------------------------------------------------
// PrintArt
//-----------------------------------------------------------------------------------------------------
void PrintArt()
{
	std::ifstream Reader("Cow.txt");
	FString Art = GetFileContents(Reader);
	std::cout << Art;
	std::cout << std::endl;
	Reader.close();
	return;
}
//-----------------------------------------------------------------------------------------------------
// PrintBye
//-----------------------------------------------------------------------------------------------------
void PrintBye() { DoMessagebox(bye_bye); return; }
//-----------------------------------------------------------------------------------------------------
// PrintAnswer
//-----------------------------------------------------------------------------------------------------
void PrintAnswer() { std::cout << "Answer is : " << BCGame.GetHiddenWord() << std::endl; }
//-----------------------------------------------------------------------------------------------------
// PrintGameSummary
//-----------------------------------------------------------------------------------------------------
// Game summary: WIN or LOSE with option to see answer
void PrintGameSummary()
{
	if (BCGame.IsGameWon()) {
		DoMessagebox(game_won);
	}
	else {
		int32 reply = DoMessagebox(game_lost);
		if(reply == IDOK)
			PrintAnswer();
	}
	return;
}
//-----------------------------------------------------------------------------------------------------
// PrintGuess
//-----------------------------------------------------------------------------------------------------
// Repeat the guess back to them : a testing convenience method
// until I get the hang of the debugger
void PrintGuess(FText Guess)
{
	std::cout << "Your guess was ";
	std::cout << Guess;
	std::cout << std::endl;
	return;
}
//-----------------------------------------------------------------------------------------------------
// PrintScore
//-----------------------------------------------------------------------------------------------------
// Print the bull and cow score for the current guess
void PrintScore(FBullCowCount BullCowCount) {
	std::cout << "Bulls = " << BullCowCount.Bulls << "; ";
	std::cout << "Cows = " << BullCowCount.Cows << std::endl;
	return;
}
//-----------------------------------------------------------------------------------------------------
// PrintWelcome
//-----------------------------------------------------------------------------------------------------
// Introduce the game, complete with System console commands and colour variations
// OS Dependant, this is for Windows.
void PrintWelcome()
{
	// Yellow on Blue, my personal favourite
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_RED | FOREGROUND_GREEN |	FOREGROUND_INTENSITY |
		BACKGROUND_BLUE
	);
	// If you don't do this, you only get a colour change on displayed text
	if (!DEBUG) { // combat side effect of destroying my debug info!

		ClearScreen();

	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Welcome to " << BCGame.GetName() << ", a fun word game!";
	std::cout << std::endl;
	PrintArt();
	std::cout << "Can you deduce the ";
	std::cout << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?";
	std::cout << std::endl;
	return;
}
//-----------------------------------------------------------------------------------------------------
// SubmitValidGuess
//-----------------------------------------------------------------------------------------------------
// Get and return a valid guess from the player, give feedback on invalid input
// NOTE that player will stay in here until valid input received (or forced exit!)
FText SubmitValidGuess()
{
	FText Guess = "";
	EGuessStatus EGStatus = EGuessStatus::Invalid_Status;
	do
	{
		std::cout << std::endl;
		if (DEBUG) { std::cout << BCGame.GetHiddenWord() << std::endl; }
		std::cout << "Try ";
		std::cout << BCGame.GetCurrentTry();
		std::cout << " of ";
		std::cout << BCGame.GetMaxTries();
		std::cout << ". Enter your guess : ";

		// PROTO single character input with inline validation
		Guess = GetAValidGuess();
		// Player has requested to QUIT
		if (Guess == BCGame.GetQUIT()) {
			PlayerHasQuit = true;
			return Guess;
		}
		// STANDARD read a line from console
		// std::getline(std::cin, Guess);

		if (DEBUG) {
			std::cout << std::endl << "I have received your guess of " << Guess << std::endl;
			std::cout << "Handing you over to alternate input method, please repeat input." << std::endl;
			FText SecondGuess = GetAValidGuess();
			std::cout << "Thank you for your cooperation." << std::endl;
			std::cout << std::endl << "Your alternate (validated) input is " << SecondGuess << std::endl;
		}
		//		PROTO END

		// Perform guess validity checks, action as appropriate
		EGStatus = BCGame.CheckGuessValidity(Guess);
		// NOTE use of enum for switch/case
		switch (EGStatus)
		{
		case EGuessStatus::OK:
			break;
		case EGuessStatus::Wrong_Length:
			DoMessagebox(err_isogramwronglength);
			break;
		case EGuessStatus::Invalid_Character:
			DoMessagebox(err_invchar);
			break;
		case EGuessStatus::Not_Isogram:
			DoMessagebox(err_dupchar);
			break;
		default:
			break;
		}
	} while (EGStatus != EGuessStatus::OK);
	return Guess;
}
