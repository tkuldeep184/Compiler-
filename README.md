# Scriptum — Custom Language Interpreter (C++)

A tree-walking interpreter for a small C-like scripting language, built from scratch in C++17. This is a from-first-principles learning project: no lexer/parser generators, no external dependencies — just hand-written code covering tokenizing, recursive-descent parsing, and AST evaluation.

## What it can run today

```
// Week 4 core feature check
function fib(n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

let i = 0;
while (i < 8) {
    print fib(i);
    i = i + 1;
}

let a = 5;
if (a > 0 && a < 10) {
    print a;
}
```

This exact program is what [src/main.cpp](src/main.cpp) currently lexes, parses, and interprets on every run — there's no file/CLI input yet (see [Roadmap](#roadmap)).

## Language features

| Category      | Supported |
|----------------|-----------|
| Variables      | `let` declarations, assignment |
| Types          | integers only at runtime (float/string tokens are lexed but not yet evaluated) |
| Arithmetic     | `+ - * /`, with divide-by-zero as a runtime error |
| Comparison     | `== != < > <= >=` |
| Logical        | `&&`, `\|\|` (short-circuiting) |
| Control flow   | `if` / `else`, `while` |
| Functions      | declaration, calls, `return`, recursion |
| I/O            | `print` statement |
| Comments       | `// line comments` |
| Errors         | parse errors and runtime errors are distinct, caught separately, and report line numbers |

### Known limitations (by design, for now)

- **No closures.** Function calls run in a fresh environment parented directly to `globals` — they can't capture variables from an enclosing scope.
- **No floats/strings at runtime.** The lexer recognizes `FLOAT` and `STRING` tokens, but the parser/interpreter only construct and evaluate `IntegerLiteral`.
- **Functions are hoisted, not first-class.** A pre-pass registers every `FunctionStatement` by name before execution (so call order and recursion work), but functions aren't values you can pass around.
- **Global scope for functions.** There's one flat function table; no nested/local function declarations.
- **Single hardcoded program.** The interpreter doesn't yet read `.txt`/script files from disk or argv.

## Architecture

```
source string
      │
      ▼
   Lexer            src/lexer/lexer.{h,cpp}, token.h
   (chars → tokens)
      │
      ▼
   Parser           src/parser/parser.{h,cpp}
   (tokens → AST)   src/parser/ast.h   (expression nodes)
                     src/parser/stmt.h  (statement nodes)
      │
      ▼
   Interpreter       src/interpreter/interpreter.{h,cpp}
   (AST → result)    src/interpreter/environment.{h,cpp}
```

- **Lexer** — hand-rolled scanner (`start`/`current`/`line`/`column` bookkeeping) producing a flat `std::vector<Token>`. Two-character operators (`==`, `!=`, `<=`, `>=`, `&&`, `||`) are matched via one-token lookahead (`peek()`). Unrecognized single characters (lone `&`, `|`, `!`) become `UNKNOWN` tokens rather than lexer errors.
- **Parser** — recursive-descent with an explicit precedence chain, loosest-binding first:
  `expression (||) → logicalAnd (&&) → equality (==, !=) → comparison (< > <= >=) → additive (+ -) → term (* /) → primary`.
  `primary` handles integer literals, identifiers, function calls, and parenthesized expressions. Malformed input throws `ParseError`, which carries the offending line number.
- **AST** — two visitor hierarchies rather than virtual `eval()`/`print()` methods on each node: `Visitor` for expressions (`BinaryExpr`, `IntegerLiteral`, `Identifier`, `CallExpr`) and `StmtVisitor` for statements (`LetStatement`, `AssignmentStatement`, `IfStatement`, `WhileStatement`, `BlockStatement`, `PrintStatement`, `FunctionStatement`, `ReturnStatement`). Adding a new operation over the tree (e.g. a type checker or a bytecode emitter) means writing one new visitor, not touching every node class. `PrintVisitor` is the first example of this — it renders an expression tree back out as text.
- **Interpreter** — implements both visitor interfaces directly (`class Interpreter : public Visitor, public StmtVisitor`). Evaluates expressions to `int`, executes statements for effect. `return` is implemented as a `ReturnSignal` value-carrying struct thrown and caught around call evaluation — deliberately not derived from `std::exception`, since it's control flow, not an error condition.
- **Environment** — a singly-linked chain of scopes (`unordered_map<string, int>` plus a `parent` pointer). `get`/`assign` walk up the chain; `define` always creates in the current scope. Each function call gets a new `Environment` parented to `globals` (see closures limitation above).

## Building

Requires CMake 3.15+ and a C++17 compiler (tested with MSVC/MinGW on Windows).

```
cmake -S . -B build
cmake --build build
```

Run it:

```
./build/compiler.exe
```

Expected output is the first 8 Fibonacci numbers (one per line via `print`), followed by `5`.

## Project layout

```
src/
  lexer/
    token.h          TokenType enum + Token struct + tokenTypeToString
    lexer.h/.cpp      Lexer: source string -> vector<Token>
  parser/
    ast.h             Expr node types + Visitor interface
    stmt.h            Stmt node types + StmtVisitor interface
    print_visitor.h   PrintVisitor — renders an expression tree as text
    parser.h/.cpp     Parser: vector<Token> -> AST (Expr / Stmt trees)
  interpreter/
    environment.h/.cpp  Scoped variable storage (parent-chained)
    interpreter.h/.cpp  Tree-walking evaluator (Visitor + StmtVisitor)
  main.cpp            Wires lexer -> parser -> interpreter around a sample program
CMakeLists.txt         Globs src/**/*.cpp, C++17, target "compiler"
```

