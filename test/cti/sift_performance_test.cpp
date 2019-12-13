#include <gtest/gtest.h>
#include <include/matcher.hpp>
#include <include/metadata_reader.hpp>
#include <include/ticket_match.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <include/text.hpp>
#include <include/metadata.hpp>
#include <include/matching/matching_algorithm.hpp>
#include <include/matching/matching_algorithms.hpp>
#include <include/matching/matching_options.hpp>
#include <include/extraction/extraction_algorithm.hpp>
#include <include/extraction/extraction_algorithms.hpp>
#include <include/extraction/extraction_options.hpp>
#include <reader/reader.hpp>
#include <reader/test_case.hpp>
#include <reader/confusion_matrix.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "../cti/timer/timer.hpp"

using cti::Matcher;
using cti::MetadataReader;
using cti::TicketImage;
using cti::TicketMatch;
using cti::Ticket;
using cti::Text;
using cti::Metadata;
using cti::MatchingAlgorithm;
using cti::MatchingAlgorithms;
using cti::MatchingOptions;
using cti::ExtractionAlgorithm;
using cti::ExtractionAlgorithms;
using cti::ExtractionOptions;
using cti::reader::getAllTemplatesOf;
using cti::reader::getAllTestsOf;
using cti::reader::TestCase;

double calcExtractionScore(TestCase* testcase, const cti::Metadata* const metadata);
cti::reader::ConfusionMatrix calcMatchingScore(std::vector<const Ticket *>& tickets, TestCase* testcase, const std::optional<const TicketMatch>& matchedOpt);
double runExtraction(std::vector<const Ticket *>& tickets, std::vector<TestCase*>& testcases, MetadataReader& reader);
double runMatching(std::vector<const Ticket *>& tickets, std::vector<TestCase*>& testcases, Matcher& matcher);
std::vector<std::vector<const Ticket*>> getBatchesOfTwoThirds(std::vector<const Ticket*>& tickets);

TEST(siftMatching, performance) {

    std::ofstream out("sift_matching.log");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::vector<const Ticket*> tickets = getAllTemplatesOf("resources/templates");
    std::vector<TestCase*> testcases = getAllTestsOf("resources/tickets");

    double totalScore = 0.0;
    int numberOfBatches = 0;
    for(auto& batch : getBatchesOfTwoThirds(tickets)) {
        std::shared_ptr<MatchingAlgorithm> matchingAlgorithm = MatchingAlgorithms::sift(MatchingOptions());
        Matcher matcher(*matchingAlgorithm);
        totalScore += runMatching(batch, testcases, matcher);
        numberOfBatches++;
    }
    std::cout << "TOTAL SCORE: " << (totalScore / numberOfBatches) << std::endl;

    std::cout.rdbuf(coutbuf);
}

TEST(orbMatching, performance) {

    std::ofstream out("orb_matching.log");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::vector<const Ticket*> tickets = getAllTemplatesOf("resources/templates");
    std::vector<TestCase*> testcases = getAllTestsOf("resources/tickets");

    double totalScore = 0.0;
    int numberOfBatches = 0;
    for(auto& batch : getBatchesOfTwoThirds(tickets)) {
        std::shared_ptr<MatchingAlgorithm> matchingAlgorithm = MatchingAlgorithms::orb(MatchingOptions());
        Matcher matcher(*matchingAlgorithm);
        totalScore += runMatching(batch, testcases, matcher);
        numberOfBatches++;
    }
    std::cout << "TOTAL SCORE: " << (totalScore / numberOfBatches) << std::endl;

    std::cout.rdbuf(coutbuf);
}

TEST(siftExtraction, performance) {

    std::ofstream out("sift_extraction.log");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::vector<const Ticket*> tickets = getAllTemplatesOf("resources/templates");
    std::vector<TestCase*> testcases = getAllTestsOf("resources/tickets");

    std::shared_ptr<ExtractionAlgorithm> extractionAlgorithm = ExtractionAlgorithms::sift(ExtractionOptions());
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

    std::shared_ptr<ExtractionAlgorithm> extractionAlgorithm = ExtractionAlgorithms::orb(ExtractionOptions());
    MetadataReader reader(*extractionAlgorithm);

    runExtraction(tickets, testcases, reader);

    std::cout.rdbuf(coutbuf);
}

