#include <optional>
#include <sstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "wiki_article.h"
#include "wiki_iter.h"

namespace revfad_wiki {
namespace {
using ::testing::Eq;
using ::testing::Optional;
// The following contains two pages, "AccessibleComputing" and "Anarchism".
constexpr char kWikipediaXml1[] =
    R"(<mediawiki xmlns="http://www.mediawiki.org/xml/export-0.11/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.mediawiki.org/xml/export-0.11/ http://www.mediawiki.org/xml/export-0.11.xsd" version="0.11" xml:lang="en">
  <siteinfo>
    <sitename>Wikipedia</sitename>
    <dbname>enwiki</dbname>
    <base>https://en.wikipedia.org/wiki/Main_Page</base>
    <generator>MediaWiki 1.45.0-wmf.3</generator>
    <case>first-letter</case>
    <namespaces>
      <namespace key="-2" case="first-letter">Media</namespace>
      <namespace key="-1" case="first-letter">Special</namespace>
      <namespace key="0" case="first-letter" />
      <namespace key="1" case="first-letter">Talk</namespace>
      <namespace key="2" case="first-letter">User</namespace>
      <namespace key="3" case="first-letter">User talk</namespace>
      <namespace key="4" case="first-letter">Wikipedia</namespace>
      <namespace key="5" case="first-letter">Wikipedia talk</namespace>
      <namespace key="6" case="first-letter">File</namespace>
      <namespace key="7" case="first-letter">File talk</namespace>
      <namespace key="8" case="first-letter">MediaWiki</namespace>
      <namespace key="9" case="first-letter">MediaWiki talk</namespace>
      <namespace key="10" case="first-letter">Template</namespace>
      <namespace key="11" case="first-letter">Template talk</namespace>
      <namespace key="12" case="first-letter">Help</namespace>
      <namespace key="13" case="first-letter">Help talk</namespace>
      <namespace key="14" case="first-letter">Category</namespace>
      <namespace key="15" case="first-letter">Category talk</namespace>
      <namespace key="100" case="first-letter">Portal</namespace>
      <namespace key="101" case="first-letter">Portal talk</namespace>
      <namespace key="118" case="first-letter">Draft</namespace>
      <namespace key="119" case="first-letter">Draft talk</namespace>
      <namespace key="126" case="first-letter">MOS</namespace>
      <namespace key="127" case="first-letter">MOS talk</namespace>
      <namespace key="710" case="first-letter">TimedText</namespace>
      <namespace key="711" case="first-letter">TimedText talk</namespace>
      <namespace key="828" case="first-letter">Module</namespace>
      <namespace key="829" case="first-letter">Module talk</namespace>
      <namespace key="1728" case="first-letter">Event</namespace>
      <namespace key="1729" case="first-letter">Event talk</namespace>
    </namespaces>
  </siteinfo>
  <page>
    <title>AccessibleComputing</title>
    <ns>0</ns>
    <id>10</id>
    <redirect title="Computer accessibility" />
    <revision>
      <id>1219062925</id>
      <parentid>1219062840</parentid>
      <timestamp>2024-04-15T14:38:04Z</timestamp>
      <contributor>
        <username>Asparagusus</username>
        <id>43603280</id>
      </contributor>
      <comment>Restored revision 1002250816 by [[Special:Contributions/Elli|Elli]] ([[User talk:Elli|talk]]): Unexplained redirect breaking</comment>
      <origin>1219062925</origin>
      <model>wikitext</model>
      <format>text/x-wiki</format>
      <text bytes="111" sha1="kmysdltgexdwkv2xsml3j44jb56dxvn" xml:space="preserve">#REDIRECT [[Computer accessibility]]

{{rcat shell|
{{R from move}}
{{R from CamelCase}}
{{R unprintworthy}}
}}</text>
      <sha1>kmysdltgexdwkv2xsml3j44jb56dxvn</sha1>
    </revision>
  </page>
  <page>
    <title>Anarchism</title>
    <ns>0</ns>
    <id>12</id>
    <revision>
      <id>1292578716</id>
      <parentid>1291931490</parentid>
      <timestamp>2025-05-27T19:06:38Z</timestamp>
      <contributor>
        <username>Wabbuh</username>
        <id>48227547</id>
      </contributor>
      <origin>1292578716</origin>
      <model>wikitext</model>
      <format>text/x-wiki</format>
      <text bytes="110428" sha1="ljhya9ijz3ikzyfdws1ujd1tnq6oxw0" xml:space="preserve">{{Short description|Political philosophy and movement}}
{{Other uses|Anarchy|Anarchism (disambiguation)|Anarchist (disambiguation)}}
{{Good article}}
{{Pp-semi-indef}}
{{Use British English|date=August 2021}}
{{Use dmy dates|date=October 2024}}
{{Use shortened footnotes|date=May 2023}}
{{Anarchism sidebar}}

'''Anarchism''' is a [[political philosophy]] and [[Political movement|movement]] that seeks to abolish all institutions that perpetuate [[authority]]
== External links ==
&lt;!-- Attention! The external link portion of this article regularly grows far beyond manageable size. PLEASE only list an outside link if it applies to anarchism in general, is somewhat noteworthy, and has consensus on the talkpage. Links to sites which cover anarchist submovements will be routinely moved to subarticles to keep this article free of clutter. --&gt;
{{Prone to spam|date=November 2014}}
* [http://dwardmac.pitzer.edu Anarchy Archives] - an online research center on the history and theory of anarchism.

{{Anarchism}}
{{Navboxes
|list=
{{Libertarian socialism}}
{{Libertarianism}}
{{Philosophy topics}}
{{Political culture}}
{{Political ideologies}}
{{Political philosophy}}
}}
{{Sister bar|wikt=yes|commons=yes|q=yes|s=yes|b=yes|d=yes|v=yes|n=yes}}
{{Authority control}}

[[Category:Anarchism| ]]
[[Category:Anti-capitalism]]
[[Category:Anti-fascism]]
[[Category:Economic ideologies]]
[[Category:Far-left politics]]
[[Category:Left-wing ideologies]]
[[Category:Libertarian socialism]]
[[Category:Libertarianism]]
[[Category:Political culture]]
[[Category:Political ideologies]]
[[Category:Political movements]]
[[Category:Social theories]]
[[Category:Types of socialism]]</text>
      <sha1>ljhya9ijz3ikzyfdws1ujd1tnq6oxw0</sha1>
    </revision>
  </page>
</mediawiki>)";
TEST(WikiIterTest, EmptyContent) {
  std::istringstream input("");
  WikiIter iter(input);
  ASSERT_EQ(iter.next(), std::nullopt);
}

TEST(WikiIterTest, TwoArticles) {
  std::string wikipedia_xml(std::begin(kWikipediaXml1),
                            std::end(kWikipediaXml1));
  std::string page_end("</page>");
  std::istringstream input(wikipedia_xml);
  WikiIter iter(input);
  long int first_page_index =
      static_cast<long int>(wikipedia_xml.find("<page>"));
  long int first_page_length =
      static_cast<long int>(wikipedia_xml.find(page_end)) - first_page_index +
      static_cast<long int>(page_end.size());

  EXPECT_THAT(iter.next(),
              Optional(Eq(WikiArticle("AccessibleComputing",
                                      /*position=*/first_page_index,
                                      /*length=*/first_page_length))));

  long int second_page_index =
      static_cast<long int>(wikipedia_xml.rfind("<page>"));
  long int second_page_length =
      static_cast<long int>(wikipedia_xml.rfind(page_end)) - second_page_index +
      static_cast<long int>(page_end.size());
  EXPECT_THAT(
      iter.next(),
      Optional(Eq(WikiArticle("Anarchism", /*position=*/second_page_index,
                              /*length=*/second_page_length))));
  EXPECT_THAT(iter.next(), Eq(std::nullopt));
}

} // namespace
} // namespace revfad_wiki