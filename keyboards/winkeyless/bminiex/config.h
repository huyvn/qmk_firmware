/*
Copyright 2017 Luiz Ribeiro <luizribeiro@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

#define VENDOR_ID       0x20A0
#define PRODUCT_ID      0x422E
#define DEVICE_VER      0x0200
#define MANUFACTURER    winkeyless.kr
#define PRODUCT         B.mini Ex

#define RGBLED_NUM 20

/* matrix size */
#define MATRIX_ROWS 8
#define MATRIX_COLS 15

#define RGBLIGHT_ANIMATIONS

#define BACKLIGHT_LEVELS 5

#define NO_UART 1
