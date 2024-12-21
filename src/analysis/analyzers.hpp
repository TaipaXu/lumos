#pragma once

#include "./analyzer.hpp"
#include "./multiLanguageAnalyzer.hpp"

#define CREATE_ANALYZER(name, ...) \
    Analyzer *name##Analyzer = new Analyzer(#name, __VA_ARGS__);

#define CREATE_ANALYZER_WITH_DIFFERENT_NAME(className, languageName, ...) \
    Analyzer *className##Analyzer = new Analyzer(languageName, __VA_ARGS__);

#define CREATE_MULTI_LANGUAGE_ANALYZER(name, ...) \
    MultiLanguageAnalyzer *name##Analyzer = new MultiLanguageAnalyzer(#name, __VA_ARGS__);

CREATE_ANALYZER(Ada, {"--"})
CREATE_ANALYZER(Assembly, {";"})

CREATE_ANALYZER(Bash, {"#"}, {{": << 'EOF'", "EOF"}, {": << EOF", "EOF"}, {": '", "'"}, {": \"", "\""}, {"<<COMMENT", "COMMENT"}}) // TODO Multi-line comment
CREATE_ANALYZER(Batch, std::vector<std::string>{"::"})

CREATE_ANALYZER_WITH_DIFFERENT_NAME(Cpp, "C++", {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(CMake, {"#"})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(CSharp, "C#", {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(Css, "CSS", {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(Cobol, "COBOL", {"*"})

CREATE_ANALYZER(D, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Dart, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Erlang, {"%"})

CREATE_ANALYZER_WITH_DIFFERENT_NAME(FSharp, "F#", {"//"}, {{"(*", "*)"}})
CREATE_ANALYZER(Fortran, {"!"})

CREATE_ANALYZER(Go, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Haskell, {"--"}, {{"{-", "-}"}})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(HtmlTemplate, "HTML", std::vector<Analyzer::MultiLineComment>{{"<!--", "-->"}})

CREATE_ANALYZER(Java, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(JavaScript, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(Json, "JSON", {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Kotlin, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Less, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Lisp, {";"}, {{"#|", "|#"}})
CREATE_ANALYZER(Lua, {"--"}, {{"--[[", "]]"}})

CREATE_ANALYZER(Markdown, std::vector<Analyzer::MultiLineComment>{{"<!--", "-->"}})
// CREATE_ANALYZER(MATLAB, {"%"}, {{"%{", "%}"}})

CREATE_ANALYZER(Nim, {"#"}, {{"#[", "]#"}})

CREATE_ANALYZER_WITH_DIFFERENT_NAME(ObjectiveC, "Objective-C", {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Pascal, {"//"}, {{"{", "}"}, {"(*", "*)"}})
CREATE_ANALYZER(Perl, {"#"}, {{"=pod", "=cut"}})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(Php, "PHP", {"//", "#"}, {{"/*", "*/"}})
CREATE_ANALYZER(Python, {"#"}, {{"'''", "'''"}, {R"(""")", R"(""")"}})

CREATE_ANALYZER_WITH_DIFFERENT_NAME(Qml, "QML", {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(R, {"#"})
CREATE_ANALYZER(Ruby, {"#"}, {{"=begin", "=end"}})
CREATE_ANALYZER(Rust, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Sass, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Scala, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(Sql, "SQL", {"--"}, {{"/*", "*/"}})
CREATE_ANALYZER(Swift, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Tcl, {"#"})
CREATE_ANALYZER(TypeScript, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER_WITH_DIFFERENT_NAME(Vhdl, "VHDL", {"--"}, {{"/*", "*/"}})
CREATE_ANALYZER_WITH_DIFFERENT_NAME(VisualBasic, "Visual Basic", std::vector<std::string>{"'", "REM"})

CREATE_ANALYZER(Wolfram, std::vector<Analyzer::MultiLineComment>{{"(*", "*)"}})

CREATE_ANALYZER_WITH_DIFFERENT_NAME(Xml, "XML", std::vector<Analyzer::MultiLineComment>{{"<!--", "-->"}})

CREATE_ANALYZER_WITH_DIFFERENT_NAME(Yaml, "YAML", {"#"})

CREATE_ANALYZER(Zig, {"//"}, {{"/*", "*/"}})

CREATE_MULTI_LANGUAGE_ANALYZER(Html, std::vector<MultiLanguageAnalyzer::LanguageBlock>{
                                         {"<script", "</script>", JavaScriptAnalyzer},
                                         {"<style", "</style>", CssAnalyzer},
                                     },
                               HtmlTemplateAnalyzer)

CREATE_MULTI_LANGUAGE_ANALYZER(Vue, std::vector<MultiLanguageAnalyzer::LanguageBlock>{
                                        {"<template>", "</template>", HtmlAnalyzer},
                                        {"<script", "</script>", JavaScriptAnalyzer},
                                        {"<style", "</style>", CssAnalyzer},
                                    })
