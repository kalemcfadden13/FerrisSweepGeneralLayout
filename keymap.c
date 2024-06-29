
#include QMK_KEYBOARD_H


enum tap_dance_codes {
  DANCE_1,
  DANCE_2,
  LEFT_HME,
  RIGHT_END,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_Y,          KC_C,             KC_L,              KC_M,                 KC_K,  KC_Z,          KC_F,           KC_U,          TD(DANCE_2),          KC_BSPC,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  MT(MOD_LCTL,KC_I),          KC_S,       KC_R,       KC_T,    MT(MOD_LGUI,KC_G),  MT(MOD_RGUI,KC_P), KC_N,       KC_E,             KC_A,    MT(MOD_RCTL,KC_O),
  //-----------------------------------------------------------------------------  ---------------------------------------------------------------------------
  KC_Q,          KC_V,             KC_W,              KC_D,                 KC_J,  KC_B,            KC_H,         TO(3),         TD(DANCE_1),             KC_X,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_SPACE, OSM(MOD_LSFT),                                                         KC_RALT, TO(1)),
  [1] = LAYOUT(
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_ESC   ,     KC_1,             KC_2,               KC_3,                KC_4,  KC_5,          KC_6,           KC_7,             KC_8,              KC_BSPC,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  MT(MOD_LCTL,KC_TAB), KC_9,       KC_0,            KC_MINS, MT(MOD_LGUI,KC_EQL),  MT(MOD_RGUI,KC_LBRC), KC_RBRC, KC_SCLN,      KC_QUOT, MT(MOD_RCTL,KC_ENTER),
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_MEDIA_PLAY_PAUSE, KC_AUDIO_VOL_DOWN, KC_AUDIO_VOL_UP, OSL(2), OSM(MOD_LCTL),  KC_COMM,       KC_DOT,         TO(3),           KC_SLSH,            KC_BSLS,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  TO(0), OSM(MOD_LSFT),                                                            KC_RALT, KC_NO),
  [2] = LAYOUT(
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_TRNS,      KC_F1,             KC_F2,              KC_F3,              KC_F4,  KC_F5,         KC_F6,          KC_F7,            KC_F8,             KC_TRNS,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_TRNS,      KC_F9,             KC_F10,            KC_F11,             KC_F12,  KC_TRNS,      KC_TRNS,        KC_TRNS,          KC_TRNS,            KC_TRNS,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_TRNS,     KC_TRNS,            KC_TRNS,           KC_TRNS,           KC_TRNS,  KC_TRNS,      KC_TRNS,        KC_TRNS,          KC_TRNS,            KC_TRNS,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_TRNS, KC_TRNS,                                                                KC_TRNS, KC_TRNS),
  [3] = LAYOUT(
  KC_ESC,    KC_MS_WH_LEFT,       KC_MS_UP,       KC_MS_WH_RIGHT,        KC_PSCR,  S(LALT(KC_ESC)), LCTL(LSFT(KC_TAB)), RCTL(KC_TAB), LALT(KC_ESC),    KC_BSPC,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  MT(MOD_LCTL,KC_TAB), KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT,  MT(MOD_LGUI,KC_DEL),  TD(LEFT_HME),    KC_DOWN,       KC_UP, TD(RIGHT_END), MT(MOD_RCTL,KC_ENTER),     
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  KC_MEDIA_PLAY_PAUSE, KC_MS_WH_UP, KC_MS_WH_DOWN,  KC_MS_BTN2,    OSM(MOD_LCTL),  RCTL(KC_V),       KC_MS_BTN1,       KC_NO,    RCTL(KC_C),      KC_MS_ACCEL2,
  //-----------------------------------------------------------------------------  ----------------------------------------------------------------------------
  TO(0), OSM(MOD_LSFT),                                                            KC_RALT, TO(1)),
};





extern bool g_suspend_state;
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF
uint16_t last_keycode = KC_NO;
uint8_t last_modifier = 0;

typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

static tap dance_state = {
    .is_press_action = true,
    .step = 0
};

void on_dance_2(tap_dance_state_t *state, void *user_data);
uint8_t dance_2_dance_step(tap_dance_state_t *state);
void dance_2_finished(tap_dance_state_t *state, void *user_data);
void dance_2_reset(tap_dance_state_t *state, void *user_data);

void on_dance_2(tap_dance_state_t *state, void *user_data) {
	if(state->count == 3) {
		tap_code16(KC_COMM);
		tap_code16(KC_COMM);
		tap_code16(KC_COMM);
	}
	if(state->count > 3) {
		tap_code16(KC_COMM);
	}
}

uint8_t dance_2_dance_step(tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	return MORE_TAPS;
}
void dance_2_finished(tap_dance_state_t *state, void *user_data) {
	dance_state.step = dance_2_dance_step(state);
	switch (dance_state.step) {
		case SINGLE_TAP: register_code16(KC_COMM); break;
		case SINGLE_HOLD: register_code16(KC_QUOT); break;
		case DOUBLE_TAP: register_code16(KC_COMM); register_code16(KC_COMM); break;
		case DOUBLE_SINGLE_TAP: tap_code16(KC_COMM); register_code16(KC_COMM);
	}
}

