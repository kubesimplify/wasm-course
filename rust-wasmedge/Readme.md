This demo is for creating a simple rust application usign `cargo new` and then running it using wasmedge WebAssembly runtime. 

For building we used 

```
cargo build --target wasm32-wasi --release
```
Use wasmedge to run the wasm file created in target

```
wasmedge target/wasm32-wasi/release/demo.wasm
Hello, from Kubesimplify!
```
