/* v1.2.0 */

#include "NSControlLibrary.h"
#include <math.h>

/* ここからSwitchControlLibrary */
#if defined(_USING_HID)

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  0x05, 0x01,        //   USAGE_PAGE (Generic Desktop)
  0x09, 0x05,        //   USAGE (Game Pad)
  0xa1, 0x01,        //   COLLECTION (Application)
  0x15, 0x00,        //   LOGICAL_MINIMUM (0)
  0x25, 0x01,        //   LOGICAL_MAXIMUM (1)
  0x35, 0x00,        //   PHYSICAL_MINIMUM (0)
  0x45, 0x01,        //   PHYSICAL_MAXIMUM (1)
  0x75, 0x01,        //   REPORT_SIZE (1)
  0x95, 0x10,        //   REPORT_COUNT (16)
  0x05, 0x09,        //   USAGE_PAGE (Button)
  0x19, 0x01,        //   USAGE_MINIMUM (1)
  0x29, 0x10,        //   USAGE_MAXIMUM (16)
  0x81, 0x02,        //   INPUT (Data,Var,Abs)
  0x05, 0x01,        //   USAGE_PAGE (Generic Desktop)
  0x25, 0x07,        //   LOGICAL_MAXIMUM (7)
  0x46, 0x3b, 0x01,  //   PHYSICAL_MAXIMUM (315)
  0x75, 0x04,        //   REPORT_SIZE (4)
  0x95, 0x01,        //   REPORT_COUNT (1)
  0x65, 0x14,        //   UNIT (20)
  0x09, 0x39,        //   USAGE (Hat Switch)
  0x81, 0x42,        //   INPUT (Data,Var,Abs)
  0x65, 0x00,        //   UNIT (0)
  0x95, 0x01,        //   REPORT_COUNT (1)
  0x81, 0x01,        //   INPUT (Cnst,Arr,Abs)
  0x26, 0xff, 0x00,  //   LOGICAL_MAXIMUM (255)
  0x46, 0xff, 0x00,  //   PHYSICAL_MAXIMUM (255)
  0x09, 0x30,        //   USAGE (X)
  0x09, 0x31,        //   USAGE (Y)
  0x09, 0x32,        //   USAGE (Z)
  0x09, 0x35,        //   USAGE (Rz)
  0x75, 0x08,        //   REPORT_SIZE (8)
  0x95, 0x04,        //   REPORT_COUNT (4)
  0x81, 0x02,        //   INPUT (Data,Var,Abs)
  0x06, 0x00, 0xff,  //   USAGE_PAGE (Vendor Defined 65280)
  0x09, 0x20,        //   USAGE (32)
  0x95, 0x01,        //   REPORT_COUNT (1)
  0x81, 0x02,        //   INPUT (Data,Var,Abs)
  0x0a, 0x21, 0x26,  //   USAGE (9761)
  0x95, 0x08,        //   REPORT_COUNT (8)
  0x91, 0x02,        //   OUTPUT (Data,Var,Abs)
  0xc0               // END_COLLECTION
};

SwitchControlLibrary_::SwitchControlLibrary_() {
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  CustomHID().AppendDescriptor(&node);

  memset(&_joystickInputData, 0, sizeof(USB_JoystickReport_Input_t));
  _joystickInputData.LX = Stick::NEUTRAL;
  _joystickInputData.LY = Stick::NEUTRAL;
  _joystickInputData.RX = Stick::NEUTRAL;
  _joystickInputData.RY = Stick::NEUTRAL;
  _joystickInputData.Hat = Hat::NEUTRAL;
}

void SwitchControlLibrary_::sendReport() {
  CustomHID().SendReport(&_joystickInputData, sizeof(USB_JoystickReport_Input_t));
}
void SwitchControlLibrary_::pressButton(uint16_t button) {
  _joystickInputData.Button |= button;
}
void SwitchControlLibrary_::releaseButton(uint16_t button) {
  _joystickInputData.Button &= (button ^ 0xffff);
}
void SwitchControlLibrary_::pressHatButton(uint8_t hat) {
  _joystickInputData.Hat = hat;
}
void SwitchControlLibrary_::releaseHatButton() {
  _joystickInputData.Hat = Hat::NEUTRAL;
}
void SwitchControlLibrary_::moveLeftStick(uint8_t lx, uint8_t ly) {
  _joystickInputData.LX = lx;
  _joystickInputData.LY = ly;
}
void SwitchControlLibrary_::moveRightStick(uint8_t rx, uint8_t ry) {
  _joystickInputData.RX = rx;
  _joystickInputData.RY = ry;
}
SwitchControlLibrary_ &SwitchControlLibrary() {
  static SwitchControlLibrary_ obj;
  return obj;
}

#endif

/* ここからNSControlLibrary */
// 度からラジアンに変換するヘルパー関数
#define MY_DEG_TO_RAD(angleDegrees) ((angleDegrees)*0.017453292519943295769236907684886)

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
 *  @param uint8_t       button     離すボタン
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
 *  @param uint8_t       stick       動かすスティック
 *  @param int16_t       angle_deg   方向を度数法で指定（0°は上）
 *  @param uint8_t       strength    スティックの強さを0〜100で指定
 *  @param unsigned long delay_time  待ち時間(ms、デフォルトは0)
 *
 */
void stickTilt(uint8_t stick, int16_t angle_deg, uint8_t strength, unsigned long delay_time) {
  double angle_rad = MY_DEG_TO_RAD(angle_deg - 90); // 0°が上（北）になるように調整
  int16_t x = cos(angle_rad) * strength; // 強度をかけてX座標に変換
  int16_t y = sin(angle_rad) * strength; // 強度をかけてY座標に変換

  uint8_t stickX = map(x, -100, 100, Stick::MIN, Stick::MAX); // X座標をマッピング
  uint8_t stickY = map(y, -100, 100, Stick::MIN, Stick::MAX); // Y座標をマッピング
  if (stick == Stick::LEFT) {
    SwitchControlLibrary().moveLeftStick(stickX, stickY);
  } else if (stick == Stick::RIGHT) {
    SwitchControlLibrary().moveRightStick(stickX, stickY);
  }
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}

/*  スティックを元に戻す
 *  
 *  @param uint8_t       stick       戻すスティック
 *  @param unsigned long delay_time ボタンを離した後の待ち時間(ms)
 */

void stickNeutral(uint8_t stick, unsigned long delay_time) {
  if (stick == Stick::LEFT) {
    SwitchControlLibrary().moveLeftStick(Stick::NEUTRAL, Stick::NEUTRAL);
  } else if (stick == Stick::RIGHT) {
    SwitchControlLibrary().moveRightStick(Stick::NEUTRAL, Stick::NEUTRAL);
  }
  SwitchControlLibrary().sendReport();
  delay(delay_time);
}
