#pragma once

#include <regex>

namespace Validador {
	std::regex Usuario = std::regex("^[A-Z0-9-_]{3,12}$", std::regex_constants::ECMAScript | std::regex_constants::icase);
	std::regex Archivo = std::regex("^[A-Z0-9-_]+\\.(?:txt|csv|tsv)$", std::regex_constants::ECMAScript | std::regex_constants::icase);
}
