# x-lang

The X language aims to be a system level language that incorporates functional paradigms - it may even become functional.

## Current State

X language currently able to generate LLVM IR for its main module and implicit entry and global variable declarations. No other syntax is supported at this time, it is being added.

Generated IR by this compiler and an AST dump of the source.

```
; ModuleID = '__x_lang'
source_filename = "__x_lang"

@a_fdf = external global i8
@x__ = external global i16
@y123 = external global i32
@z34 = external global i64
@zwer = external global i8
@e324 = external global i16
@iwe = external global i32
@sdif = external global i64

define void @main() {
entry:
  ret void
}
===============================================================

<1:1:variable_declaration>
  <1:1:type:u8>
  <1:4:ident:"a_fdf">
<2:1:variable_declaration>
  <2:1:type:u16>
  <2:5:ident:"x__">
<3:1:variable_declaration>
  <3:1:type:u32>
  <3:5:ident:"y123">
<4:1:variable_declaration>
  <4:1:type:u64>
  <4:5:ident:"z34">
<5:1:variable_declaration>
  <5:1:type:s8>
  <5:4:ident:"zwer">
<6:1:variable_declaration>
  <6:1:type:s16>
  <6:5:ident:"e324">
<7:1:variable_declaration>
  <7:1:type:s32>
  <7:5:ident:"iwe">
<8:1:variable_declaration>
  <8:1:type:s64>
  <8:5:ident:"sdif">
```

## Slated Features

* LLVM-C backend for code generation (Support all the things!)
* Good code practices enforced at lexical analysis (no _ or __ for variable identifiers, requiring a leading zero for floating.. etc)
* Not sure If I will take the scala approach and incorporate functional paradigms, or make it fully functional at this point.
* Inline LLVM-IR, no more remembering several assembly languages, learn an intermediate language instead.
* Libraries
  * Unsafe library for embedded developers and those who still like to roll their own - there will be no standard library, just supplied libraries that you may use. It is unjust to make libraries that users optimize every day. Do what I do, use the best ones, you shouldn't be limited by architecture based on the way this language is designed.

## Requirements

* LLVM-C
* Valgrind
* GCC

## Bugs

* I track what I find in issues, report what you find.

## Building

```
$ git clone http://github.com/typ3def/x-lang
$ make
```

## Running

```
$ make run-<*.x> # This goal will run anything in res/ (i.e: make run-ident runs res/ident.x)
```

Tests

```
$ make run-tests
```

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## History

This is a rewrite of my original, unplanned, and very spaghetti first (I use that word very loosely) language attempt, 0xDEADBEEF.

## License

Code and documentation copyright 2016 John Holly. Code released under the [MIT](LICENSE.md) license.
