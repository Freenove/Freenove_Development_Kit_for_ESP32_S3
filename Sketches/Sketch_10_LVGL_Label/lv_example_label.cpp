
#include "lvgl.h"
#include "lv_example_label.h"

/*************** Example 1 ****************************/
/**
 * Show line wrap, re-color, line align and text scrolling.
 */
void lv_example_label_1(void)
{
    lv_obj_t * label1 = lv_label_create(lv_scr_act());

    /*
    LV_LABEL_LONG_WRAP,             Keep the width of the object, wrap the line if the text is too long, and enlarge the height of the object
    LV_LABEL_LONG_DOT,              Keep the text size, and if it's too long, put dots at the end
    LV_LABEL_LONG_SCROLL,           Keep the text size and scroll back and forth
    LV_LABEL_LONG_SCROLL_CIRCULAR,  Keep the text size and scroll the text
    LV_LABEL_LONG_CLIP,             Keep the size, and clip the text (keep only the front part of the display)
    */
    
    /*Support long text, if the width is not enough to display, then continue to display the newline*/
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);          
    lv_label_set_recolor(label1, true);                           /*Enable recolor through commands in the text*/
    lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a # label, align the lines to the center and wrap long text automatically.");
    lv_obj_set_width(label1, 150);                                /*Set a small width to wrap lines automatically*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0); /*Set the label width to center*/
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);                /*Set the label height to a distance of between mid-40 pixels*/

    lv_obj_t * label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL);     /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 40);
}

/*************** Example 2 ****************************/
/**
 * Create a fake text shadow
 */
void lv_example_label_2(void)
{
    /*Create a style for the shadow*/
    static lv_style_t style_shadow;//Apply for a style object
    lv_style_init(&style_shadow);  //Initialize the object
    lv_style_set_text_opa(&style_shadow, LV_OPA_30);//Set the opacity to 30
    lv_style_set_text_color(&style_shadow, lv_color_black());//Set the text color to black

    /*Create a label for the shadow first (it's in the background)*/
    lv_obj_t * shadow_label = lv_label_create(lv_scr_act());
    lv_obj_add_style(shadow_label, &style_shadow, 0);

    /*Create the main label*/
    lv_obj_t * main_label = lv_label_create(lv_scr_act());
    lv_label_set_text(main_label, "A simple method to create\n"
                      "shadows on a text.\n"
                      "It even works with\n\n"
                      "newlines     and spaces.");

    /*Set the same text for the shadow label*/
    lv_label_set_text(shadow_label, lv_label_get_text(main_label));

    /*Position the main label*/
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 0);

    /*Shift the second label down and to the right by 2 pixel*/
    lv_obj_align_to(shadow_label, main_label, LV_ALIGN_TOP_LEFT, 2, 2);
}
