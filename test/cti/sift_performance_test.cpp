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
#include <include/sift/sift_matching_algorithm.hpp>
#include <include/sift/sift_extraction_algorithm.hpp>
#include <reader/reader.hpp>
#include <reader/test_case.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

using cti::Matcher;
using cti::MetadataReader;
using cti::TicketImage;
using cti::TicketMatch;
using cti::Ticket;
using cti::Text;
using cti::Metadata;
using cti::SiftMatchingAlgorithm;
using cti::SiftExtractionAlgorithm;
using cti::reader::getAllTemplatesOf;
using cti::reader::getAllTestsOf;
using cti::reader::TestCase;

TEST(siftMatching, tenTemplatesOneShouldMatch) {
    std::vector<Ticket*> tickets = getAllTemplatesOf("resources/input_data");
    for(auto* ticket : tickets) {
        std::cout << "Template Id: " << ticket->name() << std::endl;
        std::cout << "Template image: " << std::endl;
        std::cout << "Texts:" << std::endl;
        for (const Text* text : ticket->texts()) {
            std::cout << "Text-Key: " << text->key() << std::endl;
            std::cout << "Point Top-Left -> x: " << text->boundingBox().topLeft().x() << " ";
            std::cout << "y: " << text->boundingBox().topLeft().y() << std::endl;
            std::cout << "Point Bottom-Right -> x: " << text->boundingBox().bottomRight().x() << " ";
            std::cout << "y: " << text->boundingBox().bottomRight().y() << std::endl;
        }
    }

    std::vector<TestCase*> testCases = getAllTestsOf("resources/test_cases");
    for(auto* testCase : testCases) {
        std::cout << "Expected Template Id: " << testCase->getExpectedTemplateId() << std::endl;
        std::cout << "Image: " << testCase->getImage() << std::endl;
        std::cout << "Texts:" << std::endl;
        for (std::pair< std::string, std::string > element : testCase->getExpectedTexts()) {
            std::cout << "Text-Key: " << element.first << std::endl;
            std::cout << "Expected value: " << element.second << std::endl;
        }
    }

    std::cout << "Create Matching Algo" << std::endl;
    SiftMatchingAlgorithm matchingAlgorithm;
    std::cout << "Create Reading Algo" << std::endl;
    SiftExtractionAlgorithm extractionAlgorithm;

    std::cout << "Create Matcher" << std::endl;
    Matcher matcher(matchingAlgorithm);
    std::cout << "Create Reader" << std::endl;
    MetadataReader reader(extractionAlgorithm);

    // Act
    std::cout << "Act" << std::endl;
    const std::optional<const TicketMatch> matchedOpt = matcher.match(tickets.at(0)->image());
    const cti::Metadata* const metadata = reader.read(Ticket("", tickets.at(0)->image(), *(new vector<const Text*>)), *(new cti::TicketImage("resources/images/test.png")));

    // Assert
    ASSERT_TRUE(matchedOpt);
    ASSERT_EQ("this_template_matched", matchedOpt.value().name());
    ASSERT_EQ("100", metadata->texts().at("TourCode"));
}