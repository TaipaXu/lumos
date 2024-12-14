#include "./analyzer.hpp"
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "./analyzers.hpp"

#define CREATE_ANALYZER_CHECK_BY_NAME(name, analyzer) \
    if (fileName == #name)                            \
    {                                                 \
        return analyzer##Analyzer;                    \
    }

#define CREATE_ANALYZER_CHECK(extension, analyzer) \
    if (fileName.ends_with("." #extension))        \
    {                                              \
        return analyzer##Analyzer;                 \
    }

Analyzer::Analyzer(const std::string &name, const std::vector<std::string> &singleLineCommentSymbols, const std::vector<MultiLineComment> &multiLineCommentSymbols)
    : name{name},
      singleLineCommentSymbols{singleLineCommentSymbols},
      multiLineCommentSymbols{multiLineCommentSymbols},
      hasSingleLineCommentSymbols{!singleLineCommentSymbols.empty()},
      hasMultiLineCommentSymbols{!multiLineCommentSymbols.empty()}
{
}

Analyzer::Analyzer(const std::string &name, const std::vector<std::string> &singleLineCommentSymbols)
    : name{name},
      singleLineCommentSymbols{singleLineCommentSymbols},
      hasSingleLineCommentSymbols{!singleLineCommentSymbols.empty()},
      hasMultiLineCommentSymbols{false}
{
}

Analyzer::Analyzer(const std::string &name, const std::vector<MultiLineComment> &multiLineCommentSymbols)
    : name{name},
      multiLineCommentSymbols{multiLineCommentSymbols},
      hasSingleLineCommentSymbols{false},
      hasMultiLineCommentSymbols{!multiLineCommentSymbols.empty()}
{
}

Analyzer *Analyzer::create(const std::string &fileName)
{
    // Ada
    CREATE_ANALYZER_CHECK(adb, Ada)
    CREATE_ANALYZER_CHECK(ads, Ada)

    // Bash
    CREATE_ANALYZER_CHECK(sh, Bash)

    // Assembly
    CREATE_ANALYZER_CHECK(asm, Assembly)
    CREATE_ANALYZER_CHECK(s, Assembly)

    // Batch
    CREATE_ANALYZER_CHECK(bat, Batch)
    CREATE_ANALYZER_CHECK(cmd, Batch)

    // C / C++
    CREATE_ANALYZER_CHECK(h, Cpp)
    CREATE_ANALYZER_CHECK(hpp, Cpp)
    CREATE_ANALYZER_CHECK(hxx, Cpp)
    CREATE_ANALYZER_CHECK(hh, Cpp)
    CREATE_ANALYZER_CHECK(cpp, Cpp)
    CREATE_ANALYZER_CHECK(cc, Cpp)
    CREATE_ANALYZER_CHECK(cxx, Cpp)
    CREATE_ANALYZER_CHECK(c, Cpp)
    CREATE_ANALYZER_CHECK(C, Cpp)
    CREATE_ANALYZER_CHECK(c++, Cpp)
    CREATE_ANALYZER_CHECK(inl, Cpp)
    CREATE_ANALYZER_CHECK(ipp, Cpp)
    CREATE_ANALYZER_CHECK(tpp, Cpp)
    CREATE_ANALYZER_CHECK(txx, Cpp)
    CREATE_ANALYZER_CHECK(cppm, Cpp)

    // CMake
    CREATE_ANALYZER_CHECK_BY_NAME(CMakeLists.txt, CMake)
    CREATE_ANALYZER_CHECK(cmake, CMake)

    // C#
    CREATE_ANALYZER_CHECK(cs, CSharp)

    // CSS
    CREATE_ANALYZER_CHECK(css, Css)

    // COBOL
    CREATE_ANALYZER_CHECK(cbl, Cobol)
    CREATE_ANALYZER_CHECK(cob, Cobol)

    // D
    CREATE_ANALYZER_CHECK(d, D)

    // Dart
    CREATE_ANALYZER_CHECK(dart, Dart)

    // Erlang
    CREATE_ANALYZER_CHECK(erl, Erlang)

    // F#
    CREATE_ANALYZER_CHECK(fs, FSharp)
    CREATE_ANALYZER_CHECK(fsi, FSharp)
    CREATE_ANALYZER_CHECK(fsx, FSharp)

    // Fortran
    CREATE_ANALYZER_CHECK(f, Fortran)
    CREATE_ANALYZER_CHECK(for, Fortran)
    CREATE_ANALYZER_CHECK(f90, Fortran)

    // Go
    CREATE_ANALYZER_CHECK(go, Go)

    // Haskell
    CREATE_ANALYZER_CHECK(hs, Haskell)

    // HTML
    CREATE_ANALYZER_CHECK(html, Html)
    CREATE_ANALYZER_CHECK(htm, Html)

    // Java
    CREATE_ANALYZER_CHECK(java, Java)

    // JavaScript
    CREATE_ANALYZER_CHECK(js, JavaScript)

    // JSON
    CREATE_ANALYZER_CHECK(json, Json)

    // Kotlin
    CREATE_ANALYZER_CHECK(kt, Kotlin)
    CREATE_ANALYZER_CHECK(kts, Kotlin)

    // Less
    CREATE_ANALYZER_CHECK(less, Less)

    // Lisp
    CREATE_ANALYZER_CHECK(lisp, Lisp)
    CREATE_ANALYZER_CHECK(lsp, Lisp)
    CREATE_ANALYZER_CHECK(cl, Lisp)

    // Lua
    CREATE_ANALYZER_CHECK(lua, Lua)

    // Markdown
    CREATE_ANALYZER_CHECK(md, Markdown)

    // MATLAB
    // CREATE_ANALYZER_CHECK(m, MATLAB)

    // Nim
    CREATE_ANALYZER_CHECK(nim, Nim)

    // Objective-C
    CREATE_ANALYZER_CHECK(m, ObjectiveC)

    // Pascal
    CREATE_ANALYZER_CHECK(pas, Pascal)

    // Perl
    CREATE_ANALYZER_CHECK(pl, Perl)

    // PHP
    CREATE_ANALYZER_CHECK(php, Php)

    // Python
    CREATE_ANALYZER_CHECK(py, Python)

    // QML
    CREATE_ANALYZER_CHECK(qml, Qml)

    // R
    CREATE_ANALYZER_CHECK(r, R)

    // Ruby
    CREATE_ANALYZER_CHECK(rb, Ruby)

    // Rust
    CREATE_ANALYZER_CHECK(rs, Rust)

    // Sass
    CREATE_ANALYZER_CHECK(sass, Sass)
    CREATE_ANALYZER_CHECK(scss, Sass)

    // Scala
    CREATE_ANALYZER_CHECK(scala, Scala)

    // SQL
    CREATE_ANALYZER_CHECK(sql, Sql)

    // Swift
    CREATE_ANALYZER_CHECK(swift, Swift)

    // Tcl
    CREATE_ANALYZER_CHECK(tcl, Tcl)

    // TypeScript
    CREATE_ANALYZER_CHECK(ts, TypeScript)

    // VHDL
    CREATE_ANALYZER_CHECK(vhd, Vhdl)
    CREATE_ANALYZER_CHECK(vhdl, Vhdl)

    // Visual Basic
    CREATE_ANALYZER_CHECK(vb, VisualBasic)

    // Vue
    CREATE_ANALYZER_CHECK(vue, Vue)

    // Wolfram
    CREATE_ANALYZER_CHECK(wl, Wolfram)

    // XML
    CREATE_ANALYZER_CHECK(xml, Xml)

    // Yaml
    CREATE_ANALYZER_CHECK(yaml, Yaml)
    CREATE_ANALYZER_CHECK(yml, Yaml)

    // Zig
    CREATE_ANALYZER_CHECK(zig, Zig)

    return nullptr;
}

Model::CodeStats Analyzer::start(std::string &path) const
{
    std::ifstream file(path, std::ios::in);
    Model::CodeStats stats;
    stats.name = name;
    std::string line;
    bool inBlockComment = false;
    std::string lastMultiLineCommentStart;

    while (std::getline(file, line))
    {
        stats.totalLineCount++;
        std::string trimmed = boost::algorithm::trim_copy(line);

        if (trimmed.empty())
        {
            stats.emptyLineCount++;
        }
        else
        {
            if (hasMultiLineCommentSymbols)
            {
                if (inBlockComment)
                {
                    stats.commentLineCount++;

                    if (isMultiLineCommentEnd(trimmed, lastMultiLineCommentStart))
                    {
                        inBlockComment = false;
                        lastMultiLineCommentStart.clear();
                    }
                }
                else
                {
                    if (isMultiLineCommentInOneLine(trimmed))
                    {
                        stats.commentLineCount++;
                    }
                    else
                    {
                        if (isMultiLineCommentStart(trimmed, lastMultiLineCommentStart))
                        {
                            stats.commentLineCount++;
                            inBlockComment = true;
                        }
                        else
                        {
                            if (hasSingleLineCommentSymbols && isSingleLineComment(trimmed))
                            {
                                stats.commentLineCount++;
                            }
                            else
                            {
                                stats.codeLineCount++;
                            }
                        }
                    }
                }
            }
            else
            {
                if (hasSingleLineCommentSymbols && isSingleLineComment(trimmed))
                {
                    stats.commentLineCount++;
                }
                else
                {
                    stats.codeLineCount++;
                }
            }
        }
    }

    return stats;
}

bool Analyzer::isSingleLineComment(const std::string &line) const
{
    for (const auto &symbol : singleLineCommentSymbols)
    {
        if (line.starts_with(symbol))
        {
            return true;
        }
    }
    return false;
}

bool Analyzer::isMultiLineCommentStart(const std::string &line, std::string &multilineCommentStart) const
{
    for (const auto &symbol : multiLineCommentSymbols)
    {
        if (line.starts_with(symbol.start))
        {
            multilineCommentStart = symbol.start;
            return true;
        }
    }
    return false;
}

bool Analyzer::isMultiLineCommentEnd(const std::string &line, const std::string &multilineCommentStart) const
{
    auto it = std::find_if(multiLineCommentSymbols.begin(), multiLineCommentSymbols.end(), [&](const MultiLineComment &symbol) { return symbol.start == multilineCommentStart; });
    if (it != multiLineCommentSymbols.end())
    {
        return line.ends_with(it->end);
    }
    return false;
}

bool Analyzer::isMultiLineCommentInOneLine(const std::string &line) const
{
    for (const MultiLineComment &symbol : multiLineCommentSymbols)
    {
        size_t startPos = line.find(symbol.start);
        size_t endPos = line.find(symbol.end, startPos + symbol.start.length());
        if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos)
        {
            return true;
        }
    }
    return false;
}
