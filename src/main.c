#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x28, 0xD7, 0x91, 0x5F, 0xC9, 0x0E, 0x4B, 0x1D, 0x83, 0x70, 0xDB, 0x0D, 0xAA, 0x23, 0x2E, 0x2D }
PBL_APP_INFO(MY_UUID,
             "Tough Guy Quotes", "Alex Key",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer toughText_layer;

char a[4][1000];


static void handle_minute_tick(AppContextRef app_ctx, PebbleTickEvent* e) {
 	text_layer_set_text(&toughText_layer, a[rand() % 3]);
}



void handle_init(AppContextRef ctx) {
	(void)ctx;

strcpy(a[0], "Alex Key doesn't look both ways before he crosses the street... he just roundhouses any cars that get too close.");
strcpy(a[1], "Alex Key's brain waves are suspected to be harmful to cell phones.");
strcpy(a[2], "Alex Key's database has only one table, 'Kick', which he DROPs frequently.");
strcpy(a[2], "The opening scene of the movie 'Saving Private Ryan' is loosely based on games of dodgeball Alex Key played in second grade.");



	window_init(&window, "Tough Guy");
	window_stack_push(&window, true /* Animated */);
	
	text_layer_init(&toughText_layer, GRect(0, 0, 144, 168));
	text_layer_set_text_alignment(&toughText_layer, GTextAlignmentCenter);
	text_layer_set_text(&toughText_layer, a[rand() % 3]);
	text_layer_set_font(&toughText_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	layer_add_child(&window.layer, &toughText_layer.layer);
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