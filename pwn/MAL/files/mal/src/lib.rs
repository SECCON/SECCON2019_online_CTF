use std::alloc::GlobalAlloc;
use std::alloc::Layout;

#[link(name = "mal", kind = "static")]
extern "C" {
    fn allocate(size: usize) -> *mut u8;
    fn deallocate(ptr: *mut u8);
}

pub struct Heap;
#[global_allocator]
static A: Heap = Heap;

unsafe impl GlobalAlloc for Heap {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        allocate(layout.size())
    }
    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        deallocate(ptr)
    }
}
