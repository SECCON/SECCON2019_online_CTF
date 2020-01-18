# LAZY

## 問題文

なし（アクセス先のサーバ名（IP）とポート番号のみ）


## テーマ

- 簡易なファイル転送サービス
- 以下の機能をもつ
-- 公開ファイルのダウンロード機能（ログイン不要）
-- ログイン機能
-- 管理者用ファイルのダウンロード機能（要ログイン）

## 脆弱性

- スタックベースのBOF,BOR
- FSB

## 想定されるQ&A

- ユーザ名がわからない
-- 問題をくまなく探してください

- パスワードがわからない
-- その質問に対する回答はできません

- ユーザ名やパスワードの文字数はいくつ？
-- その質問に対する回答はできません

- フラグを取得するのにDOSは必要？
-- 必要ありません

- 一定時間接続していると切断される
-- 仕様です



## その他

filesはありません。


# en
# LAZY

## Challenge text

None（Servername(IP) and port）


## Theme

- Simple file transfer service
- following functions
-- Public file download function (no login required)
-- Login function
-- File download function for administrator (login required)

## Vulnerability

- Stack-based BOF,BOR
- FSB

## Expected Q&A

- I don't know my username
-- Find through the challenge

- I don't know my password
-- We can't answer that question

- How many characters for username and password?
-- We can't answer that question

- Do I need DOS to get flags?
-- No need

- Disconnected after connecting for a certain period of time
-- It is a specification.



## Others

no files
