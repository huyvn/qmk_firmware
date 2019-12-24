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
  BSLS_PIPE,
  LALT_CURLY_BKT,
  RALT_CURLY_BKT
};

int cur_dance (qk_tap_dance_state_t *state);
void spc_finished (qk_tap_dance_state_t *state, void *user_data);
void spc_reset (qk_tap_dance_state_t *state, void *user_data);
void lalt_finished (qk_tap_dance_state_t *state, void *user_data);
void lalt_reset (qk_tap_dance_state_t *state, void *user_data);
void ralt_finished (qk_tap_dance_state_t *state, void *user_data);
void ralt_reset (qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = KEYMAP(
                   KC_ESC  , KC_F1  , KC_F2  , KC_F3 , KC_F4 , KC_F5 , KC_F6  , KC_F7, KC_F8  , KC_F9  , KC_F10 , KC_F11    , KC_F12  , KC_PSCR , KC_HOME, KC_END, \
    KC_P8 , KC_P9, KC_GRAVE, KC_1   , KC_2   , KC_3  , KC_4  , KC_5  , KC_6   , KC_7 , KC_8   , KC_9   , KC_0   , KC_MINUS  , KC_EQUAL, KC_BSPC , KC_BSPC, KC_INS, \
    KC_P6 , KC_P7, KC_TAB  , KC_Q   , KC_W   , KC_E  , KC_R  , KC_T  , KC_Y   , KC_U , KC_I   , KC_O   , KC_P   , KC_LBRC   , KC_RBRC , TD(BSLS_PIPE) , KC_DEL , \
    KC_P4 , KC_P5, LCTL_T(KC_ESC), KC_A   , KC_S   , KC_D  , KC_F  , KC_G  , KC_H   , KC_J , KC_K   , KC_L   , KC_SCLN, KC_QUOT   , KC_ENTER, KC_ENTER, KC_PGUP, \
    KC_P2 , KC_P3, KC_LSPO , KC_LSFT, KC_Z   , KC_X  , KC_C  , KC_V  , KC_B   , KC_N , KC_M   , KC_COMM, KC_DOT , KC_SLSH   , KC_RSPC , KC_UP   , KC_PGDN, \
    KC_P0 , KC_P1, MO(1) , KC_LGUI, TD(LALT_CURLY_BKT), TD(SPC_SHIFT), KC_SPC, TD(SPC_SHIFT), TD(RALT_CURLY_BKT), MO(1), KC_RCTL, KC_LEFT, KC_DOWN, KC_RIGHT \
    ),
    [1] = KEYMAP(
                      RESET,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
    KC_COPY, KC_PASTE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
    KC_CUT , KC_UNDO, KC_TRNS, KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, \
    KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, \
    KC_TRNS, KC_TRNS, KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP,  KC_TRNS, \
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     RESET, KC_NLCK, RESET,                   KC_TRNS,  KC_TRNS, KC_TRNS,         KC_HOME, KC_PGDN,  KC_END) \
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
    case SINGLE_TAP: register_code(KC_SPC); break;
    default: register_code(KC_LSHIFT);
  }
}

void spc_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (spctap_state.state) {
    case SINGLE_TAP: unregister_code(KC_SPC); break;
    default: unregister_code(KC_LSHIFT); break;
  }
  spctap_state.state = 0;
}

// Define action for Left Curly Bracket on tap, LAlt on hold
static tap lalttap_state = {
  .is_press_action = true,
  .state = 0
};

void lalt_finished (qk_tap_dance_state_t *state, void *user_data) {
  lalttap_state.state = cur_dance(state);
  switch (lalttap_state.state) {
    case SINGLE_TAP:
        register_code(KC_LSFT);
        register_code(KC_LBRC);
        break;
    default: register_code(KC_LALT);
  }
}

void lalt_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (lalttap_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_LSFT);
        unregister_code(KC_LBRC);
        break;
    default: unregister_code(KC_LALT); break;
  }
  lalttap_state.state = 0;
}

// Define action for Right Curly Bracket on tap, RAlt on hold
static tap ralttap_state = {
  .is_press_action = true,
  .state = 0
};

void ralt_finished (qk_tap_dance_state_t *state, void *user_data) {
  ralttap_state.state = cur_dance(state);
  switch (ralttap_state.state) {
    case SINGLE_TAP:
        register_code(KC_RSFT);
        register_code(KC_RBRC);
        break;
    default: register_code(KC_RALT);
  }
}

void ralt_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (ralttap_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_RBRC);
        unregister_code(KC_RSFT);
        break;
    default: unregister_code(KC_RALT); break;
  }
  ralttap_state.state = 0;
}

// Map actions to be used in keymap
qk_tap_dance_action_t tap_dance_actions[] = {
  [SPC_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, spc_finished, spc_reset),
  [BSLS_PIPE] = ACTION_TAP_DANCE_DOUBLE(KC_BSLS, KC_PIPE),
  [LALT_CURLY_BKT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lalt_finished, lalt_reset),
  [RALT_CURLY_BKT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ralt_finished, ralt_reset),
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

