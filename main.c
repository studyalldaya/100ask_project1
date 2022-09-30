
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "./include/disp_manager.h"
#include "./include/font_manager.h"
#include "./include/UI.h"

/* ---- button UI test ---- */
int main(int argc, char **argv)
{
    Display_buffer *ptBuffer;
    int error;
    Button tButton;
    Region tRegion;

    if (argc != 2) {
        printf("Usage: %s <font_size>\n", argv[0]);
        return -1;
    }

    display_init();
    select_default_display("fb");

    init_default_display();

    ptBuffer = get_display_buffer();
    font_init();
    //FontsRegister();
    if (select_and_init_font("freetype", argv[1])) {
        printf("SelectAndInitFont err\n");
        return -1;
    }

    tRegion.x = 200;
    tRegion.y = 200;
    tRegion.width = 300;
    tRegion.height = 100;

    init_button(&tButton, "test", &tRegion, NULL, NULL);
    tButton.on_draw(&tButton, ptBuffer);
    while (1) {
        tButton.on_clicked(&tButton, ptBuffer, NULL);
        sleep(2);
    }

    return 0;
}
