#include <gtest/gtest.h>
#include <include/metadata_reader.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <include/text.hpp>
#include <include/metadata.hpp>
#include <include/extraction/extraction_algorithm.hpp>
#include <include/extraction/extraction_algorithms.hpp>
#include <reader/reader.hpp>
#include <reader/test_case.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <regex>
#include "../cti/timer/timer.hpp"
#include "./test_util.hpp"

using cti::MetadataReader;
using cti::TicketImage;
using cti::Ticket;
using cti::Text;
using cti::Metadata;
using cti::ExtractionAlgorithm;
using cti::ExtractionAlgorithms;
using cti::reader::getAllTemplatesOf;
using cti::reader::getAllTestsOf;
using cti::reader::TestCase;

double calcExtractionScore(TestCase* testcase, const cti::Metadata& metadata);
double runExtraction(std::vector<const Ticket *>& tickets, std::vector<TestCase*>& testcases, MetadataReader& reader);
std::vector<std::vector<const Ticket*>> getBatchesOfTwoThirds(std::vector<const Ticket*>& tickets);

TEST(siftExtraction, performance) {

    std::ofstream out("sift_extraction.log");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::vector<const Ticket*> tickets = getAllTemplatesOf("resources/templates");
    std::vector<TestCase*> testcases = getAllTestsOf("resources/tickets");
//    std::vector<TestCase*> testcases = getAllTestsOf("resources/tickets/extraction_isolated");

    std::shared_ptr<ExtractionAlgorithm> extractionAlgorithm = ExtractionAlgorithms::sift();
    MetadataReader reader(*extractionAlgorithm);

    runExtraction(tickets, testcases, reader);

    std::cout.rdbuf(coutbuf);
}

TEST(orbExtraction, performance) {

    std::ofstream out("orb_extraction.log");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::vector<const Ticket *> tickets = getAllTemplatesOf("resources/templates");
    std::vector<TestCase*> testcases = getAllTestsOf("resources/tickets");

    std::shared_ptr<ExtractionAlgorithm> extractionAlgorithm = ExtractionAlgorithms::orb();
    MetadataReader reader(*extractionAlgorithm);

    runExtraction(tickets, testcases, reader);

    std::cout.rdbuf(coutbuf);
}

double runExtraction(std::vector<const Ticket *>& tickets, std::vector<TestCase*>& testcases, MetadataReader& reader) {

    double totalScore = 0.0;
    int totalCases = 0;

    int time = 0;

    for(auto& testcase : testcases) {
        for (auto& imagePath : testcase->getImages()) {
            totalCases++;
            std::cout << "RUN TESTCASE " << imagePath << std::endl;

            const Ticket* matchedTicket = cti::reader::findTicket(tickets, testcase->getExpectedTemplateId());
            if(matchedTicket == nullptr) {
                std::cout << "ERROR: Unable to find template" << std::endl;
                continue;
            }

            time += cti::Timer::timed([&imagePath, &matchedTicket, &reader, &totalScore, &testcase] () {
                TicketImage inputImage { imagePath };
                const cti::Metadata metadata = reader.read(*matchedTicket, inputImage);
                totalScore += calcExtractionScore(testcase, metadata);
            });
        }
    }

    std::cout << "SCORE: " << totalScore / totalCases << " testcases=" << totalCases
              << " time=" << time << "ms"  << " = " << ((double)time / testcases.size()) << "ms/testcase"<< std::endl;
    return totalScore / totalCases;
}

// Source: https://stackoverflow.com/a/3418285
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

// Date in Format dd.MM.yyyy which is used for some of our testcases
const std::regex DATE_REGEX {"([0-9]{1,2})[. \\/-]?([0-9]{1,2})[. \\/-]?([0-9]{2,4})" };

bool isDate(std::string& str) {
    return std::regex_match(str, DATE_REGEX);
}

std::string getComparableDateString(std::string& str) {
    std::smatch matches;
    if(std::regex_search(str, matches, DATE_REGEX) && matches.size() == 3) {
        // Looks like a date
        std::string dateString;
        for (const auto& match : matches) {
            dateString += match;
        }
        return dateString;
    }
    // Not a date
    return "";
}

double calcExtractionScore(TestCase* testcase, const cti::Metadata& metadata) {

    int correct = 0;
    int incorrect = 0;

    unordered_map<string, string> actualTexts = metadata.texts();

    for (auto &expectedText : testcase->getExpectedTexts()) {

        unordered_map<string, string>::const_iterator actualTextIt = actualTexts.find(expectedText.first);
        if(actualTextIt == actualTexts.end()) {
            std::cout << "Missing text: key=" << expectedText.first
                      << " expected_text='" << expectedText.second << "'"
                      << std::endl;
            incorrect++;
        } else {
            string actualText = actualTextIt->second;

            // Tesseract has a tendency to recognize '—' instead of '-'. Should we replace this?
            // Any application using this library will need to sanitize the text retrieved using this library
            // therefor, that application will know, whether a hyphen can occur in the specific string
            replaceAll(actualText, "—", "-");

            // Check if text looks like a date and compare in a smarter way by reading out the numbers and ignoring
            // any dot, dash or slash that may be between the date components. Any application using this library should
            // know which texts are supposed to be dates, and in which format, and read the text accordingly.
            if(isDate(expectedText.second)
                && getComparableDateString(expectedText.second) == getComparableDateString(actualText)) {

                correct++;

            } else if(actualText == expectedText.second) {
                correct++;
            } else {
                std::cout << "Incorrect text: key=" << expectedText.first
                          << " expected_text='" << expectedText.second << "'"
                          << " actual_text='" << actualText << "'"
                          << std::endl;
                incorrect++;
            }
        }
    }
    return (double) correct / (correct + incorrect);
}

