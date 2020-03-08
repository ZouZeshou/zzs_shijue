#include "armor_detect/armor_detect.h"
#include "armor_detect/armor_param.h"
#include "algorithm/include/usr_math.h"


armor_detect::armor_detect()
{
    init();
};
armor_detect::~armor_detect(){};
void armor_detect::init()
{
    kalman1_init(&m_angle_projection_kf, 0, 200, 5, 50);
    armor_param_init();
    memset(&m_targetinfo,0,sizeof(TargetInfo));
    m_target_center = {0,0,0};
    m_is_debug = 0;
    m_roi_rect = {0,0,0,0};
    m_is_show_img = false;
    m_lightbars.clear();
    m_useful_armors.clear();
}
