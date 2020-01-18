extern crate mal;

fn read<T: std::str::FromStr>() -> T {
    let mut s = String::new();
    std::io::stdin().read_line(&mut s).ok();
    s.trim().parse().ok().unwrap()
}

fn main() {
    let x: u64 = read();
    let x = Box::new(x);

    let v = vec![0; 0x1000000];
    println!("{}", x);
}
