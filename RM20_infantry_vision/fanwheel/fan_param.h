#ifndef FAN_PARAM_H
#define FAN_PARAM_H
typedef struct
{
    int GRAY_THRESHOLD;

    int RED_H_MAX;
    int RED_H_MIN ;
    int RED_S_MAX ;
    int RED_S_MIN ;
    int RED_V_MAX ;
    int RED_V_MIN ;
    int BLUE_H_MAX;
    int BLUE_H_MIN ;
    int BLUE_S_MAX ;
    int BLUE_S_MIN ;
    int BLUE_V_MAX ;
    int BLUE_V_MIN ;

    int RED_Y_MAX;
    int RED_Y_MIN ;
    int RED_CR_MAX ;
    int RED_CR_MIN ;
    int RED_CB_MAX ;
    int RED_CB_MIN ;
    int BLUE_Y_MAX;
    int BLUE_Y_MIN ;
    int BLUE_CR_MAX ;
    int BLUE_CR_MIN ;
    int BLUE_CB_MAX ;
    int BLUE_CB_MIN ;

    int ARMOR_CONTOUR_AREA_MAX;//装甲板面积最大值
    int ARMOR_CONTOUR_AREA_MIN;//装甲板面积最小值
    int ARMOR_CONTOUR_LENGTH_MIN;//装甲板长边长度最小值
    int ARMOR_CONTOUR_WIDTH_MIN;//装甲板长边长度最大值
    int ARMOR_CONTOUR_LENGTH_MAX;//装甲板宽边长度最小值
    int ARMOR_CONTOUR_WIDTH_MAX;//装甲板宽边长度最大值
    int ARMOR_CONTOUR_HW_RATIO_MAX;//装甲板长宽比最大值
    int ARMOR_CONTOUR_HW_RATIO_MIN;//装甲板长宽比最小值
    int ARMOR_CONTOUR_AREA_RATIO_MIN;//装甲板轮廓占旋转矩形面积比最小值
    int ARMOR_INTERSETION_AREA_MIN;//扇叶与装甲板匹配时的最小重合面积

    int CENTER_R_CONTOUR_AREA_MAX;//风车中心R面积最大值
    int CENTER_R_CONTOUR_AREA_MIN;//风车中心R面积最小值
    int CENTER_R_CONTOUR_LENGTH_MIN;//风车中心R长边长度最小值
    int CENTER_R_CONTOUR_WIDTH_MIN;//风车中心R长边长度最大值
    int CENTER_R_CONTOUR_LENGTH_MAX;//风车中心R宽边长度最小值
    int CENTER_R_CONTOUR_WIDTH_MAX;//风车中心R宽边长度最大值
    int CENTER_R_CONTOUR_HW_RATIO_MAX;//风车中心R长宽比最大值
    int CENTER_R_CONTOUR_HW_RATIO_MIN;//风车中心R长宽比最小值
    int CENTER_R_CONTOUR_AREA_RATIO_MIN;//装甲板轮廓占旋转矩形面积比最小值
    int CENTER_R_CONTOUR_INTERSETION_AREA_MIN;//中心R占ROI区的面积最小值

    int FLOW_STRIP_FAN_CONTOUR_AREA_MAX;//流动条扇叶（待击打）面积最大值
    int FLOW_STRIP_FAN_CONTOUR_AREA_MIN;//流动条扇叶（待击打）面积最小值
    int FLOW_STRIP_FAN_CONTOUR_LENGTH_MIN;//流动条扇叶（待击打）长边长度最小值
    int FLOW_STRIP_FAN_CONTOUR_WIDTH_MIN;//流动条扇叶（待击打）宽边长度最小值
    int FLOW_STRIP_FAN_CONTOUR_LENGTH_MAX;//流动条扇叶（待击打）长边长度最大值
    int FLOW_STRIP_FAN_CONTOUR_WIDTH_MAX;//流动条扇叶（待击打）宽边长度最大值
    int FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MAX;//流动条扇叶（待击打）长宽比最大值
    int FLOW_STRIP_FAN_CONTOUR_HW_RATIO_MIN;//流动条扇叶（待击打）长宽比最小值
    int FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MAX;//流动条扇叶轮廓占旋转矩形面积比最小值
    int FLOW_STRIP_FAN_CONTOUR_AREA_RATIO_MIN;//流动条扇叶占旋转矩形面积比最小值

    int FLOW_STRIP_CONTOUR_AREA_MAX;//流动条（待击打）面积最大值
    int FLOW_STRIP_CONTOUR_AREA_MIN;//流动条（待击打）面积最小值
    int FLOW_STRIP_CONTOUR_LENGTH_MIN;//流动条（待击打）长边长度最小值
    int FLOW_STRIP_CONTOUR_WIDTH_MIN;//流动条（待击打）宽边长度最小值
    int FLOW_STRIP_CONTOUR_LENGTH_MAX;//流动条（待击打）长边长度最大值
    int FLOW_STRIP_CONTOUR_WIDTH_MAX;//流动条（待击打）宽边长度最大值
    int FLOW_STRIP_CONTOUR_HW_RATIO_MAX;//流动条（待击打）长宽比最大值
    int FLOW_STRIP_CONTOUR_HW_RATIO_MIN;//流动条（待击打）长宽比最小值
    int FLOW_STRIP_CONTOUR_AREA_RATIO_MIN;//流动条占旋转矩形面积比最小值
    int FLOW_STRIP_CONTOUR_INTERSETION_AREA_MIN;//流动条占旋转矩形面积比最小值

    int TARGET_CHANGE_ANGLE;
    int PRE_SPEED_OFFSET;
    int PRE_SPEED_RATIO;
    int PRE_SLOPE_SACLE;
    int PRE_SPEED_PROBABILITY;
    int PRE_SLOPE_PROBABILITY;
    int PRE_TIME_PROBABILITY;
    int PRE_SLOPE_OFFSET;
    int PRE_SECTION_OFFSET;
}fan_param_t;
void fan_param_init();

extern fan_param_t    fan_param;
#endif // FAN_PARAM_H
