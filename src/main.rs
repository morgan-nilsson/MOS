#![no_std]
#![no_main]
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]
#![reexport_test_harness_main = "test_main"]
#![feature(abi_x86_interrupt)]

extern crate alloc;

pub mod stdio;
pub mod test;
pub mod clock;
pub mod keyboard;
pub mod types;
pub mod interrupts;
pub mod memory;

#[cfg(not(test))]
use core::panic::PanicInfo;
#[cfg(not(test))]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    use crate::println;
    println!("{}", info);
    hlt_loop();
}

use bootloader::{BootInfo, entry_point};
use memory::frame_allocator::BootFrameAllocator;
use memory::allocator;
use alloc::{boxed::Box, vec, vec::Vec, rc::Rc};

#[cfg(not(test))]
entry_point!( kernel_main );

#[unsafe(no_mangle)]
fn kernel_main(boot_info: &'static BootInfo) -> ! {

    use x86_64::VirtAddr;

    init_os();

    let phys_mem_offset = VirtAddr::new(boot_info.physical_memory_offset);
    let mut mapper = unsafe { memory::paging::init_offset_page_table(phys_mem_offset) };
    let mut frame_allocator = BootFrameAllocator::init(&boot_info.memory_map);

    allocator::init_heap(&mut mapper, &mut frame_allocator)
        .expect("heap initialization failed");


    let heap_value = Box::new(41);
    println!("heap_value at {:p}", heap_value);

    let mut vec = Vec::new();
    for i in 0..500 {
        vec.push(i);
    }
    println!("vec at {:p}", vec.as_slice());

    let reference_counted = Rc::new(vec![1, 2, 3]);
    let cloned_reference = reference_counted.clone();
    println!("current reference count is {}", Rc::strong_count(&cloned_reference));
    core::mem::drop(reference_counted);
    println!("reference count is {} now", Rc::strong_count(&cloned_reference));

    println!("It did not crash!");

    #[cfg(test)]
    test_main();

    hlt_loop();

}

#[cfg(test)]
entry_point!(test_kernel_main);

/// Entry point for `cargo test`
#[cfg(test)]
fn test_kernel_main(boot_info: &'static BootInfo) -> ! {

    use self::memory::allocator;
    use self::memory::frame_allocator::BootFrameAllocator;
    use x86_64::VirtAddr;

    init_os();
    let phys_mem_offset = VirtAddr::new(boot_info.physical_memory_offset);
    let mut mapper = unsafe { memory::paging::init_offset_page_table(phys_mem_offset) };
    let mut frame_allocator = BootFrameAllocator::init(&boot_info.memory_map);
    allocator::init_heap(&mut mapper, &mut frame_allocator)
        .expect("heap initialization failed");

    test_main();
    hlt_loop();
}

pub fn hlt_loop() -> ! {
    loop {
        x86_64::instructions::hlt();
    }
}

#[cfg(test)]
pub fn test_runner(tests: &[&dyn test::testing::Testable]) {
    test::testing::test_runner(tests);
}

fn init_os() {
    interrupts::interrupts::init_idt();
    memory::gdt::init();
    unsafe { interrupts::interrupts::PICS.lock().initialize() };
    x86_64::instructions::interrupts::enable();
}