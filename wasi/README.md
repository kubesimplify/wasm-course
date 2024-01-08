# WASI Examples

## Starter Example

1. Run simple Hello World example

```bash
export WASI_SDK_PATH=/workspaces/WasmBook/wasi-sdk-20.0
CC="${WASI_SDK_PATH}/bin/clang --sysroot=${WASI_SDK_PATH}/share/wasi-sysroot"
$CC helloworld.c -o helloworld-c.wasm
```

2. Run simple Hello World example with Rust

```bash
cd examples/wasi/helloworld-rs/
rustup target add wasm32-wasi
cargo build --release --target wasm32-wasi
```

Run with a Wasm runtime using:

```bash
wasmtime helloworld-*.wasm
```

## A larger example

1. Copy files

```bash
$CC copyfiles.c -o copyfiles-c.wasm
```

2. Copy files in Rust

```bash
cd examples/wasi/copyfiles-rs/
cargo build --release --target wasm32-wasi
```

Run with a Wasm runtime using:

```bash
wasmtime --dir=. copyfiles-rs.wasm random.txt tmp/random.txt
```