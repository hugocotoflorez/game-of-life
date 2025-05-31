#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TP_IMPLEMENTATION
#include "tp.h"

#define width 80
#define height 24
/* microseconds */
#define sleep_time 200000

#define A '#' // alive cell
#define D ' ' // dead cell

// clang-format off
static char screen_buffer[width * height] = {
      "                                                                                "
      "                                                            ###   ###           "
      "                               #                                                "
      "                             # #                          #    # #    #         "
      "                   ##      ##            ##               #    # #    #         "
      "                  #   #    ##            ##               #    # #    #         "
      "       ##        #     #   ##                               ###   ###           "
      "       ##        #   # ##    # #                                                "
      "                 #     #       #                            ###   ###           "
      "                  #   #                                   #    # #    #         "
      "                   ##                                     #    # #    #         "
      "                                                          #    # #    #         "
      "                                                                                "
      "                                                            ###   ###           "
      "                                                                                "
      "                                                                                "
      "                                                                                "
      "                                                                                "
      "                                                                                "
      "                                                                                "
      "                                                                                "
      "                                                                                "
      "                                                                                "
      "                                                                                "

};
// clang-format on

static char next_buffer[width * height];

void
calc_cell(char *cell)
{
        ptrdiff_t pos = cell - next_buffer;
        int h = pos / width;
        int w = pos % width;
        int lived_neightbours = 0;

        int i, j, position;
        for (i = -1; i <= 1; i++)
                for (j = -1; j <= 1; j++) {
                        if (i == 0 && j == 0) continue;
                        if (h + i >= height || w + j >= width) continue;
                        if (h + i < 0 || w + j < 0) continue;
                        if (screen_buffer[(h + i) * width + w + j] != D) ++lived_neightbours;
                }

        switch (lived_neightbours) {
        case 2:
                *cell = screen_buffer[pos];
                break;
        case 3:
                *cell = A;
                break;
        default:
                *cell = D;
                break;
        }
}

void
print_state()
{
        printf("\e[H");
        for (int i = 0; i < height; i++) {
                write(STDOUT_FILENO, screen_buffer + i * width, width);
                write(STDOUT_FILENO, &"\n", 1);
        }
        fflush(stdout);
}


int
main()
{
        printf("\e[H\e[2J");
        fflush(stdout);
        while (1) {
                print_state();
                usleep(sleep_time);

                thread_pool(next_buffer, width * height, sizeof(char), calc_cell, 16);
                memcpy(screen_buffer, next_buffer, sizeof screen_buffer / sizeof *screen_buffer);
        }
        return 0;
}
