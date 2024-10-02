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

CREATE_ANALYZER(Ada, {"--"})
CREATE_ANALYZER(Assembly, {";"})

CREATE_ANALYZER(Bash, {"#"})   // TODO Multi-line comment
CREATE_ANALYZER(Batch, {"::"}) // TODO Single-line comment REM

CREATE_ANALYZER(Cpp, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(CSharp, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Css, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(COBOL, {"*"})

CREATE_ANALYZER(D, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Dart, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Erlang, {"%"})

CREATE_ANALYZER(FSharp, {"//"}, {{"(*", "*)"}})
CREATE_ANALYZER(Fortran, {"!"})

CREATE_ANALYZER(Go, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Haskell, {"--"}, {{"{-", "-}"}})
CREATE_ANALYZER(Html, {}, {{"<!--", "-->"}})

CREATE_ANALYZER(Java, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(JavaScript, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Json, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Kotlin, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Less, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Lisp, {";"}, {{"#|", "|#"}})
CREATE_ANALYZER(Lua, {"--"}, {{"--[[", "]]"}})

CREATE_ANALYZER(Markdown, {}, {{"<!--", "-->"}})
// CREATE_ANALYZER(MATLAB, {"%"}, {{"%{", "%}"}})

CREATE_ANALYZER(Nim, {"#"}, {{"#[", "]#"}})

CREATE_ANALYZER(ObjectiveC, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Pascal, {"//"}, {{"{", "}"}, {"(*", "*)"}})
CREATE_ANALYZER(Perl, {"#"}, {{"=pod", "=cut"}})
CREATE_ANALYZER(PHP, {"//"}, {{"/*", "*/"}}) // TODO Single-line comment #
CREATE_ANALYZER(Python, {"#"}, {{"'''", "'''"}, {R"(""")", R"(""")"}})

CREATE_ANALYZER(QML, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(R, {"#"})
CREATE_ANALYZER(Ruby, {"#"}, {{"=begin", "=end"}})
CREATE_ANALYZER(Rust, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Sass, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(Scala, {"//"}, {{"/*", "*/"}})
CREATE_ANALYZER(SQL, {"--"}, {{"/*", "*/"}})
CREATE_ANALYZER(Swift, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Tcl, {"#"})
CREATE_ANALYZER(TypeScript, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(VHDL, {"--"}, {{"/*", "*/"}})
CREATE_ANALYZER(VisualBasic, {"'"}) // TODO Single-line comment REM
CREATE_ANALYZER(Vue, {"//"}, {{"/*", "*/"}})

CREATE_ANALYZER(Wolfram, {""}, {{"(*", "*)"}}) // TODO only multi-line comment

CREATE_ANALYZER(XML, {}, {{"<!--", "-->"}})

CREATE_ANALYZER(Yaml, {"#"}, {})

CREATE_ANALYZER(Zig, {"//"}, {{"/*", "*/"}})
