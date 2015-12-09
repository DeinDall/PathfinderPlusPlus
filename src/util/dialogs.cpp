#include "dialogs.h"
#include "../tinyfiledialogs/tinyfiledialogs.h"

namespace dialogs {

static const char* dialogButtonsValues[3] = { "ok", "okcancel", "yesno" };
static const char* dialogLevelValues[4] = { "error", "warning", "info", "question" };

std::string getOpenFileName(std::string title, std::string path, std::string filterDesc, std::vector<std::string> filters) {
	const char* formattedFilters[filters.size()];

	for (unsigned int i=0; i<filters.size(); ++i)
		formattedFilters[i] = filters[i].c_str();

	const char* out = tinyfd_openFileDialog(title.c_str(), path.c_str(),
											filters.size(), formattedFilters, filterDesc.c_str(), 0);
	if (out!=nullptr)
		return std::string(out);
	return std::string("error");
}

std::string getSaveFileName(std::string title, std::string path, std::string filterDesc, std::vector<std::string> filters) {
	const char* formattedFilters[filters.size()];

	for (unsigned int i=0; i<filters.size(); ++i)
		formattedFilters[i] = filters[i].c_str();

	const char* out = tinyfd_saveFileDialog(title.c_str(), path.c_str(),
											filters.size(), formattedFilters, filterDesc.c_str());
	if (out!=nullptr)
		return std::string(out);
	return std::string("error");
}

bool openMessage(std::string title, std::string message, DialogLevel level, DialogButtons buttons, DialogChoice defaultChoice) {
	return tinyfd_messageBox(title.c_str(), message.c_str(),
							 dialogButtonsValues[buttons], dialogLevelValues[level], defaultChoice);
}

}
