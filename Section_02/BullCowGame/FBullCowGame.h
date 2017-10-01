#pragma once
#include <string>
#include <iostream>
#include <windows.h>
#include <map>
#include <cstdlib>
#include <ctime>

// Custom debugging: Eg: I use "if(DEBUG) {std::cout << something;}" as a quick check
#define DEBUG false // mutually exclusive
//#define DEBUG true  // move to set off/on
#define DEMO false
//#define DEMO true

// Unreal Engine Data types
// https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/#portablealiasesforbasicc++types

#define TMap std::map
using FString = std::string;
using FText = std::string;
using int32 = int;

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum EGuessStatus {
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Invalid_Character
};

//-----------------------------------------------------------------------------------------------------
// CLASS declaration: FBullGame designed to contain game logic only
//-----------------------------------------------------------------------------------------------------
class FBullCowGame
{
public:
	FBullCowGame();

	int32 GetBullCount() const;
	int32 GetCurrentTry() const;
	FString GetHiddenWord() const;
	FString GetName() const;
	std::wstring GetWideName() const;
	int32 GetHiddenWordLength() const;
	int32 GetMaxTries() const;
	FString GetQUIT() const;

	bool IsGameWon() const;
	bool IsValidsogram(FString) const;
	bool CanAdjustDifficulty(int32) const;

	void SetBullCowCount(int32, int32);
	void SetCurrrentTry(int32);
	FString SetHiddenWord(int32);
	void SetWordLengthCounts();

	EGuessStatus CheckGuessValidity(FString) const;
	bool AdjustSizeOfIsogram(int32);
	void Reset();
	FBullCowCount SubmitGuess(FString);

private:

	const FString GAME = "Bulls and Cows";
	const FString QUIT = "bye";

	// A wide string version of the game name for MessageBox
	const std::wstring WIDENAME = std::wstring(GAME.begin(),GAME.end());

	const int32 DEFAULT_HIDDEN_WORD_LENGTH = 3;
	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;

	int32 MyCurrentTry;
	int32 MyMaxTries;
	FString MyHiddenWord;
	FBullCowCount BullCowCount;
};