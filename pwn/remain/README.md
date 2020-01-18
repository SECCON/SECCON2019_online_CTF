# ja
# Remain

## テーマ
glibc 2.29 以降では double free が key によってチェックされるようになったが，それを回避する問題  
glibc 2.30 では tcache の entries が NULL かどうかではなく，counts が1以上かどうかを見て割り当てを行うようになったので，その挙動の違いについても考慮して解いてほしい  
プログラム自体は出力機能のないメモ帳で追加・編集・削除が可能  

下記 Stickey からはインタフェースは継承しているが，登録時のid指定等の一部仕様を変更しているため，解き方も変更されている  

## 脆弱性
- use after free
- double free


# en
# Remain

## Theme

Since glibc 2.29, double free is checked by key, but this challenge is to avoid it
In glibc 2.30, allocation is performed based on whether counts is 1 or more instead of NULL in entries in tcache, so please consider the difference in behavior and solve it
Program itself can be added / edited / deleted with notepad without output function

The interface is inherited from Stickey below, but the method of solving has also been changed because some specifications such as id specification at the time of registration have been changed


## Vulnerability
- use after free
- double free
