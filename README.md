# x-lang (UNDER HEAVY REFACTOR, against my better judgement - it made its way into master. Be patient.)

The X language aims to be a system level language that incorporates functional paradigms - it may even become functional.

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
