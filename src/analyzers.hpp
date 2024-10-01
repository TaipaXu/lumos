#pragma once

#include "./analyzer.hpp"

#define CREATE_ANALYZER(name, ...)               \
    class name##Analyzer : public Analyzer       \
    {                                            \
    public:                                      \
        name##Analyzer() : Analyzer(__VA_ARGS__) \
        {                                        \
        }                                        \
    };

CREATE_ANALYZER(Cpp, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Python, {"#"}, {{"'''", "'''"}, {R"(""")", R"(""")"}})
CREATE_ANALYZER(XML, {}, {{"<!--", "-->"}})
CREATE_ANALYZER(Html, {}, {{"<!--", "-->"}})
CREATE_ANALYZER(Css, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Sass, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Less, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(JavaScript, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(TypeScript, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Json, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Markdown, {}, {{"<!--", "-->"}})
CREATE_ANALYZER(Java, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Kotlin, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Dart, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Yaml, {"#"}, {})
