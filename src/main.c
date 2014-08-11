#include <pebble.h>
#include <stdio.h>
  
static Window *main_window;
static TextLayer *time_layer;
static GFont time_font;

static void update_time(){
  static char buffer[11];
  
  snprintf(buffer, 11, "%d", (int)time(NULL));
  
  text_layer_set_text(time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
}

static void main_window_load(Window *window){
  time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_23));
  
  time_layer = text_layer_create(GRect(5, 52, 139, 50));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_text(time_layer, "00:00");
  
  text_layer_set_font(time_layer, time_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  update_time();
}

static void main_window_unload(Window *window){
  text_layer_destroy(time_layer);
  fonts_unload_custom_font(time_font);
}

static void init(){
  main_window = window_create();
  
  window_set_window_handlers(main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(main_window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(){
  window_destroy(main_window);
}

int main(){
  init();
  app_event_loop();
  deinit();
}