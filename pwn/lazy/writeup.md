## 想定解

### Step1 : パスワードリーク
1. 公開ファイルの中にユーザ名が書かれているので、それを見つけ出す
2. ログイン機能を攻撃し、Buffer Over Readによってパスワードリーク

### Step2 : シェル起動
3. 1と2で取得したユーザ名とパスワードを使ってログインし、サービスのバイナリとlibc.so.6を入手
4. サービスのバイナリを解析し、フォーマットストリングバグを見つけ出す
5. 1と2で取得したユーザ名とパスワードを使ってログイン。管理者用ファイルのダウンロード機能を書式文字列攻撃することで、libc\_start\_mainのGOTをリーク
6. libcのベースアドレスからone-gadget-RCEのアドレスを計算
7. ログイン機能を攻撃し、Buffer Over Flowによってリターンアドレスを書き換え、シェル起動


# en
## Assumed solution

### Step1 : Password leak
1. Find the username in the public file.
2. Attack login function and password leak by Buffer Over Read

### Step2 : Get shell
3. Log in using the user name and password obtained in steps 1 and 2, and obtain the service binary and libc.so.6
4. Parse the service binary to find format string bugs
5. Log in using the user name and password obtained in steps 1 and 2. GOT of libc\ _start\_main leaked by attacking the format string of the file download function for administrator

6. Calculate one-gadget-RCE address from libc base address
7. Attack login function, rewrite return address by Buffer Over Flow, and start shell
