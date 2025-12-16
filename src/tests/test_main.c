#include <check.h>
#include <stdlib.h>

#include "../brick_game/tetris/code.h"
#include "../game_info.h"

Suite *game_suite(void);
Suite *blocks_suite(void);
Suite *movement_suite(void);
Suite *rotation_suite(void);
Suite *utils_suite(void);

int main(void) {
	int number_failed;
	SRunner *sr;

	sr = srunner_create(game_suite());
	srunner_add_suite(sr, blocks_suite());
	srunner_add_suite(sr, movement_suite());
	srunner_add_suite(sr, rotation_suite());
	srunner_add_suite(sr, utils_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}