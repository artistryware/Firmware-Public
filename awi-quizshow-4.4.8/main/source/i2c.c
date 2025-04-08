
#include "i2c.h"

void i2c_init()
{
    // Initialize I2C Master
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_MASTER_FREQ
    };

    // Set I2C Port Number
    i2c_param_config(I2C_MASTER_NUM, &conf);

    // Install I2C Driver
    i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);


    // initialize SX1508
    /**
     * SX1508 is a READ_WRITE I2C SLAVE - Read datasheet of SX1508 for more details
     * 
     *  IO[0]   - I     ORG
     *  IO[1]   - I     RED
     *  IO[2]   - I     GRN
     *  IO[3]   - I     BLU
     *  IO[4]   - I     Board ID bit 0
     *  IO[5]   - I     Board ID bit 1
     *  IO[6]   - I     Board ID bit 2
     *  IO[7]   - I     Board ID bit 3
     */

    // Reset SX1508 first
    /* uint8_t rstval[] = { 
        0x7d, 
        0x12, 
        0x34
    };
    i2c_master_write_to_device(
        I2C_MASTER_NUM, 
        I2C_SLAVE_ADD, 
        rstval, sizeof(rstval),
        I2C_MASTER_TIMEOUT
    ); */
    
     // Initialize input buffer, disable if LED use
    /*uint8_t inputbuff[] = { 
        0x00, 
        0b00001111
    };
    i2c_write(I2C_1508_ADD, inputbuff, sizeof(inputbuff));*/

    // Initialize pullup on Input { RegPullUp, val } - address is autoincreament
    uint8_t pullval[] = { 
        0x03,
        0b11110000     // Example: Enable pull-ups for pins 4–7, disable for pins 0–3
    };
    i2c_write(I2C_1508_ADD, pullval, sizeof(pullval));

    // Initialize Input/Ouput and Interrupt - { RegOpenDrain, RegPolarity, RegDir, RegData, RegInterruptMask, RegSenseHigh, RegSenseLow } - address is autoincrement
    uint8_t ioinitval[] = { 
        0x05,       // Reg 4 - RegOpenDrain - Starting address
        0b00000000, // RegOpenDrain Val - Open Drain 0,1,2,3 - Push/Pull 4,5,6,7
        0b11110000, // RegPolarity val - Invert Inputs 4,5,6,7
        0b11110000, // RegDir val - OUT=[0,1,2,3]; IN=[4,5,6,7]
        0xff,       // RegData val - default 0xff
        0b00001111, // RegInterruptMask val - interrupt on [4,5,6,7]
        0b11111111, // RegSenseHigh val - both rising and falling [4,5,6,7]
        0b00000000  // RegSenseLow val - none [0,1,2,3]
    };
    i2c_write(I2C_1508_ADD, ioinitval, sizeof(ioinitval));

    // Initialize Register Clock, use for debouncing
    uint8_t clkval[] = { 0x0f, 0b01000000 };
    i2c_write(I2C_1508_ADD, clkval, sizeof(clkval));

    // Initialize Misc, LED driver, and Debounce Registers
    uint8_t miscval[] = { 
        0x10,       // Reg 4 - RegMisc - Starting address
        0b00010000, // RegMisc value
        0b00001111, // RegLEDDriverEnable value
        0b00000101, // RegDebounceConfig value
        0b11110000  // RegDebounceEnable value
    };
    i2c_write(I2C_1508_ADD, miscval, sizeof(miscval));

    // Initialize LED driver
    //uint8_t ledval[] = { 0x11, 0b00001111 };
    //i2c_write(I2C_1508_ADD, ledval, sizeof(ledval));

    // Initialize Debouncing
    //uint8_t debval[] = { 0x12, 0b00000101, 0b11110000 };
    //i2c_write(I2C_1508_ADD, debval, sizeof(debval));

    // Clear current interrupt flag
    // Note: After Initializing Interrupt of SX1508, NINT pin is currently in low state and interrupt is accord.
    // Setting RegInterruptSourceB and RegInterruptSourceA to 0xffff respectiveky will clear current interrupt
    //WRITE_SX1508(((uint8_t[]){ 0x18, 0xff, 0xff }), 3);
    //uint8_t readint[] = {};
    //i2c_write(I2C_1508_ADD, readint, sizeof(readint));

    // Set intensity of LEDs
    i2c_write(I2C_1508_ADD, (uint8_t[]) { 0x16, 0x10 }, 2); // set ORG intensity
    i2c_write(I2C_1508_ADD, (uint8_t[]) { 0x17, 0x07 }, 2); // set RED intensity
    i2c_write(I2C_1508_ADD, (uint8_t[]) { 0x19, 0x07 }, 2); // set GRN intensity
    i2c_write(I2C_1508_ADD, (uint8_t[]) { 0x1C, 0x40 }, 2); // set BLU intensity

    vTaskDelay(pdMS_TO_TICKS(1000));    // Delay to check LEDs functioning

    // initialy turn on green led only
    i2c_write(I2C_1508_ADD, (uint8_t[]) { 0x08, 0b00001011 }, 2);

    // initialize UE101
    /**
     * PCAL9555 is a READ_WRITE I2C SLAVE - Read datasheet of PCAL9555 for more details
     * 
     * BANK A
     *  IO[0]   - I     PLAYER 1 PUSHBUTTON
     *  IO[1]   - I     PLAYER 2 PUSHBUTTON
     *  IO[2]   - I     PLAYER 3 PUSHBUTTON
     *  IO[3]   - I     PLAYER 4 PUSHBUTTON
     *  IO[4]   - I     PLAYER 5 PUSHBUTTON
     *  IO[5]   - I     PLAYER 6 PUSHBUTTON
     *  IO[6]   - I     Not Connected on PCAL9555
     *  IO[7]   - I     Not Connected on PCAL9555
     * 
     * BANK B
     *  IO[8]   - I     Not Used
     *  IO[9]   - I     Not Used
     *  IO[10]  - I     Not Used
     *  IO[11]  - I     Not Used
     *  IO[12]  - O     Not Used
     *  IO[13]  - O     Not Used
     *  IO[14]  - O     Not Connected on PCAL9555
     *  IO[15]  - O     Not Connected on PCAL9555
     */

     // Initialize polarity and configuration { RegPolarity, val, val, val, val } - address is autoincreament
    uint8_t ue101_config[] = { 
        0x04,       // Reg 4 - Polarity Port 0 - Starting address
        0b11111111, // Reg 4 val - Port 0 inputs inverted
        0b11111111, // Reg 5 val - Port 1 inputs inverted
        0b11111111, // Reg 6 val - Port 0 all inputs
        0b11111111  // Reg 7 val - Port 1 all inputs

    };
    i2c_write(I2C_UE101_ADD, ue101_config, sizeof(ue101_config));

    // initialize UE102
    /**
     * PCAL9555 is a READ_WRITE I2C SLAVE - Read datasheet of PCAL9555 for more details
     * 
     * BANK A
     *  IO[0]   - I     PLAYER 1 PUSHBUTTON LAMP
     *  IO[1]   - I     PLAYER 2 PUSHBUTTON LAMP
     *  IO[2]   - I     PLAYER 3 PUSHBUTTON LAMP
     *  IO[3]   - I     PLAYER 4 PUSHBUTTON LAMP
     *  IO[4]   - I     PLAYER 5 PUSHBUTTON LAMP
     *  IO[5]   - I     PLAYER 6 PUSHBUTTON LAMP
     *  IO[6]   - I     Not Connected on PCAL9555
     *  IO[7]   - I     Not Connected on PCAL9555
     * 
     * BANK B
     *  IO[8]   - I     PLAYER 1 FRONT INDICATOR
     *  IO[9]   - I     PLAYER 2 FRONT INDICATOR
     *  IO[10]  - I     PLAYER 3 FRONT INDICATOR
     *  IO[11]  - I     PLAYER 4 FRONT INDICATOR
     *  IO[12]  - O     PLAYER 5 FRONT INDICATOR
     *  IO[13]  - O     PLAYER 6 FRONT INDICATOR
     *  IO[14]  - O     Not Connected on PCAL9555
     *  IO[15]  - O     Not Connected on PCAL9555
     */

     // Initialize UE102 configuration { RegConfiguration, val, val, val, val } - address is autoincreament
    uint8_t ue102_config[] = { 
        0x06,       // Reg 6 - Configuration Port 0 - Starting address
        0b00000000, // Reg 6 val - Port 0 all outputs
        0b00000000  // Reg 7 val - Port 1 all outputs

    };
    i2c_write(I2C_UE102_ADD, ue102_config, sizeof(ue102_config));

}

