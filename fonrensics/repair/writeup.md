## ja
# 解答方法
1. バイナリエディタで問題ファイルを確認する。
2. 00dc（ビデオチャンク(**dc)）があるのを確認する。
3. 先頭に管理情報がないため、管理情報を付け加える。
4. AVIファイルフォーマットを確認する。
5. 管理情報を作成する。
　RIFF AVI
　　LIST hdrl
　　　avih(MainAVIHeader)
　　　LIST strl(StreamHeaderList)
　　　　 strh(StreamHeader要素) -> fccTypeに「vids(ビデオ)」を設定
　　　　 strf(StreamHeader要素) -> Bitmap Information Headerを確認
　　　　 4バイト目 biWidth（画像の幅）「0x3C0(960)」より大きいサイズに設定
　　　　 8バイト目 biHeght（画像の高さ）「0x31C(540)」より大きいサイズに設定
　　　　 16バイト目 コーデックを「cvid」（cinepak）に設定
　　LIST movi
　・上記の管理情報を作成して、問題ファイルの先頭に付けると再生可能となる。
　・strfは、ビデオはBITMAPINFOHEADER構造体である。（参考資料参照）
　・フリーの動画再生ソフトであるVLC media Player、MPC-HC(Media Player Classic Home Cinema)、SMPlayer等で再生を確認する。
　・Riffpadで、RIFFフォーマットを確認する。

参考資料
　AVIファイルフォーマット
　https://makiuchi-d.github.io/mksoft/doc/avifileformat.html

　Bitmapファイルフォーマット => Bitmap Information Header
　http://www.umekkii.jp/data/computer/file_format/bitmap.cgi
　https://www.setsuki.com/hsp/ext/bmp.htm

　動画コーデックの詳細表示機能 -> コーデック一覧
　http://www.area61.com/codecs.html

　Riffpad
　https://www.menasoft.com/blog/?p=34


## en
# How to solve

1. Check the files using binary editor.
2. Recognize 00dc（video chunk (**dc)) exist.
3. Since there is no management information at the beginning, management information is added.
4. Check the AVI file format.
5. Create management information.
　RIFF AVI
　　LIST hdrl
　　　avih(MainAVIHeader)
　　　LIST strl(StreamHeaderList)
　　　　 strh(StreamHeader element) -> set "vids(video)"" on fccTypeに
　　　　 strf(StreamHeader element) -> Recognize Bitmap Information Header
　　　　 in 4byte biWidth（Width of image）set to larger "0x3C0(960)"
　　　　 in 8byte biHeght（Hight of image）set to larger "0x31C(540)"
　　　　 in 16byte set codec "cvid"（cinepak）
　　LIST movi
　・Add above management information to the head of the file, it will be playble.
　・strf, video is a BITMAPINFOHEADER structure（See the References）
　・Check playback with free video playback software such as VLC media Player, MPC-HC (Media Player Classic Home Cinema), SMPlayer, etc.
　・Check the RIFF format with Riffpad.


Reference (JPN)
　AVI file format
　https://makiuchi-d.github.io/mksoft/doc/avifileformat.html

　Bitmap file format => Bitmap Information Header
　http://www.umekkii.jp/data/computer/file_format/bitmap.cgi
　https://www.setsuki.com/hsp/ext/bmp.htm

　Video codec detail display function -> List of codec
　http://www.area61.com/codecs.html

　Riffpad
　https://www.menasoft.com/blog/?p=34
