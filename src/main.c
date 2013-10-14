#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"
 
/* If compiling this for iOS, set ANDROID to be false. */
#define ANDROID false
 
#if ANDROID
#define MY_UUID { 0x91, 0x41, 0xB6, 0x28, 0xBC, 0x89, 0x49, 0x8E, 0xB1, 0x47, 0x10, 0x34, 0xBF, 0xBE, 0x12, 0x98 }
#else
#define MY_UUID HTTP_UUID
#endif

#define HTTP_COOKIE 4887
 


void http_success(int32_t request_id, int http_status, DictionaryIterator* received, void* context);
void http_failure(int32_t request_id, int http_status, void* context);
void httpebble_error(int error_code);
void request_quotes();

PBL_APP_INFO(MY_UUID,
             "Tough Guy Quotes", "Alex Key",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
TextLayer toughText_layer;



static void handle_minute_tick(AppContextRef app_ctx, PebbleTickEvent* e) {
 	
}


void http_success(int32_t request_id, int http_status, DictionaryIterator* received, void* context) {
  if (request_id != HTTP_COOKIE) {
    return;
  }
 
  //Tuple* tuple1 = dict_find(received, 0);
  //text_layer_set_text(&toughText_layer, tuple1->value->cstring);
 
  Tuple* tuple = dict_read_next(received);

	if(tuple) {
		  text_layer_set_text(&toughText_layer, tuple->value->cstring);
	}
	
}
 
void http_failure(int32_t request_id, int http_status, void* context) {
  httpebble_error(http_status >= 1000 ? http_status - 1000 : http_status);
}


void window_appear(Window* me) {
 

}



void handle_init(AppContextRef ctx) {
	(void)ctx;

	http_set_app_id(76782702);
	
	http_register_callbacks((HTTPCallbacks) {
		.success = http_success,
		.failure = http_failure
	}, NULL);




	window_init(&window, "Tough Guy");
	window_stack_push(&window, true /* Animated */);
	
	window_set_window_handlers(&window, (WindowHandlers){
		.appear  = window_appear
	});
	
	text_layer_init(&toughText_layer, GRect(0, 0, 144, 168));
	text_layer_set_text_alignment(&toughText_layer, GTextAlignmentCenter);

	text_layer_set_font(&toughText_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	layer_add_child(&window.layer, &toughText_layer.layer);
}



void request_quotes() {

	DictionaryIterator *body;
	HTTPResult result = http_out_get("http://toughguyquotes.azurewebsites.net/", HTTP_COOKIE, &body);
	if(result != HTTP_OK) {

		return;
	}

	// Send it.
	if(http_out_send() != HTTP_OK) {
		return;
	}
}



void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    },
    .messaging_info = {
      .buffer_sizes = {
        .inbound = 124,
        .outbound = 256
      }
}
  };
  app_event_loop(params, &handlers);
}

 
void httpebble_error(int error_code) {
 
  static char error_message[] = "UNKNOWN_HTTP_ERRROR_CODE_GENERATED";
 
  switch (error_code) {
    case HTTP_SEND_TIMEOUT:
      strcpy(error_message, "HTTP_SEND_TIMEOUT");
    break;
    case HTTP_SEND_REJECTED:
      strcpy(error_message, "HTTP_SEND_REJECTED");
    break;
    case HTTP_NOT_CONNECTED:
      strcpy(error_message, "HTTP_NOT_CONNECTED");
    break;
    case HTTP_BRIDGE_NOT_RUNNING:
      strcpy(error_message, "HTTP_BRIDGE_NOT_RUNNING");
    break;
    case HTTP_INVALID_ARGS:
      strcpy(error_message, "HTTP_INVALID_ARGS");
    break;
    case HTTP_BUSY:
      strcpy(error_message, "HTTP_BUSY");
    break;
    case HTTP_BUFFER_OVERFLOW:
      strcpy(error_message, "HTTP_BUFFER_OVERFLOW");
    break;
    case HTTP_ALREADY_RELEASED:
      strcpy(error_message, "HTTP_ALREADY_RELEASED");
    break;
    case HTTP_CALLBACK_ALREADY_REGISTERED:
      strcpy(error_message, "HTTP_CALLBACK_ALREADY_REGISTERED");
    break;
    case HTTP_CALLBACK_NOT_REGISTERED:
      strcpy(error_message, "HTTP_CALLBACK_NOT_REGISTERED");
    break;
    case HTTP_NOT_ENOUGH_STORAGE:
      strcpy(error_message, "HTTP_NOT_ENOUGH_STORAGE");
    break;
    case HTTP_INVALID_DICT_ARGS:
      strcpy(error_message, "HTTP_INVALID_DICT_ARGS");
    break;
    case HTTP_INTERNAL_INCONSISTENCY:
      strcpy(error_message, "HTTP_INTERNAL_INCONSISTENCY");
    break;
    case HTTP_INVALID_BRIDGE_RESPONSE:
      strcpy(error_message, "HTTP_INVALID_BRIDGE_RESPONSE");
    break;
    default: {
      strcpy(error_message, "HTTP_ERROR_UNKNOWN");
    }
  }
 
  text_layer_set_text(&toughText_layer, error_message);
}

