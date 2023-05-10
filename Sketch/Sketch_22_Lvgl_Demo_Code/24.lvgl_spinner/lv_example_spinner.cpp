
#include "lvgl.h"
#include "lv_example_spinner.h"

/*************** Example 1 ****************************/
void lv_example_spinner_1(void)
{
    /*Create a spinner*/
    lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);//创建旋转控件，设置旋转一周时间1s，设置旋转条长度中值为60
    lv_obj_set_size(spinner, 200, 200);//设置控件大小为100,100（当两个值大小不一致时，以最小值创建旋转控件）
    lv_obj_center(spinner);
}