const cti::Ticket* findTicket(std::vector<const Ticket*>& tickets, const std::string& name) {
    for (auto* ticket : tickets) {
        if (ticket->name() == name) {
            return ticket;
        }
    }
    return nullptr;
}

double runMatching(std::vector<const Ticket *>& tickets, std::vector<TestCase*>& testcases, Matcher& matcher) {

    int trainingTime = cti::Timer::timed([&matcher, &tickets] () {
        matcher.train(tickets);
    });

    cti::reader::ConfusionMatrix totalMatchingScore { 0, 0, 0, 0 };

    int totalMatchingTime = 0;
    int totalTestcases = 0;

    for(auto& testcase : testcases) {
        for (auto& imagePath : testcase->getImages()) {
            totalTestcases++;

            std::cout << "RUN TESTCASE " << imagePath << std::endl;
            TicketImage inputImage { imagePath };

            bool correctMatch = false;
            string matchName = "";
            string expectedName = "";
            int matchingTime = cti::Timer::timed([&matcher, &correctMatch, &matchName, &expectedName, &tickets, &inputImage, &testcase, &totalMatchingScore] () {

                const std::optional<const TicketMatch> matchedOpt = matcher.match(inputImage);
                cti::reader::ConfusionMatrix matchingScore = calcMatchingScore(tickets, testcase, matchedOpt);
                totalMatchingScore = totalMatchingScore + matchingScore;

                bool shouldMatch = findTicket(tickets, testcase->getExpectedTemplateId()) != nullptr;
                bool matchedAnything = matchedOpt.has_value();
                bool matchedCorrectly = matchedAnything && matchedOpt.value().ticket().name() == testcase->getExpectedTemplateId();
                correctMatch = (!shouldMatch && !matchedAnything) || (shouldMatch && matchedCorrectly);
                matchName = (matchedOpt.has_value() ? matchedOpt.value().ticket().name() : "NONE");
                expectedName = shouldMatch ? testcase->getExpectedTemplateId() : "NONE";
            });

            std::cout << (correctMatch ? "correct" : "incorrect")
                      << " match: " << matchName
                      << " expected: " << expectedName
                      << " took " << matchingTime << "ms" << std::endl;
            totalMatchingTime += matchingTime;
        }
    }

    std::cout << "ACCURACY: " << totalMatchingScore.accuracy()
              << " TP=" << totalMatchingScore.truePositives()
              << " TN="<< totalMatchingScore.trueNegatives()
              << " FP=" << totalMatchingScore.falsePositives()
              << " FN=" << totalMatchingScore.falseNegatives()
              << " Training took " << trainingTime << " ms for " << tickets.size() << " Tickets"
              << " = " << (double)trainingTime / tickets.size() << " ms/Ticket"
              << " Matching took " << totalMatchingTime << " ms for " << totalTestcases << " Testcases"
              << " = " << (double)totalMatchingTime / totalTestcases << "ms/Testcase"
              << std::endl;

    return totalMatchingScore.accuracy();
}

