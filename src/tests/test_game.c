#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../brick_game/tetris/code.h"
#include "../game_info.h"

START_TEST(test_initialize_state) {
    initializeState();
    State_t *state = getCurrentState();
    
    ck_assert_int_eq(state->status, Initial);
    ck_assert_int_eq(state->score, 0);
    ck_assert_int_eq(state->level, 1);
    ck_assert_int_eq(state->speed, SPEED);
    ck_assert_int_eq(state->pause, 0);
    ck_assert_int_eq(state->coordX, -1);
    ck_assert_int_eq(state->coordY, 4);
    
    ck_assert_ptr_nonnull(state->field);
    ck_assert_ptr_nonnull(state->block);
    ck_assert_ptr_nonnull(state->nextBlock);
    
    ck_assert_int_eq(state->blockSize, state->nextBlockSize);
    
    finishGame();
}
END_TEST

START_TEST(test_start_game) {
    initializeState();
    userInput(Start);
    State_t *state = getCurrentState();
    
    ck_assert_int_eq(state->status, Spawn);
    
    finishGame();
}
END_TEST

START_TEST(test_pause_game) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    userInput(Pause);
    ck_assert_int_eq(state->pause, 0);
    
    finishGame();
}
END_TEST

START_TEST(test_game_over) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < FIELD_W; j++) {
            state->field[i][j] = 1;
        }
    }
    
    spawnNewBlock();
    attachBlock();
    
    ck_assert_int_eq(state->status, GameOver);
    
    finishGame();
}
END_TEST

START_TEST(test_update_current_state) {
    initializeState();
    
    GameInfo_t info = updateCurrentState();
    
    ck_assert_int_eq(info.score, 0);
    ck_assert_int_eq(info.level, 1);
    ck_assert_int_eq(info.speed, SPEED);
    ck_assert_int_eq(info.highScore, 0);
    ck_assert_int_eq(info.pause, GameStart);
    
    ck_assert_ptr_nonnull(info.field);
    ck_assert_ptr_nonnull(info.next);
    
    for (int i = 0; i < FIELD_H; i++) free(info.field[i]);
    for (int i = 0; i < 4; i++) free(info.next[i]);
    free(info.field);
    free(info.next);
    
    finishGame();
}
END_TEST

Suite *game_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Game Logic");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_initialize_state);
    tcase_add_test(tc_core, test_start_game);
    tcase_add_test(tc_core, test_pause_game);
    tcase_add_test(tc_core, test_game_over);
    tcase_add_test(tc_core, test_update_current_state);
    
    suite_add_tcase(s, tc_core);

    return s;
}