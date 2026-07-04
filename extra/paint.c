#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define WIDTH  80
#define HEIGHT 25
#define MOUSE_DEV "/mnt/host/mouse_stream"

int main(void)
{
    int fd, x = WIDTH / 2, y = HEIGHT / 2;
    int dx, dy, buttons;
    int old_x = WIDTH / 2, old_y = HEIGHT / 2;    
    char screen[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            screen[i][j] = ' ';

    fd = open(MOUSE_DEV, O_RDONLY);;
    if (fd < 0) {
        printf("No se puede abrir %s\n", MOUSE_DEV);
        return 1;
    }
    lseek(fd, 0, SEEK_END);

    printf("\033[2J");  
    fflush(stdout);
    usleep(100000);    

    while (1) {
        char buf[3];
        int n = read(fd, buf, 3);

        if (n == 0) {
            usleep(1000);
            continue;
        }
        if (n != 3) continue;

        buttons = buf[0] & 0x07;
        
        dx = (signed char)buf[1];
        dy = (signed char)buf[2];        

        if (dx == 0 && dy == 0 && buttons == 0) continue;

        old_x = x;
        old_y = y;
        x += dx;
        y -= dy;
        if (x < 0) x = 0;
        if (x >= WIDTH) x = WIDTH - 1;
        if (y < 0) y = 0;
        if (y >= HEIGHT) y = HEIGHT - 1;

        if (buttons & 1)
            screen[y][x] = '#';
        else if (buttons & 2)
            screen[y][x] = ' ';

        printf("\033[%d;%dH%c", old_y + 1, old_x + 1, screen[old_y][old_x]);
        printf("\033[%d;%dH%c", y + 1, x + 1, screen[y][x]);
        printf("\033[%d;%dHX", y + 1, x + 1);
        usleep(5000);
        fflush(stdout);
    }
    close(fd);
    return 0;
}
