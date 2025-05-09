
#include "esp_log.h"
#include "qs_logic.h"
#include "qs_i2c.h"

TaskHandle_t scanHandle;

// variable player pushbutton inputs
static uint8_t* read;
// variable for which player pressed first
static uint8_t player;

// initlaize the UB board
void logic_init()
{
    printf("Logic init started\n");
    // initial value of first player is false
    player = 0;

    // send the player pressed to BT
    send_player();
    
    // Clear lamps for scanning
    i2c_write_lamps((uint8_t[]) CLEARLAMPS);

    // start the scan task
    vTaskResume(scanHandle);
}

// main motor logic
void logic_task()
{
    // value passed by tasknotify
    uint32_t ulval;

    // variable player inputs
    read = malloc(sizeof(uint16_t));

    // itirate here
    while (true)
    {
        // task wait from take notification
        if (xTaskNotifyWait(0, 0, &ulval, portMAX_DELAY))
        {
            // determine if bt rcv value and the specific task state of RESET
            if (ulval == RST_VAL) {
                printf("Restart request received\n");
                logic_init();
            }
            else if (ulval == CHK_VAL) {
                // DO NOTHING
            }
        }
        // When scan task is supended, check for restart button input
        if (eTaskGetState(scanHandle) == eSuspended) {
            // Read port 1 of UE101A
            *read = i2c_get_restart();
            // Check if restart button pressed
            if (RSTRT(read)) {
                logic_init();
            }
        }
    }
    
}

// scan for player input task
void scan_task()
{
    while (true)
    {
        // initialy pause this task
        vTaskSuspend(scanHandle);

        // get value of player inputs
        //i2c_write_read(I2C_UE101_ADD, (uint8_t[]) { 0x00 }, 1, read, 1);

        printf("Scanning for Player input\n");
        do {
            // get value of player inputs
            //i2c_write_read(devUE101Handle, (uint8_t[]) { REG_9555_INPUT_0 }, 1, read, 1);
            *read = i2c_get_player();

            // small delay between input reads
            vTaskDelay(pdMS_TO_TICKS(20));

        } while(*read == 0);

        printf("Player input received\n");
        if (PLYR1(read)) {
            player = 1;
            i2c_write_lamps((uint8_t[]) PLYR1FIRST);
        }
        else if (PLYR2(read)) {
            player = 2;
            i2c_write_lamps((uint8_t[]) PLYR2FIRST);
        }
        else if (PLYR3(read)) {
            player = 3;
            i2c_write_lamps((uint8_t[]) PLYR3FIRST);
        }
        else if (PLYR4(read)) {
            player = 4;
            i2c_write_lamps((uint8_t[]) PLYR4FIRST);
        }
        else if (PLYR5(read)) {
            player = 5;
            i2c_write_lamps((uint8_t[]) PLYR5FIRST);
        }
        else if (PLYR6(read)) {
            player = 6;
            i2c_write_lamps((uint8_t[]) PLYR6FIRST);
        }

        // send the player pressed to BT
        //send_player();
        send_player();

        // turn on buzzer
        BUZZER_ON;
        // delay for buzzer
        vTaskDelay(pdMS_TO_TICKS(BUZZ_TIME));
        // turn off buzzer
        BUZZER_OFF;


    }
    
}

void send_player() {
    blue_send((uint8_t*) &player);
}