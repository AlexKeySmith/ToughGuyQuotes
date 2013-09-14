#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x28, 0xD7, 0x91, 0x5F, 0xC9, 0x0E, 0x4B, 0x1D, 0x83, 0x70, 0xDB, 0x0D, 0xAA, 0x23, 0x2E, 0x2D }
PBL_APP_INFO(MY_UUID,
             "Tough Guy Quotes", "Alex Key",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer toughText_layer;


void handle_init(AppContextRef ctx) {
	(void)ctx;

	window_init(&window, "Tough Guy");
	window_stack_push(&window, true /* Animated */);
	
	text_layer_init(&toughText_layer, GRect(0, 65, 144, 30));
	text_layer_set_text_alignment(&toughText_layer, GTextAlignmentCenter);
	text_layer_set_text(&toughText_layer, "Alex Key's brain waves are suspected to be harmful to cell phones.");
	text_layer_set_font(&toughText_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	layer_add_child(&window.layer, &toughText_layer.layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
