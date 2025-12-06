.PHONY: all, install, uninstall, clean, dvi, dist, test, gcov_report clang

.PHONY: clang

clang:
	@echo "🔍 Format check..."
	@clang-format -style=file:../materials/linters/.clang-format -n --Werror *.[ch] brick_game/*.[ch] gui/*.[ch] tests/*.[ch] >/dev/null 2>&1 && \
		echo "✅ OK" || (echo "❌ Fixing..." && clang-format -style=file:../materials/linters/.clang-format -i *.[ch] brick_game/*.[ch] gui/*.[ch] tests/*.[ch] && echo "🎉 Done")
