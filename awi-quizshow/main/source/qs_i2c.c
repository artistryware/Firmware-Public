
#include "qs_i2c.h"

i2c_master_bus_handle_t     busHandle;
i2c_master_dev_handle_t     dev1508Handle;
i2c_master_dev_handle_t     devUE101Handle;
i2c_master_dev_handle_t     devUE102Handle;

/**
 * Initialize I2C master bus and configure devices
 *
 */ 
void i2c_init()
{
    // Initialize I2C Master
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = PORT_NUMBER,
        .sda_io_num = SDA_IO_PIN,
        .scl_io_num = SCL_IO_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup =  false
    };
    i2c_master_bus_handle_t busHandle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &busHandle));

    // initialize SX1508B
    /**
     * SX1508B is a READ_WRITE I2C SLAVE - Read datasheet of SX1508 for more details
     *  IO[0]   - I     ORG
     *  IO[1]   - I     RED
     *  IO[2]   - I     GRN
     *  IO[3]   - I     BLU
     *  IO[4]   - I     Board ID bit 0
     *  IO[5]   - I     Board ID bit 1
     *  IO[6]   - I     Board ID bit 2
     *  IO[7]   - I     Board ID bit 3
     */

     i2c_device_config_t dev_1508_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_1508_ADDR,
        .scl_speed_hz = I2C_DEVICE_SPEED,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle, &dev_1508_cfg, &dev1508Handle));

    // Initialize pullup on Input { RegPullUp, val } - address is autoincreament
    uint8_t pullval[] = { 
        REG_1508_PULL_UP,       // Reg 03 - Starting address
        0b11110000              // Enable pull-ups for pins 4–7, disable for pins 0–3
    };
    i2c_write(dev1508Handle, pullval, sizeof(pullval));

    // Initialize Input/Ouput and Interrupt - { RegOpenDrain, RegPolarity, RegDir, RegData, RegInterruptMask, RegSenseHigh, RegSenseLow } - address is autoincrement
    uint8_t ioinitval[] = { 
        REG_1508_OPEN_DRAIN,    // Reg 05 - Starting address - address is autoincreament
        0b00000000,             // RegOpenDrain Val - Open Drain 0,1,2,3 - Push/Pull 4,5,6,7
        0b11110000,             // RegPolarity val - Invert Inputs 4,5,6,7
        0b11110000,             // RegDir val - OUT=[0,1,2,3]; IN=[4,5,6,7]
        0xff,                   // RegData val - default 0xff
        0b00001111,             // RegInterruptMask val - interrupt on [4,5,6,7]
        0b11111111,             // RegSenseHigh val - both rising and falling [4,5,6,7]
        0b00000000              // RegSenseLow val - none [0,1,2,3]
    };
    i2c_write(dev1508Handle, ioinitval, sizeof(ioinitval));

    // Initialize Register Clock, use for debouncing
    uint8_t clkval[] = { REG_1508_CLOCK, 0b01000000 };
    i2c_write(dev1508Handle, clkval, sizeof(clkval));

    // Initialize Misc, LED driver, and Debounce Registers
    uint8_t miscval[] = { 
        REG_1508_MISC,  // Reg 10 - Starting address - address is autoincreament
        0b00010000,     // RegMisc value
        0b00001111,     // RegLEDDriverEnable value
        0b00000101,     // RegDebounceConfig value
        0b11110000      // RegDebounceEnable value
    };
    i2c_write(dev1508Handle, miscval, sizeof(miscval));

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
    i2c_write(dev1508Handle, (uint8_t[]) { REG_1508_I_ON_0, 0x10 }, 2); // set ORG intensity
    i2c_write(dev1508Handle, (uint8_t[]) { REG_1508_I_ON_1, 0x07 }, 2); // set RED intensity
    i2c_write(dev1508Handle, (uint8_t[]) { REG_1508_I_ON_2, 0x07 }, 2); // set GRN intensity
    i2c_write(dev1508Handle, (uint8_t[]) { REG_1508_I_ON_3, 0x40 }, 2); // set BLU intensity

    // initialy turn on all LEDs for test
    i2c_write(dev1508Handle, (uint8_t[]) { REG_1508_DATA, 0b00000000 }, 2);

    vTaskDelay(pdMS_TO_TICKS(500));    // Delay to check LEDs functioning

    // after test, turn on green led only
    i2c_write(dev1508Handle, (uint8_t[]) { REG_1508_DATA, 0b00001011 }, 2);

    // initialize UE101
    /**
     * PCAL9555 is a READ_WRITE I2C SLAVE - Read datasheet of PCAL9555 for more details
     * BANK A
     *  IO[0]   - I     PLAYER 1 PUSHBUTTON
     *  IO[1]   - I     PLAYER 2 PUSHBUTTON
     *  IO[2]   - I     PLAYER 3 PUSHBUTTON
     *  IO[3]   - I     PLAYER 4 PUSHBUTTON
     *  IO[4]   - I     PLAYER 5 PUSHBUTTON
     *  IO[5]   - I     PLAYER 6 PUSHBUTTON
     *  IO[6]   - I     Not Connected on PCAL9555
     *  IO[7]   - I     Not Connected on PCAL9555
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

     i2c_device_config_t dev_UE101_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_UE101_ADDR,
        .scl_speed_hz = I2C_DEVICE_SPEED
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle, &dev_UE101_cfg, &devUE101Handle));

     // Initialize polarity and configuration { RegPolarity, val, val, val, val } - address is autoincreament
     // All other registers power up to acceptable defaults
    uint8_t ue101_config[] = { 
        REG_9555_POLARITY_0,    // Polarity Port 0 - Starting address - address is autoincreament
        0b11111111,             // Reg 4 val - Port 0 inputs inverted
        0b11111111,             // Reg 5 val - Port 1 inputs inverted
        0b11111111,             // Reg 6 val - Port 0 all inputs
        0b11111111              // Reg 7 val - Port 1 all inputs

    };
    i2c_write(devUE101Handle, ue101_config, sizeof(ue101_config));

    // initialize UE102
    /**
     * PCAL9555 is a READ_WRITE I2C SLAVE - Read datasheet of PCAL9555 for more details
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

     i2c_device_config_t dev_UE102_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_UE102_ADDR,
        .scl_speed_hz = I2C_DEVICE_SPEED
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle, &dev_UE102_cfg, &devUE102Handle));

     // Initialize UE102 configuration { RegConfiguration, val, val, val, val } - address is autoincreament
     // All other registers power up to acceptable defaults
    uint8_t ue102_config[] = { 
        REG_9555_CONFIG_0,  // Configuration Port 0 - Starting address
        0b00000000,         // Reg 6 val - Port 0 all outputs
        0b00000000          // Reg 7 val - Port 1 all outputs

    };
    i2c_write(devUE102Handle, ue102_config, sizeof(ue102_config));

}

/**
 * Perform a write operation on an I2C device on the master bus.
 * 
 * @param   device   Device handle added to master bus.
 * @param   data[]   Data to write to I2C device. Include register address and register value.
 * @param   size     Size of the data being written.
 * 
 */
