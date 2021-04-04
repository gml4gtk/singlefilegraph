#!/bin/sh -x
echo "tested with clang-8"
alias clang=clang-8
alias wasm-ld=/usr/lib/llvm-8/bin/wasm-ld
export PATH=/usr/lib/llvm-8/bin/:$PATH
clang --version
rm -v -f ./graph2svg.wasm
echo "adding memory support with -Wl,--import-memory for graph2svg.c"
clang  -I. --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--import-memory -Wl,--no-entry -o graph2svg.wasm graph2svg.c clib.c sfg.c dlmalloc.c
# -v
exit 0
clang -c -v -I. -D__wasi__ --target=wasm32-unknown-wasi --sysroot /home/gold/notabug/sfgraph/wasm/test02/wasi-sysroot -nostartfiles  -Wl,--export-all -Wl,--import-memory -Wl,--no-entry  graph2svg.c clib.c sfg.c 
"/usr/bin/lld" -flavor wasm -L/home/gold/notabug/sfgraph/wasm/test02/wasi-sysroot/lib/wasm32-wasi/ --export-all --import-memory --no-entry *.o libc.a /usr/lib/llvm-7/lib/clang/7.0.1/lib/libclang_rt.builtins-wasm32.a -o graph2svg.wasm


on debian with clang version 7, lld-7 as separate package installed
missing link to /usr/bin/ldd added to /usr/bin/ldd-7

The options passed to clang are:

    --target=wasm32 Set the build target to wasm.
    --no-standard-libraries Do not use libc (more on this later).
    -Wl,--export-all Export all symbols (i.e., add) from the compiled wasm file so that they can be used from JavaScript.
    -Wl,--no-entry Don't check for a main function.
    -o Set the output destination.
    -Wl,--import-memory for memory usage

Options preceded by -Wl, are passed from the LLVM compiler to the linker.

The webserver to use is:
pythom -m SimpleHTTPServer

Then the page is on localhost at port 8000

todo: compiling with this sdk appears to be troublesome:
https://github.com/WebAssembly/wasi-sdk/releases
