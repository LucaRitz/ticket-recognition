#pragma once

#include <string_view>
#include <unordered_map>

using std::string_view;
using std::unordered_map;

namespace cti::reader {
    class TestCase {
    public:
        TestCase(string_view expectedTemplateId, unordered_map<string_view, string_view> expectedTexts, string_view image)
                : _expectedTemplateId(std::move(expectedTemplateId)), _expectedTexts(expectedTexts),
                  _image(std::move(image)){};

        TestCase(const TestCase &) = default;

        TestCase(TestCase &&) = default;

        TestCase &operator=(const TestCase &) = default;

        TestCase &operator=(TestCase &&) = default;

    private:
        const string_view _expectedTemplateId;
        const unordered_map<string_view, string_view> _expectedTexts;
        const string_view _image;
    };
}