void i2c_write(i2c_master_dev_handle_t device, uint8_t data[], uint8_t size)
{
    i2c_master_transmit(device, data, size, I2C_MASTER_TIMEOUT_MS);
}

/**
 * Perform a write/read operation on an I2C device on the master bus.
 * 
 * @param   device      Device handle added to master bus.
 * @param   wdata[]     Starting register address to read data from.
 * @param   wsize       Size of the register address.
 * @param   rdata[]     Variable to store the read value.
 * @param   rsize       Size of the read variable.
 * 
 */
void i2c_write_read(i2c_master_dev_handle_t device, uint8_t wdata[], uint8_t wsize, uint8_t rdata[], uint8_t rsize)
{
    i2c_master_transmit_receive(device, wdata, wsize, rdata, rsize, I2C_MASTER_TIMEOUT_MS);
}

/**
 * Perform a write operation on an I2C device on the master bus.
 * Uses a single 8 bit register.
 * 
 * @param   device  Device handle added to master bus.
 * @param   reg     Register for setting the bit.
 * @param   bit     Bit position to set/reset.
 * @param   val     Requested value of the bit.
 * 
 */
void i2c_write_bit(i2c_master_dev_handle_t device, uint8_t reg, uint8_t bit, bool val)
{
    //uint8_t* read = malloc(sizeof(uint16_t));
    uint8_t* read = malloc(sizeof(uint8_t));

    i2c_write_read(device, (uint8_t[]) { reg }, 1, read, 1);

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

    i2c_write(device, (uint8_t[]) {reg, *read }, 2);

    free(read);
}

/**
 * Turn ON/OFF the LED outputs on the SX1508
 * 
 * @param   led     LED to turn ON/OFF.
 * @param   val     Use defines LED_ON or LED_OFF.
 * 
 */
void i2c_write_led(uint8_t led, bool val) 
{
    i2c_write_bit(dev1508Handle, REG_1508_DATA, led, val);
}
