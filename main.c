#include <unistd.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 랜덤 키 목록 (영문자, 숫자, 공백)
KeySym random_keys[] = {
    XK_a, XK_b, XK_c, XK_d, XK_e, XK_f, XK_g, XK_h, XK_i, XK_j, XK_k, XK_l, XK_m,
    XK_n, XK_o, XK_p, XK_q, XK_r, XK_s, XK_t, XK_u, XK_v, XK_w, XK_x, XK_y, XK_z,
    XK_A, XK_B, XK_C, XK_D, XK_E, XK_F, XK_G, XK_H, XK_I, XK_J, XK_K, XK_L, XK_M,
    XK_N, XK_O, XK_P, XK_Q, XK_R, XK_S, XK_T, XK_U, XK_V, XK_W, XK_X, XK_Y, XK_Z,
    XK_0, XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9,
    XK_space, XK_Return, XK_Tab
};

// 랜덤 시간 생성 함수
int generate_random_wait_time() {
    return 1 + rand() % 600; // 1초 ~ 10분 (600초)
}

// 카운트다운 함수
void countdown(int seconds) {
    printf("%d초...\n", seconds);
    
    for (int i = seconds; i > 0; i--) {
        printf("\r남은 시간: %d초   ", i);
        fflush(stdout);
        sleep(1);
    }
    printf("\r남은 시간: 0초   \n");
}

// 키보드 입력 함수
void generate_keyboard_input(Display *dpy) {
    int key_count = sizeof(random_keys) / sizeof(random_keys[0]);
    int random_index = rand() % key_count;
    KeySym selected_key = random_keys[random_index];
    
    KeyCode kcode = XKeysymToKeycode(dpy, selected_key);
    
    if (kcode != 0) {
        Window focused;
        int revert;
        XGetInputFocus(dpy, &focused, &revert);
        
        XTestFakeKeyEvent(dpy, kcode, True, 0);
        XTestFakeKeyEvent(dpy, kcode, False, 0);
        XFlush(dpy);
        
        char *key_name = XKeysymToString(selected_key);
        printf("키 입력: %s\n", key_name ? key_name : "error");
    } else {
        printf("키코드 변환 실패\n");
    }
}

// 마우스 이동 함수
void generate_mouse_move(Display *dpy) {
    Screen *screen = DefaultScreenOfDisplay(dpy);
    int screen_width = WidthOfScreen(screen);
    int screen_height = HeightOfScreen(screen);
    
    int x = rand() % screen_width;
    int y = rand() % screen_height;
    
    XTestFakeMotionEvent(dpy, 0, x, y, 0);
    XFlush(dpy);
    
    printf("마우스 이동: (%d, %d)\n", x, y);
}

// 랜덤 이벤트 실행 함수
void execute_random_event(Display *dpy) {
    // 키보드 60%, 마우스 이동 40%
    int event_type = rand() % 100;
    
    if (event_type < 60) {
        generate_keyboard_input(dpy);
    } else {
        generate_mouse_move(dpy);
    }
}

int main() {
    Display *dpy = XOpenDisplay(NULL);
    
    if (!dpy) {
        printf("X11 error\n");
        return -1;
    }
    
    // 랜덤 시드 초기화
    srand(time(NULL));
    
    printf("program on :)\n");
    
    while (1) {
        int wait_seconds = generate_random_wait_time();
        countdown(wait_seconds);
        execute_random_event(dpy);
    }
    
    XCloseDisplay(dpy);
    return 0;
}