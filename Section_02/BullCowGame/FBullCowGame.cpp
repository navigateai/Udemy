#include "FBullCowGame.h"

using int32 = int;
TMap<int32, int32> WordLengthToMaxTries{ { 3,4 },{ 4,7 },{ 5,10 },{ 6,16 },{ 7,20 } };
// These should be in a file, which could be validated on loading - all the checks
// required are available!
FString Words[] =
{
	"alp","bet","cat","dig","eat",
	"abet","bray","curd","digs","etch",
	"amber","barge","curse","dream","extra",
	"anchor","barnet","cheats","damper","export",
	"abridge","bangers","clamber","dongles","exposed",
};
TMap <int32, int32> WordLengthCounts;
TMap <int32, FString> WordsOfSelectedLength;
//-----------------------------------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------------------------------
FBullCowGame::FBullCowGame()
{
	SetHiddenWord(DEFAULT_HIDDEN_WORD_LENGTH);
	Reset();
}
//-----------------------------------------------------------------------------------------------------
// GET functions
//-----------------------------------------------------------------------------------------------------
int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
FString FBullCowGame::GetHiddenWord() const { return MyHiddenWord; }
FString FBullCowGame::GetName() const {	return GAME; }
std::wstring FBullCowGame::GetWideName() const { return WIDENAME; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
int32 FBullCowGame::GetBullCount() const { return BullCowCount.Bulls;  }
int32 FBullCowGame::GetMaxTries() const { return WordLengthToMaxTries[GetHiddenWordLength()]; }
FString FBullCowGame::GetQUIT() const {	return QUIT; }
//-----------------------------------------------------------------------------------------------------
// IS & CAN Functions
//-----------------------------------------------------------------------------------------------------
bool FBullCowGame::IsGameWon() const { return GetBullCount() == GetHiddenWordLength(); }
bool FBullCowGame::IsValidsogram(FString) const { return false; }
bool FBullCowGame::CanAdjustDifficulty(int32 direction) const
{
	return (WordLengthToMaxTries[GetHiddenWordLength() + direction] > 0);
}
//-----------------------------------------------------------------------------------------------------
// IsLowerCase
//-----------------------------------------------------------------------------------------------------
bool FBullCowGame::IsLowercase(FString Word) const
{
	if (Word.empty()) { return true; }
	if (Word.find_first_not_of(' ') == Word.npos) { return true; }
	for (auto Letter : Word) {
		if ( ! islower(Letter)) { return false; }
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------
// IsIsogram
//-----------------------------------------------------------------------------------------------------
// An isogram is a word with no repeating letters
bool FBullCowGame::IsIsogram(FString Word) const
{
	if (Word.length() < 2) { return true; }
	TMap<char, bool> LetterSeen;
	for (auto Letter : Word)
	{
		Letter = tolower(Letter);
		if (LetterSeen[Letter]) { return false; }
		LetterSeen[Letter] = true;
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------
// SetBullCount
//-----------------------------------------------------------------------------------------------------
void FBullCowGame::SetBullCowCount(int32 Bulls, int32 Cows)
{
	BullCowCount.Bulls = Bulls;
	BullCowCount.Cows = Cows;
	return;
}
//-----------------------------------------------------------------------------------------------------
// SetCurrentTry
//-----------------------------------------------------------------------------------------------------
void FBullCowGame::SetCurrrentTry(int32 value)
{
	MyCurrentTry = value;
	return;
}
//-----------------------------------------------------------------------------------------------------
// SetHiddenWord
// returns MyHiddenWord
//-----------------------------------------------------------------------------------------------------
FString FBullCowGame::SetHiddenWord(int32 WordLength)
{
	// Clear map from any previous use
	WordsOfSelectedLength.clear();
	int32 key = 0;
	// Build map of words of required length, key is map entry number
	for(FString word : Words)
	{
		if (word.length() == WordLength) {
			if (DEBUG) { std::cout << "Adding key: " << key << ", value: " << word << std::endl; }
			WordsOfSelectedLength[key++] = word;
		}
	}
	// Generate random number from 0 to number of map entries
	int32 count = WordsOfSelectedLength.size();
	if (DEBUG) { std::cout << "Words in list: " << count << std::endl; }
	srand((unsigned int)time(NULL));
	key = rand() % count;
	// Select word at randomly generated key
	MyHiddenWord = WordsOfSelectedLength[key];
	return MyHiddenWord;
}
//-----------------------------------------------------------------------------------------------------
// SetWordLengthCounts
//-----------------------------------------------------------------------------------------------------
void FBullCowGame::SetWordLengthCounts()
{
	for (FString word : Words)
	{
		WordLengthCounts[word.length()]++;
	}
	return;
}
//-----------------------------------------------------------------------------------------------------
// CheckGuessValidity
//-----------------------------------------------------------------------------------------------------
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if ( ! IsIsogram(Guess)) {
		return EGuessStatus::Not_Isogram;
	}
	else if ( ! IsLowercase(Guess))
	{
		return EGuessStatus::Invalid_Character;
	}
	else if (Guess.length() != GetHiddenWordLength())
	{
		return EGuessStatus::Wrong_Length;
	}
	return EGuessStatus::OK;
}
//-----------------------------------------------------------------------------------------------------
// IncreaseSizeOfIsogram
//-----------------------------------------------------------------------------------------------------
// Checks that there is an entry in the Maximum tries map for the increased word length
bool FBullCowGame::AdjustSizeOfIsogram(int32 direction)
{
	if (WordLengthToMaxTries[GetHiddenWordLength() + direction]) {
		// yuk! Bodge to record new word length
		// TODO review methods for increased functionality
		SetHiddenWord(GetHiddenWordLength() + direction);
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------------
// Reset
//-----------------------------------------------------------------------------------------------------
void FBullCowGame::Reset()
{
	MyCurrentTry = 0;
	BullCowCount.Bulls = 0;
	BullCowCount.Cows = 0;
	return;
}
//-----------------------------------------------------------------------------------------------------
// SubmitGuess
//-----------------------------------------------------------------------------------------------------
FBullCowCount FBullCowGame::SubmitGuess(FString Guess)
{
	SetBullCowCount(0, 0);
//	MyCurrentTry++;
	int32 HiddenWordLength = MyHiddenWord.length();
	int32 GuessLength = Guess.length();
	for (int32 idxGuess = 0; idxGuess < GuessLength; idxGuess++)
	{
		for (int32 idxMyHiddenWord = 0; idxMyHiddenWord < HiddenWordLength; idxMyHiddenWord++)
		{
			if (Guess[idxGuess] == MyHiddenWord[idxMyHiddenWord]) {
				if (idxGuess == idxMyHiddenWord) {
					BullCowCount.Bulls++;
				}
				else {
					BullCowCount.Cows++;
				}
			}
		}
	}
	return BullCowCount;
}
