#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
	static char buffer[60];
	char hour[10];
	char nearly[15];
	char prefix[20];
	char suffix[15] = "\0";
	char hh12[2];
	char hh24[2];
	char mm[2];
	int h12;
	int h24;
	int mins;
	int modmin;

	strftime(hh12, sizeof("00"), "%I", tick_time);
	strftime(hh24, sizeof("00"), "%H", tick_time);
	strftime(mm, sizeof("00"), "%M", tick_time);
	
	h12 = atoi(hh12);
	h24 = atoi(hh24);
	mins = atoi(mm);
	modmin = mins % 5;
	
	if ((mins > 31) && (mins < 61)) {
		h12++;
		h24++;
		if (h12 == 13){
			h12 = 1;
		}
		if (h24 == 25){
			h24 = 1;
		}
	}
	if (h24 == 24){
		h24 = 0;
	}

	if ((modmin > 1) && (modmin < 4)){
		strcpy(nearly, "nearly\n");
	}
	else{
		strcpy(nearly, "\n");
	}

	if ((((mins > 56) && (mins < 61))) || (mins == 0) || (mins == 1)) {
		strcpy(prefix, "\n");
		strcpy(suffix, "o\' clock\0");
	}
	if ((mins > 1) && (mins < 7)) {
		strcpy(prefix, "five\npast\n");
	}
	if ((mins > 6) && (mins < 12)){
		strcpy(prefix, "ten\npast\n");
	}
	if ((mins > 11) && (mins < 17)){
		strcpy(prefix, "quarter\npast\n");
	}
	if ((mins > 16) && (mins < 22)){
		strcpy(prefix, "twenty\npast\n");
	}
	if ((mins > 21) && (mins < 27)){
		strcpy(prefix, "twenty\nfive past\n");
//		if ((modmin > 1) && (modmin < 4)){
//			strcpy(nearly, "\n");
//			strcpy(suffix, " -ish\0");
//		}
	}
	if ((mins > 26) && (mins < 32)){
		strcpy(prefix, "half\npast\n");
	}
	if ((mins > 31) && (mins < 37)){
		strcpy(prefix, "twenty\nfive to\n");
	}
	if ((mins > 36) && (mins < 42)){
		strcpy(prefix, "twenty\nto\n");
	}
	if ((mins > 41) && (mins < 47)){
		strcpy(prefix, "quarter\nto\n");
	}
	if ((mins > 46) && (mins < 52)){
		strcpy(prefix, "ten\nto\n");
	}
	if ((mins > 51) && (mins < 57)){
		strcpy(prefix, "five\nto\n");
	}
	
	if (h12 == 1) {
		strcpy(hour, "one\n");
	}
	if (h12 == 2) {
		strcpy(hour, "two\n");
	}
	if (h12 == 3) {
		strcpy(hour, "three\n");
	}
	if (h12 == 4) {
		strcpy(hour, "four\n");
	}
	if (h12 == 5) {
		strcpy(hour, "five\n");
	}
	if (h12 == 6) {
		strcpy(hour, "six\n");
	}
	if (h12 == 7) {
		strcpy(hour, "seven\n");
	}
	if (h12 == 8) {
		strcpy(hour, "eight\n");
	}
	if (h12 == 9) {
		strcpy(hour, "nine\n");
	}
	if (h12 == 10) {
		strcpy(hour, "ten\n");
	}
	if (h12 == 11) {
		strcpy(hour, "eleven\n");
	}
	//if (h12 == 12) {
	//	strcpy(hour, "twelve");
	//}

	
	if (h24 == 12) {
		strcpy(hour, "midday\n");
		strcpy(suffix, "\0");
	}
	if (h24 == 0) {
		strcpy(hour, "midnight\n");
		strcpy(suffix, "\0");
	}
	
	strcpy(buffer, "");
	strcat(buffer, nearly);
	strcat(buffer, prefix);
	strcat(buffer, hour);
	strcat(buffer, suffix);

//  // Write the current hours and minutes into the buffer
//  if(clock_is_24h_style() == true) {
//    // Use 24 hour format
//    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
//  } else {
//    // Use 12 hour format
//    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
//  }
	
	// Display this time on the TextLayer
	
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, -6, 144, 177));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_text(s_time_layer, "");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  //text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	//static GFont boldFont;
	//boldFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_GOTHAM_BOLD_36));
	//text_layer_set_font(s_time_layer, boldFont);
	
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
}

static void init() {
	
	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}