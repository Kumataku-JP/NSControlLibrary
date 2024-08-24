# NSControlLibrary
- v1.1.1

Arduino を Nintendo Switch にコントローラーとして認識させ操作コード記述の簡素化を目的としたライブラリです。

# このライブラリについて

このライブラリは、celclow 氏の"SwitchControlLibrary"(https://github.com/celclow/SwitchControlLibrary)を元にArduinoSTLを別途必要としないよう改良を加え、
NSControlLibrary にて、ボタンを、押して離す、押しっぱなし、複数ボタンを同時押し、順番に押しっぱなし、順番に離す、連打など自動化するボタン操作、アナログスティックの傾きの方向と強さを数値的にわかりやすくし、必要なコード行数を簡素にし自動化したい操作手順を認識しやくすることを目指したライブラリです。

# 動作環境

動作環境は下記の通りです。

- Arduino IDE（2.3.2）
- Arduino AVR Boards（1.8.6）

Arduino IDE は Arduino AVR Boards は最新(2024.7 現在)の 1.8.6 で動作確認しております。

# 使用方法

NSControlLibrary フォルダを「スケッチブックの保存場所」内にある libraries フォルダに入れてください。
ライブラリを読み込むコードをファイルの先頭に下記の通りに記載してください。

```
#include <NSControlLibrary.h>
```

NSControlLibrary は、SwitchControlLibrary(2.0)を機軸に動作するライブラリです。
ArduinoSTL を利用している**オリジナルの SwitchControlLibrary**を読み込むことはできません。
NSControlLibrary フォルダ下層の SwitchControlLibrary が動作します。

**別途、Arduino を Switch に認識させるために boards.txt の Arduino Leonardo または Arduino Micro の欄の vid.1 と pid.1 を書換えが必要です。**

## ボタン操作方法

### アクションボタン

- ボタンを押して離す

  - `void pushButton(uint16_t button, unsigned long input_time = 40, unsigned long delay_time = 40, unsigned int loop = 1);`

    - button		 : 押すボタン種類
    - input_time	 : ボタンを押している時間(省略可、ms、デフォルトは 40)
    - delay_time	 : ボタンを押した後の待ち時間(省略可、ms、デフォルトは 40)
    - loop			 : 繰り返す回数(省略可、デフォルトは 1)

  - 使用例
    ```
    pushButton(Button::A);                // Aボタンを、0.04秒間押して離し、0.04秒間待つ、を1回する
    pushButton(Button::A, 500);           // Aボタンを0.5秒間押して離し、0.04秒間待つ、を1回する
    pushButton(Button::B, 40, 2000, 10);  // 0.04秒間押して離し、2秒間待つ、を10回繰り返す
    ```
	- ボタン操作を安定的かつ高速に入力できる間隔で設定しています。
	待ち時間や繰り返し回数をデフォルト以外の値にする場合は、ボタンを押している時間や待ち時間がデフォルト値でも省略出来ません。必ず値を入れてください。
*(ボタンを離した間隔、繰り返し回数がデフォルトの設定値なので省略化)(押している時間、離してからの待ち時間、繰り返し回数がデフォルトの設定値なので省略化)*

- ボタンを押し続ける

  - `holdButton(uint16_t button, unsigned long delay_time = 0);`

    - button		 : 押し続けるボタン
    - delay_time	 : ボタンを押してから次のコマンドへの待ち時間(ms)同時押しは 0(デフォルト)

  - `releaseButton(uint16_t button, unsigned long delay_time = 0);`

    - button		   : 離すボタン
    - delay_time	 : ボタンを離してから次のコマンドへの待ち時間(ms)同時離しは0(デフォルト)

  - 使用例
    - L ボタンと A ボタンを時間差で同時に押すコマンド
    ```
    holdButton(Button::L, 500);   	// Lボタンを押し続け次に押すボタンまでの間隔は0.5秒
    holdButton(Button::A, 2000);  	// Aボタンを押し続け次のコマンドまで2秒間待つ
    ```
	 - L ボタンと A ボタンを同時押し、2 秒後に同時にボタンを離すコマンド
    ```
    holdButton(Button::L); 		// L ボタンを押し続ける(次のコマンドへの間隔は 0)
    holdButton(Button::A, 2000); 	// A ボタンを押し続け次のコマンドまで 2 秒間待つ
    releaseButton(Button::L); 	// L ボタンを離す(次のコマンドへの間隔は 0)
    releaseButton(Button::A, 100); 	// A ボタンを離す(次のコマンドへの間隔は 0.1 秒)
    ```

    ホールドコマンドを実行するとリリースするまで押し続けます。その間ほかのコマンドを入力することが可能です。
    複数ボタンを同時押しする時は入力するボタンコマンドを続けて入力


- `Button` 定義一覧

  ```
  Button::Y
  Button::B
  Button::A
  Button::X
  Button::L
  Button::R
  Button::ZL
  Button::ZR
  Button::MINUS
  Button::PLUS
  Button::LCLICK
  Button::RCLICK
  Button::HOME
  Button::CAPTURE
  ```

### 十字キー（方向ボタン）

- 十字キーを押して離す(ボタン操作と同様の解釈)

  - `pushHat(uint8_t hat, unsigned long input_time = 40, unsigned long delay_time = 40, unsigned int loop = 1);`

    - hat			: 押す十字キーのボタン
    - input_time	: ボタンを押している時間(省略可、ms、デフォルトは 40)
    - delay_time	: ボタンを押した後の待ち時間(省略可、ms、デフォルトは 40)
    - loop 			: 繰り返す回数(省略可、デフォルトは 1)

  - 使用例
    ```
    pushHat(Hat::UP);               // 上キーを0.04秒間押して離し、0.04秒間待つ、を1回する
    pushHat(Hat::RIGHT, 50, 1000);  // 右キーを0.05秒間押して離し、1秒間待つ、を1回する
    pushHat(Hat::DOWN, 50, 50, 5);  // 下キーを0.05秒間押して離し、0.05秒間待つ、を5回繰り返す
    ```

- ボタンを押し続ける

  - `holdHat(uint8_t hat, unsigned long delay_time = 0);`

    - hat			     : 押し続ける十字キーボタン
    - delay_time	 : 十字キーボタンを押してから次のコマンドへの待ち時間(ms)同時押しは0(デフォルト)

  - `releaseHat(uint8_t hat, unsigned long delay_time = 0);`

    - button		   : 離す十字キーボタン
    - delay_time	 : 十字キーボタンを離してから次のコマンドへの待ち時間(ms)同時離しは0(デフォルト)

  - 使用例
    ```
    holdHat(Hat::DOWN, 100);          // 下キーを0.1秒間押し続けてから
    holdHat(Hat::DOWN_LEFT, 500);     // 左下方向に押し続け次のコマンド入力まで0.5秒待つ
    releaseHat(Hat::DOWN);            // 下キーを離し
    releaseHat(Hat::DOWN_LEFT, 500);  // 左下キーを離し次のコマンド入力まで0.5秒待つ
    ```

- `Hat` 定義一覧

  ```
  Hat::UP
  Hat::UP_RIGHT
  Hat::RIGHT
  Hat::DOWN_RIGHT
  Hat::DOWN
  Hat::DOWN_LEFT
  Hat::LEFT
  Hat::UP_LEFT
  Hat::NEUTRAL
  ```

### アナログスティック

- スティックの座標は、北方向を0°と基点にし 時計回りに360°(0〜360または0〜180、0〜-180)の値を指定します。<br>傾きの強さを 0〜100 の値で指定します。

- スティックを操作するコマンド

  - `leftStickTilt(int16_t angle_deg, uint8_t strength, unsigned long delay_time = 0);`
  - `leftStickNeutral(unsigned long delay_time = 0);`

  - `rightStickTilt(int16_t angle_deg, uint8_t strength, unsigned long delay_time = 0);`
  - `rightStickNeutral(unsigned long delay_time = 0);`

    - angle_deg	   : スティックの方向
    - strength		 : スティックの傾きの強さ
    - delay_time	 : スティックを倒してから次の入力まで間隔

  - 使用例
    ```
    leftStickTilt(0, 100, 1000);    // 左スティックを上に100%傾け、次のコマンドまで1秒間待つ
    leftStickNeutral(1000);         // 左スティックをニュートラルに戻し、次のコマンドまで1秒間待つ

    rightStickTilt(270, 50, 1500);  // 右スティックを左に50%傾け、次のコマンドまで1.5秒間待つ
    rightStickTilt(180, 100, 1000); // 右スティックを下に100%傾け、次のコマンドまで1秒間待つ
    rightStickNeutral(2000);        // 右スティックをニュートラルに戻し、次のコマンドまで2秒間待つ
    ```

## 修正履歴
  - 1.1.1 アナログスティックの定義重複の修正
  - 1.1.0 アナログスティックのコマンド記述方法の変更
    
## license
  - MIT
