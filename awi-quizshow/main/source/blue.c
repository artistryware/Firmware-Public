
#include "blue.h"
#include "i2c.h"
#include "logic.h"

// address of device base on dip switch
//#define ADDRESS gpio_get_level(GPIO_NUM_34) << 0 | gpio_get_level(GPIO_NUM_25) << 1 | gpio_get_level(GPIO_NUM_26) << 2 | gpio_get_level(GPIO_NUM_27) << 3

#define SPP_TAG "SPP_ACCEPTOR"
#define SPP_SERVER_NAME "SPP_SERVER"
#define DEVICE_NAME "AWI_QUIZ_SHOW"

static const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;
static const esp_spp_sec_t sec_mask = ESP_SPP_SEC_AUTHENTICATE;
static const esp_spp_role_t role_slave = ESP_SPP_ROLE_SLAVE;


uint8_t ADDRESS = 0x00;

// define this variable here
// this variable can be use on other source file
TaskHandle_t btHandler = NULL;

//static uint32_t* handleConn;
static uint32_t gHandle;

// device name, need to concatinate the ADDRESS that way there is another variable for device name
static char device_name[] = DEVICE_NAME;

// SPP confugration callback, when initializing the SPP, the esp_spp_cb callback will called by the esp_spp_register_callback
static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    //printf("\n SPP EVENT - %d\n", event);
    switch (event)
    {
        case ESP_SPP_INIT_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
            esp_spp_start_srv(sec_mask, role_slave, 0, SPP_SERVER_NAME);
            break;
        }
        case ESP_SPP_DISCOVERY_COMP_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT");
            break;
        }
        case ESP_SPP_OPEN_EVT:
        {
            printf("SPP HANDLER %ld\n", param->open.handle);
            ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT");
            break;
        }
        case ESP_SPP_CLOSE_EVT:
        {
            // turn off blue led for bluetooth
            i2c_write_led(BLU, LED_OFF);
            
            ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT");
            break;
        }
        case ESP_SPP_START_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_START_EVT");
            //esp_bt_dev_set_device_name(DEVICE_NAME);
            esp_bt_gap_set_device_name(device_name);
            // this will not broadcast the NAME of the BL device
            //esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
            esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
            break;
        }
        case ESP_SPP_CL_INIT_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_CL_INIT_EVT");
            break;
        }
        case ESP_SPP_DATA_IND_EVT:
        {
            uint32_t val = 0;

            memcpy(&val, param->data_ind.data, param->data_ind.len);

            xTaskNotify(btHandler, val, eSetValueWithOverwrite);

            break;
        }
        case ESP_SPP_CONG_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_CONG_EVT");
            break;
        }
        case ESP_SPP_WRITE_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_WRITE_EVT");
            break;
        }
        case ESP_SPP_SRV_OPEN_EVT:
        {
            gHandle = param->open.handle;

            // turn on blue led for bluetooth
            i2c_write_led(BLU, LED_ON);

            // initially send current player when BT connects
            //SENDPLAYER;
            send_player();

            ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT");
            break;
        }
        case ESP_SPP_SRV_STOP_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_STOP_EVT");
            break;
        }
        case ESP_SPP_UNINIT_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_SPP_UNINIT_EVT");
            break;
        }
        default:
        {
            break;
        }
    }
}


// GAP configuration callback
static void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    //printf("\n GAP EVENT - %d\n", event);
    switch (event)
    {
        case ESP_BT_GAP_AUTH_CMPL_EVT:
        {
            if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS)
            {
                ESP_LOGI(SPP_TAG, "authentication success: %s", param->auth_cmpl.device_name);
                ESP_LOG_BUFFER_HEX(SPP_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
            }
            else
            {
                ESP_LOGE(SPP_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
            }
            break;
        }

        case ESP_BT_GAP_PIN_REQ_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
            if (param->pin_req.min_16_digit)
            {
                ESP_LOGI(SPP_TAG, "Input pin code: 0000 0000 0000 0000");
                esp_bt_pin_code_t pin_code = {0};
                esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
            }
            else
            {
                ESP_LOGI(SPP_TAG, "Input pin code: 1234");
                esp_bt_pin_code_t pin_code;
                pin_code[0] = '1';
                pin_code[1] = '2';
                pin_code[2] = '3';
                pin_code[3] = '4';
                esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
            }
            break;
        }

#if (CONFIG_BT_SSP_ENABLED == true)
        case ESP_BT_GAP_CFM_REQ_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", param->cfm_req.num_val);
            esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
            break;
        }

        case ESP_BT_GAP_KEY_NOTIF_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
            break;
        }

        case ESP_BT_GAP_KEY_REQ_EVT: 
        {
            ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
            break;
        }
#endif

        case ESP_BT_GAP_MODE_CHG_EVT:
        {
            //ESP_LOGI(SPP_TAG, "ESP_BT_GAP_MODE_CHG_EVT mode:%d", param->mode_chg.mode);
            break;
        }

        // not important to put GAP_CONFIG_EIR_DATA
        case ESP_BT_GAP_CONFIG_EIR_DATA_EVT:
        {
            ESP_LOGI(SPP_TAG, "ESP_BT_GAP_READ_RSSI_DELTA_EVT mode:%d", param->config_eir_data.stat);
            break;
        }

        default:
        {
            ESP_LOGI(SPP_TAG, "event: %d", event);
            break;
        }
    }

    return;
}

// Initialize bluetooth and SPP
void bt_init() 
{
    // variable for holding error key value pair
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    // release the bt controller memory
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    // initialize bt controller to default config
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    // determine if bt controller initialized
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // GAP is an acronym for the Generic Access Profile, and it controls connections and advertising in Bluetooth.
    // GAP is what makes your device visible to the outside world, and determines how two devices can (or can't) interact with each other.
    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s gap register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // register the SPP callback
    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    //esp_bt_dev_set_device_name(DEVICE_NAME);
    //esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

    // initialize the SPP, when initializing the SPP, the esp_spp_cb callback will called by the esp_spp_register_callback
    if ((ret = esp_spp_init(esp_spp_mode)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s spp init failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

#if (CONFIG_BT_SSP_ENABLED == true)
    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
#endif

    /*
     * Set default parameters for Legacy Pairing
     * Use variable pin, input pin code when pairing
     */
    //esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
    //esp_bt_pin_code_t pin_code;
    //esp_bt_gap_set_pin(pin_type, 0, pin_code);
}

// send data to remote
void blue_send(uint8_t* data)
{
    esp_spp_write(gHandle, sizeof(data), data);
}