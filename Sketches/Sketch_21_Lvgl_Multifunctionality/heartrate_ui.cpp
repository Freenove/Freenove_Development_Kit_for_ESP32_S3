#include "heartrate_ui.h"
#include "main_ui.h"
#include "lv_img.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 heartrate;                  //apply a module object
static lv_chart_series_t *series;   //apply an lvgl chart variable
lvgl_heartrate_ui guider_heartrate_ui;//heartrate ui structure
int heartrate_task_flag = 0;         //heartrate thread running flag
TaskHandle_t heartrateTaskHandle;    //heartrate thread task handle

const byte RATE_SIZE = 4;           //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];              //Array of heart rates
byte rateSpot = 0;                  //Record the number of bits in the heart rate array
long lastBeat = 0;                  //Time at which the last beat occurred
float beatsPerMinute;               //store the heartbeat value per minute
int beatAvg = 0;                    //Heart rate average
int beatAvgLast = 0;                //Average heart rate from last time
byte irvalueSpot = 0;               //Record the number of bits in the heartrate irvalue array
const byte AVERAGE_NUM = 10;        //Increase this for more averaging. 10 is good.
long heartrate_irvalue[AVERAGE_NUM]; //Array of heartrate irvalue
long show_value = 0;                //The data values of the points in the heart rate line plot

//Initialization heartrate module
int heartrate_init(void) {
  if (!heartrate.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    return -1;
  }
  heartrate.setup();  //Configure sensor with these settings
  heartrate_shutdown();
}

//The heartrate module is enabled to enter low power mode
void heartrate_shutdown(void) {
  heartrate.shutDown();
}

//Wake up the heartrate module
void heartrate_wake_up(void) {
  heartrate.wakeUp();
}

//Calculate the average value of the array
long heartrate_average(long array[], int num) {
  long average = 0;
  for (int i = 0; i < num; i++) {
    average += array[i];
  }
  average = average / num;
  return average;
}

//heartrate task thread
void loopTask_heartrate(void *pvParameters) {
  Serial.println("loopTask_heartrate start...");

  while (heartrate_task_flag) {
    long irValue = heartrate.getIR();  //Get the raw data
    if (irValue < 50000) {
      delay(100);
    }
    else {
      if (checkForBeat(irValue) == true) {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();
        beatsPerMinute = 60.0 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 50) {
          rates[rateSpot++] = (byte)beatsPerMinute;  //Store this reading in the array
          rateSpot %= RATE_SIZE;                     //Wrap variable
          //Take average of readings
          beatAvg = 0;
          for (byte x = 0; x < RATE_SIZE; x++)
            beatAvg += rates[x];
          beatAvg /= RATE_SIZE;
        }
        if (beatAvg != beatAvgLast) {
          beatAvgLast = beatAvg;
          lv_label_set_text_fmt(guider_heartrate_ui.heartrate_label, "HeartRate: %d",beatAvg);
        }
      }
      heartrate_irvalue[irvalueSpot++] = irValue;  //Put the raw data into an array
      irvalueSpot %= AVERAGE_NUM;
      long average = heartrate_average(heartrate_irvalue, AVERAGE_NUM);
      show_value = irValue - average + (CHART_HIGH_LIMIT / 2);

#if HEARTRATE_SERIAL
      Serial.printf("%ld %ld %ld\r\n", irValue, average, show_value);
#endif
      if (irValue > 50000 && show_value > CHART_LOW_LIMIT && show_value < CHART_HIGH_LIMIT)
        lv_chart_set_next_value(guider_heartrate_ui.heartrate_chart, series, show_value);
    }
  }
  heartrate_shutdown();
  vTaskDelete(heartrateTaskHandle);
}

//Create heartrate task thread
void create_heartrate_task(void) {
  if (heartrate_task_flag == 0) {
    heartrate_task_flag = 1;
    xTaskCreate(loopTask_heartrate, "loopTask_heartrate", 8192, NULL, 1, &heartrateTaskHandle);
  } else {
    Serial.println("loopTask_heartrate is running...");
  }
}

