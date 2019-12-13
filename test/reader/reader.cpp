#include "reader.hpp"
#include "test_case.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/text.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <fstream>
#include <istream>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <experimental/filesystem>
#include <regex>
#include "../io/file_util.hpp"

using cti::Text;
namespace fs = std::experimental::filesystem;

template<char delimiter>
class Delimiter : public std::string {
public:
    friend std::istream& operator>>(std::istream& is, Delimiter& output) {
        std::getline(is, output, delimiter);
        return is;
    }
};

///////////////////////////////////////////////////////////////////////
// Templates
///////////////////////////////////////////////////////////////////////

struct TemplatePoint {
    int _x;
    int _y;
};

struct TemplateText {
    std::string _key;
    std::vector<TemplatePoint*> _points;

    ~TemplateText() {
        for (auto point : _points) {
            delete point;
        }
        _points.clear();
    }
};

struct Template {
    std::string _template_id;
    std::string _imagePath;
    std::vector<TemplateText*> _texts;

    ~Template() {
        for (auto text : _texts) {
            delete text;
        }
        _texts.clear();
    }
};

typedef void (*TemplateFunctionPointer)(Template &, const std::string&);
typedef std::unordered_map<std::string_view, TemplateFunctionPointer> TemplatePointerMapping;

namespace cti::reader {
    cti::Ticket* getTemplateOf(std::string_view);
}

std::vector<const cti::Ticket*> cti::reader::getAllTemplatesOf(std::string_view directoryPath) {
    std::vector<const cti::Ticket*> tickets;
    std::string normalizedPath = cti::io::replaceDirectorySeparator(directoryPath);
    for (const auto& entry : fs::directory_iterator(normalizedPath)) {
        tickets.push_back(getTemplateOf(entry.path().u8string()));
    }
    return tickets;
}

std::vector<std::string> splitBySemicolon(const std::string& value) {
    std::istringstream iss(value);
    return std::vector<std::string>((std::istream_iterator<Delimiter<';'>>(iss)),
                                     std::istream_iterator<Delimiter<';'>>());
}

std::vector<std::string> splitByComa(const std::string& value) {
    std::istringstream iss(value);
    return std::vector<std::string>((std::istream_iterator<Delimiter<','>>(iss)),
                                    std::istream_iterator<Delimiter<','>>());
}

void addText(Template& templateRef, const std::string& value) {
    std::vector<std::string> results = splitBySemicolon(value);
    std::vector<TemplateText*> texts = templateRef._texts;

    std::for_each(results.begin(), results.end(), [&results, &texts](std::string split){
        if (split == results.at(0)) {
            auto text = new TemplateText;
            text->_key = split;
            texts.push_back(text);
        } else {
            TemplateText* text = texts.back();
            std::vector<std::string> byComa = splitByComa(split);
            auto point = new TemplatePoint;
            point->_x = std::stoi(byComa.at(0));
            point->_y = std::stoi(byComa.at(1));
            text->_points.push_back(point);
        }
    });

    templateRef._texts = texts;
}

cti::Ticket* cti::reader::getTemplateOf(std::string_view path) {
    std::string normalizedPath = cti::io::replaceDirectorySeparator(path);
    Template templateValues;
    TemplatePointerMapping mapping;
    mapping[std::string_view("template_id")] = [](Template &templateRef,
                                                  const std::string& value) { templateRef._template_id = value; };
    mapping[std::string_view("image")] = [](Template &templateRef,
                                            const std::string& value) { templateRef._imagePath = cti::io::replaceDirectorySeparator(value); };
    mapping[std::string_view("text")] = &addText;

    std::fstream file(normalizedPath);

    std::string key;
    std::string value;

    while (file >> key >> value) {
        if (mapping.find(key) != mapping.end()) {
            mapping[key](templateValues, value);
        }
    }

    auto texts = new std::vector<const Text*>;
    std::for_each(templateValues._texts.begin(), templateValues._texts.end(), [&texts](const TemplateText* templateText) {
       auto topLeftPoint = new cti::Point(templateText->_points.at(0)->_x, templateText->_points.at(0)->_y);
       auto bottomRightPoint = new cti::Point(templateText->_points.at(1)->_x, templateText->_points.at(1)->_y);

       auto boundingBox = new cti::BoundingBox(*topLeftPoint, *bottomRightPoint);
       texts->push_back(new cti::Text(templateText->_key, *boundingBox));
    });

    return new cti::Ticket(templateValues._template_id, *(new cti::TicketImage(templateValues._imagePath)), *texts);
}

///////////////////////////////////////////////////////////////////////
// Test Cases
///////////////////////////////////////////////////////////////////////

struct Case {
    std::string _expectedTemplateId;
    std::unordered_map< std::string, std::string > _expectedTexts;
    std::vector<std::string> _images;
};

typedef void (*TestCaseFunctionPointer)(Case&, const std::string&);
typedef std::unordered_map<std::string_view, TestCaseFunctionPointer> TestCasePointerMapping;

namespace cti::reader {
    cti::reader::TestCase* getTestOf(std::string_view);
}

std::vector<cti::reader::TestCase*> cti::reader::getAllTestsOf(std::string_view directoryPath) {
    std::vector<cti::reader::TestCase*> testCases;
    std::string normalizedPath = cti::io::replaceDirectorySeparator(directoryPath);
    for (const auto& entry : fs::directory_iterator(normalizedPath)) {
        testCases.push_back(getTestOf(entry.path().u8string()));
    }
    return testCases;
}

void addExpectedText(Case& caseRef, const std::string& value) {
    std::vector<std::string> results = splitBySemicolon(value);
    std::unordered_map< std::string, std::string > expectedTexts = caseRef._expectedTexts;

    std::string previous_key;
    int isKey {0};
    std::for_each(results.begin(), results.end(), [&results, &expectedTexts, &previous_key, &isKey](std::string split){
        if (isKey % 2 == 0) {
            previous_key.assign(split);
        } else {
            const std::regex re("(.*)(__)(.*)");
            std::string replaced = split;
            while (std::regex_search(replaced, re)) {
                replaced = std::regex_replace(replaced, re, "$1 $3");
            }
            expectedTexts[previous_key] = replaced;
        }
        isKey++;
    });

    caseRef._expectedTexts = expectedTexts;
}

cti::reader::TestCase* cti::reader::getTestOf(std::string_view path) {
    std::string normalizedPath = cti::io::replaceDirectorySeparator(path);
    Case caseValues;
    TestCasePointerMapping mapping;
    mapping[std::string_view("expected_template_id")] = [](Case& caseValuesRef, const std::string& value) {
        caseValuesRef._expectedTemplateId = value;
    };
    mapping[std::string_view("image")] = [](Case& caseValuesRef, const std::string& value) {
        caseValuesRef._images.push_back(cti::io::replaceDirectorySeparator(value));
    };
    mapping[std::string_view("expected_text")] = &addExpectedText;

    std::fstream file(path.data());

    std::string key;
    std::string value;

    while (file >> key >> value) {
        if (mapping.find(key) != mapping.end()) {
            mapping[key](caseValues, value);
        }
    }

    return new cti::reader::TestCase(caseValues._expectedTemplateId, caseValues._expectedTexts, caseValues._images);
}