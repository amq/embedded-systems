#include "progressio.h"
#include <unistd.h>

#define SLEEP_MS 50

int main(int argc, char *argv[]) {
    unsigned int progress, sum;
    unsigned int percent = 0;
    char *active = "|/-\\";

    init_io();

    for (int i = 0; percent < 100; i++) {

#ifdef DEBUG
        wait_key();
#else
        usleep(1000 * SLEEP_MS);
#endif

        progress = get_progress();
        sum = 0;

        for (int j = 0; j < sizeof(progress); j++) {
            sum += (progress >> j * 8) & 0xFF;
        }

        percent = sum * 100 / 255 / sizeof(progress);

        progressbar('=', active[i % 4], ' ', percent);
    }

    shutdown_io();
    return 0;
}
