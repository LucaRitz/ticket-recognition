#include <gtest/gtest.h>
#include <include/matcher.hpp>
#include <include/ticket_match.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include <include/text.hpp>
#include <include/matching/matching_algorithm.hpp>
#include <include/matching/matching_algorithms.hpp>
#include <reader/reader.hpp>
#include <reader/test_case.hpp>
#include <reader/confusion_matrix.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "../cti/timer/timer.hpp"
#include "./test_util.hpp"

using cti::Matcher;
using cti::TicketImage;
using cti::TicketMatch;
using cti::Ticket;
using cti::Text;
using cti::MatchingAlgorithm;
using cti::MatchingAlgorithms;
using cti::reader::getAllTemplatesOf;
using cti::reader::getAllTestsOf;
using cti::reader::TestCase;

cti::reader::ConfusionMatrix calcMatchingScore(std::vector<const Ticket *>& tickets, TestCase* testcase, const std::optional<const TicketMatch>& matchedOpt);
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
        std::shared_ptr<MatchingAlgorithm> matchingAlgorithm = MatchingAlgorithms::sift();
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
        std::shared_ptr<MatchingAlgorithm> matchingAlgorithm = MatchingAlgorithms::orb();
        Matcher matcher(*matchingAlgorithm);
        totalScore += runMatching(batch, testcases, matcher);
        numberOfBatches++;
    }
    std::cout << "TOTAL SCORE: " << (totalScore / numberOfBatches) << std::endl;

    std::cout.rdbuf(coutbuf);
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

                bool shouldMatch = cti::reader::findTicket(tickets, testcase->getExpectedTemplateId()) != nullptr;
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

cti::reader::ConfusionMatrix calcMatchingScore(std::vector<const Ticket *>& tickets, TestCase* testcase, const std::optional<const TicketMatch>& matchedOpt) {

    int truePositives = 0;
    int trueNegatives = 0;
    int falsePositives = 0;
    int falseNegatives = 0;

    bool shouldMatch = cti::reader::findTicket(tickets, testcase->getExpectedTemplateId()) != nullptr;
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
