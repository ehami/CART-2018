/**
 * @file libLCD
 * a modified version of libLCD
 */
#include "lcd.h"
#include "string.h"

#define LCD_TASK_DELTA 20

static FILE *lcdPort;
int selection = 1;
unsigned int scriptDisplayed = 1;

void lcdPrintCentered(unsigned char line, const char *string) {
  char out[16];
  for (int i = 0; i < 16; i++)
    out[i] = ' ';
  for (unsigned int i = (16 - (strlen(string) % 16)) / 2, j = 0;
       j < strlen(string); i++, j++)
    out[i] = string[j];

  lcdPrint(lcdPort, line, out);
}

void lcdScriptInit(FILE *port) {
  lcdInit(port);
  lcdClear(port);
  lcdSetBacklight(port, true);
  lcdPort = port;
}
// called in a looping task in lcdScriptSelect
void _lcdScriptSelect(void *parameters) {
  while (true) {
    // check for button press
    unsigned int button = lcdReadButtons(uart1);
    switch (button) {
    case 1: // left button
      printf("lb pressed\n");
      if (scriptDisplayed > 0) { // not on first script
        scriptDisplayed--;
      }
      break;
    case 2: // center button
      printf("cb pressed\n");
      selection = scriptDisplayed;
      break;
    case 4:
      printf("rb pressed\n");
      if (scriptDisplayed < (NUM_SCRIPTS - 1)) { // not on last script
        scriptDisplayed++;
      }
      break;
    }
    if (selection == -1 || scriptDisplayed != selection) { // no script selected
      if (scriptDisplayed == 0) { // showing first script
        lcdPrint(uart1, 1, "  %s >", titles[scriptDisplayed]);
      } else if (scriptDisplayed == (NUM_SCRIPTS)) { // showing last script
        lcdPrint(uart1, 1, "< %s  ", titles[scriptDisplayed]);
      } else {
        lcdPrint(uart1, 1, "< %s >", titles[scriptDisplayed]);
      }
    } else {                      // script has been selected and is shown
      if (scriptDisplayed == 0) { // showing first script
        lcdPrint(uart1, 1, " *%s*>", titles[scriptDisplayed]);
      }
      if (scriptDisplayed ==
          (sizeof(&scripts) / sizeof(&scripts[0]) - 1)) { // showing last script
        lcdPrint(uart1, 1, "<*%s* ", titles[scriptDisplayed]);
      } else {
        lcdPrint(uart1, 1, "<*%s*>", titles[scriptDisplayed]);
      }
    }
    if (button != 0) { // give time to release button
      while (lcdReadButtons(uart1) != 0) {
        delay(20);
      }
    }
    delay(20);
  }
}

void lcdScriptSelect() {
  TaskHandle lcdTask = taskCreate(_lcdScriptSelect, TASK_DEFAULT_STACK_SIZE,
                                  NULL, TASK_PRIORITY_DEFAULT);
}

void lcdScriptExecute() {
  if (selection < 0)
    return; // Don't run a script if one has not been selected.
  scripts[selection]();
}
