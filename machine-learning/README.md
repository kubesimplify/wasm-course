# Machine Learning Demos

## Run Models with WasmEdge TensorFlow

Build the QuickJS interpreter with the WasmEdge TensorFlow extension

```sh
git clone https://github.com/second-state/wasmedge-quickjs.git
cd wasmedge-quickjs
cargo build --target wasm32-wasi --release --features=tensorflow
cd ..
```

Run JS Examples locally

```sh
wget -qO- https://raw.githubusercontent.com/WasmEdge/WasmEdge/master/utils/install.sh | bash -s -- -e all -v 0.12.1
source /home/codespace/.bashrc
cd js_mobilenet
wasmedge-tensorflow-lite --dir .:. wasmedge_quickjs.wasm main.js
cd ..
```

Run Rust examples locally

```sh
cd rust_mobilenet
cargo build --target wasm32-wasi --release
```

We can AOT compile our Rust code to machine native code, and then use WasmEdge sandbox to run the native code.

```sh
wasmedgec-tensorflow target/wasm32-wasi/release/classify.wasm classify.so
wasmedge-tensorflow-lite classify.so < grace_hopper.jpg
```

## Deploy as a FAAS

Build the Rust program to WebAssembly bytecode

```sh
cd faas
cd api/functions/image-classification/
cargo build --release --target wasm32-wasi
cp target/wasm32-wasi/release/classify.wasm ../../
```

Deploy the function

```sh
cd ../../../
vercel deploy
```

## Deploy as a container

Build container

```sh
sudo buildah build --annotation "module.wasm.image/variant=compat" -t classify .
```

## Run PyTorch Models with WASI-NN

(Taken directly from https://github.com/second-state/WasmEdge-WASINN-examples.)

```sh
curl -sSf https://raw.githubusercontent.com/WasmEdge/WasmEdge/master/utils/install.sh | bash -s -- --plugins wasi_nn-pytorch
```

Compile the application to WebAssembly:

```bash
cargo build --target=wasm32-wasi --release
wasmedgec rust/target/wasm32-wasi/release/wasmedge-wasinn-example-mobilenet-image-named-model.wasm wasmedge-wasinn-example-mobilenet-image-named-model-aot.wasm
```

First generate the JIT pre-trained mobilenet with the script:

```bash
pip3 install torch==1.11.0 torchvision pillow --extra-index-url https://download.pytorch.org/whl/lts/1.8/cpu
# generate the model fixture
python3 gen_mobilenet_model.py
```

The testing image `input.jpg` is downloaded from <https://github.com/bytecodealliance/wasi-nn/raw/main/rust/examples/images/1.jpg> with license Apache-2.0

To load the image, resize it to the expected image size and normalize the pixels according to the ImageNet dataset and save the expanded dimensions tensor, you can run:

```bash
python3 gen_tensor.py input.jpg image-1x3x224x224.rgb
```

### Execute

Execute the WASM with the `wasmedge` with PyTorch:

```bash
wasmedge --dir .:. --nn-preload demo:PyTorch:CPU:mobilenet.pt wasmedge-wasinn-example-mobilenet-image-named-model.wasm demo input.jpg
```
