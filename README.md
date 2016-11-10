# x-lang

The X language aims to be a system level language that incorporates functional paradigms - it may even become functional.


## Slated Features

* LLVM-C backend for code generation (Support all the things!)
* Good code practices enforced at lexical analysis (no _ or __ for variable identifiers, requiring a leading zero for floating.. etc)
* Not sure If I will take the scala approach and incorporate functional paradigms, or make it fully functional at this point.
* Inline LLVM-IR, no more remembering several assembly languages, learn an intermediate language instead.
* Libraries
  * Unsafe library for embedded developers and those who still like to roll their own - there will be no standard library, just supplied libraries that you may use. It is unjust to make libraries that users optimize every day. Do what I do, use the best ones, you shouldn't be limited by architecture based on the way this language is designed.

## Current State

X language is currently able to generate LLVM IR, compile to LLVM bitcode, dump native assembly and compile a native binary. Top-level statements are supported, the main module and function are implicitly generated. No other syntax but variable declarations are supported at this time - it is being added now that the backend is functional.

Proof

```
$ # Dumping bitcode, assembly, LLVM IR and the AST.
$ ./bin/x-lang -b bin/worked.bc --asm bin/worked.S --ir --ast \
                -o bin/variable_declaration res/variable_declaration.x
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

===============================

; ModuleID = '__x_lang'
source_filename = "__x_lang"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

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

$ cat bin/worked.S
	.text
	.file	"/tmp/.bc-tmp"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc


	.section	".note.GNU-stack","",@progbits

$ hexdump bin/worked.bc
0000000 4342 dec0 1435 0000 0005 0000 0c62 2430
0000010 5949 e6be f7ed 2d3e 0144 0532 0000 0000
0000020 0c21 0000 00cc 0000 820b 0020 0002 0000
0000030 0013 0000 8107 9123 c841 4904 1006 3932
0000040 0192 0c84 0525 1908 041e 628b 1080 0245
0000050 9242 420b 1084 1432 0838 4b18 320a 8842
0000060 9048 2014 4643 a588 1900 4232 48e4 900e
0000070 2211 50c4 5141 8c81 83e1 8ae5 2104 0646
0000080 2089 0000 000e 0000 2232 0908 6420 0485
0000090 2213 84a4 1304 e322 a184 1490 4c12 8c88
00000a0 840b 4c84 3010 0473 c108 011c 3244 0047
00000b0 4406 0473 40a0 0236 0250 1ca4 0008 0000
00000c0 ca1b 1ca0 a0da f41d 1ca0 20da ec1d 0e80
00000d0 c0f4 e80e 0da0 01cc e00f 0f40 40e2 f00e
00000e0 0da0 01dc f40f 0e20 40ec e60f 0e40 c0f4
00000f0 e80e 0da0 2166 e40e 0f00 1e74 20c8 4643
0000100 804a 8400 0000 004a 2900 8000 0016 0840
0000110 a000 0004 0290 6800 0001 9e43 0004 0000
0000120 0000 0000 0000 3400 1842 f916 ebff 805f
0000130 90c6 20d8 3850 0018 2000 0316 005c 0000
0000140 0833 1c80 e1c4 661c 0114 883d 3843 c384
0000150 428c 0780 7879 7307 7198 e60c 0f00 10ed
0000160 f40e 0e80 0c33 1e42 c1c2 ce1d 1ca1 3066
0000170 3d05 4388 8438 1b83 03cc c83d 3d43 038c
0000180 cc3d 8c78 7074 7b07 0708 4879 7087 0770
0000190 707a 7603 8778 2070 1987 11cc ec0e 0e90
00001a0 30e1 6e0f 0f30 f0e3 f00e 0e50 1033 1dc4
00001b0 21de d81c 1d21 61c2 661e 8930 bc3b 3b83
00001c0 43d0 b439 3c03 83bc 843c 3b03 f0cc 7614
00001d0 0760 687b 3707 8768 6872 3707 8780 9070
00001e0 7087 0760 2876 7607 05f8 7876 7787 8780
00001f0 085f 7187 8718 9872 7987 8198 ee2c 0ef0
0000200 e0ee f50e 0ec0 30ec 6203 a1c8 e41c 1ca1
0000210 a1cc e41c 1ca1 61dc ca1c 1c21 81c4 ca1d
0000220 0661 90d6 3943 43c8 9839 3943 43c8 b839
0000230 38c3 4394 8838 3b03 c394 bc2f 3c83 82fc
0000240 d43b 3b03 c3b0 c70c 8769 5870 7287 8370
0000250 6874 7807 8760 1874 7487 87a0 ce19 0f53
0000260 00ee f20f 0e50 90e4 e30e 0f40 20e1 ec0e
0000270 0e50 2033 1d28 c1dc c21e 1e41 21d2 dc1c
0000280 1e81 e0dc e41c 1de1 01ea 661e 5118 b038
0000290 3a43 839c cc3b 2450 6076 7b07 0768 6037
00002a0 7787 0778 9878 4c51 90f4 f00f 0e50 0000
00002b0 18a9 0000 000b 0000 0a0b 2872 7787 0780
00002c0 587a 9870 3d43 c3b8 b038 3943 c3d0 e682
00002d0 c61c 0da1 41e8 c21e 1dc1 21e6 e81d 1d21
00002e0 c1de 001d 2061 0000 0006 0000 0413 8601
00002f0 0103 0000 0002 0000 5007 cd10 6114 0000
0000300 0000 0000 2071 0000 0012 0000 0e52 2210
0000310 8264 1ca4 4420 e4c8 3948 8840 a190 058c
0000320 0884 4019 fc01 50c5 c0d8 dd90 22b8 06e4
0000330 0c10 3480 3016 ff5c 076f 3484 8520 1011
0000340 4599 6644 2030 6116 0405 db71 09b8 5810
0000350 df6d 0000 0000 0000                    
0000358

$ hexdump bin/variable_declaration
-- SNIP --
0000000 457f 464c 0102 0001 0000 0000 0000 0000
0000010 0002 003e 0001 0000 03b0 0040 0000 0000
0000020 0040 0000 0000 0000 18e0 0000 0000 0000
0000030 0000 0000 0040 0038 0009 0040 001c 0019
0000040 0006 0000 0005 0000 0040 0000 0000 0000
0000050 0040 0040 0000 0000 0040 0040 0000 0000
0000060 01f8 0000 0000 0000 01f8 0000 0000 0000
0000070 0008 0000 0000 0000 0003 0000 0004 0000
0000080 0238 0000 0000 0000 0238 0040 0000 0000
0000090 0238 0040 0000 0000 001c 0000 0000 0000
00000a0 001c 0000 0000 0000 0001 0000 0000 0000
00000b0 0001 0000 0005 0000 0000 0000 0000 0000
00000c0 0000 0040 0000 0000 0000 0040 0000 0000
00000d0 0634 0000 0000 0000 0634 0000 0000 0000
00000e0 0000 0020 0000 0000 0001 0000 0006 0000
00000f0 0e48 0000 0000 0000 0e48 0060 0000 0000
0000100 0e48 0060 0000 0000 01e0 0000 0000 0000
0000110 01e8 0000 0000 0000 0000 0020 0000 0000
0000120 0002 0000 0006 0000 0e60 0000 0000 0000
0000130 0e60 0060 0000 0000 0e60 0060 0000 0000
0000140 0190 0000 0000 0000 0190 0000 0000 0000
0000150 0008 0000 0000 0000 0004 0000 0004 0000
0000160 0254 0000 0000 0000 0254 0040 0000 0000
0000170 0254 0040 0000 0000 0044 0000 0000 0000
0000180 0044 0000 0000 0000 0004 0000 0000 0000
0000190 e550 6474 0004 0000 0544 0000 0000 0000
00001a0 0544 0040 0000 0000 0544 0040 0000 0000
00001b0 002c 0000 0000 0000 002c 0000 0000 0000
00001c0 0004 0000 0000 0000 e551 6474 0006 0000
00001d0 0000 0000 0000 0000 0000 0000 0000 0000
*
00001f0 0000 0000 0000 0000 0010 0000 0000 0000
0000200 e552 6474 0004 0000 0e48 0000 0000 0000
0000210 0e48 0060 0000 0000 0e48 0060 0000 0000
0000220 01b8 0000 0000 0000 01b8 0000 0000 0000
0000230 0001 0000 0000 0000 6c2f 6269 3436 6c2f
0000240 2d64 696c 756e 2d78 3878 2d36 3436 732e
0000250 2e6f 0032 0004 0000 0010 0000 0001 0000
0000260 4e47 0055 0000 0000 0002 0000 0006 0000
0000270 0020 0000 0004 0000 0014 0000 0003 0000
0000280 4e47 0055 c584 00f6 610e e9f4 6a7e 38a5
0000290 9c18 0a13 4c7c d460 0001 0000 0001 0000
00002a0 0001 0000 0000 0000 0000 0000 0000 0000
00002b0 0000 0000 0000 0000 0000 0000 0000 0000
*
00002d0 000b 0000 0012 0000 0000 0000 0000 0000
00002e0 0000 0000 0000 0000 001d 0000 0020 0000
00002f0 0000 0000 0000 0000 0000 0000 0000 0000
0000300 6c00 6269 2e63 6f73 362e 5f00 6c5f 6269
0000310 5f63 7473 7261 5f74 616d 6e69 5f00 675f
0000320 6f6d 5f6e 7473 7261 5f74 005f 4c47 4249
0000330 5f43 2e32 2e32 0035 0000 0002 0000 0000
0000340 0001 0001 0001 0000 0010 0000 0000 0000
0000350 1a75 0969 0000 0002 002c 0000 0000 0000
-- SNIP --

```

## Requirements

* LLVM-C
* GCC/G++
* Valgrind
* Doxygen


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
