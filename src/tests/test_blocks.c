#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/code.h"

START_TEST(test_generate_new_block) {
	int blockSize = 3;
	
	for (int attempt = 0; attempt < 10; attempt++) {
		int **block = generateNewBlock(&blockSize);
		ck_assert_ptr_nonnull(block);
		ck_assert(blockSize == 2 || blockSize == 3 || blockSize == 4);
		
		int blockCount = 0;
		for (int i = 0; i < blockSize; i++) {
			for (int j = 0; j < blockSize; j++) {
				if (block[i][j] == 1) blockCount++;
			}
		}
		
		ck_assert_int_eq(blockCount, 4);
		
		freeMatrix(block, blockSize);
	}
}
END_TEST

START_TEST(test_spawn_new_block) {
	initializeState();
	userInput(Start);
	
	State_t *state = getCurrentState();
	int oldBlockSize = state->blockSize;
	int **oldNextBlock = state->nextBlock;
	
	spawnNewBlock();
	
	ck_assert_int_eq(state->blockSize, oldBlockSize);
	ck_assert_ptr_eq(state->block, oldNextBlock);
	
	ck_assert_ptr_nonnull(state->nextBlock);
	ck_assert_int_ge(state->nextBlockSize, 2);
	ck_assert_int_le(state->nextBlockSize, 4);
	
	finishGame();
}
END_TEST

START_TEST(test_block_is_attached) {
	initializeState();
	userInput(Start);
	
	State_t *state = getCurrentState();
	
	int attached = blockIsAttached();
	ck_assert_int_eq(attached, 0);
	
	int lowestCellInBlock = 0;
	for (int i = 0; i < state->blockSize; i++) {
		for (int j = 0; j < state->blockSize; j++) {
			if (state->block[i][j] == 1 && i > lowestCellInBlock) {
				lowestCellInBlock = i;
			}
		}
	}

	state->coordX = FIELD_H - 1 + lowestCellInBlock;
	
	attached = blockIsAttached();
	ck_assert_int_eq(attached, 1);
	
	finishGame();
}
END_TEST

START_TEST(test_create_matrix) {
	int **matrix = createMatrix(5, 5);
	
	ck_assert_ptr_nonnull(matrix);
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			ck_assert_int_eq(matrix[i][j], 0);
		}
	}
	
	freeMatrix(matrix, 5);
}
END_TEST

START_TEST(test_copy_matrix) {
	int **src = createMatrix(3, 3);
	int **dest = createMatrix(3, 3);
	
	src[0][0] = 1;
	src[1][1] = 1;
	src[2][2] = 1;
	
	copyMatrix(dest, src, 3, 3);
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			ck_assert_int_eq(dest[i][j], src[i][j]);
		}
	}
	
	freeMatrix(src, 3);
	freeMatrix(dest, 3);
}
END_TEST

Suite *blocks_suite(void) {
	Suite *s;
	TCase *tc_core;

	s = suite_create("Blocks");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_generate_new_block);
	tcase_add_test(tc_core, test_spawn_new_block);
	tcase_add_test(tc_core, test_block_is_attached);
	tcase_add_test(tc_core, test_create_matrix);
	tcase_add_test(tc_core, test_copy_matrix);
	
	suite_add_tcase(s, tc_core);

	return s;
}