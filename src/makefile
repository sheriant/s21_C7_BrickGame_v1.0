CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic
LDFLAGS = -lcheck -pthread -lrt -lm -lsubunit -lncurses

# Директории
GUI_DIR = gui/cli
GAME_DIR = brick_game/tetris
INC_DIR = .
OUTPUT_DIR = output
DIST_DIR = dist
LIB_DIR = lib

# Директория тестов
TEST_DIR = tests
TEST_LOG_DIR = report/tests
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(TEST_SOURCES:.c=.o)
TEST_EXEC = $(OUTPUT_DIR)/tetris_tests
GCOV_REPORT_DIR = report/coverage

# Исходные файлы (относительно src/)
GAME_SRCS = $(GAME_DIR)/code.c
GUI_SRCS = $(GUI_DIR)/interface.c
MAIN_SRC = game.c

# Объектные файлы
GAME_OBJS = $(GAME_DIR)/code.o
GUI_OBJS = $(GUI_DIR)/interface.o
MAIN_OBJ = game.o

# Конфигурация документации
DOC_SRC_DIR = src
DOC_OUT_DIR = docs
DOXYFILE = Doxyfile

# Имена
PROJECT_NAME = tetris
LIBRARY_NAME = libbrickgame.a
EXECUTABLE = $(OUTPUT_DIR)/$(PROJECT_NAME)
LIBRARY = $(LIB_DIR)/$(LIBRARY_NAME)

# Все цели
.PHONY: all build library install run uninstall clean dvi dist test gcov_report clang cpp valgrind help

# Основная цель: сборка библиотеки, установка и запуск
all: build install run

# Сборка исполняемого файла
build: $(EXECUTABLE)

$(EXECUTABLE): $(MAIN_OBJ) $(LIBRARY) | $(OUTPUT_DIR)
	@echo "🔨 Компоновка исполняемого файла с библиотекой..."
	@$(CC) $(CFLAGS) $(MAIN_OBJ) -L$(LIB_DIR) -lbrickgame -o $@ $(LDFLAGS)
	@echo "✅ Исполняемый файл создан: $(EXECUTABLE)"

# Сборка статической библиотеки
library: $(LIBRARY)

$(LIBRARY): $(GAME_OBJS) $(GUI_OBJS) | $(LIB_DIR)
	@echo "📚 Сборка статической библиотеки..."
	@ar rcs $@ $^
	@ranlib $@
	@echo "✅ Библиотека создана: $(LIBRARY)"

# Создание директорий
$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

# Правила компиляции для файлов в поддиректориях
$(GAME_DIR)/%.o: $(GAME_DIR)/%.c
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(GUI_DIR)/%.o: $(GUI_DIR)/%.c
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

%.o: %.c
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Установка
install: build
	@echo "📦 Программа установлена в $(OUTPUT_DIR)"

# Запуск
run: install
	@echo "🚀 Запуск программы..."
	@$(EXECUTABLE)

# Удаление установленной программы
uninstall:
	@echo "🗑️  Удаление программы..."
	@rm -rf *.txt
	@rm -rf $(OUTPUT_DIR) $(LIB_DIR) 2>/dev/null || true
	@echo "✅ Программа удалена"

clean: uninstall
	@echo "🧹 Очистка временных файлов..."
	@rm -f $(GAME_OBJS) $(GUI_OBJS) $(MAIN_OBJ) 2>/dev/null || true
	@find . -type f \( \
		-name "*.o" \
		-o -name "*.a" \
		-o -name "*.gcno" \
		-o -name "*.gcda" \
		-o -name "*.gcov" \
	\) -delete 2>/dev/null || true
	@rm -rf $(OUTPUT_DIR) $(DIST_DIR) $(LIB_DIR) html/ docs/ report/ 2>/dev/null || true
	@echo "✅ Все временные файлы удалены"

