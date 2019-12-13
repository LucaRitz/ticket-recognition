#include <include/metadata.hpp>

cti::Metadata::Metadata(const unordered_map<string, string> texts) : _texts(texts) {

}

const unordered_map<string, string>& cti::Metadata::texts() const {
    return _texts;
}
