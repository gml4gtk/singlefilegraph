# wasmtest01

test compiling c to wasm emscripten using clang-8 and ./0m shell script

graph2svg.wasm creates svg image data inlined in the html page when pressing button.

The javascript checks for too much text in the textarea.

because graph2svg.wasm uses wasm memory add linker flag -Wl,--import-memory to clang

It needs lld package installed and python webserver but not thttpd

python -m SimpleHTTPDServer

Then browse to localhost:8000

Using firefox developers console to see output from console.log()

This is a test version with bugs to be fixed.

end.
