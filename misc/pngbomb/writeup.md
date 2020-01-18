write up
====

## 解説

PNG ファイルの IHDR の画素サイズは 32bit unsigned integer ですが、その最大サイズ近辺の大きな画像は大変扱いにくいです。
画像データは単一の zlib ストリームとして表現されているので、特定の場所を切り出すにはそこまでのデータをシーケンシャルに展開している必要があります。（つまりピクセルにランダムアクセス出来ません）
さらに悪いことに、PNGの各行(scanline)は単純なフィルタによってエンコードされているので、デコーダは必ず直前の1行のデータにアクセスできる必要があります（リングバッファで実装しても最大で1行分のメモリが必須です）
以上のことから、横幅が大きな PNG ファイルは容易には扱えないことが分かります。
（逆に、縦幅だけが大きな PNG ファイルでは、時間はかかりますがメモリはそこまで必要になりません）
世の中のデコーダは大抵の場合、全画素を保持するフレームバッファを作成してしまうので、この状況をさらに悪化させています。


## Method 1 (Not recommended)
Bulid a custom libpng so that you can handle the png file, open the png file with your favorite tool like gimp, and find a barcode by your eyes.


## Method 2 (Straight method)
1. .gz ファイルを解凍した結果の .png ファイルの圧縮率の高さから、png ファイルのバイナリのエントロピーの低さに気づく。
2. png ファイルを開くと、短い繰り返しがたくさん観測できる。
3. PNGの仕様書から IDATチャンク が zlib であることを瞬時に理解する。
4. その zlib の中身はただの DEFLATE 圧縮であることを瞬時に理解する。
5. DEFLATE圧縮ストリームの構造（とLZSS圧縮やハフマン符号の基礎）を理解する（少し難しい部分）
6. エントロピーが高い部分を見つけ、その部分を含むDEFLATEブロックだけを取り出す。
7. そこだけを zlib が扱えるツールでさくっと展開する。
8. PNG ヘッダの情報から、1bit 画像なので、画像化する。
9. バーコードが出現する。


## ヒントの場所や優しさ
* png ファイルの圧縮率が高い。 すなわちエントロピーが低い。 繰り返しを多く含んでいる。 ゴミをゴミと認識しやすい。
* ゴミデータは必ず順番通りに繰り返しで出現するので、一定のルールを守りながらなら途中を抜かしても大丈夫になっている。 注意深やれば、横幅を減らす操作が行える。
* PNGのフィルタタイプが2になっている行では、00 が一定数連続する部分が出現する。 圧縮ストリーム上でもわかりやすい。 00 はその行が直上の画素と全く同じデータであることを示しているので、まさに1次元バーコードのような縦線の絵になることを示している。
* PNGのフィルタタイプが2になっている行の真上は、フィルタタイプが0（そのまま格納）になっている優しさ。 その行だけ取り出してバーコード検出ツールに掛けるとすぐ検出できる。
* 問題文で 「A」 barcode と言っている。 （邪魔データをたくさん混ぜるという嫌がらせは見送り）
* モノクロ 1bpp、高さを 32px にした（フレームバッファタイプを素直に使って 2Gpx * 32px * 8bpp = 64GB で済む。 1bpp なら 8GB）
* 1次元にした（QRだとかなり見つけにくいと思う。 1次元だとフィルタタイプ2 が生きる）


# en

## Description

Although the pixel size of the IHDR of a PNG file is 32bit unsigned integer, it is very difficult to handle a large image near its maximum size.
Image data is represented as a single zlib stream, so to extract a specific location, the data up to that point must be sequentially expanded. (In other words, you ca n’t randomly access pixels)
To make matters worse, each line of the PNG (scanline) is encoded by a simple filter, so the decoder must always have access to the data of the previous line (Even if implemented with a ring buffer, a maximum of one line of memory is required)
From the above, it can be understood that PNG files with large width cannot be handled easily.
（Conversely, a PNG file with a large height only takes longer but does not require much memory）
This situation is exacerbated by the fact that most decoders in the world create frame buffers that hold all pixels.


## Method 1 (Not recommended)
Bulid a custom libpng so that you can handle the png file, open the png file with your favorite tool like gimp, and find a barcode by your eyes.


## Method 2 (Straight method)
1. Due to the high compression ratio of the .png file resulting from decompressing the .gz file, Notice the low entropy of the binary in the png file.
2. When you open the png file, you can observe many short repetitions.
3. Instantly understand that the IDAT chunk is zlib from the PNG specification.
4. Instantly realize that the contents of the zlib are just DEFLATE compression.
5. Understand the structure of the DEFLATE compressed stream (and the basics of LZSS compression and Huffman coding) (a bit difficult part)
6. Find the part with high entropy and extract only the DEFLATE block containing that part.
7. Extract only that with a tool that zlib can handle.
8. Since it is a 1-bit image from the information in the PNG header, convert to image.
9. Find barcode.


## Hint point and kindness :)
* High compression rate for png files. That is, the entropy is low. Contains a lot of repetition. It is easy to recognize garbage as garbage.
* Since garbage data always appears repeatedly in order, it is okay to skip the middle if you follow certain rules. If you are careful, you can reduce the width.
* In rows where the filter type of PNG is 2, a part where 00 continues for a certain number of times appears. Easy to understand even on compressed streams.00 indicates that the row has exactly the same data as the pixel immediately above it, indicating that the picture is just a vertical line like a one-dimensional barcode.
* Just above the row where the PNG filter type is 2, it is kindness that the filter type is 0 (stored as is).You can detect it as soon as you take out that line and apply it to a barcode detection tool.
* The problem sentence says "A" barcode. (I will not give up harassment of mixing a lot of disturbing data.)
* Use 1bpp monochrome and 32px height (2Gpx * 32px * 8bpp = 64GB using the framebuffer type, if use 1bpp need 8GB)
* 1D (I think it is hard to find if it is QR. Filter type 2 will live if it is 1D)