void dance_2_reset(tap_dance_state_t *state, void *user_data) {
	wait_ms(10);
	switch (dance_state.step) {
		case SINGLE_TAP: unregister_code16(KC_COMM); break;
		case SINGLE_HOLD: unregister_code16(KC_QUOT); break;
		case DOUBLE_TAP: unregister_code16(KC_COMM); break;
		case DOUBLE_SINGLE_TAP: unregister_code16(KC_QUOT); break;
	}
	dance_state.step = 0;
}

void on_dance_1(tap_dance_state_t *state, void *user_data);
uint8_t dance_1_dance_step(tap_dance_state_t *state);
void dance_1_finished(tap_dance_state_t *state, void *user_data);
void dance_1_reset(tap_dance_state_t *state, void *user_data);

void on_dance_1(tap_dance_state_t *state, void *user_data) {
	if(state->count == 3) {
		tap_code16(KC_DOT);
		tap_code16(KC_DOT);
		tap_code16(KC_DOT);
	}
	if(state->count > 3) {
		tap_code16(KC_DOT);
	}
}

uint8_t dance_1_dance_step(tap_dance_state_t *state) {
	if (state->count == 1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		else return SINGLE_HOLD;
	} else if (state->count == 2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	return MORE_TAPS;
}
void dance_1_finished(tap_dance_state_t *state, void *user_data) {
	dance_state.step = dance_1_dance_step(state);
	switch (dance_state.step) {
		case SINGLE_TAP: register_code16(KC_DOT); break;
		case SINGLE_HOLD: register_code16(KC_QUES); break;
		case DOUBLE_TAP: register_code16(KC_DOT); register_code16(KC_DOT); break;
		case DOUBLE_SINGLE_TAP: tap_code16(KC_DOT); register_code16(KC_DOT);
	}
}

void dance_1_reset(tap_dance_state_t *state, void *user_data) {
	wait_ms(10);
	switch (dance_state.step) {
		case SINGLE_TAP: unregister_code16(KC_DOT); break;
		case SINGLE_HOLD: unregister_code16(KC_QUES); break;
		case DOUBLE_TAP: unregister_code16(KC_DOT); break;
		case DOUBLE_SINGLE_TAP: unregister_code16(KC_DOT); break;
	}
	dance_state.step = 0;
}

void on_left_hme(tap_dance_state_t *state, void *user_data);
uint8_t left_hme_dance_step(tap_dance_state_t *state);
void left_hme_finished(tap_dance_state_t *state, void *user_data);
void left_hme_reset(tap_dance_state_t *state, void *user_data);

void on_left_hme(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_LEFT);
        tap_code16(KC_LEFT);
        tap_code16(KC_LEFT);
    }
    if(state->count > 3) {
        tap_code16(KC_LEFT);
    }
}

uint8_t left_hme_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void left_hme_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = left_hme_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP: register_code16(KC_LEFT); break;
        case SINGLE_HOLD: register_code16(KC_HOME); break;
        case DOUBLE_TAP: register_code16(KC_LEFT); register_code16(KC_LEFT); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_LEFT); register_code16(KC_LEFT);
    }
}

void left_hme_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP: unregister_code16(KC_LEFT); break;
        case SINGLE_HOLD: unregister_code16(KC_HOME); break;
        case DOUBLE_TAP: unregister_code16(KC_LEFT); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_LEFT); break;
    }
    dance_state.step = 0;
}

void on_right_end(tap_dance_state_t *state, void *user_data);
uint8_t right_end_dance_step(tap_dance_state_t *state);
void right_end_finished(tap_dance_state_t *state, void *user_data);
void right_end_reset(tap_dance_state_t *state, void *user_data);

void on_right_end(tap_dance_state_t *state, void *user_data) {
    if(state->count == 3) {
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
        tap_code16(KC_RIGHT);
    }
    if(state->count > 3) {
        tap_code16(KC_RIGHT);
    }
}

uint8_t right_end_dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}
void right_end_finished(tap_dance_state_t *state, void *user_data) {
    dance_state.step = right_end_dance_step(state);
    switch (dance_state.step) {
        case SINGLE_TAP: register_code16(KC_RIGHT); break;
        case SINGLE_HOLD: register_code16(KC_END); break;
        case DOUBLE_TAP: register_code16(KC_RIGHT); register_code16(KC_RIGHT); break;
        case DOUBLE_SINGLE_TAP: tap_code16(KC_RIGHT); register_code16(KC_RIGHT);
    }
}

void right_end_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state.step) {
        case SINGLE_TAP: unregister_code16(KC_RIGHT); break;
        case SINGLE_HOLD: unregister_code16(KC_END); break;
        case DOUBLE_TAP: unregister_code16(KC_RIGHT); break;
        case DOUBLE_SINGLE_TAP: unregister_code16(KC_RIGHT); break;
    }
    dance_state.step = 0;
}

tap_dance_action_t tap_dance_actions[] = {
        [DANCE_1] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_1, dance_1_finished, dance_1_reset),
        [DANCE_2] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_2, dance_2_finished, dance_2_reset),
        [LEFT_HME] = ACTION_TAP_DANCE_FN_ADVANCED(on_left_hme, left_hme_finished, left_hme_reset),
        [RIGHT_END] = ACTION_TAP_DANCE_FN_ADVANCED(on_right_end, right_end_finished, right_end_reset),
};
