#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
	static char buffer[50];
	char hour[10];
	char nearly[10];
	char prefix[20];
	char suffix[10];
	char hh12[2];
	char hh24[2];
	char mm[2];
	char finaltext[50];
	int h12;
	int h24;
	int mins;
	int modmin = 0;

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
			h24 = 1;
		}
	}

	if ((modmin > 1) && (modmin < 4)){
		strcpy(nearly, "nearly ");
	}
	else{
		strcpy(nearly, "");
	}

	if ((((mins > 56) && (mins < 61))) || (mins == 0) || (mins == 1)) {
		strcpy(suffix, " o\' clock");
	}
	if ((mins > 1) && (mins < 7)) {
		strcpy(prefix, "five past ");
	}
	if ((mins > 6) && (mins < 12)){
		strcpy(prefix, "ten past ");
	}
	if ((mins > 11) && (mins < 17)){
		strcpy(prefix, "quarter past ");
	}
	if ((mins > 16) && (mins < 22)){
		strcpy(prefix, "twenty past ");
	}
	if ((mins > 21) && (mins < 27)){
		strcpy(prefix, "twenty five past ");
	}
	if ((mins > 26) && (mins < 32)){
		strcpy(prefix, "half past ");
	}
	if ((mins > 31) && (mins < 37)){
		strcpy(prefix, "twenty five to ");
	}
	if ((mins > 36) && (mins < 42)){
		strcpy(prefix, "twenty to ");
	}
	if ((mins > 41) && (mins < 47)){
		strcpy(prefix, "quarter to ");
	}
	if ((mins > 46) && (mins < 52)){
		strcpy(prefix, "ten to ");
	}
	if ((mins > 51) && (mins < 57)){
		strcpy(prefix, "five to ");
	}
	
	if (h12 == 1) {
		strcpy(hour, "one");
	}
	if (h12 == 2) {
		strcpy(hour, "two");
	}
	if (h12 == 3) {
		strcpy(hour, "three");
	}
	if (h12 == 4) {
		strcpy(hour, "four");
	}
	if (h12 == 5) {
		strcpy(hour, "five");
	}
	if (h12 == 6) {
		strcpy(hour, "six");
	}
	if (h12 == 7) {
		strcpy(hour, "seven");
	}
	if (h12 == 8) {
		strcpy(hour, "eight");
	}
	if (h12 == 9) {
		strcpy(hour, "nine");
	}
	if (h12 == 10) {
		strcpy(hour, "ten");
	}
	if (h12 == 11) {
		strcpy(hour, "eleven");
	}
	if (h24 == 12) {
		strcpy(hour, "midday");
		strcpy(suffix, "");
	}
	if (h24 == 0) {
		strcpy(hour, "midnight");
		if ((modmin >= 2) && (modmin <= 3)){
			strcpy(nearly, "");
			strcpy(suffix, " -ish");
		}
		else{
			strcpy(suffix, "");
		}

	}
	
	
	
	strcpy(finaltext, "");
	strcat(finaltext, nearly);
	//strcat(buffer, nearly);
	strcat(finaltext, prefix);
	strcat(finaltext, hour);
	strcat(finaltext, suffix);
	strcpy(buffer, "");
	strcat(buffer, finaltext);

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
  s_time_layer = text_layer_create(GRect(0, -9, 144, 177));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_text(s_time_layer, "...");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  //text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

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