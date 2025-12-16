#include "game.h"

int main() {
  initscr();
  curs_set(0);
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  initializeState();
  render(updateCurrentState());

  while (1) {
    State_t *state = getCurrentState();
    if (state->status == Initial)
      updateUserAction();
    else if (state->status == Spawn)
      spawnNewBlock();
    else if (state->status == Moving)
      updateUserAction();
    else if (state->status == Shifting)
      shiftBlock();
    else if (state->status == Attaching)
      attachBlock();
    else if (state->status == GameOver)
      finishGame();
    render(updateCurrentState());
  }

  return 0;
}

void updateUserAction() {
  State_t *state = getCurrentState();
  unsigned long long timeLeft = processTimer();
  timeout(timeLeft);
  int key = getch();

  if (key == START_BUTTON)
    userInput(Start);
  else if (key == KEY_LEFT)
    userInput(Left);
  else if (key == KEY_RIGHT)
    userInput(Right);
  else if (key == ACTION_BUTTON)
    userInput(Action);
  else if ((key == KEY_DOWN) && (state->status == Moving))
    while (blockIsAttached() == 0) {
      (state->coordX++);
      render(updateCurrentState());
      napms(10);
    }
  else if ((key == PAUSE_BUTTON) && (state->status != Initial)) {
    userInput(Pause);
    render(updateCurrentState());
    key = getch();
    while ((key != PAUSE_BUTTON) && (key != TERMINATE_BUTTON)) key = getch();
    if (key == PAUSE_BUTTON)
      userInput(Pause);
    else {
      userInput(Terminate);
      terminateGame();
    }
  } else if (key == TERMINATE_BUTTON) {
    userInput(Terminate);
    terminateGame();
  } else if (key == ERR)
    state->status = Shifting;
}