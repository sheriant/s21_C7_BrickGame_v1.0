#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/code.h"

START_TEST(test_move_block_left) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    int initialY = state->coordY;
    
    moveBlockLeft();
    
    ck_assert_int_eq(state->coordY, initialY - 1);
    ck_assert(state->status == Moving || state->status == Attaching);
    
    finishGame();
}
END_TEST

START_TEST(test_move_block_right) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    int initialY = state->coordY;
    
    moveBlockRight();
    
    ck_assert_int_eq(state->coordY, initialY + 1);
    ck_assert(state->status == Moving || state->status == Attaching);
    
    finishGame();
}
END_TEST

START_TEST(test_move_block_left_boundary) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    for (int i = 0; i < 10; i++) {
        moveBlockLeft();
    }
    
    ck_assert_int_ge(state->coordY, 0);
    
    finishGame();
}
END_TEST

START_TEST(test_move_block_right_boundary) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    for (int i = 0; i < 10; i++) {
        moveBlockRight();
    }
    
    ck_assert_int_lt(state->coordY, FIELD_W);
    
    finishGame();
}
END_TEST

START_TEST(test_shift_block) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    int initialX = state->coordX;
    
    shiftBlock();
    
    ck_assert_int_eq(state->coordX, initialX + 1);
    ck_assert(state->status == Moving);
    
    finishGame();
}
END_TEST

START_TEST(test_attach_block) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    state->coordX = FIELD_H - state->blockSize;
    
    attachBlock();
    
    ck_assert(state->status == Spawn);
    
    int hasBlocks = 0;
    for (int i = 0; i < FIELD_H; i++) {
        for (int j = 0; j < FIELD_W; j++) {
            if (state->field[i][j] == 1) hasBlocks = 1;
        }
    }
    ck_assert_int_eq(hasBlocks, 1);
    
    finishGame();
}
END_TEST

Suite *movement_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Movement");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_move_block_left);
    tcase_add_test(tc_core, test_move_block_right);
    tcase_add_test(tc_core, test_move_block_left_boundary);
    tcase_add_test(tc_core, test_move_block_right_boundary);
    tcase_add_test(tc_core, test_shift_block);
    tcase_add_test(tc_core, test_attach_block);
    
    suite_add_tcase(s, tc_core);

    return s;
}