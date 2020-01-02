#include "matching_algorithm_impl.hpp"
#include "../timer/timer.hpp"
#include <include/ticket.hpp>
#include <include/ticket_image.hpp>
#include <include/ticket_match.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using std::string;
using std::vector;

cti::impl::MatchingAlgorithmImpl::MatchingAlgorithmImpl(
        double ratioTestThreshold,
        double scoreTestThreshold,
        double scoreThreshold,
        cv::Ptr<cv::Feature2D> feature2d,
        cv::Ptr<cv::DescriptorMatcher> matcher) :
        ratioTestThreshold(ratioTestThreshold),
        scoreTestThreshold(scoreTestThreshold),
        scoreThreshold(scoreThreshold),
        feature2d(std::move(feature2d)),
        matcher(std::move(matcher)) {

}

void cti::impl::MatchingAlgorithmImpl::train(const cti::Ticket& ticketTemplate) {
    vector<const cti::Ticket*> tickets;
    tickets.push_back(&ticketTemplate);
    train(tickets);
}

void cti::impl::MatchingAlgorithmImpl::train(const vector<const cti::Ticket*>& ticketTemplates) {
    unordered_map<string, image_template_t> existingTicketsToLearn = this->_trained;
    for (auto* ticket : ticketTemplates) {
        auto positionInMap = existingTicketsToLearn.find(ticket->name());
        if (positionInMap != existingTicketsToLearn.end()) {
            existingTicketsToLearn.erase(positionInMap);
        }
    }

    bool relearnSomeTemplates = !this->_trained.empty() && existingTicketsToLearn.size() < this->_trained.size();
    if (relearnSomeTemplates) {
        clear();

        std::vector<const cti::Ticket*> tickets;
        std::for_each(existingTicketsToLearn.begin(), existingTicketsToLearn.end(),
                      [&tickets](const unordered_map<string, image_template_t>::value_type& type){
                          tickets.push_back(type.second._ticket);
                      });
        trainAll(tickets);
    }

    trainAll(ticketTemplates);
}

void cti::impl::MatchingAlgorithmImpl::untrain(const cti::Ticket& ticket) {
    unordered_map<string, image_template_t> ticketMap = this->_trained;
    auto positionInMap = ticketMap.find(ticket.name());
    if (positionInMap != ticketMap.end()) {
        ticketMap.erase(positionInMap);
        std::vector<const cti::Ticket*> tickets;
        std::for_each(ticketMap.begin(), ticketMap.end(),
                [&tickets](const unordered_map<string, image_template_t>::value_type& type){
            tickets.push_back(type.second._ticket);
        });
        clear();
        trainAll(tickets);
    }
}

vector<cti::TicketMatch> cti::impl::MatchingAlgorithmImpl::execute(const cti::TicketImage& ticketImage) {

    const Mat inputImage = Mat(ticketImage.height(), ticketImage.width(), CV_8UC(ticketImage.bytesPerPixel()), ticketImage.image());

    vector<KeyPoint> inputKeypoints;
    int keypointTime = cti::Timer::timed([this, &inputImage, &inputKeypoints] () {
        feature2d->detect(inputImage, inputKeypoints);
    });

    Mat inputDescriptors;
    int descriptorTime = cti::Timer::timed([this, &inputImage, &inputKeypoints, &inputDescriptors] () {
        feature2d->compute(inputImage, inputKeypoints, inputDescriptors);
    });

    auto matches = vector<cti::TicketMatch>();

    vector<vector<DMatch>> knnMatches;
    int knnTime = cti::Timer::timed([this, &inputDescriptors, &knnMatches] () {
        this->matcher->knnMatch(inputDescriptors, knnMatches, 2);
    });

    vector<DMatch> bestMatches = ratioTest(knnMatches, ratioTestThreshold);

    vector<int> imgMatchCount;
    imgMatchCount.reserve(this->_trained.size());
    for(size_t i = 0; i < this->_trained.size(); i++) {
        imgMatchCount.push_back(0);
    }

    for (auto &m : bestMatches) {
        if(m.imgIdx < this->_trained.size()) {
            imgMatchCount.at(m.imgIdx) = imgMatchCount.at(m.imgIdx) + 1;
        }
    }

    const Ticket* bestMatch = nullptr;
    int bestCount = 0;
    const Ticket* secondBestMatch = nullptr;
    int secondBestCount = 0;

    std::stringstream test;
    for(size_t i = 0; i < imgMatchCount.size(); i++) {

        int count = imgMatchCount.at(i);
        const Ticket* match = findTicketByImagePosition(this->_trained, i);
        #ifdef CTI_DEBUG
            test << match->name() << "=" << count << " ";
        #endif

        if(bestMatch == nullptr || count > bestCount) {
            secondBestMatch = bestMatch;
            secondBestCount = bestCount;
            bestMatch = match;
            bestCount = count;
        } else if(secondBestMatch == nullptr ||count > bestCount){
            secondBestMatch = match;
            secondBestCount = count;
        }
    }

    bool matched = false;

    if(bestMatch != nullptr) {
        if(secondBestMatch != nullptr) {
            // Best match must be a good match
            if(bestCount >= scoreThreshold) {
                // Best match must be considerably better than the second best match
                if(bestCount * scoreTestThreshold >= secondBestCount) {
                    matched = true;
                }
            }
        } else {
            // Best match must be a good match
            if(bestCount >= scoreThreshold) {
                matched = true;
            }
        }
    }

    #ifdef CTI_DEBUG
        int totalTime = knnTime + keypointTime + descriptorTime;
        std::cout << "SCORE: " << bestCount << " MATCHES: " << (matched ? bestMatch->name() : "NONE") << " TIME=" << totalTime << "ms " << " REST: " << test.str() << std::endl;
    #endif
    if(matched) {
        matches.emplace_back(*bestMatch, (double)bestCount);
    }

    return matches;
}

