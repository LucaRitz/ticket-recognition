#include <include/extraction/extraction_options.hpp>

cti::ExtractionOptions::ExtractionOptions(double ratioTestThreshold, std::string language) :
        _ratioTestThreshold(ratioTestThreshold),
        _language(std::move(language)) {

}

double cti::ExtractionOptions::ratioTestThreshold() const {
    return _ratioTestThreshold;
}

const std::string& cti::ExtractionOptions::language() const {
    return _language;
}