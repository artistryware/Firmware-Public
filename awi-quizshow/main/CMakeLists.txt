idf_component_register(SRCS "quiz.c" "source/gpio.c" "source/i2c.c" "source/blue.c" "source/logic.c"
                        INCLUDE_DIRS "." "header"
                        REQUIRES esp_bt)