# Документация (DVI)
dvi:
	@echo "📄 Генерация DVI документации для Tetris..."
	@echo "PROJECT_NAME = \"Tetris Game\"" > $(DOXYFILE)
	@echo "PROJECT_BRIEF = \"Реализация игры Тетрис на C\"" >> $(DOXYFILE)
	@echo "INPUT = $(DOC_SRC_DIR)" >> $(DOXYFILE)
	@echo "RECURSIVE = YES" >> $(DOXYFILE)
	@echo "EXTRACT_ALL = YES" >> $(DOXYFILE)
	@echo "EXTRACT_PRIVATE = YES" >> $(DOXYFILE)
	@echo "EXTRACT_STATIC = YES" >> $(DOXYFILE)
	@echo "GENERATE_LATEX = YES" >> $(DOXYFILE)
	@echo "GENERATE_HTML = NO" >> $(DOXYFILE)
	@echo "LATEX_OUTPUT = $(DOC_OUT_DIR)/latex" >> $(DOXYFILE)
	
	@mkdir -p $(DOC_OUT_DIR)
	@doxygen $(DOXYFILE)
	
	@cd $(DOC_OUT_DIR)/latex && latex refman.tex
	@cd $(DOC_OUT_DIR)/latex && latex refman.tex
	
	@echo "✅ DVI документация создана: $(DOC_OUT_DIR)/latex/refman.dvi"
	@echo "📖 Для просмотра используйте: xdvi $(DOC_OUT_DIR)/latex/refman.dvi"
	@echo "📄 Для конвертации в PDF: cd $(DOC_OUT_DIR)/latex && dvipdf refman.dvi refman.pdf"
	
	# Очистка временного Doxyfile
	@rm -f $(DOXYFILE)

# Дистрибутив
dist: clean
	@echo "📦 Создание дистрибутива..."
	@rm -rf $(DIST_DIR) 2>/dev/null || true
	@mkdir -p $(DIST_DIR)/temp
	@cp -r . $(DIST_DIR)/temp/ 2>/dev/null || true
	@find $(DIST_DIR)/temp -type f \( -name "*.o" -o -name "*.a" -o -name "*.gcno" -o -name "*.gcda" -o -name "*.gcov" \) -delete 2>/dev/null || true
	@find $(DIST_DIR)/temp -type d \( -name "output" -o -name "dist" -o -name "lib" -o -name "report" \) -exec rm -rf {} + 2>/dev/null || true
	@cd $(DIST_DIR)/temp && tar -czvf ../$(PROJECT_NAME).tar.gz . >/dev/null 2>&1
	@rm -rf $(DIST_DIR)/temp 2>/dev/null || true
	@echo "✅ Дистрибутив создан: $(DIST_DIR)/$(PROJECT_NAME).tar.gz"

# Тесты
test: $(TEST_EXEC)
	@mkdir -p $(TEST_LOG_DIR)
	@echo "🧪 Тестирование (10 итераций)..."
	@FAILED=""; \
	for i in $$(seq 1 10); do \
		rm -f max_score.txt 2>/dev/null; \
		./$(TEST_EXEC) >$(TEST_LOG_DIR)/run_$$i.log 2>&1; \
		if [ $$? -eq 0 ]; then \
			echo "Итерация $$i: ✅"; \
		else \
			echo "Итерация $$i: ❌"; \
			FAILED="$$FAILED $$i"; \
		fi; \
	done; \
	echo ""; \
	if [ -n "$$FAILED" ]; then \
		for i in $$FAILED; do \
			echo "--- Итерация $$i ---"; \
			grep -B1 "FAIL\|ERROR" $(TEST_LOG_DIR)/run_$$i.log | grep "tests/" || \
			grep "tests/" $(TEST_LOG_DIR)/run_$$i.log | tail -1; \
		done; \
		echo "\n📋 Полные логи в директории: $(TEST_LOG_DIR)/"; \
		exit 1; \
	else \
		echo "✅ Все тесты успешны!"; \
		rm -rf $(TEST_LOG_DIR) 2>/dev/null || true; \
	fi

# Сборка исполняемого файла тестов
$(TEST_EXEC): $(TEST_OBJS) $(GAME_OBJS) $(GUI_OBJS) | $(OUTPUT_DIR)
	@echo "🧪 Компоновка тестов..."
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(GAME_OBJS) $(GUI_OBJS) -o $@ $(LDFLAGS)
	@echo "✅ Тесты собраны: $(TEST_EXEC)"

