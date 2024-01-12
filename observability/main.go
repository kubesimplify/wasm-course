package main

import (
    "context"
    "log"
    "os"
    "time"
    "github.com/dylibso/observe-sdk/go/adapter/otel_stdout"
    "github.com/tetratelabs/wazero"
    "github.com/tetratelabs/wazero/imports/wasi_snapshot_preview1"
)

func main() {
    // create a context
    ctx := context.Background()

    // create a new OpenTelemetry adapter with config

    adapter := otel_stdout.NewOtelStdoutAdapter()

    // start the Observe SDK adapter
    adapter.Start(ctx)

    // load some Wasm from disk
    wasm, err := os.ReadFile(os.Args[1])
    if err != nil {
        log.Panicln(err)
    }

    // setup a new Wazero runtime with config
    wacfg := wazero.NewRuntimeConfig().WithCustomSections(true)
    rt := wazero.NewRuntimeWithConfig(ctx, wacfg)

    // apply the Observe SDK adapter to the Wazero runtime
    traceCtx, err := adapter.NewTraceCtx(ctx, rt, wasm, nil)
    if err != nil {
        log.Panicln(err)
    }

    wasi_snapshot_preview1.MustInstantiate(ctx, rt)

    // create a new Wazero configuration
    modconfig := wazero.NewModuleConfig().
        WithStdin(os.Stdin).
        WithStdout(os.Stdout).
        WithStderr(os.Stderr).
        WithArgs(os.Args[1:]...).
        WithStartFunctions("_start")

    // instantiate the Wasm module
    mod, err := rt.InstantiateWithConfig(ctx, wasm, modconfig)
    if err != nil {
        log.Println("module instance error:", err)
        return
    }
    defer mod.Close(ctx)

    // associate some additional metadata with the trace
    meta := map[string]string{
        "http.url":         "https://example.com/my-endpoint",
        "http.status_code": "200",
        "http.client_ip":   "192.168.1.0",
    }
    traceCtx.Metadata(meta)

    // stop the trace 
    traceCtx.Finish()
    time.Sleep(10*time.Second)
}

