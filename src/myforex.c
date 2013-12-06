#include <pebble.h>

static Window *window;

static TextLayer *result_txt_layer;

static void send_msg(void) {
	Tuplet value = TupletCString(0, "");

	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);

	if (iter == NULL) {
		return;
	}

  	dict_write_tuplet(iter, &value);
 	dict_write_end(iter);
	
	app_message_outbox_send();
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
	char txt[100];

	strcat(txt, "");

	Tuple *date_tuple = dict_find(iter, 0);
	
	if(date_tuple) {
		txt = text_layer_get_text(result_txt_layer);

		text_layer_set_text(result_txt_layer, strcat(strcat(txt, date_tuple->value->cstring), "\n"));
	}

	Tuple *currency_tuple = dict_find(iter, 1);
	
	if(currency_tuple) {
		txt = text_layer_get_text(result_txt_layer);

		text_layer_set_text(result_txt_layer, strcat(strcat(txt, currency_tuple->value->cstring), "\n"));
	}

	Tuple *buy_rate_tuple = dict_find(iter, 2); 
	
	if(buy_rate_tuple) {
		txt = text_layer_get_text(result_txt_layer);

		text_layer_set_text(result_txt_layer, strcat(strcat(txt, buy_rate_tuple->value->cstring), "\n"));
	}

	Tuple *sell_rate_tuple = dict_find(iter, 3); 
	
	if(sell_rate_tuple) {
		txt = text_layer_get_text(result_txt_layer);

		text_layer_set_text(result_txt_layer, strcat(strcat(txt, sell_rate_tuple->value->cstring), "\n"));
	}
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	text_layer_set_text(result_txt_layer, "Refreshing...");

	send_msg();
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	
	result_txt_layer = text_layer_create(bounds);

	text_layer_set_text_alignment(result_txt_layer, GTextAlignmentCenter);
	text_layer_set_background_color(result_txt_layer, GColorBlack);
	text_layer_set_text_color(result_txt_layer, GColorWhite);
  	text_layer_set_font(result_txt_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  	
  	text_layer_set_text(result_txt_layer, "Initialised!");

  	/*
  	GSize result_txt_layer_size = text_layer_get_content_size(result_txt_layer); 

  	layer_set_frame	(
						 text_layer_get_layer(result_txt_layer),
						(GRect) { 
								.origin = {(bounds.size.w-result_txt_layer_size.w)/2, (bounds.size.h-result_txt_layer_size.h)/2}, 
								.size = result_txt_layer_size
							}
					);
	*/

  	layer_add_child(window_layer, text_layer_get_layer(result_txt_layer));
}

static void window_unload(Window *window) {
	text_layer_destroy(result_txt_layer);
}

static void deinit(void) {
	window_destroy(window);
}

static void app_message_init(void) {
	app_message_register_inbox_received(in_received_handler);
  	app_message_register_inbox_dropped(in_dropped_handler);
  	app_message_register_outbox_failed(out_failed_handler);

	app_message_open(128, 128);
}

static void init(void) {
	window = window_create();

	window_set_window_handlers	(
									window, 
									(WindowHandlers) {
    									.load = window_load,
    									.unload = window_unload
  									}
								);

	window_set_click_config_provider(window, click_config_provider);

	window_stack_push(window, true);

	app_message_init();
}

int main(void) {
	init();

	app_event_loop();
	
	deinit();
}