std::pair<cti::TicketMatch*, cti::TicketMatch*> cti::impl::MatchingAlgorithmImpl::findBestTwoMatches(
        vector<cti::TicketMatch>& matches) const {

    cti::TicketMatch* bestMatch = nullptr;
    cti::TicketMatch* secondBestMatch = nullptr;
    for(auto& match : matches) {
        if(bestMatch == nullptr || match.score() > bestMatch->score()) {
            secondBestMatch = bestMatch;
            bestMatch = &match;
        } else if(secondBestMatch == nullptr || match.score() > secondBestMatch->score()){
            secondBestMatch = &match;
        }
    }
    return std::make_pair(bestMatch, secondBestMatch);
}

bool cti::impl::MatchingAlgorithmImpl::isBestMatchGood(
        cti::TicketMatch* bestMatch, cti::TicketMatch* secondBestMatch) const {

    if(bestMatch != nullptr && secondBestMatch != nullptr) {
        // Best match must be a good match
        if(bestMatch->score() >= scoreThreshold) {
            // Best match must be considerably better than the second best match
            if(bestMatch->score() * scoreTestThreshold >= secondBestMatch->score()) {
                return true;
            }
        }
    }
    return false;
}

const optional<cti::TicketMatch> cti::impl::MatchingAlgorithmImpl::select(vector<cti::TicketMatch>& matches) const {

    if(matches.size() > 1) {
        auto bestTwo = findBestTwoMatches(matches);
        if(isBestMatchGood(bestTwo.first, bestTwo.second)) {
            return std::make_optional(*bestTwo.first);
        }
        return std::nullopt;
    }

    if(matches.size() == 1) {
        // Match must be a good match
        if(matches.at(0).score() >= scoreThreshold) {
            return std::make_optional(matches.at(0));
        }
        return std::nullopt;
    }

    return std::nullopt;
}

vector<cv::DMatch> cti::impl::MatchingAlgorithmImpl::ratioTest(
        const vector<vector<cv::DMatch>>& knnMatches, double threshold) const {

    vector<DMatch> matches;
    for (auto &match : knnMatches) {
        if(match.size() >= 2) {
            auto &first = match[0];
            auto &second = match[1];
            if (first.distance < threshold * second.distance) {
                matches.push_back(first);
            }
        }
    }
    return matches;
}

const cti::Ticket *const
cti::impl::MatchingAlgorithmImpl::findTicketByImagePosition(
        const unordered_map<string, cti::impl::image_template_t> &templates, int imagePosition) const {
    for (auto& ticketImage : templates) {
        if (ticketImage.second._imagePosition == imagePosition) {
            return ticketImage.second._ticket;
        }
    }

    return nullptr;
}

void cti::impl::MatchingAlgorithmImpl::clear() {
    this->feature2d->clear();
    this->matcher->clear();
    this->_trained.clear();
}

void cti::impl::MatchingAlgorithmImpl::trainAll(const vector<const Ticket*>& tickets) {
    for (auto* ticketTemplate : tickets) {
        const string name = ticketTemplate->name();

        const TicketImage& image = ticketTemplate->image();
        const Mat templateImage = Mat(image.height(), image.width(), CV_8UC(image.bytesPerPixel()), image.image());

        vector<KeyPoint> keypoints;
        this->feature2d->detect(templateImage, keypoints);

        Mat descriptors;
        this->feature2d->compute(templateImage, keypoints, descriptors);

        this->matcher->add(descriptors);

        image_template_t imageTemplate {
                ticketTemplate,
                this->_trained.size()
        };

        this->_trained.insert(std::pair<string, image_template_t>(name, imageTemplate));
    }

    if (!this->_trained.empty()) {
        this->matcher->train();
    }
}