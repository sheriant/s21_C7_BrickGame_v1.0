#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/code.h"

START_TEST(test_rotate_block_3x3) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    while (state->blockSize != 3) {
        spawnNewBlock();
    }
    
    int **originalBlock = createMatrix(3, 3);
    copyMatrix(originalBlock, state->block, 3, 3);
    
    rotateBlock();
    
    int isDifferent = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (originalBlock[i][j] != state->block[i][j]) {
                isDifferent = 1;
            }
        }
    }
    ck_assert_int_eq(isDifferent, 1);
    
    freeMatrix(originalBlock, 3);
    finishGame();
}
END_TEST

START_TEST(test_rotate_block_4x4) {
    initializeState();
    userInput(Start);
    
    State_t *state = getCurrentState();
    
    while (state->blockSize != 4) {
        spawnNewBlock();
    }
    
    int **originalBlock = createMatrix(4, 4);
    copyMatrix(originalBlock, state->block, 4, 4);
    
    rotateBlock();
    
    int isDifferent = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (originalBlock[i][j] != state->block[i][j]) {
                isDifferent = 1;
            }
        }
    }
    ck_assert_int_eq(isDifferent, 1);
    
    freeMatrix(originalBlock, 4);
    finishGame();
}
END_TEST

START_TEST(test_can_rotate_block) {
    initializeState();
    userInput(Start);
        
    int **testBlock = createMatrix(3, 3);
    testBlock[0][0] = 1;
    testBlock[1][0] = 1;
    testBlock[2][0] = 1;
    
    int canRotate = canRotateBlock(testBlock);
    
    ck_assert_int_eq(canRotate, 1);
    
    freeMatrix(testBlock, 3);
    finishGame();
}
END_TEST

START_TEST(test_rotate_function) {
    int **oldBlock = createMatrix(3, 3);
    int **newBlock = createMatrix(3, 3);
    
    oldBlock[0][0] = 1;
    oldBlock[1][0] = 1;
    oldBlock[2][0] = 1;
    oldBlock[2][1] = 1;
    
    rotate(newBlock, oldBlock, 3);
    
    ck_assert_int_eq(newBlock[0][2], 1);
    ck_assert_int_eq(newBlock[1][2], 1);
    ck_assert_int_eq(newBlock[2][1], 1);
    ck_assert_int_eq(newBlock[2][2], 1);
    
    freeMatrix(oldBlock, 3);
    freeMatrix(newBlock, 3);
}
END_TEST

START_TEST(test_rotate_o_block) {
    int **oldBlock = createMatrix(2, 2);
    int **newBlock = createMatrix(2, 2);
    
    oldBlock[0][0] = 1;
    oldBlock[0][1] = 1;
    oldBlock[1][0] = 1;
    oldBlock[1][1] = 1;
    
    rotate(newBlock, oldBlock, 2);
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ck_assert_int_eq(newBlock[i][j], oldBlock[i][j]);
        }
    }
    
    freeMatrix(oldBlock, 2);
    freeMatrix(newBlock, 2);
}
END_TEST

Suite *rotation_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Rotation");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_rotate_block_3x3);
    tcase_add_test(tc_core, test_rotate_block_4x4);
    tcase_add_test(tc_core, test_can_rotate_block);
    tcase_add_test(tc_core, test_rotate_function);
    tcase_add_test(tc_core, test_rotate_o_block);
    
    suite_add_tcase(s, tc_core);

    return s;
}