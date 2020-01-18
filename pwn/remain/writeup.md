# ja
## 想定解
1. 一度確保し，解放した領域に対して編集を行う
2. UAF で key をつぶすことで，double free を可能にする
3. チャンクを偽装してfreeし，libcのアドレスをヒープ内に置く
4. libc のアドレスをパーシャル書き換えで stdout を差すようにし，\_IO\_write\_ptr を改竄
5. stdout のリークによって libc のベースアドレスを特定
6. tcahce の next を改竄して free\_hook からヒープを確保
7. free\_hook を改竄し，system関数を呼び出す
8. シェル

# en
## Expected solution
1. Edit the area that has been allocated and released
2. Double free is possible by crushing the key with UAF
3. Fake the chunk and free it, and put the libc address in the heap
4. Partial rewrite libc address to stdout and falsify \ _IO \ _write \ _ptr
5. Identify libc base address by stdout leak
6. Falsify next of tcahce and secure heap from free \ _hook
7. Falsify free \ _hook and call system function
8. Shell
