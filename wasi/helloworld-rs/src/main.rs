use std::time::Duration;
use std::thread::sleep;

fn main() {
    loop {
        println!("Hello, World!");
        sleep(Duration::from_secs(1));
    }
}