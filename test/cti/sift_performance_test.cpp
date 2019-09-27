#include <gtest/gtest.h>
#include <include/matcher.hpp>
#include <include/metadata_reader.hpp>
#include <include/ticket_match.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket.hpp>
#include <include/text.hpp>
#include <include/metadata.hpp>
#include <sift/sift_matching_algorithm.hpp>
#include <sift/sift_extraction_algorithm.hpp>

using cti::Matcher;
using cti::MetadataReader;
using cti::TicketImage;
using cti::TicketMatch;
using cti::Ticket;
using cti::Text;
using cti::Metadata;
using cti::SiftMatchingAlgorithm;
using cti::SiftExtractionAlgorithm;

TEST(siftMatching, tenTemplatesOneShouldMatch) {
    std::cout << "Create Matching Algo" << std::endl;
    SiftMatchingAlgorithm matchingAlgorithm;
    std::cout << "Create Reading Algo" << std::endl;
    SiftExtractionAlgorithm extractionAlgorithm;

    std::cout << "Create Matcher" << std::endl;
    Matcher matcher(matchingAlgorithm);
    std::cout << "Create Reader" << std::endl;
    MetadataReader reader(extractionAlgorithm);

    TicketImage image("test_image.png");

    // Act
    std::cout << "Act" << std::endl;
    const std::optional<const TicketMatch> matchedOpt = matcher.match(image);
    const cti::Metadata* const metadata = reader.read(Ticket("", image, vector<const Text*>()), image);

    // Assert
    ASSERT_TRUE(matchedOpt);
    ASSERT_EQ("this_template_matched", matchedOpt.value().name());
    ASSERT_EQ("100", metadata->texts().at("TourCode"));
}