#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]
#![reexport_test_harness_main = "test_main"]

pub mod stdio;
pub mod testing;

#[cfg(not(test))]
use core::panic::PanicInfo;
#[cfg(not(test))]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    use crate::println;
    println!("{}", info);
    loop {}
}

#[unsafe(no_mangle)]
pub extern "C" fn _start() -> ! {

    #[cfg(test)]
    test_main();

    println!("Hello, world!");

    loop {}
}

#[cfg(test)]
pub fn test_runner(tests: &[&dyn testing::testing::Testable]) {
    testing::testing::test_runner(tests);
}


