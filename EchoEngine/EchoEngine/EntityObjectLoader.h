#pragma once
#include "EntityObject.h"
#include "Name.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <map>

namespace EntityObjectLoader {
    static std::string errorStr = "";

    inline void removeCharsFromString(std::string& str, const char* charsToRemove) {
        for (unsigned int i = 0; i < strlen(charsToRemove); ++i)
            str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
    }
    inline std::vector<std::string> split(const std::string& str, const std::string& delim) {
        std::vector<std::string> tokens;
        size_t prev = 0, pos = 0;
        do {
            pos = str.find(delim, prev);
            if (pos == std::string::npos) pos = str.length();
            std::string token = str.substr(prev, pos - prev);
            if (!token.empty()) tokens.push_back(token);
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
        return tokens;
    }
    inline bool isDigits(const std::string& s) {
        for (char c : s) if (!isdigit(c)) return false;
        return true;
    }
    inline bool isFloat(const std::string& s) {
        FixedPoint<> f;
        return f.fromString(s);
    }



    inline void cleanText(std::string& text) {
        {
            size_t currentIndex = 0;
            while (true) {
                size_t pos = text.find("/#", currentIndex);
                if (pos == std::string::npos) break;
                size_t endPos = text.find("#/", pos);
                if (endPos == std::string::npos) {
                    size_t lineCount = 0;
                    for (size_t i = 0; i < pos; i++)
                        if (text[i] == '\n')
                            lineCount++;
                    std::cout << "Error no matching #/ at line " << lineCount + 1 << std::endl;
                    return;
                }
                currentIndex = endPos + 1;
            }
        }
        {
            size_t currentIndex = 0;
            while (true) {
                size_t pos = text.find("/#");
                if (pos == std::string::npos) break;
                size_t endPos = text.find("#/", pos);
                text.erase(pos, (endPos + 2) - pos);
            }
        }
        {
            size_t currentIndex = 0;
            while (true) {
                size_t pos = text.find("#");
                if (pos == std::string::npos) break;
                size_t endPos = text.find("\n", pos);
                text.erase(pos, (endPos + 1) - pos);
            }
        }
        removeCharsFromString(text, " \n\r\t");
    }

    inline bool isTokenString(const std::string& token) {
        unsigned int count = 0;
        if (token[0] != '"') return false;
        for (char c : token) {
            if (c == '"')
                count++;
        }
        if(count == 2)
            return true;
        errorStr += "Statement should only have two \"\n";
        return false;
    }
    inline bool isTokenArray(const std::string& token) {
        unsigned int left = 0, right = 0;
        if (token[0] != '[') return false;
        for (char c : token) {
            if (c == '[')
                left++;
            if (c == ']')
                right++;
        }
        if (left != 1) {
            errorStr = "Statement should have exactly one [";
            return false;
        }
        if (right != 1) {
            errorStr += "Statement should have exactly one ]";
            return false;
        }
        return true;
    }
    inline bool isTokenDArray(const std::string& token) {
        unsigned int left = 0, right = 0;
        if (token[0] != '<') return false;
        for (char c : token) {
            if (c == '<')
                left++;
            if (c == '>')
                right++;
        }
        if (left != 1) {
            errorStr = "Statement should have exactly one <";
            return false;
        }
        if (right != 1) {
            errorStr += "Statement should have exactly one >";
            return false;
        }
        return true;
    }
    inline bool isTokenNULL(const std::string& token) {
        if (token == "NULL")
            return true;
        return false;
    }

    inline ComponentObject createComponentInteger(const std::string& name, const std::string& value) {
        ComponentObject retValue = ComponentObject();
        int num = atoi(value.c_str());
        retValue.setInt((uint32_t)num);
        retValue.name = name;
        return retValue;
    }
    inline ComponentObject createComponentFixedPoint(const std::string& name, const std::string& value) {
        ComponentObject retValue = ComponentObject();
        FixedPoint<> fixedPoint;
        fixedPoint.fromString(value);
        retValue.setFixedPoint(fixedPoint);
        retValue.name = name;
        return retValue;
    }
    inline ComponentObject createComponentString(const std::string& name, const std::string& value) {
        ComponentObject retValue = ComponentObject();
        std::string cleanedValue = value;
        removeCharsFromString(cleanedValue, "\"");
        retValue.setString(cleanedValue.c_str());
        retValue.name = name;
        return retValue;
    }
    inline ComponentObject createComponentArray(const std::string& name, const std::string& value) {
        ComponentObject retValue = ComponentObject();
        std::string cleanedValue = value;
        removeCharsFromString(cleanedValue, "[]");
        std::vector<std::string> elements = split(cleanedValue, ",");
        std::vector<uint32_t> values;
        for (auto& element : elements) {
            if (isFloat(element)) {
                FixedPoint<> fixedPoint;
                fixedPoint.fromString(element);
                values.push_back(fixedPoint.getRaw());
                continue;
            }
            if (isDigits(element)) {
                int num = atoi(element.c_str());
                values.push_back((uint32_t)num);
                continue;
            }
            if (isTokenString(element)) {
                removeCharsFromString(element, "\"");
                Name nameElement = element;
                values.push_back(nameElement.getID());
                continue;
            }
            errorStr += "Statement's array has an invalid element, elements may only be of type integer or string\n";
            return retValue;
        }
        retValue.setArray(values);
        retValue.name = name;
        return retValue;
    }
    inline ComponentObject createComponentDArray(const std::string& name, const std::string& value) {
        ComponentObject retValue = ComponentObject();
        std::string cleanedValue = value;
        removeCharsFromString(cleanedValue, "<>");
        std::vector<std::string> elements = split(cleanedValue, ",");
        std::vector<uint32_t> values;
        for (auto& element : elements) {
            if (isFloat(element)) {
                FixedPoint<> fixedPoint;
                fixedPoint.fromString(element);
                values.push_back(fixedPoint.getRaw());
                continue;
            }
            if (isDigits(element)) {
                int num = atoi(element.c_str());
                values.push_back((uint32_t)num);
                continue;
            }
            if (isTokenString(element)) {
                removeCharsFromString(element, "\"");
                Name nameElement = element;
                values.push_back(nameElement.getID());
                continue;
            }
            errorStr += "Statement's array has an invalid element, elements may only be of type integer or string\n";
            return retValue;
        }
        DArray<uint32_t> dArray;
        dArray.initCopy(values.data(), values.size());
        retValue.setDArray(dArray);
        retValue.name = name;
        return retValue;
    }
    inline ComponentObject createComponentNULL(std::string& name) {
        ComponentObject retValue = ComponentObject();
        retValue.setNULL();
        retValue.name = name;
        return retValue;
    }

    inline ComponentObject createComponentObjectFromString(const std::string& str) {
        errorStr = "";
        //ComponentObject retValue = ComponentObject();
        std::vector<std::string> tokens = split(str, "=");
        if (tokens.size() != 2) {
            errorStr += "Statement should have one = symbol\n";
            return ComponentObject();
        }
        if (isTokenArray(tokens[1]))
            return createComponentArray(tokens[0], tokens[1]);
        if (isTokenDArray(tokens[1]))
            return createComponentDArray(tokens[0], tokens[1]);
        if (isFloat(tokens[1]))
            return createComponentFixedPoint(tokens[0], tokens[1]);
        if (isDigits(tokens[1]))
            return createComponentInteger(tokens[0], tokens[1]);
        if (isTokenString(tokens[1]))
            return createComponentString(tokens[0], tokens[1]);
        if (isTokenNULL(tokens[1]))
            return createComponentNULL(tokens[0]);

        errorStr += "Value is not of array, integer, string, or NULL type\n";
        return ComponentObject();
    }
    inline EntityObject createEntityObjectFromString(std::string str) {
        cleanText(str);
        EntityObject retValue = EntityObject();
        size_t currentPos = 0;
        uint32_t statementCount = 0;
        while (true) {
            statementCount++;
            size_t newPos = str.find(';', currentPos);
            if (newPos == std::string::npos) break;
            std::string statement = str.substr(currentPos, newPos - currentPos);
            retValue.components.push_back(createComponentObjectFromString(statement));
            currentPos = newPos + 1;
        }
        return retValue;
    }

    inline EntityObject createEntityObjectFromFile(const std::string& filename) {
        std::string line;
        std::ifstream myfile(filename);
        std::string fileText = "";
        if (myfile.is_open()) {
            while (getline(myfile, line))
                fileText += line + "\n";
            myfile.close();
        }
        else {
            HAL_ERROR("Error: createEntityObjectFromFile()'s designated file from 'filename' couldn't be opened\n");
            HAL_ERROR("filename was {}\n", filename);
        }
        auto v = createEntityObjectFromString(fileText);
        return v;
    }
    inline void writeComponentObjectToFile(const std::string& filename, const std::string& componentName, const std::string& val) {
        std::string line;
        std::fstream myfile(filename);
        std::string fileText = "";
        if (myfile.is_open()) {
            while (getline(myfile, line))
                fileText += line + "\n";
            myfile.close();
        }
        else {
            HAL_ERROR("Error: writeComponentObjectToFile()'s designated file from 'filename' couldn't be opened\n");
            HAL_ERROR("filename was {}\n", filename);
        }

        std::string newText = "";
        bool hasComponent = false;

        cleanText(fileText);
        EntityObject retValue = EntityObject();
        size_t currentPos = 0;
        uint32_t statementCount = 0;
        while (true) {
            statementCount++;
            size_t newPos = fileText.find(';', currentPos);
            if (newPos == std::string::npos) break;
            std::string statement = fileText.substr(currentPos, newPos - currentPos);
            {
                size_t equalPos = statement.find('=', 0);
                std::string var = statement.substr(0, equalPos);
                if (var == componentName) {
                    statement = componentName + "=";
                    statement += val + ";\n";
                    currentPos = newPos + 1;
                    hasComponent = true;
                    continue;
                }
            }
            newText += statement + ";\n";
            currentPos = newPos + 1;
        }
        if (hasComponent == false) {
            newText += componentName + "=";
            newText += val + ";\n";
        }
        HAL::file_append_str(filename.c_str(), newText.c_str());
    }







    ComponentObject processItemComponent(const std::string& line) {
        auto tokens = split(line, " ");
        const std::string& componentName = tokens[0];
        //ComponentObject retValue = {};

        //GUN
        if (componentName == "MAGAZINE_SIZE") {
            std::string& arg1 = tokens.at(1);
            if (isDigits(arg1) == false) {
                HAL_ERROR("processItemComponent(): component MAGAZINE_SIZE arg1: {} not integer");
                return {};
            }
            return createComponentInteger("MAGAZINE_SIZE", arg1);
        }
        if (componentName == "MUZZLE_VELOCITY") {

        }
        if (componentName == "BULLET_WEIGHT") {

        }
        if (componentName == "RATE_OF_FIRE") {

        }
        if (componentName == "RELOAD_TIME") {

        }
        if (componentName == "DEGRADATION_PER_USE") {

        }

        //GENERIC
        if (componentName == "MAX_DURABILITY") {

        }
        if (componentName == "SCRAPS_INTO") {

        }
        if (componentName == "DEGRADATION_PER_USE") {

        }
        if (componentName == "MAX_STACK") {

        }

        //MELEE
        if (componentName == "ENTITY_DAMAGE") {

        }
        if (componentName == "POWER_ATTACK_ENTITY_DAMAGE") {

        }
        if (componentName == "BLOCK_DAMAGE") {

        }
        if (componentName == "POWER_ATTACK_BLOCK_DAMAGE") {

        }
        if (componentName == "RANGE") {

        }
        if (componentName == "ATTACKS_PER_MINUTE") {

        }
        if (componentName == "STAMINA_USAGE") {

        }
        if (componentName == "POWER_ATTACK_STAMINA_USAGE") {

        }
        if (componentName == "BUTCHER_DAMAGE_MULTIPLIER") {

        }
        if (componentName == "ATTACKS_PER_MINUTE") {

        }

        //BLOCKS
        if (componentName == "HIT_POINTS") {

        }
        if (componentName == "ELECTRICAL_POWER_REQUIRED") {

        }
        if (componentName == "HORIZONTAL_SUPPORT") {

        }
        if (componentName == "MASS") {

        }

        //TOOLS

        //AMMO
        if (componentName == "EFFECTIVE_RANGE") {

        }
        if (componentName == "EXPLOSION_RANGE_ENTITIES") {

        }
        if (componentName == "EXPLOSION_RANGE_BLOCKS") {

        }
        if (componentName == "VELOCITY") {

        }

        //ARMOR
        if (componentName == "ARMOR_RATING") {

        }
        if (componentName == "ELEMENTAL_PROTECTION") {

        }
        if (componentName == "EFFECT_PROTECTION") {

        }
        if (componentName == "MOBILITY_REDUCTION") {

        }
        if (componentName == "STAMINA_REGEN_REDUCTION") {

        }
        if (componentName == "NOISE_INCREASE") {

        }

        //RAW RESOURCES
        if (componentName == "BURN_TIME") {

        }
        if (componentName == "MATERIAL") {

        }
        if (componentName == "MOBILITY_REDUCTION") {

        }
        if (componentName == "MOBILITY_REDUCTION") {

        }

        //INTERMEDIATE RESOURCES

        //FOOD
        if (componentName == "RESTORES_FOOD") {

        }
        if (componentName == "RESTORES_HYDRATION") {

        }
        if (componentName == "DECREASES_FOOD") {

        }
        if (componentName == "DECREASES_HYDRATION") {

        }

        //MEDICAL
        if (componentName == "CURES_INFECTION") {

        }
        if (componentName == "RESTORES_HEALTH") {

        }
        if (componentName == "CREATES_ITEM_ON_USE") {

        }
        if (componentName == "REDUCES_BROKEN_LIMB_TIME") {

        }
        if (componentName == "HAS_USE_COOLDOWN") {

        }
        if (componentName == "BUFF_NO_ECUMBERENCE") {

        }
        if (componentName == "BUFF_MOVEMENT_SPEED") {

        }
        if (componentName == "BUFF_RANGED_DAMAGE") {

        }
        return {};
    }
    //Takes .ITEM format/extension files
    inline std::map<std::string, EntityObject> createEntityObjectsFromFileITEM(const std::string& filename) {
        std::string line;
        std::fstream myfile(filename);
        std::string fileText = "";
        std::map<std::string, EntityObject> retValue;

        if (myfile.is_open() == false) {
            HAL_ERROR("Error: createEntityObjectFromFileITEM()'s designated file from 'filename' couldn't be opened\n");
            HAL_ERROR("filename was {}\n", filename);
            return retValue;
        }

        EntityObject eo;
        while (getline(myfile, line)) {
            size_t dashPos = line.find('-');
            if (dashPos != std::string::npos)
                line = line.substr(dashPos);
            if (line.size() == 0)
                continue;

            static bool isSecondLine = false;
            //Declaring our item name...
            if (line[0] == '#') {
                //Pushing our last object if it was built...
                if (eo.components.size()) {
                    auto c = eo.getComponent("NAME");
                    if (c == nullptr) {
                        HAL_ERROR("createEntityObjectsFromFileITEM(): from file {}\n", filename);
                        HAL_ERROR("THIS SHOULD NOT BE HAPPENING!");
                        HAL_ERROR("last object doesn't have a NAME component somehow?");
                        continue;
                    }
                    if (c->type != ComponentObject::TYPE::TYPE_STRING) {
                        HAL_ERROR("createEntityObjectsFromFileITEM(): from file {}\n", filename);
                        HAL_ERROR("THIS SHOULD NOT BE HAPPENING!");
                        HAL_ERROR("last object's NAME component isn't a name/string?");
                        continue;
                    }
                    retValue[c->getString().c_str()] = eo;
                }
                eo = {};
                ComponentObject cName;
                if (line.size() == 1) {
                    HAL_ERROR("createEntityObjectsFromFileITEM(): from file {}\n", filename);
                    HAL_ERROR("THIS SHOULD NOT BE HAPPENING!");
                    HAL_ERROR("new object's would be name is empty? # is by itself...");
                    continue;
                }
                const char* str = line.c_str();
                cName.name = "NAME";
                cName.setString(&str[1]);
                eo.components.push_back(cName);
                isSecondLine = true;
            }

            if (isSecondLine) {
                ComponentObject cDescription;
                cDescription.name = "DESCRIPTION";
                cDescription.setString(line.c_str());
                eo.components.push_back(cDescription);
                isSecondLine = false;
                continue;
            }

            auto tokens = split(line, " ");
            if (tokens.size() == 1) {  //We are handling an attribute...
                ComponentObject cAttrib;
                cAttrib.name = tokens[0];
                cAttrib.setNULL();
                continue;
            }

            processItemComponent(line);

        }
        myfile.close();
    }
}