//Close the heartrate thread
void stop_heartrate_task(void) {
  if (heartrate_task_flag == 1) {
    heartrate_task_flag = 0;
    while (1) {
      if (eTaskGetState(heartrateTaskHandle) == eDeleted) {
        break;
      }
      vTaskDelay(10);
    }
    Serial.println("loopTask_heartrate deleted!");
  }
}

//Click the logo icon, callback function: goes to the main ui interface
static void heartrate_home_event_handler(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  switch (code) {
    case LV_EVENT_CLICKED:
      {
        Serial.println("Clicked the logo button.");
      }
      break;
    case LV_EVENT_RELEASED:
      {
        stop_heartrate_task();
        delay(100);
        if (!lv_obj_is_valid(guider_main_ui.main))
          setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_heartrate_ui.heartrate);
      }
      break;
    default:
      break;
  }
}

//Parameter configuration function on the heartrate screen
void setup_scr_heartrate(lvgl_heartrate_ui *ui) {
  ui->heartrate = lv_obj_create(NULL);

  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->heartrate, &bg_style, LV_PART_MAIN);

  lv_img_home_init();

  /*Init the pressed style*/
  static lv_style_t style_pr;//Apply for a style
  lv_style_init(&style_pr);  //Initialize it
  lv_style_set_translate_y(&style_pr, 5);//Style: Every time you trigger, move down 5 pixels

  /*Create a chart1*/
  ui->heartrate_chart = lv_chart_create(ui->heartrate);

  lv_obj_set_size(ui->heartrate_chart, 180, 200);
  lv_obj_set_pos(ui->heartrate_chart, 60, 120);
  lv_chart_set_type(ui->heartrate_chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/
  lv_chart_set_div_line_count(ui->heartrate_chart, 5, 10);          //Set chart to 5 rows and 10 columns
  lv_chart_set_point_count(ui->heartrate_chart, 100);               //chart shows the data for 100 points
  lv_obj_set_style_size(ui->heartrate_chart, 0, LV_PART_INDICATOR); //Make each data point unsalient

  lv_chart_set_update_mode(ui->heartrate_chart, LV_CHART_UPDATE_MODE_SHIFT);//Move chart to update data
  /*Add two data series*/
  series = lv_chart_add_series(ui->heartrate_chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_SECONDARY_Y);//Let the data be on the right y axis
  lv_chart_set_range(ui->heartrate_chart, LV_CHART_AXIS_SECONDARY_Y, CHART_LOW_LIMIT, CHART_HIGH_LIMIT);         //Set the value range of right y axis

  lv_chart_set_axis_tick(ui->heartrate_chart, LV_CHART_AXIS_PRIMARY_Y, 3, 1, 11, 2, true, 60);                   //Let the axis be on the left y axis
  lv_chart_set_range(ui->heartrate_chart, LV_CHART_AXIS_PRIMARY_Y, CHART_LOW_LIMIT, CHART_HIGH_LIMIT);           //Set the value range of left y axis

  ui->heartrate_label = lv_label_create(ui->heartrate);
  lv_obj_set_pos(ui->heartrate_label, 20, 50);
  lv_obj_set_size(ui->heartrate_label, 140, 40);
  lv_label_set_text(ui->heartrate_label, "HeartRate: 0");
  lv_label_set_long_mode(ui->heartrate_label, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui->heartrate_label, LV_TEXT_ALIGN_LEFT, 0);

  ui->heartrate_home = lv_imgbtn_create(ui->heartrate);
  lv_obj_set_pos(ui->heartrate_home, 150, 20);
  lv_obj_set_size(ui->heartrate_home, 80, 80);
  lv_img_set_src(ui->heartrate_home, &img_home);
  lv_obj_add_style(ui->heartrate_home, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed

  lv_obj_add_event_cb(ui->heartrate_home, heartrate_home_event_handler, LV_EVENT_ALL, NULL);
  heartrate_init();
  create_heartrate_task();
  heartrate_wake_up();
}
