#pragma once
#include "EntityObject.h"
#include "Name.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

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

    inline EntityObject createEntityObjectFromFile(const std::string &filename) {
        std::string line;
        std::ifstream myfile(filename);
        std::string fileText = "";
        if (myfile.is_open()) {
            while (getline(myfile, line))
                fileText += line + "\n";
            myfile.close();
        }
        else {
            std::cout << "Error: createEntityObjectFromFile()'s designated file from 'filename' couldn't be opened" << std::endl;
            std::cout << "filename was " << filename << std::endl;
        }
        auto v = createEntityObjectFromString(fileText);
        return v;
    }
}