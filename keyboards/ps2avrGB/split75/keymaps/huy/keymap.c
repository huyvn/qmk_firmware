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

#include "split75.h"

#include QMK_KEYBOARD_H

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

//Tap dance enums
enum {
  SPC_SHIFT = 0,
  TAB_SHIFT,
  BSLS_PIPE,
};

int cur_dance (qk_tap_dance_state_t *state);
void spc_finished (qk_tap_dance_state_t *state, void *user_data);
void spc_reset (qk_tap_dance_state_t *state, void *user_data);
void tabthumb_finished (qk_tap_dance_state_t *state, void *user_data);
void tabthumb_reset (qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = KEYMAP(
                                    KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10 , KC_F11, KC_F12  , KC_PSCR , KC_DEL , KC_INS, \
    LCTL(KC_C), LCTL(KC_X),         KC_GRAVE, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7 , KC_8, KC_9, KC_0, KC_MINUS, KC_EQUAL, KC_BSPC , KC_BSPC        , KC_PGUP, \
    LCTL(KC_V), LCTL(KC_Z),         KC_TAB  , KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U , KC_I, KC_O, KC_P, KC_LBRC , KC_RBRC , TD(BSLS_PIPE) ,            KC_PGDN , \
    LALT(S(KC_TAB)), LALT(KC_TAB), LCTL_T(KC_ESC), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J , KC_K, KC_L,  KC_SCLN, KC_QUOT   , KC_ENTER, KC_ENTER,       KC_HOME, \
    LCTL(S(KC_TAB)), LCTL(KC_TAB), KC_LSPO , KC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N , KC_M   , KC_COMM, KC_DOT , KC_SLSH   , KC_RSPC , KC_UP ,       KC_END, \
    LGUI(KC_L), LCTL(LALT(KC_DEL)), TT(1),KC_LGUI,KC_LALT, KC_SPC,CTL_T(KC_SPC),SFT_T(KC_SPC), KC_RALT,TT(2),KC_RCTL,KC_LEFT,KC_DOWN,KC_RIGHT \
    ),
    [1] = KEYMAP(
                      RESET,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
    KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, \
    DYN_REC_STOP , KC_NO, KC_TRNS, KC_MPLY, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP  ,KC_RIGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, \
    DYN_REC_START1, DYN_REC_START2, KC_CAPS,KC_CAPS, KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP,  KC_TRNS, \
    DYN_MACRO_PLAY1, DYN_MACRO_PLAY2, KC_TRNS, KC_TRNS, KC_TRNS, LSFT(KC_LCBR), KC_NLCK, LSFT(KC_RBRC),    KC_TRNS,  KC_TRNS, KC_TRNS,  KC_HOME, KC_PGDN,  KC_END \
    ),
    [2] = KEYMAP(
                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
    KC_P8 , KC_P9, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,        KC_7, KC_8, KC_9, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
    KC_P6 , KC_P7, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_KP_PLUS,     KC_4, KC_5, KC_6, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, \
    KC_P4 , KC_P5, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_KP_MINUS, KC_1, KC_2, KC_3, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, \
    KC_P2 , KC_P3, KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS, KC_TRNS, KC_KP_ASTERISK, KC_0, KC_0, KC_DOT, KC_KP_SLASH, KC_TRNS,KC_TRNS,KC_TRNS, \
    KC_P0 , KC_P1, KC_TRNS, KC_TRNS, KC_TRNS,               KC_TRNS, KC_TRNS, KC_KP_ENTER,  KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS \
    ),
};

int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

// Define action for Space on tap, Shift on hold
static tap spctap_state = {
  .is_press_action = true,
  .state = 0
};

void spc_finished (qk_tap_dance_state_t *state, void *user_data) {
  spctap_state.state = cur_dance(state);
  switch (spctap_state.state) {
    case SINGLE_TAP:
        tap_code(KC_SPC);
        break;
    default:
        register_code(KC_LSHIFT);
  }
}

void spc_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (spctap_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_SPC);
        break;
    default:
        unregister_code(KC_LSHIFT);
  }
  spctap_state.state = 0;
}

// Define action for Tab on tap, Shift on hold
static tap tabthumbtap_state = {
  .is_press_action = true,
  .state = 0
};

void tabthumb_finished (qk_tap_dance_state_t *state, void *user_data) {
  tabthumbtap_state.state = cur_dance(state);
  switch (tabthumbtap_state.state) {
    case SINGLE_TAP:
        tap_code(KC_TAB);
        break;
    default:
        register_code(KC_LSHIFT);
  }
}

void tabthumb_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (tabthumbtap_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_TAB);
        break;
    default:
        unregister_code(KC_LSHIFT);
  }
  tabthumbtap_state.state = 0;
}

// Map actions to be used in keymap
qk_tap_dance_action_t tap_dance_actions[] = {
  [SPC_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, spc_finished, spc_reset),
  [TAB_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tabthumb_finished, tabthumb_reset),
  [BSLS_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, KC_PIPE),
};

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {
    case KC_TRNS:
    case KC_NO:
      /* Always cancel one-shot layer when another key gets pressed */
      if (record->event.pressed && is_oneshot_layer_active())
      clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
      return true;
    case RESET:
      /* Don't allow reset from oneshot layer state */
      if (record->event.pressed && is_oneshot_layer_active()){
        clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
        return false;
      }
      return true;
    default:
      return true;
  }
  return true;
}

