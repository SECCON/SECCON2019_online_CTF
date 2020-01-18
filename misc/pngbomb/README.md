pngbomb
=======

## 問題文
pngbomb? or just a barcode in a haystack.

## 内容

2147483647 x 32 の PNGファイルです。
問題ファイルはファイル名の通り gzip されています。

## 難易度

libpng では、保護機構によりエラーになります。
node や go には直接読めるライブラリがあるかもしれません。

どうにかしてファイルを開き自由に中を読み取ることができれば、ノイズの中から 1次元（普通の）バーコードを探すだけです。

問題は、どうやってこの巨大なPNG画像を読める状態にするか、です。


# en

## Challenge Text
pngbomb? or just a barcode in a haystack.

## Contents

This is a 2147483647 x 32 PNG file.
The problem file is gzipped according to the file name.

## Difficulty

In libpng, an error occurs due to a protection mechanism.
node or go may have a library that can be read directly.

If you can open the file and read inside freely, all you have to do is look for one-dimensional (ordinary) barcodes in the noise.

This challenge is how to make this huge PNG image readable.
