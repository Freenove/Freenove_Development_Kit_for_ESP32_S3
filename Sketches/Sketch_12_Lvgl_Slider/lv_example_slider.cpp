#include "lvgl.h"
#include "lv_example_slider.h"

/*************** Example 1 ****************************/
static lv_obj_t * slider_label;
static void slider_event_cb_1(lv_event_t * e){
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[8];
    lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
    lv_label_set_text(slider_label, buf);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

//A default slider with a label displaying the current value
void lv_example_slider_1(void){
    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_center(slider);
    lv_obj_set_size(slider, 200, 10);
    lv_obj_add_event_cb(slider, slider_event_cb_1, LV_EVENT_VALUE_CHANGED, NULL);

    /*Create a label below the slider*/
    slider_label = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label, "0%");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

//Show how to style a slider.
void lv_example_slider_2(void){
    /*Create a transition*/
    static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR, (lv_style_prop_t)0};
    static lv_style_transition_dsc_t transition_dsc;
    lv_style_transition_dsc_init(&transition_dsc, props, lv_anim_path_linear, 300, 0, NULL);

    static lv_style_t style_main;
    static lv_style_t style_indicator;
    static lv_style_t style_knob;
    static lv_style_t style_pressed_color;
    lv_style_init(&style_main);
    lv_style_set_bg_opa(&style_main, LV_OPA_COVER);
    lv_style_set_bg_color(&style_main, lv_color_hex3(0xbbb));
    lv_style_set_radius(&style_main, LV_RADIUS_CIRCLE);
    lv_style_set_pad_ver(&style_main, -2); /*Makes the indicator larger*/

    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indicator, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_indicator, LV_RADIUS_CIRCLE);
    lv_style_set_transition(&style_indicator, &transition_dsc);

    lv_style_init(&style_knob);
    lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
    lv_style_set_bg_color(&style_knob, lv_palette_main(LV_PALETTE_CYAN));
    lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_CYAN, 2));
    lv_style_set_border_width(&style_knob, 2);
    lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&style_knob, 10); /*Makes the knob larger*/
    lv_style_set_transition(&style_knob, &transition_dsc);

    lv_style_init(&style_pressed_color);
    lv_style_set_bg_color(&style_pressed_color, lv_palette_darken(LV_PALETTE_CYAN, 2));

    /*Create a slider and add the style*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_remove_style_all(slider);        /*Remove the styles coming from the theme*/
    lv_obj_add_style(slider, &style_main, LV_PART_MAIN);//Set the slider base color to gray
    lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);//Set the slider color to cyan
    lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);//Set the slider ball handle larger and more prominent
    lv_obj_add_style(slider, &style_pressed_color, LV_PART_INDICATOR | LV_PART_KNOB | LV_STATE_PRESSED);// When pressed, the slider color is 2 color numbers darker

    lv_obj_set_size(slider, 200, 20);
    lv_obj_center(slider);
}

/*************** Example 3 ****************************/
static void slider_event_cb_3(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *slider = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
      char buf[16];
      lv_snprintf(buf, sizeof(buf), "%d - %d", (int)lv_slider_get_left_value(slider), (int)lv_slider_get_value(slider));
      LV_LOG_USER("Slider Value Range: %s",buf);
      lv_label_set_text(slider_label, buf);
    }
}

//Show the current value when the slider is pressed by extending the drawer
void lv_example_slider_3(void){
    /*Create a slider in the center of the display*/
    lv_obj_t * slider;
    slider = lv_slider_create(lv_scr_act());

    lv_obj_set_size(slider, 200, 20);
    lv_obj_center(slider);

    lv_slider_set_mode(slider, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(slider, 70, LV_ANIM_OFF);
    lv_slider_set_left_value(slider, 20, LV_ANIM_OFF);

    lv_obj_add_event_cb(slider, slider_event_cb_3, LV_EVENT_ALL, NULL);
    lv_obj_refresh_ext_draw_size(slider);

    slider_label = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label, "20-70");

    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}
