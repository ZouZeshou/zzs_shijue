#include "main/thread.h"
int main()
{
    task c_task;
    thread get_image(&task::get_image,&c_task);
    thread deal_image(&task::deal_image,&c_task);
    //thread test(&task::test,&c_task);
#ifdef DEBUG_PLOT
    thread plot(&task::plot,&c_task);
    plot.join();
#endif
    get_image.join();
    deal_image.join();
    //test.join();

    return 1;
}
