#include <stdio.h>
#include <stdlib.h>

#define ROWS 20
#define COLS 50
#define MAX_OBJS 50

typedef enum { LINE, RECT, CIRCLE, TRI } Type;

typedef struct {
    Type type;
    int x1, y1, x2, y2, x3, y3, r;
    int active;
} Object;

Object objs[MAX_OBJS];
char canvas[ROWS][COLS];
const char* names[] = {"Line", "Rectangle", "Circle", "Triangle"};

void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    while (1) {
        if (x1 >= 0 && x1 < COLS && y1 >= 0 && y1 < ROWS) canvas[y1][x1] = '*';
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void draw_rect(int x1, int y1, int x2, int y2) {
    draw_line(x1, y1, x2, y1);
    draw_line(x2, y1, x2, y2);
    draw_line(x2, y2, x1, y2);
    draw_line(x1, y2, x1, y1);
}

void draw_circle(int cx, int cy, int r) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            int dx = x - cx;
            int dy = (y - cy) * 2;
            int d2 = dx * dx + dy * dy;
            int r2 = r * r;
            if (d2 >= r2 - r && d2 <= r2 + r) canvas[y][x] = '*';
        }
    }
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

void display() {
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            canvas[y][x] = '_';

    for (int i = 0; i < MAX_OBJS; i++) {
        if (!objs[i].active) continue;
        if (objs[i].type == LINE) draw_line(objs[i].x1, objs[i].y1, objs[i].x2, objs[i].y2);
        else if (objs[i].type == RECT) draw_rect(objs[i].x1, objs[i].y1, objs[i].x2, objs[i].y2);
        else if (objs[i].type == CIRCLE) draw_circle(objs[i].x1, objs[i].y1, objs[i].r);
        else if (objs[i].type == TRI) draw_triangle(objs[i].x1, objs[i].y1, objs[i].x2, objs[i].y2, objs[i].x3, objs[i].y3);
    }

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) putchar(canvas[y][x]);
        putchar('\n');
    }
}

void input_shape(Object *o) {
    Object temp = {0};
    int t;
    printf("Type (0:Line, 1:Rect, 2:Circle, 3:Tri): ");
    int res = scanf("%d", &t);
    if (res == EOF) return;
    if (res != 1 || t < 0 || t > 3) {
        printf("Invalid type!\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }
    temp.type = (Type)t;
    if (temp.type == LINE || temp.type == RECT) {
        printf("Enter x1 y1 x2 y2: ");
        int r = scanf("%d %d %d %d", &temp.x1, &temp.y1, &temp.x2, &temp.y2);
        if (r == EOF) return;
        if (r != 4) {
            printf("Invalid inputs!\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return;
        }
    } else if (temp.type == CIRCLE) {
        printf("Enter cx cy r: ");
        int r = scanf("%d %d %d", &temp.x1, &temp.y1, &temp.r);
        if (r == EOF) return;
        if (r != 3 || temp.r < 0) {
            printf("Invalid inputs!\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return;
        }
    } else if (temp.type == TRI) {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        int r = scanf("%d %d %d %d %d %d", &temp.x1, &temp.y1, &temp.x2, &temp.y2, &temp.x3, &temp.y3);
        if (r == EOF) return;
        if (r != 6) {
            printf("Invalid inputs!\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return;
        }
    }
    temp.active = 1;
    *o = temp;
}

int main() {
    int choice;
    while (1) {
        printf("\n1. Add  2. Delete  3. Modify  4. Display  5. Exit\nChoice: ");
        int res = scanf("%d", &choice);
        if (res == EOF) {
            break;
        }
        if (res != 1) {
            printf("Invalid choice!\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        if (choice == 1) {
            int found = -1;
            for (int i = 0; i < MAX_OBJS; i++) {
                if (!objs[i].active) { found = i; break; }
            }
            if (found != -1) input_shape(&objs[found]);
            else printf("Canvas is full of objects!\n");
        } else if (choice == 2 || choice == 3) {
            printf("Active objects:\n");
            int has_active = 0;
            for (int i = 0; i < MAX_OBJS; i++) {
                if (objs[i].active) {
                    printf("[%d] %s\n", i, names[objs[i].type]);
                    has_active = 1;
                }
            }
            if (!has_active) {
                printf("No active objects!\n");
                continue;
            }
            printf("Enter index: ");
            int idx;
            int r = scanf("%d", &idx);
            if (r == EOF) {
                break;
            }
            if (r == 1 && idx >= 0 && idx < MAX_OBJS && objs[idx].active) {
                if (choice == 2) objs[idx].active = 0;
                else input_shape(&objs[idx]);
            } else {
                printf("Invalid index!\n");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
        } else if (choice == 4) {
            display();
        } else if (choice == 5) {
            break;
        } else {
            printf("Invalid choice! Choose between 1 and 5.\n");
        }
    }
    return 0;
}