// Write -> SX1508
void i2c_write(uint8_t addr, uint8_t data[], uint8_t size)
{
    i2c_master_write_to_device(
        I2C_MASTER_NUM,
        addr,
        data, size,
        I2C_MASTER_TIMEOUT
    );
}

// Write and Read <-> SX1508
void i2c_write_read(uint8_t addr, uint8_t wdata[], uint8_t wsize, uint8_t rdata[], uint8_t rsize)
{
    i2c_master_write_read_device(
        I2C_MASTER_NUM,
        addr,
        wdata, wsize,
        rdata, rsize,
        I2C_MASTER_TIMEOUT
    );
}

// for writing single bit on i2c SX1508
void i2c_write_bit(uint8_t addr, uint8_t bit, bool val)
{
    //uint8_t* read = malloc(sizeof(uint16_t));
    uint8_t* read = malloc(sizeof(uint8_t));

    i2c_write_read(addr, (uint8_t[]) { 0x08 }, 1, read, 1);

    //printf("SX1508- %x\n", *read);
    if (val)
    {
        *read |= (1<<bit);
    }
    else
    {
        *read &= ~(1<<bit);
    }

    //printf("SX1508 - %x\n", *read);

    i2c_write(addr, (uint8_t[]) {0x08, *read }, 2);

    free(read);
}
