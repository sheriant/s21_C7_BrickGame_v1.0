.PHONY: all, install, uninstall, clean, dvi, dist, test, gcov_report clang

all: install test gcov_report

.PHONY: clang

clang:
	@echo "🔍 Format check..."
	@clang-format -style=file:../materials/linters/.clang-format -n --Werror *.[ch] brick_game/tetris/*.[ch] gui/cli/*.[ch] >/dev/null 2>&1 && \
		echo "✅ OK" || (echo "❌ Fixing..." && clang-format -style=file:../materials/linters/.clang-format -i *.[ch] brick_game/tetris/*.[ch] gui/cli/*.[ch] && echo "🎉 Done")
