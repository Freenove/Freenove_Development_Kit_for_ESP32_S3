
#include "lvgl.h"
#include "lv_example_arc.h"

/*************** Example 1 ****************************/
void lv_example_arc_1(void)
{
  /*Create an Arc*/
  lv_obj_t* arc = lv_arc_create(lv_scr_act()); //在当前显示画面中申请一个arc对象
  lv_obj_set_size(arc, 150, 150);              //设置arc对象: 宽150个像素点,高150个像素点
  lv_arc_set_rotation(arc, 180);               //设置arc控件滑条起始点位置
  lv_arc_set_bg_angles(arc, 0, 360);           //设置弧形控件背景部分的度数范围
  lv_arc_set_range(arc, 0, 360);               //设置弧形控件显示部分数值范围
  lv_arc_set_value(arc, 90);                   //设置弧形控件显示部分的数值
  lv_obj_center(arc);                          //居中显示
}


/*************** Example 2 ****************************/
static void set_angle(void * obj, int32_t v)
{
  lv_arc_set_value((lv_obj_t*)obj, v);
}

/**
   Create an arc which acts as a loader.
*/
void lv_example_arc_2(void)
{
  /*Create an Arc*/
  lv_obj_t * arc = lv_arc_create(lv_scr_act());
  lv_arc_set_rotation(arc, 270);
  lv_arc_set_bg_angles(arc, 0, 360);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
  lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
  lv_obj_center(arc);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, arc);
  lv_anim_set_exec_cb(&a, set_angle);
  lv_anim_set_time(&a, 1000);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
  lv_anim_set_repeat_delay(&a, 500);
  lv_anim_set_values(&a, 0, 100);
  lv_anim_start(&a);
}
