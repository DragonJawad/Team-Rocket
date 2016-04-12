#include <inttypes.h>
#include "drivers/mss_ace/mss_ace.h"

#define SCOREMAX 10
#define GAMELENGTH 300 // In seconds, 5min * 60secs/min = 300 secs

typedef struct team_struct {
    uint32_t score;
    ace_channel_handle_t team_ace_handler;

} team_struct_t ;

void
ARENA_initTeam(team_struct_t* teamPtr, const char* adc_name);

// Returns 0 if didn't score
uint8_t
ARENA_checkIfScored(team_struct_t* teamPtr);

// Outputs score to screen, clears screen beforehand!
void
ARENA_outputScoreToScreen(team_struct_t* teamPtrA, team_struct_t* teamPtrB);

// Outputs timer to first line (does not clear screen)
void
ARENA_outputTimeLeft(uint32_t timeLeftInSeconds);

// Outputs start message to screen
void
ARENA_outputStartToScreen();

// Outputs end message to screen
void
ARENA_outputEndToScreen(int teamWonFlag);

// Returns interrupt status from hardware
uint32_t
ARENA_getInterruptStatus();

// Outputs PWM signal to open the ball release system
void
ARENA_openBallRelease();

// Outputs PWM signal to close the ball release system
void
ARENA_closeBallRelease();

