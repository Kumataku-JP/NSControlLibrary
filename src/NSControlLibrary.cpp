/* v1.1.1 */

#include "NSControlLibrary.h"
#include <math.h>

// 度からラジアンに変換するヘルパー関数
#define MY_DEG_TO_RAD(angleDegrees) ((angleDegrees) * 0.017453292519943295769236907684886)

uint8_t scaleValue(int8_t value, int8_t fromLow, int8_t fromHigh, uint8_t toLow, uint8_t toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}
/*  ボタンを押して離す
 *
 *  @param uint16_t      button     押すボタン
 *  @param unsigned long input_time ボタンを押してから離すまでの時間(ms、デフォルトは40)
 *  @param unsigned long delay_time ボタンを押した後の待ち時間(ms、デフォルトは40)
 *  @param unsigned int  loop       繰り返す回数(省略可、デフォルトは1)
 */
void pushButton(uint16_t button, unsigned long input_time, unsigned long delay_time, unsigned int loop) {
  for (unsigned int i = 0; i < loop; i++) {
    SwitchControlLibrary().pressButton(button);
    SwitchControlLibrary().sendReport();
    delay(input_time);
    SwitchControlLibrary().releaseButton(button);
    SwitchControlLibrary().sendReport();
    delay(delay_time);
  }
}

/*  ボタンを押し続ける
 *
 *  @param uint16_t      button    押し続けるボタン
 *  @param unsigned long delay_time ボタンを押した後の待ち時間(ms)同時押しは0
 */
void holdButton(uint16_t button, unsigned long delay_time) {
  SwitchControlLibrary().pressButton(button);
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}
/*  ボタンを離す
 *
 *  @param uint16_t      button     離すボタン
 *  @param unsigned long delay_time ボタンを離した後の待ち時間(ms)
 */
void releaseButton(uint16_t button, unsigned long delay_time) {
  SwitchControlLibrary().releaseButton(button);
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}


/*  十字キーを押して離す
 *
 *  @param uint8_t       hat        押す十字キーのボタン
 *  @param unsigned long input_time ボタンを押してから離すまでの時間(ms、デフォルトは50)
 *  @param unsigned long delay_time ボタンを押した後の待ち時間(ms、デフォルトは50)
 *  @param unsigned int  loop       繰り返す回数(省略可、デフォルトは1)
 */
void pushHat(uint8_t hat, unsigned long input_time, unsigned long delay_time, unsigned int loop) {
  for (unsigned int i = 0; i < loop; i++) {
    SwitchControlLibrary().pressHatButton(hat);
    SwitchControlLibrary().sendReport();
    delay(input_time);
    SwitchControlLibrary().releaseHatButton();
    SwitchControlLibrary().sendReport();
    delay(delay_time);
    }
}

/*  十字キーを押し続ける
 *
 *  @param uint8_t       hat:       押し続ける十字キーのボタン
 *  @param unsigned long delay_time ボタンを押した後の待ち時間(ms)
 */
void holdHat(uint8_t hat, unsigned long delay_time) {
  SwitchControlLibrary().pressHatButton(hat);
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}
/*  十字キーを離す
 *
 *  @param uint8_t      button     離すボタン
 *  @param unsigned long delay_time ボタンを離した後の待ち時間(ms)
 */
void releaseHat(unsigned long delay_time) {
  SwitchControlLibrary().releaseHatButton();
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}


/*  スティックを動かす
 *  方向を度数法(0〜360)、強さを0〜100の範囲で指定する
 *
 *  @param int16_t       angle_deg   方向を度数法で指定（0°は上）
 *  @param uint8_t       strength    スティックの強さを0〜100で指定
 *  @param unsigned long delay_time  待ち時間(ms、デフォルトは0)
 *
 */

 //  左スティック
void leftStickTilt(int16_t angle_deg, uint8_t strength, unsigned long delay_time) {
    double adjusted_angle_deg = angle_deg - 90; // 0°が北になるように調整
    double angle_rad = MY_DEG_TO_RAD(adjusted_angle_deg);
    int8_t lx = (int8_t)(cos(angle_rad) * strength);
    int8_t ly = (int8_t)(sin(angle_rad) * strength);
    
    uint8_t scaledLX = map(lx, -100, 100, 0, 255);
    uint8_t scaledLY = map(ly, -100, 100, 0, 255);

    SwitchControlLibrary().moveLeftStick(scaledLX, scaledLY);
    SwitchControlLibrary().sendReport();
    delay(delay_time);
}

//  右スティック
void rightStickTilt(int16_t angle_deg, uint8_t strength, unsigned long delay_time) {
    double adjusted_angle_deg = angle_deg - 90;
    double angle_rad = MY_DEG_TO_RAD(adjusted_angle_deg);
    int8_t rx = (int8_t)(cos(angle_rad) * strength);
    int8_t ry = (int8_t)(sin(angle_rad) * strength);

    uint8_t scaledRX = map(rx, -100, 100, 0, 255);
    uint8_t scaledRY = map(ry, -100, 100, 0, 255);

    SwitchControlLibrary().moveRightStick(scaledRX, scaledRY);
    SwitchControlLibrary().sendReport();
    delay(delay_time);
}

/*  スティックを元に戻す
 *   
 *  @param unsigned long delay_time ボタンを離した後の待ち時間(ms)
 */

 //  左スティック
void leftStickNeutral(unsigned long delay_time) {
  SwitchControlLibrary().moveLeftStick(128, 128);
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}
//  右スティック
void rightStickNeutral(unsigned long delay_time) {
  SwitchControlLibrary().moveRightStick(128, 128);
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}
