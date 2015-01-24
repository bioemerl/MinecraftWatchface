#include <pebble.h>
#include <stdio.h>
//icon should change every 120 mins
//noon is at 1440 mins, midnight is twice that
//clock will start with 0 at noon, and move up from there, one image every 120 mins
static GBitmap *clockbitmap;
static BitmapLayer *watchfacelayer;
  
static Window *my_window;
static TextLayer *text_layer;


void allocateclockbitmap(int i){
   APP_LOG(APP_LOG_LEVEL_INFO, "I is: %i", i);
  //if(clockbitmap != NULL)
  gbitmap_destroy(clockbitmap);
  if(i == 0)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_0_WHITE);
  if(i == 1)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_1_WHITE);
  if(i == 2)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_2_WHITE);
  if(i == 3)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_3_WHITE);
  if(i == 4)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_4_WHITE);
  if(i == 5)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_5_WHITE);
  if(i == 6)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_6_WHITE);
  if(i == 7)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_7_WHITE);
  if(i == 8)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_8_WHITE);
  if(i == 9)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_9_WHITE);
  if(i == 10)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_10_WHITE);
  if(i == 11)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_11_WHITE);
  if(i == 12)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_12_WHITE);
  if(i == 13)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_13_WHITE);
  if(i == 14)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_14_WHITE);
  if(i == 15)
    clockbitmap = gbitmap_create_with_resource(RESOURCE_ID_WATCHCYCLE_15_WHITE);
}

static void updatewatchimage(){
  //get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static int hour = 0;
  static int minuite = 0;
  static int clockpositionnumber = 0;
  int totaltime = 0;
  
  hour = tick_time->tm_sec;
  minuite = tick_time->tm_min;

  
  APP_LOG(APP_LOG_LEVEL_INFO, "Hour, %i", hour);
  APP_LOG(APP_LOG_LEVEL_INFO, "minuite, %i", minuite);
  totaltime = minuite + (hour * 60);
  APP_LOG(APP_LOG_LEVEL_INFO, "totaltime, %i", totaltime);
  totaltime = totaltime / 96;
  APP_LOG(APP_LOG_LEVEL_INFO, "totaltime/96, %i", totaltime);
  clockpositionnumber = ((totaltime + 8)%15);
  APP_LOG(APP_LOG_LEVEL_INFO, "position, %i", clockpositionnumber);
    
  allocateclockbitmap(clockpositionnumber);
  bitmap_layer_set_bitmap(watchfacelayer, clockbitmap);
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  APP_LOG(APP_LOG_LEVEL_INFO, "RanTickHandler");
  updatewatchimage();
}

static void main_window_load(Window *window){
  //get the "layer" then get the bounds of that layer and store them in a GRect?
  Layer *window_layer = window_get_root_layer(window);
  //GRect window_bounds = layer_get_bounds(window_layer);
  
  //create a gbitmap from image
  //here would be "if time, use this image!"
  //I hope
  //this is bad practice, but no idea how to set RESOURCE_ID_ETC to change with a variable
    
  // Create BitmapLayer and show the GBitmap
  watchfacelayer = bitmap_layer_create(GRect(16, 20, 112, 128));
  layer_add_child(window_layer, bitmap_layer_get_layer(watchfacelayer));
  updatewatchimage();
}

static void main_window_unload(Window *window){
  gbitmap_destroy(clockbitmap);
  bitmap_layer_destroy(watchfacelayer);
}

void handle_init(void) {
  my_window = window_create();
  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  
  //subscribe a ticktimerservice
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  
  window_stack_push(my_window, true);
  
  
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop(); //regular function, not defined by me
  handle_deinit();
}