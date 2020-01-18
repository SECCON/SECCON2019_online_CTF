# HakoniwaPay
# ja

## テーマ
箱庭、Web（バイナリ）、QRコード、●●Pay、クソ問、「絶対やると思った」  

## 脆弱性
クロスサイトスクリプティング（XSS）  

## 解答までの流れ
1. アプリを起動するとメッセージ送受信画面が表示される  
2. 相手から「僕から君に7777円を振り込ませることができたらフラグをあげるよ」と会話  
3. 送金、送金依頼、設定などの機能がある  
4. 「送金依頼QRコード」生成時に、「メッセージ」にHTMLタグを入れた場合はHTMLエンコードされてQR化される  
5. アプリ内では問題は起きないが、QRコードを自分で作れば、HTMLタグを実行させられる  
6. XSSを経由して、「送金するボタン」をクリックと同等の送金APIを踏ませる  
7. 相手から7777円を奪取できたらフラグが得られる  



 # en

 ## Theme
 Hakoniwa, Web(Binary), QR Code, xx Pay, Kusomon, "I thought you would do it."

 ## Vulnerability
 Cross Site Scripting (XSS)

 ## step to solve
 1. Message sending / receiving screen is displayed when the app is started.
 2. Conversation from ymzkei5: "If you can take 7777 yen from me, I will give you a FLAG."
 3. There are functions such as Request Money, Send Money, and Settings.
 4. When generating "Request Money QR Code", if you put an HTML tag in "Message", it will be HTML encoded.
 5. If you create the QR code by yourself, you can use HTML tags.
 6. Via XSS, send API request, equivalent to clicking on the "Send Money" button.
 7. Then, you can take 7777 yen from ymzkei5, and you can get a FLAG.
