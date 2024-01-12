If you want to do the end to end demo you can use `spin new` and select Rust. It will then give you `spin.toml` file `src/lib.rs` file and other Cargo files. You can change the code, run `spin build` and then `spin up` to run the appliction. 
If you just want to try this demo, just run `spin up`.


For the Dockerfile you can build the image using 

```
docker buildx build --platform wasi/wasm --provenance=false -t saiyam911/wasm-course:v1 .

docker run -i --runtime=io.containerd.spin.v1 --platform=wasi/wasm -p 3000:80 docker.io/saiyam911/wasm-course:v1
``` 

For Kubernetes you can have Kwasm installed on the cluster using 

```
# Add HELM repository if not already done
helm repo add kwasm http://kwasm.sh/kwasm-operator/
# Install KWasm operator
helm install -n kwasm --create-namespace kwasm-operator kwasm/kwasm-operator
# Provision Nodes
kubectl annotate node --all kwasm.sh/kwasm-node=true
```

Create the deployment 

```
kubectl apply -f deploy.yaml
```

port-forward 
```
kubectl port-forward deployment/wasm-course 3000:80
```
Curl to see the output

```
curl 127.0.0.1:3000
```
