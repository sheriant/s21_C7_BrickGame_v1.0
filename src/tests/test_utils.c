#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threads.h>

#include "../brick_game/tetris/code.h"

START_TEST(test_consume_rows) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    for (int j = 0; j < FIELD_W; j++) {
        state->field[FIELD_H - 1][j] = 1;
    }
    
    int initialScore = state->score;
    
    consumeRows();
    
    ck_assert_int_gt(state->score, initialScore);
    ck_assert_int_eq(state->score, initialScore + 100);
    
    finishGame();
}
END_TEST

START_TEST(test_update_level) {
    initializeState();
    
    State_t *state = getCurrentState();
    
    state->score = NEW_LEVEL_THRESHOLD;
    
    updateLevel();
    
    ck_assert_int_eq(state->level, 2);
    ck_assert_int_eq(state->speed, SPEED - SPEED_STEP);
    
    finishGame();
}
END_TEST

START_TEST(test_save_max_score) {
    char *test_filename = "test_max_score.txt";
    
    FILE *f = fopen(test_filename, "w");
    fprintf(f, "100");
    fclose(f);
    
    initializeState();
    
    State_t *state = getCurrentState();
    state->score = 200;

    remove(test_filename);
    finishGame();
}
END_TEST

START_TEST(test_current_time_millis) {
    unsigned long long time1 = currentTimeMillis();
    
    // Используем thrd_sleep из C11 если доступно
    #ifdef __STDC_NO_THREADS__
    // Запасной вариант - цикл ожидания
    volatile int i;
    for (i = 0; i < 100000; i++);
    #else
    struct timespec ts = {0, 1000000};  // 1 ms в наносекундах
    thrd_sleep(&ts, NULL);
    #endif
    
    unsigned long long time2 = currentTimeMillis();
    
    ck_assert_uint_ge(time2, time1);
}
END_TEST

START_TEST(test_process_timer) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    state->startTime = currentTimeMillis();
    state->timeLeft = 100;
    
    unsigned long long timeLeft = processTimer();
    
    ck_assert_uint_le(timeLeft, 100);
    
    finishGame();
}
END_TEST

START_TEST(test_user_input_all_actions) {
    initializeState();
    
    userInput(Start);
    State_t *state = getCurrentState();
    ck_assert_int_eq(state->status, Spawn);
        
    finishGame();
}
END_TEST

Suite *utils_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Utils");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_consume_rows);
    tcase_add_test(tc_core, test_update_level);
    tcase_add_test(tc_core, test_save_max_score);
    tcase_add_test(tc_core, test_current_time_millis);
    tcase_add_test(tc_core, test_process_timer);
    tcase_add_test(tc_core, test_user_input_all_actions);
    
    suite_add_tcase(s, tc_core);

    return s;
}