#Go to [Killercoda Ubuntu Playground](https://killercoda.com/playgrounds/scenario/ubuntu)
Make sure to have Go Updated 1.19+, you can use below commands 

```
sudo apt-get remove golang-go
sudo apt-get remove --auto-remove golang-go
sudo rm -rf /usr/local/go
wget https://golang.org/dl/go1.20.linux-amd64.tar.gz
sudo tar -C /usr/local -xzf go1.20.linux-amd64.tar.gz
export PATH=$PATH:/usr/local/go/bin
go version

```

Go inside observability directory 

```
cd observability
echo "hello world" | go run main.go count_vowels.rs.instr.wasm
```

