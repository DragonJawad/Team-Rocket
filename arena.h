#include <inttypes.h>
#include "drivers/mss_ace/mss_ace.h"


typedef struct team_struct {
	int already_scored;
    uint32_t score;
    ace_channel_handle_t team_ace_handler;

} team_struct_t ;

void
ARENA_initTeam(team_struct_t* teamPtr, const char* adc_name);

// Returns 0 if didn't score
uint8_t
ARENA_checkIfScored(team_struct_t* teamPtr);

void
ARENA_outputScoreToScreen(team_struct_t* teamPtrA, team_struct_t* teamPtrB);