# Правило компиляции тестовых файлов
$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Отчет о покрытии кода
gcov_report:
	@echo "📊 Сборка отчета о покрытии кода..."
	@mkdir -p $(GCOV_REPORT_DIR)
	
	@$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage \
		-I$(INC_DIR) $(TEST_SOURCES) $(GAME_SRCS) $(GUI_SRCS) \
		-o $(GCOV_REPORT_DIR)/test_gcov $(LDFLAGS) -lgcov 2>/dev/null
	
	@echo
	@cd $(GCOV_REPORT_DIR) && ./test_gcov 2>&1 | grep -E "100%:|FAIL|ERROR" || true
	@echo
	
	@lcov --capture --directory . --output-file $(GCOV_REPORT_DIR)/coverage.info 1>/dev/null 2>&1
	
	# Исключаем тестовые файлы и gui из отчета
	@lcov --remove $(GCOV_REPORT_DIR)/coverage.info \
		"$(TEST_DIR)/*" \
		"$(GUI_DIR)/*" \
		-o $(GCOV_REPORT_DIR)/coverage_filtered.info 1>/dev/null 2>&1
	
	@genhtml $(GCOV_REPORT_DIR)/coverage_filtered.info --output-directory $(GCOV_REPORT_DIR)/html 1>/dev/null 2>&1
	
	@echo "Overall coverage rate:"
	@lcov --summary $(GCOV_REPORT_DIR)/coverage_filtered.info 2>/dev/null | tail -4
	@echo
	@echo "✅ HTML отчет создан: $(GCOV_REPORT_DIR)/html/index.html"

# Проверка стиля кода
clang:
	@echo "🔍 Проверка стиля кода..."
	@clang-format -style=file:../materials/linters/.clang-format -n --Werror *.[ch] brick_game/tetris/*.[ch] gui/cli/*.[ch] >/dev/null 2>&1 && \
		echo "✅ OK" || (echo "❌ Исправление..." && clang-format -style=file:../materials/linters/.clang-format -i *.[ch] brick_game/tetris/*.[ch] gui/cli/*.[ch] >/dev/null 2>&1 && echo "🎉 Готово")

# Статический анализ исходного кода
cpp:
	@echo "🔄 Запуск cppcheck..."
	@mkdir -p report
	@cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive \
		-I$(INC_DIR) \
		$(MAIN_SRC) \
		$(GAME_SRCS) \
		$(GUI_SRCS) \
		2>&1 | tee report/cppcheck_report.txt >/dev/null
	@echo "📄 Отчет сохранен в report/cppcheck_report.txt"

# Проверка на утечки памяти
valgrind: build
	@echo "🧪 Запуск valgrind..."
	@mkdir -p report
	@timeout 3s valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=report/valgrind_report.txt \
		$(EXECUTABLE) 2>/dev/null || true
	@echo "📄 Отчет сохранен в report/valgrind_report.txt"
	@echo "🔍 Краткий анализ утечек:"
	@grep -A5 "LEAK SUMMARY" report/valgrind_report.txt || echo "Не удалось найти информацию об утечках"

# Помощь
help:
	@echo "Доступные цели:"
	@echo "  all           - Сборка библиотеки, установка и запуск программы"
	@echo "  build         - Сборка программы"
	@echo "  library       - Сборка статической библиотеки $(LIBRARY_NAME)"
	@echo "  install       - Установка программы"
	@echo "  run           - Сборка и запуск программы"
	@echo "  uninstall     - Удаление программы"
	@echo "  clean         - Очистка временных файлов"
	@echo "  dvi           - Сборка документации по проекту"
	@echo "  dist          - Создание дистрибутива"
	@echo "  test          - Запуск тестов"
	@echo "  gcov_report   - Отчет о покрытии кода"
	@echo "  clang         - Стилевая проверка по Google-style"
	@echo "  cpp           - Статический анализ кода cppcheck"
	@echo "  valgrind      - Проверка на утечки памяти"
	@echo "  help          - Показать эту справку"