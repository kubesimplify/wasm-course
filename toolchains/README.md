# Language-specific toolchains for WASI

## Run examples with `wasi-libc`

```bash
CC="${WASI_SDK_PATH}/bin/clang --sysroot=${WASI_SDK_PATH}/share/wasi-sysroot"
${CC} -o copyfiles.wasm copyfiles.c
wasmtime --mapdir=.::. copyfiles.wasm random.txt /tmp/random.txt
```

## Run examples with Rust

```bash
rustup target add wasm32-wasi
cd copyfiles-rs/
rustup target add wasm32-wasi
cargo build --release --target wasm32-wasi
file target/wasm32-wasi/release/copyfiles-rs.wasm 
wasmtime --mapdir=.::. target/wasm32-wasi/release/copyfiles-rs.wasm random.txt /tmp/random.txt
```

## Run Wasm Module in Python

```bash
pip install wasmer wasmer_compiler_cranelift
python run_wasm.py
```