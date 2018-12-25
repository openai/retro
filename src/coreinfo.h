#pragma once

#include <string>
#include <vector>

namespace Retro {

class GameData;

std::string corePath(const std::string& hint = {});
std::string libForCore(const std::string& core);
std::string coreForRom(const std::string& rom);
std::vector<std::string> buttons(const std::string& core);
std::vector<std::string> keybinds(const std::string& core);
size_t ramBase(const std::string& core);
void configureData(GameData*, const std::string& core);

bool loadCoreInfo(const std::string& json);

std::vector<std::string> cores();
std::vector<std::string> extensions();
}
