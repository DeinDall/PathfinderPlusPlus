#ifndef DIALOGS_H
#define DIALOGS_H

#include <string>
#include <vector>

namespace dialogs {

enum DialogButtons : int {
	ButtonsOkOnly,
	ButtonsOkCancel,
	ButtonsYesNo
};

enum DialogLevel : int {
	LevelError,
	LevelWarning,
	LevelInfo,
	LevelQuestion
};

enum DialogChoice : bool {
	ChoicePositive=true, // Ok     / Yes
	ChoiceNegative=false // Cancel / No
};

std::string getOpenFileName(std::string title, std::string path, std::string filterDesc, std::vector<std::string> filters);
std::string getSaveFileName(std::string title, std::string path, std::string filterDesc, std::vector<std::string> filters);
bool openMessage(std::string title, std::string message, DialogLevel level, DialogButtons buttons, DialogChoice defaultChoice = ChoicePositive);

}

#endif // DIALOGS_H
