fn main() {
    println!("cargo:rustc-link-lib=static=mal");
    println!("cargo:rustc-link-search=native=../");
}