double runExtraction(std::vector<const Ticket *>& tickets, std::vector<TestCase*>& testcases, MetadataReader& reader) {

    double totalScore = 0.0;
    int totalCases = 0;

    int time = cti::Timer::timed([&tickets, &testcases, &reader, &totalScore, &totalCases] () {

        for(auto& testcase : testcases) {
            for (auto& imagePath : testcase->getImages()) {
                totalCases++;
                std::cout << "RUN TESTCASE " << imagePath << std::endl;

                const Ticket* matchedTicket = findTicket(tickets, testcase->getExpectedTemplateId());
                if(matchedTicket == nullptr) {
                    std::cout << "ERROR: Unable to find template" << std::endl;
                    continue;
                }

                TicketImage inputImage { imagePath };
                const cti::Metadata* const metadata = reader.read(*matchedTicket, inputImage);
                totalScore += calcExtractionScore(testcase, metadata);

                // TODO: remove debugging code
//                for (auto &text : metadata->texts()) {
//                    std::cout << "Extracted text '" << text.first << "' : '" << text.second << "'" << std::endl;
//                }
            }
        }
    });
    std::cout << "SCORE: " << totalScore / totalCases << " testcases=" << totalCases
              << " time=" << time << "ms"  << " = " << ((double)time / testcases.size()) << "ms/testcase"<< std::endl;
    return totalScore / totalCases;
}

cti::reader::ConfusionMatrix calcMatchingScore(std::vector<const Ticket *>& tickets, TestCase* testcase, const std::optional<const TicketMatch>& matchedOpt) {

    int truePositives = 0;
    int trueNegatives = 0;
    int falsePositives = 0;
    int falseNegatives = 0;

    bool shouldMatch = findTicket(tickets, testcase->getExpectedTemplateId()) != nullptr;
    bool matchedAnything = matchedOpt.has_value();
    bool correctMatch = matchedAnything && matchedOpt.value().ticket().name() == testcase->getExpectedTemplateId();

    if(shouldMatch) {
        if(correctMatch) {
            truePositives++;
        } else {
            falseNegatives++;
        }
    } else {
        if(matchedAnything) {
            falsePositives++;
        } else {
            trueNegatives++;
        }
    }

    cti::reader::ConfusionMatrix result { truePositives, trueNegatives, falsePositives, falseNegatives };
    return result;
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

double calcExtractionScore(TestCase* testcase, const cti::Metadata* const metadata) {

    int correct = 0;
    int incorrect = 0;

    unordered_map<string, string> actualTexts = metadata->texts();

    for (auto &expectedText : testcase->getExpectedTexts()) {

        unordered_map<string, string>::const_iterator actualTextIt = actualTexts.find(expectedText.first);
        if(actualTextIt == actualTexts.end()) {
            std::cout << "Missing text: key=" << expectedText.first
                      << " expected_text='" << expectedText.second << "'"
                      << std::endl;
            incorrect++;
        } else {
            string actualText = actualTextIt->second;

            // TODO: Tesseract has a tendency to recognize '—' instead of '-'. Should we replace this?
            // TODO: Any application using this library will need to sanitize the text retrieved using this library
            // TODO: therefor, that application will know, whether a hyphen can occur in the specific string
            replaceAll(actualText, "—", "-");

            if(actualText == expectedText.second) {
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

std::vector<std::vector<const Ticket*>> getBatchesOfTwoThirds(std::vector<const Ticket*>& tickets) {

    std::vector<std::vector<const Ticket*>> batches;

    auto itemsPerRun = (tickets.size() / 3);
    auto remainingItems = (tickets.size() % 3);
    auto itemsFirstRun = itemsPerRun + (remainingItems-- > 0 ? 1 : 0);
    auto itemsSecondRun = itemsPerRun + (remainingItems-- > 0 ? 1 : 0);
    auto itemsThirdRun = itemsPerRun;

    std::vector<const Ticket*>::const_iterator position = tickets.begin();
    batches.emplace_back(std::vector<const Ticket*> { position, position + itemsFirstRun + itemsSecondRun});
    position += itemsFirstRun;

    batches.emplace_back(std::vector<const Ticket*> { position, position + itemsSecondRun + itemsThirdRun});
    position += itemsSecondRun;

    auto thirdRun = std::vector<const Ticket*> { position, position + itemsThirdRun};
    position = tickets.begin();
    auto thirdRunSecondPart = std::vector<const Ticket*> { position, position + itemsFirstRun};
    thirdRun.insert(thirdRun.end(), thirdRunSecondPart.begin(), thirdRunSecondPart.end());
    batches.emplace_back(thirdRun);

    return batches;
}
