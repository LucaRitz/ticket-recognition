#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::unordered_map;
using std::vector;

namespace cti::reader {
    class TestCase {
    public:
        TestCase(string expectedTemplateId, unordered_map<string, string> expectedTexts, vector<string> images)
                : _expectedTemplateId(std::move(expectedTemplateId)), _expectedTexts(expectedTexts),
                  _images(std::move(images)){};
        TestCase(const TestCase&) = default;
        TestCase(TestCase&&) = default;
        TestCase &operator=(const TestCase&) = default;
        TestCase &operator=(TestCase&&) = default;

        string getExpectedTemplateId() const {
            return _expectedTemplateId;
        }

        unordered_map< string, string > getExpectedTexts() const {
            return _expectedTexts;
        }

        const vector<string>& getImages() const {
            return _images;
        }

    private:
        const string _expectedTemplateId;
        const unordered_map< string, string > _expectedTexts;
        const vector<string> _images;
    };
}