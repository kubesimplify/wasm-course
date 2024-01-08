from wasmer import engine, wasi, Store, Module, ImportObject, Instance
from wasmer_compiler_cranelift import Compiler
import os

# Let's get the `wasi.wasm` bytes!
wasm_bytes = open('copyfiles.wasm', 'rb').read()

# Create a store.
store = Store(engine.Universal(Compiler))

# Let's compile the Wasm module, as usual.
module = Module(store, wasm_bytes)

wasi_version = wasi.get_version(module, strict=True)

wasi_env = \
    wasi.StateBuilder('wasi_test_program'). \
        argument('random.txt'). \
        argument('random2.txt'). \
        environment('COLOR', 'true'). \
        environment('APP_SHOULD_LOG', 'false'). \
        map_directory('.', '.'). \
        finalize()
import_object = wasi_env.generate_import_object(store, wasi_version)
instance = Instance(module, import_object)
instance.exports._start()