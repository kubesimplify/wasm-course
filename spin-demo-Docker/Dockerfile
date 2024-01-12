FROM scratch
COPY spin.toml /spin.toml
COPY target/wasm32-wasi/release/wasm_course.wasm /target/wasm32-wasi/release/wasm_course.wasm
ENTRYPOINT ["/spin.toml"]
