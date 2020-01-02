# Ticket Recognition
## Getting started
The first example starts with the creation of a text model. A model consists of n texts, a template name
and the path to an image. A text itself consists of a bounding-box and a key.
The bounding-box is described by two points. These two points spread a rectangle.
```
cti::Text* text_for = new cti::Text(std::string("for"), *new cti::BoundingBox(Point(85, 130), Point(385, 185)));
vector<const cti::Text*> texts = {text_for};
cti::Ticket* ticket = new cti::Ticket(std::string("template_key_1"), cti::TicketImage(
        std::string("resources/images/adventurerooms.png")), texts);
```
This ticket model can be used to train the matcher.
```
 std::unique_ptr<cti::MatchingAlgorithm> matchingAlgorithm = cti::MatchingAlgorithms::sift();
 cti::Matcher matcher(*matchingAlgorithm);
 matcher.train(*ticket);
```
Any previously trained template can be removed on runtime.
```
matcher.untrain(*ticket);
```
Then, the matcher is prepared for usage. Any image can be passed.
```
cti::TicketImage inputImage { imagePath };
const std::optional<const cti::TicketMatch> matchedOpt = matcher.match(inputImage);
```
If the optional is present (any template has matched the input) the text value can be read by a MetadataReader.
```
if (matchedOpt.has_value()) {
   cti::MetadataReader reader(*cti::ExtractionAlgorithms::sift());
   const cti::Metadata metadata = reader.read(matchedOpt.value().ticket(), inputImage);
    
   // Access extracted text
   metadata.texts();
}
```
*Full example*
```
cti::Text* text_for = new cti::Text(std::string("for"), *new cti::BoundingBox(Point(85, 130), Point(385, 185)));
vector<const cti::Text*> texts = {text_for};
cti::Ticket* ticket = new cti::Ticket(std::string("template_key_1"), cti::TicketImage(
        std::string("resources/images/adventurerooms.png")), texts);

std::unique_ptr<cti::MatchingAlgorithm> matchingAlgorithm = cti::MatchingAlgorithms::sift();
cti::Matcher matcher(*matchingAlgorithm);
matcher.train(*ticket);

cti::TicketImage inputImage { imagePath };
const std::optional<const cti::TicketMatch> matchedOpt = matcher.match(inputImage);

if (matchedOpt.has_value()) {
   cti::MetadataReader reader(*cti::ExtractionAlgorithms::sift());
   const cti::Metadata metadata = reader.read(matchedOpt.value().ticket(), inputImage);
    
   // Access extracted text
   metadata.texts();
}
```
