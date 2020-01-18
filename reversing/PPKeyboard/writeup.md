# Writeup
# ja

## 想定解法

1. 配布されたバイナリを解析すると `DDJ-XP1` というデバイスが接続されていないと処理が終了することがわかる。
2. `DDJ-XP1` を用いて `midiInOpen` が成功する。
3. `DDJ-XP1` からの入力を処理する関数は `midiInOpen` の第3引数に指定した関数となるため、主にここを解析すればよいことがわかる。
4. この関数では `DDJ-XP1` から入力された値 (16進数) の下から3桁目を表示していることがわかる。このとき、`DDJ-XP1` から入力された値 (16進数) の下から2桁が `0x97` のときは `0x` というプレフィックスが付き、`0x99` のときは ` ` というサフィックスが付くことがわかる。
5. 解析結果に則って配布された pcap ファイルに記録された `DDJ-XP1` からの入力を処理していくと `Hey guys! FLAG is SECCON{3n73r3d_fr0m_7h3_p3rf0rm4nc3_p4d_k3yb04rd}` という文字列が得られる。
6. FLAG は `SECCON{3n73r3d_fr0m_7h3_p3rf0rm4nc3_p4d_k3yb04rd}`


# en

## Expected solution

1. Analyzing the distributed binary, you can see that the process ends if the device named `DDJ-XP1` is not connected.
2. `midiInOpen` succeeds using` DDJ-XP1`.
3. Since the function that processes the input from `DDJ-XP1` is the function specified in the third argument of` midiInOpen`, it can be understood that this part should be analyzed mainly.
4. You can see that this function displays the third digit from the bottom of the value (hexadecimal) input from `DDJ-XP1`. At this time, if the last two digits of the value (hexadecimal) input from `DDJ-XP1` are` 0x97`, the prefix is ​​`0x`, and if it is` 0x99`, the prefix is ​​`` I understand.
5. If you process the input from `DDJ-XP1` recorded in the distributed pcap file based on the analysis result, you will get the character string` Hey guys! FLAG is SECCON {3n73r3d_fr0m_7h3_p3rf0rm4nc3_p4d_k3yb04rd} `.
6.  FLAG is `SECCON {3n73r3d_fr0m_7h3_p3rf0rm4nc3_p4d_k3yb04rd}`
