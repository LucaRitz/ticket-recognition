#include <gtest/gtest.h>
#include <include/matching/matching_algorithm.hpp>
#include <cti/matching/matching_algorithm_impl.hpp>
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/text.hpp>
#include <include/bounding_box.hpp>
#include <include/point.hpp>
#include "feature2d_mock.hpp"
#include "description_matcher_mock.hpp"
#include "../../io/file_util.hpp"

namespace cti::test {
    std::vector<const cti::Ticket*> createTickets(Ticket*&,Ticket*&);
    void createTicket(Ticket*&);
}

using cti::impl::MatchingAlgorithmImpl;
using cti::mock::Feature2dMock;
using cti::mock::DescriptionMatcherMock;
using cti::Ticket;
using cti::TicketImage;
using cti::Text;
using cti::BoundingBox;
using cti::Point;
using cti::test::createTickets;
using cti::test::createTicket;
using cti::io::replaceDirectorySeparator;
using ::testing::_;

class MatchingAlgorithmTest : public ::testing::Test {
public:
    explicit MatchingAlgorithmTest()
            : _feature2d(cv::makePtr<Feature2dMock>()), _matcher(cv::makePtr<DescriptionMatcherMock>()), _algorithm(0.7, 0.5, 25, _feature2d, _matcher) {

    }

protected:
    void SetUp() override {
        // NOOP
    }

    void TearDown() override {
        // NOOP
    }

    cv::Ptr<Feature2dMock> _feature2d;
    cv::Ptr<DescriptionMatcherMock> _matcher;
    MatchingAlgorithmImpl _algorithm;
};

TEST_F(MatchingAlgorithmTest, train_nothingTrainedBefore_allTrained) {
    Ticket* ticket1;
    Ticket* ticket2;
    std::vector<const Ticket*> tickets = createTickets(ticket1, ticket2);

    // Assert
    EXPECT_CALL(*this->_feature2d, detect(_,_,_)).Times(tickets.size());
    EXPECT_CALL(*this->_feature2d, compute(_,_,_)).Times(tickets.size());
    EXPECT_CALL(*this->_matcher, add(_)).Times(tickets.size());
    EXPECT_CALL(*this->_matcher, train()).Times(1);

    // Act
    this->_algorithm.train(tickets);
}

TEST_F(MatchingAlgorithmTest, train_someTrainedBeforeAndSameTemplateAppliedAgain_retrainTemplate) {
    Ticket* ticket1;
    Ticket* ticket2;
    std::vector<const Ticket*> tickets = createTickets(ticket1, ticket2);

    this->_algorithm.train(tickets);

    Ticket* ticket3;
    createTicket(ticket3);
    std::vector<const Ticket*> retrainedTickets = {
            ticket1,
            ticket3
    };

    // Assert
    EXPECT_CALL(*this->_feature2d, clear());
    EXPECT_CALL(*this->_matcher, clear());
    EXPECT_CALL(*this->_feature2d, detect(_,_,_)).Times(3);
    EXPECT_CALL(*this->_feature2d, compute(_,_,_)).Times(3);
    EXPECT_CALL(*this->_matcher, add(_)).Times(3);
    EXPECT_CALL(*this->_matcher, train()).Times(2);

    // Act
    this->_algorithm.train(retrainedTickets);
}

TEST_F(MatchingAlgorithmTest, untrain_nonTrainedTemplateApplied_doNothing) {
    Ticket* ticket1;
    Ticket* ticket2;
    std::vector<const Ticket*> tickets = createTickets(ticket1, ticket2);

    this->_algorithm.train(*ticket1);

    // Assert
    EXPECT_CALL(*this->_feature2d, clear()).Times(0);
    EXPECT_CALL(*this->_matcher, clear()).Times(0);

    // Act
    this->_algorithm.untrain(*ticket2);
}

TEST_F(MatchingAlgorithmTest, untrain_trainedTemplateApplied_removeTemplate) {
    Ticket* ticket1;
    Ticket* ticket2;
    std::vector<const Ticket*> tickets = createTickets(ticket1, ticket2);

    this->_algorithm.train(tickets);

    // Assert
    EXPECT_CALL(*this->_feature2d, clear());
    EXPECT_CALL(*this->_matcher, clear());

    // Act
    this->_algorithm.untrain(*ticket2);
}

std::vector<const cti::Ticket*> cti::test::createTickets(Ticket*& ticket1, Ticket*& ticket2) {
    vector<const Text*> texts_1;
    Text* text_1_for = new Text(std::string("for"), *new BoundingBox(Point(85, 130), Point(385, 185)));
    Text* text_1_value = new Text(std::string("value"), *new BoundingBox(Point(265, 195), Point(385, 235)));
    Text* text_1_date = new Text(std::string("date"), *new BoundingBox(Point(130, 240), Point(390, 280)));
    texts_1.push_back(text_1_for);
    texts_1.push_back(text_1_value);
    texts_1.push_back(text_1_date);
    ticket1 = new Ticket(std::string("template_key_1"), *new TicketImage(
            replaceDirectorySeparator(std::string("resources/images/adventurerooms.png"))), texts_1);

    vector<const Text*> texts_2;
    Text* text_2_value = new Text(std::string("value"), *new BoundingBox(Point(195, 231), Point(425, 291)));
    Text* text_2_code = new Text(std::string("code"), *new BoundingBox(Point(220, 310), Point(540, 370)));
    Text* text_2_for = new Text(std::string("for"), *new BoundingBox(Point(100, 145), Point(370, 200)));
    Text* text_2_from = new Text(std::string("from"), *new BoundingBox(Point(420, 140), Point(710, 200)));
    texts_2.push_back(text_2_value);
    texts_2.push_back(text_2_code);
    texts_2.push_back(text_2_for);
    texts_2.push_back(text_2_from);
    ticket2 = new Ticket(std::string("template_key_2"), *new TicketImage(
            replaceDirectorySeparator(std::string("resources/images/babymarkt.jpg"))), texts_2);

    std::vector<const cti::Ticket*> tickets;
    tickets.push_back(ticket1);
    tickets.push_back(ticket2);
    return tickets;
}

void cti::test::createTicket(Ticket*& ticket) {
    vector<const Text*> texts_1;
    Text* text_1_for = new Text(std::string("for"), *new BoundingBox(Point(85, 130), Point(385, 185)));
    Text* text_1_value = new Text(std::string("value"), *new BoundingBox(Point(265, 195), Point(385, 235)));
    Text* text_1_date = new Text(std::string("date"), *new BoundingBox(Point(130, 240), Point(390, 280)));
    texts_1.push_back(text_1_for);
    texts_1.push_back(text_1_value);
    texts_1.push_back(text_1_date);
    ticket = new Ticket(std::string("template_key_3"), *new TicketImage(
            replaceDirectorySeparator(std::string("resources/images/adventurerooms.png"))), texts_1);
}