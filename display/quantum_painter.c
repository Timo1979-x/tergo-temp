#include <qp.h>
#include "common.h"
#include "display/computer.qff.h"
#include "display/logo.qgf.h"


// Длительность показа лого в миллисекундах
#define SHOW_LOGO_DURATION 3000

#pragma message "SHOW_LOGO_DURATION " DESCRIBE_MACRO_VALUE(SHOW_LOGO_DURATION)

static painter_font_handle_t display_font;
static painter_image_handle_t logo;
static painter_device_t display;
static uint32_t display_logo_timer = 0;
static deferred_token logo_animation_token = 0;

extern char* get_current_layer_name(void); // функция должна быть определена в keymap.c
extern char os_layout[];

void display_on(bool on) {
    qp_power(display, on);
}

void init_logo_timer(void) {
  display_logo_timer = timer_read32();
  if(display_logo_timer == 0) {
    display_logo_timer++;
  }
  // показать картинку
  logo_animation_token = qp_animate(display, 0, 0, logo);
}  

void keyboard_post_init_user_display(void) {
  display = qp_sh1106_make_i2c_device(128, 32, 0x3C /* или 0x78 или 0x7A */);
  // lock_scrl_off = qp_load_image_mem(gfx_lock_scrl_OFF);
  display_font = qp_load_font_mem(font_quantum_computer_font);
  logo = qp_load_image_mem(gfx_logo);
  qp_init(display, QP_ROTATION_90);
  qp_power(display, true);
  init_logo_timer();
}

void start_display_logo(void) {
  char buf[200];
  sprintf(buf, "display: %p, display_font %p, gfx_logo: %p, gfx_logo_length: %ld, logo: %p\n", 
    display, display_font, gfx_logo, gfx_logo_length, logo);
  dprint(buf);
  init_logo_timer();
}

void ui_task(void) {
  // display_logo_timer != 0, то происходит показ логотипа
  if(display_logo_timer) {
    if (timer_elapsed32(display_logo_timer) <= SHOW_LOGO_DURATION) {
      return;
    } else {
      // отключить показ логотипа:
      display_logo_timer = 0;
      qp_stop_animation(logo_animation_token);
      logo_animation_token = 0;
      qp_clear(display);
    }
  }

  static uint32_t last_draw_time = 0;
  static uint8_t last_led_bits = 0;
  if (timer_elapsed32(last_draw_time) <= 100) { // Throttle to 10fps
    return;
  }
  last_draw_time = timer_read32();
  led_t led_state = host_keyboard_led_state();
  uint8_t led_bits = 
    (led_state.caps_lock ? 4 : 0) |
    (led_state.num_lock ? 2 : 0) |
    (led_state.scroll_lock ? 1 : 0)
  ;
  if(last_led_bits != led_bits) {
    qp_power(display, true);
    last_led_bits = led_bits;
  }
  char leds[] = { ' ', ' ', 0 };
  if(led_bits & 4) {
    leds[0] = 'C';
  }
  if(led_bits & 2) {
    leds[1] = 'N';
  }
  qp_drawtext(display, 0, 0, display_font, os_layout);
  qp_drawtext(display, 0, 25, display_font, leds);
  qp_drawtext(display, 0, 50, display_font, get_current_layer_name());
}