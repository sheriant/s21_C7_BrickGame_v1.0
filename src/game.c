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
      attackBlock();
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
  int c = getch();

  if (c == START_BUTTON)
    userInput(Start);
  else if (c == KEY_LEFT)
    userInput(Left);
  else if (c == KEY_RIGHT)
    userInput(Right);
  else if (c == ACTION_BUTTON)
    userInput(Action);
  else if ((c == KEY_DOWN) && (state->status == Moving))
    while (blockIsAttached() == 0) {
      (state->coordX++);
      render(updateCurrentState());
      usleep(10000);
    }
  else if ((c == PAUSE_BUTTON) && (state->status != Initial)) {
    userInput(Pause);
    render(updateCurrentState());
    char c = getch();
    while ((c != PAUSE_BUTTON) && (c != TERMINATE_BUTTON)) c = getch();
    if (c == PAUSE_BUTTON)
      userInput(Pause);
    else {
      userInput(Terminate);
      terminateGame();
    }
  } else if (c == TERMINATE_BUTTON) {
    userInput(Terminate);
    terminateGame();
  } else if (c == ERR)
    state->status = Shifting;
}