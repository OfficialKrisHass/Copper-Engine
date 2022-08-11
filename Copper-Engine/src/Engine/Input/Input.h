#pragma once

#include "Engine/Core/Core.h"

#define KeySpace              32
#define KeyApostrophe         39  /* ' */
#define KeyComma              44  /* , */
#define KeyMinus              45  /* - */
#define KeyPeriod             46  /* . */
#define KeySlash              47  /* / */
#define Key0                  48
#define Key1                  49
#define Key2                  50
#define Key3                  51
#define Key4                  52
#define Key5                  53
#define Key6                  54
#define Key7                  55
#define Key8                  56
#define Key9                  57
#define KeySemicolon          59  /* ; */
#define KeyEqual              61  /* = */
#define KeyA                  65
#define KeyB                  66
#define KeyC                  67
#define KeyD                  68
#define KeyE                  69
#define KeyF                  70
#define KeyG                  71
#define KeyH                  72
#define KeyI                  73
#define KeyJ                  74
#define KeyK                  75
#define KeyL                  76
#define KeyM                  77
#define KeyN                  78
#define KeyO                  79
#define KeyP                  80
#define KeyQ                  81
#define KeyR                  82
#define KeyS                  83
#define KeyT                  84
#define KeyU                  85
#define KeyV                  86
#define KeyW                  87
#define KeyX                  88
#define KeyY                  89
#define KeyZ                  90
#define KeyLeftBracket        91  /* [ */
#define KeyBackslash          92  /* \ */
#define KeyRightBracket       93  /* ] */
#define KeyGrave              96  /* ` */
#define KeyWorld1             161 /* non-US #1 */
#define KeyWorld2             162 /* non-US #2 */
#define KeyEscape             256
#define KeyEnter              257
#define KeyTab                258
#define KeyBackspace          259
#define KeyInsert             260
#define KeyDelete             261
#define KeyRight              262
#define KeyLeft               263
#define KeyDown               264
#define KeyUp                 265
#define KeyPageUp             266
#define KeyPageDown           267
#define KeyHome               268
#define KeyEnd                269
#define KeyCapsLock           280
#define KeyScrollLock         281
#define KeyNumLock            282
#define KeyPrintScreen        283
#define KeyPause              284
#define KeyF1                 290
#define KeyF2                 291
#define KeyF3                 292
#define KeyF4                 293
#define KeyF5                 294
#define KeyF6                 295
#define KeyF7                 296
#define KeyF8                 297
#define KeyF9                 298
#define KeyF10                299
#define KeyF11                300
#define KeyF12                301
#define KeyF13                302
#define KeyF14                303
#define KeyF15                304
#define KeyF16                305
#define KeyF17                306
#define KeyF18                307
#define KeyF19                308
#define KeyF20                309
#define KeyF21                310
#define KeyF22                311
#define KeyF23                312
#define KeyF24                313
#define KeyF25                314
#define KeyKP0                320
#define KeyKP1                321
#define KeyKP2                322
#define KeyKP3                323
#define KeyKp4                324
#define KeyKP5                325
#define KeyKP6                326
#define KeyKP7                327
#define KeyKP8                328
#define KeyKP9                329
#define KeyDecimal            330
#define KeyDivide             331
#define KeyMultiply           332
#define KeySubtract           333
#define KeyAdd                334
#define KeyKPEnter            335
#define KeyKPEqual            336
#define KeyLeftShift          340
#define KeyLeftControl        341
#define KeyLeftAlt            342
#define KeyLeftSuper          343
#define KeyRightShift         344
#define KeyRightControl       345
#define KeyRightAlt           346
#define KeyRightSuper         347
#define KeyMenu               348

typedef int Key;

namespace Copper {

	bool IsKey(Key key);

}