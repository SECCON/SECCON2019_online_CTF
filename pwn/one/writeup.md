## 想定解
1. double free を行い，チャンク内にヒープのアドレスを置く
2. 閲覧機能を利用してヒープのアドレスを特定
3. 偽のチャンクを作ると共に tcahce のリストを改竄
4. 偽のチャンクを確保＆解放してlibc のアドレスをヒープ内に置く
5. 閲覧機能を利用して libc のアドレスを特定
6. tcahce の next を改竄して free\_hook からヒープを確保
7. free\_hook を system に書き換えてシェル


# en
## Expected solution
1. Perform double free and place heap address in chunk
2. Use the browsing function to specify the address of the heap
3. Create fake chunks and falsify tcahce list
4. Secure and release fake chunks and put libc address in heap
5. Identify libc address using browsing function
6. Falsify next of tcahce and secure heap from free \ _hook
7. Rewrite free \ _hook to system and shell
