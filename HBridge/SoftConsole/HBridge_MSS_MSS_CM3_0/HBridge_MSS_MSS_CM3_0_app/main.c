#include <stdio.h>
#include <inttypes.h>

static volatile uint32_t REG_BASE = 0x40050000;

__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{
    //NVIC_ClearPendingIRQ( Fabric_IRQn );
}

// Duty should be between 0 and 100
void setPWMDuty(uint32_t duty) {

}

//uint32_t getPWMDuty() {

//}

void setHBridgeInputs(uint32_t in) {
 volatile uint32_t *writeAddr= (uint32_t *)(REG_BASE + 0x04);
 uint32_t val = *writeAddr;
 *writeAddr= in;
}

int main()
{

	//NVIC_EnableIRQ(Fabric_IRQn);

	while( 1 )
	{
		uint32_t in;
		in = 0x1;
		setHBridgeInputs(in);
		in= 0x2;
		setHBridgeInputs(in);
	}
}