#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../src/preferences.hpp"

#include <fstream>
#include <string>
#include <map>
#include <iostream>

TEST_CASE( "Preferences", "[preference]" ) {
    std::cout.setstate(std::ios_base::failbit); // Disable cout output

    std::map<std::string, std::string> preferencesMap;

    REQUIRE(hasOnlyDigits("0123456789") == true);
    REQUIRE(hasOnlyDigits("foo123") == false);
    REQUIRE(hasOnlyHexSymbols("#0123456789ABCDEFabcdef") == true);
    REQUIRE(hasOnlyHexSymbols("aBcDeFg") == false);

    SECTION("All values should be read correctly", "[pass]") {
        preferencesMap = getPreferencesMap("pass.ini");
        filterPreferencesMap(preferencesMap, "../src/");
        REQUIRE(preferencesMap["window_width"] == "300");
        REQUIRE(preferencesMap["window_height"] == "200");
        REQUIRE(preferencesMap["full_line_highlight"] == "1");
        REQUIRE(preferencesMap["line_highlight_color"] == "F00F00");
        REQUIRE(preferencesMap["font"] == "SourceCodePro-Regular.ttf");
        REQUIRE(preferencesMap["font_size"] == "8");
        REQUIRE(preferencesMap["spaces_per_tab"] == "8");
        REQUIRE(preferencesMap["line_number_mode"] == "hybrid");
        REQUIRE(preferencesMap["cursor_color"] == "123DEF");
    }

    SECTION("All values should be empty", "[fail]") {
        preferencesMap = getPreferencesMap("fail.ini");
        filterPreferencesMap(preferencesMap, "../src/");
        REQUIRE(preferencesMap.empty());
    }

};