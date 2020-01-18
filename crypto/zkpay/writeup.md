# ja
## 脆弱性

プレイヤーはKeyGeneratorに記載された算術演算を解読し、Proverに与える解を導きだすことになるだろう。

今回は算術演算をリバーシングではなく、KeyGeneratorの脆弱性を突くことを想定している。KeyGenerratorはProverとVerifierに与える鍵を生成する際に乱数を用いるが、この乱数はtoxic wasteとも呼ばれる本来捨てるべき変数tauである。
この値が悪意者の手にわたることで件の算術演算の解を知ることなく、偽のproofを生成することができるようになる。

今回の問題では脆弱なPRNGがtauを生成するということを想定している

## 攻撃の流れ

- KeyGeneratorを読み、tauを推測可能であることを知る
- 候補となるtauを列挙する
- 参考資料"Creating fake zkSNARK proofs"を参考にtauを用いて偽proofを作るコードを書く
- 手元のVerifierで偽proofが認証されるかを確認する
- 運営側のサーバに偽proofを送信するとflagが落ちてくる


# en
## Vulnerability

Players will be able to decipher the arithmetic operations described in KeyGenerator and derive the solution to be given to Prover.

This time, instead of reversing arithmetic operations, it is assumed to exploit the vulnerability of KeyGenerator.
KeyGenerrator uses a random number when generating keys to be given to Prover and Verifier, and this random number is a variable tau that is originally called toxic waste and should be discarded.
This value can be handed to the Evil to generate a fake proof without knowing the solution to the arithmetic operation.
In this challenge it is assumes that vulnerable PRNGs generate tau


## Attack flow

- Read KeyGenerator and find out that tau can be guessed
- List candidate tau
- Write code to create fake proof using tau with reference to reference material "Creating fake zkSNARK proofs"
- Check if the fake proof is certified by the verifier at hand
- The flag drops when sending a fake proof to the challenge server
