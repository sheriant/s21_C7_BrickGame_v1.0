CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic
LDFLAGS = 

# Определение OS и настройка ncurses
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS += -lncurses
endif
ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -lncurses
endif
# Для других систем может потребоваться -lcurses или -lpdcurses

# Директории
GUI_DIR = gui/cli
GAME_DIR = brick_game/tetris
INC_DIR = .
BUILD_DIR = build
DIST_DIR = dist
LIB_DIR = lib

# Исходные файлы (относительно src/)
GAME_SRCS = $(GAME_DIR)/code.c
GUI_SRCS = $(GUI_DIR)/interface.c
MAIN_SRC = game.c

# Объектные файлы
GAME_OBJS = $(GAME_DIR)/code.o
GUI_OBJS = $(GUI_DIR)/interface.o
MAIN_OBJ = game.o

# Имена
PROJECT_NAME = tetris
LIBRARY_NAME = libbrickgame.a
EXECUTABLE = $(BUILD_DIR)/$(PROJECT_NAME)
LIBRARY = $(LIB_DIR)/$(LIBRARY_NAME)

# Все цели
.PHONY: all install uninstall clean dvi dist test gcov_report clang cpp valgrind help

# Основная цель: сборка библиотеки, установка и запуск
all: library install run

# Сборка исполняемого файла
build: $(EXECUTABLE)

$(EXECUTABLE): $(GAME_OBJS) $(GUI_OBJS) $(MAIN_OBJ) | $(BUILD_DIR)
	@echo "🔨 Компоновка исполняемого файла..."
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "✅ Исполняемый файл создан: $(EXECUTABLE)"

# Сборка статической библиотеки с очисткой объектных файлов
library: $(LIBRARY)
	@echo "🗑️  Очистка временных объектных файлов..."
	@rm -f $(GAME_OBJS) $(GUI_OBJS) 2>/dev/null || true

$(LIBRARY): $(GAME_OBJS) $(GUI_OBJS) | $(LIB_DIR)
	@echo "📚 Сборка статической библиотеки..."
	@ar rcs $@ $^
	@ranlib $@
	@echo "✅ Библиотека создана: $(LIBRARY)"

# Создание директорий
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

# Правила компиляции для файлов в поддиректориях
$(GAME_DIR)/%.o: $(GAME_DIR)/%.c
	@echo "🔧 Компиляция $<..."
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(GUI_DIR)/%.o: $(GUI_DIR)/%.c
	@echo "🔧 Компиляция $<..."
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

%.o: %.c
	@echo "🔧 Компиляция $<..."
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Установка
install: build
	@echo "📦 Программа установлена в $(BUILD_DIR)"

# Запуск
run: install
	@echo "🚀 Запуск программы..."
	@$(EXECUTABLE)

# Удаление установленной программы
uninstall:
	@echo "🗑️  Удаление программы..."
	@rm -rf $(BUILD_DIR) $(LIB_DIR) 2>/dev/null || true
	@echo "✅ Программа удалена"

# Очистка (полная)
clean:
	@echo "🧹 Очистка временных файлов..."
	@rm -f $(GAME_OBJS) $(GUI_OBJS) $(MAIN_OBJ) 2>/dev/null || true
	@rm -f *.o *.a *.gcno *.gcda *.gcov 2>/dev/null || true
	@rm -rf $(BUILD_DIR) $(DIST_DIR) $(LIB_DIR) html/ report/ 2>/dev/null || true
	@echo "✅ Все временные файлы удалены"

# Документация (DVI)
dvi:
	@echo "📄 DVI документация временно недоступна"
	# TODO: Добавить генерацию Doxygen документации

# Дистрибутив
dist: clean
	@echo "📦 Создание дистрибутива..."
	@mkdir -p $(DIST_DIR)
	@cp -r ../src/* $(DIST_DIR)/ 2>/dev/null || true
	@cp Makefile $(DIST_DIR)/ 2>/dev/null || true
	@cd $(DIST_DIR) && tar -czvf $(PROJECT_NAME).tar.gz . >/dev/null 2>&1
	@echo "✅ Дистрибутив создан: $(DIST_DIR)/$(PROJECT_NAME).tar.gz"

# Тесты (заглушка)
test:
	@echo "🧪 Тесты временно недоступны"
	@echo "TODO: Добавить тесты"
	# TODO: Добавить тесты

# Отчет о покрытии кода (заглушка)
gcov_report:
	@echo "📊 Отчет о покрытии кода временно недоступен"
	@echo "TODO: Добавить тесты для генерации gcov отчета"
	# TODO: Добавить тесты и gcov отчет

# Проверка стиля кода
clang:
	@echo "🔍 Проверка стиля кода..."
	@clang-format -style=file:../materials/linters/.clang-format -n --Werror *.[ch] brick_game/tetris/*.[ch] gui/cli/*.[ch] >/dev/null 2>&1 && \
		echo "✅ OK" || (echo "❌ Исправление..." && clang-format -style=file:../materials/linters/.clang-format -i *.[ch] brick_game/tetris/*.[ch] gui/cli/*.[ch] >/dev/null 2>&1 && echo "🎉 Готово")

# Статический анализ исходного кода
cpp:
	@echo "🔄 Запуск cppcheck..."
	@cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive \
		-I$(INC_DIR) \
		$(MAIN_SRC) \
		$(GAME_SRCS) \
		$(GUI_SRCS) \
		2>&1 | tee cppcheck_report.txt >/dev/null
	@echo "📄 Отчет сохранен в cppcheck_report.txt"

# Проверка на утечки памяти
valgrind: build
	@echo "🧪 Запуск valgrind..."
	@valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind_report.txt \
		$(EXECUTABLE) >/dev/null 2>&1 || true
	@echo "📄 Отчет сохранен в valgrind_report.txt"

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
	@echo "  dist          - Создание дистрибутива"
	@echo "  test          - Запуск тестов (заглушка)"
	@echo "  gcov_report   - Отчет о покрытии кода (заглушка)"
	@echo "  clang         - Стилевая проверка по Google-style"
	@echo "  cpp           - Статический анализ кода cppcheck"
	@echo "  valgrind      - Проверка на утечки памяти"
	@echo "  help          - Показать эту справку"