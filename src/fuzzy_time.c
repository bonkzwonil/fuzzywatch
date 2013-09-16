#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "num2words.h"

//BONK hat: 23 42 AF FE
#define MY_UUID { 0x23, 0x42, 0xAF, 0xFE, 0x15, 0x2B, 0x46, 0x6F, 0xAF, 0xFE, 0xCA, 0xFE, 0xD9, 0xCB, 0xAA, 0x12 }
PBL_APP_INFO(MY_UUID,
             "Fuzzy Menzel",
             "MenzelMedia",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

#define BUFFER_SIZE 35  //Reicht aus: siehe find_max_buf()

static struct CommonWordsData {
  TextLayer label;
  TextLayer datelabel;
  Window window;
  char buffer[BUFFER_SIZE];
  char datebuffer[BUFFER_SIZE];
} s_data;

GSize gsize;

static void update_time(PblTm* t) {
  
  fuzzy_time_to_words(t->tm_hour, t->tm_min, s_data.buffer, BUFFER_SIZE);
  date_to_words(t->tm_mday, t->tm_mon -1, t->tm_year + 1900, s_data.datebuffer, BUFFER_SIZE);
  /*
  GContext* ctx =  app_get_current_graphics_context 	()		;
  gsize = text_layer_get_max_used_size(ctx, &s_data.label);
  int h = s_data.window.layer.frame.size.h - gsize.h;
  int w = s_data.window.layer.frame.size.w - gsize.w;

  text_layer_set_size(&s_data.label, GRect(0, h/2, s_data.window.layer.frame.size.w, gsize.h));
  */
  text_layer_set_text(&s_data.label, s_data.buffer);
  text_layer_set_text(&s_data.datelabel, s_data.datebuffer);
}

static void handle_minute_tick(AppContextRef app_ctx, PebbleTickEvent* e) {
  update_time(e->tick_time);
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  text_layer_set_text(&s_data.label, "Hallo");
}

void config_provider(ClickConfig **config, Window *window) {
  // single click / repeat-on-hold config:
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
  config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 1000; // "hold-to-repeat" gets overridden if there's a long click handler configured!

}




static void handle_init(AppContextRef ctx) {

  window_init(&s_data.window, "Menzels Fuzzy Time");
  const bool animated = true;
  window_stack_push(&s_data.window, animated);

  window_set_background_color(&s_data.window, GColorBlack);
  GFont font = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);

  window_set_click_config_provider(&s_data.window, (ClickConfigProvider) config_provider);

  text_layer_init(&s_data.label, GRect(0, 40, s_data.window.layer.frame.size.w, s_data.window.layer.frame.size.h - 48));
  text_layer_set_background_color(&s_data.label, GColorBlack);
  text_layer_set_text_color(&s_data.label, GColorWhite);
  text_layer_set_font(&s_data.label, font);
  layer_add_child(&s_data.window.layer, &s_data.label.layer);

  GFont font2 = fonts_get_system_font( FONT_KEY_GOTHIC_14);
  text_layer_init(&s_data.datelabel, GRect(0, s_data.window.layer.frame.size.h - 28, s_data.window.layer.frame.size.w, 28));
  text_layer_set_background_color(&s_data.datelabel, GColorBlack);
  text_layer_set_text_color(&s_data.datelabel, GColorWhite);
  text_layer_set_font(&s_data.datelabel, font2);
  text_layer_set_text_alignment(&s_data.datelabel, GTextAlignmentCenter);
  layer_add_child(&s_data.window.layer, &s_data.datelabel.layer);
  

  PblTm t;
  get_time(&t);
  update_time(&t);
}




void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
