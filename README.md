# tpot_bmp
A fast header only library that can store BMP files with pre-memory allocation.

[![Build Status](https://travis-ci.com/t-pot/tpot_bmp.svg?branch=master)](https://travis-ci.com/t-pot/tpot_bmp)

# How to use
Download "tpot_bmp.h" and include in your project.
An example for storeing a bmp file is below.

```
#include "tpot_bmp.h"

int main(int argc, char* argv[])
{
	tpot::bmp* bmp = new  tpot::bmp(1920, 1080);// width and height
	
	// render image! (0, 0) is left-bottom
	unsigned char* p = bmp->get_image_addr();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int idx = 3 * (width * y + x);
			p[idx+0] = 128;             // Blue
			p[idx+1] = 256 * y / width; // Green
			p[idx+2] = 256 * x / width; // Red
		}
	}

	bmp->save("result.bmp");
	delete bmp;

	return 0;
}
```
This code will make result.bmp as below.

![red and green gradation](test/sample.bmp)

# What's this?
I have participated in a [Raytraining camp](https://sites.google.com/site/raytracingcamp5/), the rule of this event is to finish the program within a certain time.
As a participant, there is a strong desire to calculate to the last micro minute, but as @gam0022 mentioned[[1]](https://gam0022.net/blog/2019/09/18/rtcamp7/#png%E3%81%AE%E3%82%A8%E3%83%B3%E3%82%B3%E3%83%BC%E3%83%89%E6%99%82%E9%96%93%E3%81%AE%E7%9F%AD%E7%B8%AE), storage time becomes a big problem.

The higher the resolution, the longer the save time. This is a big problem in the future.
We want to shoot huge rays.

So I thought about saving faster.


Basically, reduction of memory access and pre-processing as much as possible leads to high speed, 
so as a design policy, first create a file image in memory, then write the image to memory and save it only when saving we made it.
At first I used png, but I needed to compress it, and I couldn't prevent a small memory copy.

In our environment, according to stb_image, the saving time which took 840ms is now 13ms in our library.

# 日本語解説
高速なBMPファイル保存をヘッダだけのライブラリです。メモリを事前に確保することで高速化しています。

## 使い方

"tpot_bmp.h"をダウンロードして、プロジェクトにインクルードしてください。
bmpファイルを保存するサンプルコードは、上記になります。

## 説明
某[レイトレ合宿](https://sites.google.com/site/raytracingcamp5/)に参加したことがあるのですが、
このイベントのルールは、一定時間内にプログラムを終了するというものになります。
参加者としては、ぎりぎりまで計算を行いたいという気持ちが強いのですが、
この時、[@gam0022さんが指摘](https://gam0022.net/blog/2019/09/18/rtcamp7/#png%E3%81%AE%E3%82%A8%E3%83%B3%E3%82%B3%E3%83%BC%E3%83%89%E6%99%82%E9%96%93%E3%81%AE%E7%9F%AD%E7%B8%AE)されている通り、保存時間が問題になってきます。

解像度がどんどん高くなることを考えると、保存時間を多くとられるのは将来的に大きな問題です。

我々は、超レイを飛ばしたいのだ～！

ということで、保存を早くすることを考えました。

基本的にメモリアクセスを減らしたり、事前にできることは事前に行うことが高速化につながるので、
最初にファイルイメージをメモリ上に作成して、そのメモリに書き込んで、保存時はただ保存をするという方針で設計しました。
最初はpngで行っていたのですが、pngは圧縮をする必要があり、細かなメモリ移動が防げなかったのでBMPファイルにしました。

こちらの環境では、stb_image で840msかかっていた保存時間が、13msで済むようになりました。
