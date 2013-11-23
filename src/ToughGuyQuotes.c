#include <pebble.h>
#include <stdbool.h>

Window *window;
TextLayer *text_date_layer;
TextLayer *text_time_layer;
Layer *line_layer;
bool subscribeTicksWhenGetFocus;



const uint32_t inbound_size = 64;
const uint32_t outbound_size = 64;

void line_layer_update_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
 	//text_layer_set_text(text_layer, "loading..");

  	DictionaryIterator *iter;
   	app_message_outbox_begin(&iter);
   	Tuplet value = TupletInteger(1, 42);
   	dict_write_tuplet(iter, &value);
   	app_message_outbox_send();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}



void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxxx 00";

  char *time_format;


  // TODO: Only update the date when it's changed.
  strftime(date_text, sizeof(date_text), "%a %d %b %y", tick_time);
  text_layer_set_text(text_date_layer, date_text);


  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_time_layer, time_text);
}




static void window_load(Window *window) {
  

}

static void window_unload(Window *window) {
  text_layer_destroy(text_date_layer);
  text_layer_destroy(text_time_layer);
  layer_destroy(line_layer);  
}


static void focusHandler(bool in_focus) {
	if(subscribeTicksWhenGetFocus)
	{
		tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
		subscribeTicksWhenGetFocus = false;
	}
	text_layer_set_text_alignment(text_date_layer, GTextAlignmentCenter);
}

void out_sent_handler(DictionaryIterator *sent, void *context) {
  // outgoing message was delivered
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  // outgoing message failed
}


void in_received_handler(DictionaryIterator *received, void *context) {
  // incoming message received
}


void in_dropped_handler(AppMessageResult reason, void *context) {
  // incoming message dropped
}


static void init(void) {
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
    const bool animated = true;
	window_set_fullscreen(window, true);
    window_stack_push(window, animated);
	
	app_message_open(inbound_size, outbound_size);
 
    app_message_register_inbox_received(in_received_handler);
    app_message_register_inbox_dropped(in_dropped_handler);
    app_message_register_outbox_sent(out_sent_handler);
    app_message_register_outbox_failed(out_failed_handler);
  
    app_focus_service_subscribe(focusHandler);

    
  
  window_set_background_color(window, GColorBlack);

  Layer *window_layer = window_get_root_layer(window);

  text_date_layer = text_layer_create(GRect(0, 60, 144, 168-60));
  text_layer_set_text_color(text_date_layer, GColorWhite);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_STARDOSSTENCIL_REGULAR_20)));
  text_layer_set_text_alignment(text_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  text_time_layer = text_layer_create(GRect(0, 88, 144, 168-88));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_STARDOSSTENCIL_BOLD_47)));
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  GRect line_frame = GRect(2, 90, 144-2, 2);
  line_layer = layer_create(line_frame);
  layer_set_update_proc(line_layer, line_layer_update_callback);
  layer_add_child(window_layer, line_layer);

	text_layer_set_text(text_date_layer, "load quote > ");
	subscribeTicksWhenGetFocus = true;
	text_layer_set_text_alignment(text_date_layer, GTextAlignmentRight);
	//tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
	//TODO: Update display here to avoid blank display on launch?
}



static void deinit(void) {
	tick_timer_service_unsubscribe();
	text_layer_destroy(text_date_layer);
	text_layer_destroy(text_time_layer);
  	window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
