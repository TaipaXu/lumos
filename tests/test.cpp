#define BOOST_TEST_MODULE LumosTest

#include <filesystem>
#include <vector>
#include <optional>
#include <boost/test/included/unit_test.hpp>
#include "analysis/analyzer.hpp"
#include "models/result.hpp"

struct FileTestData
{
    std::string filePath;
    Model::CodeStats expectedStats;
};

void test(std::vector<FileTestData> items)
{
    for (auto item : items)
    {
        BOOST_CHECK(std::filesystem::exists(item.filePath));
        std::filesystem::path fsPath(item.filePath);
        IAnalyzer *analyzer = IAnalyzer::create(fsPath.filename().string());
        BOOST_CHECK(analyzer != nullptr);
        std::optional<Model::CodeStats> stats = analyzer->start(item.filePath);
        BOOST_REQUIRE(stats.has_value());
        BOOST_CHECK_EQUAL(stats->name, item.expectedStats.name);
        BOOST_CHECK_EQUAL(stats->totalLineCount, item.expectedStats.totalLineCount);
        BOOST_CHECK_EQUAL(stats->codeLineCount, item.expectedStats.codeLineCount);
        BOOST_CHECK_EQUAL(stats->commentLineCount, item.expectedStats.commentLineCount);
        BOOST_CHECK_EQUAL(stats->emptyLineCount, item.expectedStats.emptyLineCount);
    }
}

std::vector<FileTestData> htmlItems = {
    {"files/html/a.html", {.name = "HTML", .totalLineCount = 14, .codeLineCount = 10, .commentLineCount = 0, .emptyLineCount = 4}},
    {"files/html/b.html", {.name = "HTML", .totalLineCount = 14, .codeLineCount = 9, .commentLineCount = 1, .emptyLineCount = 4}},
    {"files/html/c.html", {.name = "HTML", .totalLineCount = 15, .codeLineCount = 11, .commentLineCount = 0, .emptyLineCount = 4}},
    {"files/html/d.html", {.name = "HTML", .totalLineCount = 17, .codeLineCount = 12, .commentLineCount = 0, .emptyLineCount = 5}},
    {"files/html/e.html", {.name = "HTML", .totalLineCount = 19, .codeLineCount = 15, .commentLineCount = 0, .emptyLineCount = 4}},
    {"files/html/f.html", {.name = "HTML", .totalLineCount = 20, .codeLineCount = 15, .commentLineCount = 1, .emptyLineCount = 4}},
    {"files/html/g.html", {.name = "HTML", .totalLineCount = 20, .codeLineCount = 16, .commentLineCount = 1, .emptyLineCount = 3}},
    {"files/html/h.html", {.name = "HTML", .totalLineCount = 22, .codeLineCount = 17, .commentLineCount = 1, .emptyLineCount = 4}},
    {"files/html/i.html", {.name = "HTML", .totalLineCount = 22, .codeLineCount = 18, .commentLineCount = 1, .emptyLineCount = 3}},
    {"files/html/j.html", {.name = "HTML", .totalLineCount = 23, .codeLineCount = 18, .commentLineCount = 2, .emptyLineCount = 3}},
};
BOOST_AUTO_TEST_CASE(HTML)
{
    test(htmlItems);
}

std::vector<FileTestData> vueItems = {
    {"files/vue/a.vue", {.name = "Vue", .totalLineCount = 11, .codeLineCount = 6, .commentLineCount = 0, .emptyLineCount = 5}},
    {"files/vue/b.vue", {.name = "Vue", .totalLineCount = 7, .codeLineCount = 4, .commentLineCount = 0, .emptyLineCount = 3}},
    {"files/vue/c.vue", {.name = "Vue", .totalLineCount = 15, .codeLineCount = 13, .commentLineCount = 0, .emptyLineCount = 2}},
    {"files/vue/d.vue", {.name = "Vue", .totalLineCount = 15, .codeLineCount = 10, .commentLineCount = 3, .emptyLineCount = 2}},
};
BOOST_AUTO_TEST_CASE(Vue)
{
    test(vueItems);
}
