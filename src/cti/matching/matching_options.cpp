#include <include/matching/matching_options.hpp>

cti::MatchingOptions::MatchingOptions(double ratioTestThreshold, double scoreTestThreshold, double scoreThreshold) :
        _ratioTestThreshold(ratioTestThreshold),
        _scoreTestThreshold(scoreTestThreshold),
        _scoreThreshold(scoreThreshold) {
}

double cti::MatchingOptions::ratioTestThreshold() const {
    return _ratioTestThreshold;
}

double cti::MatchingOptions::scoreTestThreshold() const {
    return _scoreTestThreshold;
}

double cti::MatchingOptions::scoreThreshold() const {
    return _scoreThreshold;
}