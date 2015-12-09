#ifndef STK_FILEOPENWRAPPER_H
#define STK_FILEOPENWRAPPER_H

#include <string>
#include <SFML/Window.hpp>

namespace stk {

std::wstring getOpenFileName(sf::Window& window);
std::wstring getSaveFileName(sf::Window& window);
bool yesNoWarningMessage(sf::Window& window, std::wstring texte);

} // namespace stk

#endif // STK_FILEOPENWRAPPER_H
