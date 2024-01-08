## Component Model

## Component Model demo in Rust

This example shows how to use the component model in Rust.

```bash
rustup target add wasm32-unknown-unknown
cd components-rs
cargo build -p guest --target wasm32-unknown-unknown
cargo run -p host
```

## Building and serving components with Spin

This example shows how to use the component model in Spin.

```bash
cd compoenents-spin
cargo build --target wasm32-wasi --release --manifest-path=rust-hello/Cargo.toml
RUST_LOG=spin=trace spin up
```