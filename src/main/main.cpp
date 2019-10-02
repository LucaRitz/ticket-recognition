#include <iostream>
#include <include/matcher.hpp>
#include <include/metadata_reader.hpp>
#include <include/ticket_match.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket.hpp>
#include <include/text.hpp>
#include <include/metadata.hpp>
#include <include/sift/sift_matching_algorithm.hpp>
#include <include/sift/sift_extraction_algorithm.hpp>

using std::cout;
using std::endl;
using cti::Matcher;
using cti::MetadataReader;
using cti::TicketImage;
using cti::TicketMatch;
using cti::Ticket;
using cti::Text;
using cti::Metadata;
using cti::SiftMatchingAlgorithm;
using cti::SiftExtractionAlgorithm;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
        cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }

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

    return 0;
}