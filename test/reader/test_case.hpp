#pragma once

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

namespace cti::reader {
    class TestCase {
    public:
        TestCase(string expectedTemplateId, unordered_map<string, string> expectedTexts, string image)
                : _expectedTemplateId(std::move(expectedTemplateId)), _expectedTexts(expectedTexts),
                  _image(std::move(image)){};
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

        string getImage() const {
            return _image;
        }

    private:
        const string _expectedTemplateId;
        const unordered_map< string, string > _expectedTexts;
        const string _image;
    };
}