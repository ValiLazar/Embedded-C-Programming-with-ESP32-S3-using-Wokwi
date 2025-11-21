#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class Button {
  private:
    gpio_num_t pin;
    int lastState;
  public:
    Button(int p)
    {
      pin = (gpio_num_t) p;

      gpio_reset_pin(pin);
      gpio_set_direction(pin, GPIO_MODE_INPUT);
      gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);

      lastState = 1;
    }

    bool wasPressed() {
      int currentState = gpio_get_level(pin);
      bool detection = false;
      if (currentState == 0 && lastState == 1)
      {
        detection = true;
      }
      else
      {
        detection = false;
      }

      lastState = currentState;

      return detection;
    }
};


class Led {
private:
    gpio_num_t pin;
public:
    Led(int p) {
        pin = (gpio_num_t)p;

        gpio_reset_pin(pin);
        gpio_set_direction(pin, GPIO_MODE_OUTPUT);
        gpio_set_level(pin, 0);
    }

    void on() { gpio_set_level(pin, 1); }
    void off() { gpio_set_level(pin, 0); }
};


void setup() {

    Led rosu(1);
    Led galben(2);
    Led verde(42);
    Button button(0);
    Led pieton_rosu(4);
    Led pieton_verde(5);

    printf("Semafor:\n");

    while(true) {

        printf("Culoare masini: VERDE Culoare pietoni: ROSU\n");
        rosu.off();
        galben.off();
        verde.on();
        pieton_rosu.on();
        pieton_verde.off();

        if (button.wasPressed())
        {
          printf("Buton Apasat\n");

        printf("Culoare masini: ROSU Culoare pietoni: VERDE\n" );
        rosu.on();
        galben.off();
        verde.off();
        pieton_rosu.off();
        pieton_verde.on();
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        printf("Culoare masini: GALBEN Culoare pietoni: ROSU\n");
        rosu.off();
        galben.on();
        verde.off();
        pieton_rosu.on();
        pieton_verde.off();

        for (int i = 0; i < 3; i ++)
        {
          vTaskDelay(500 / portTICK_PERIOD_MS);
          galben.on();
          vTaskDelay(500 / portTICK_PERIOD_MS);
          galben.off();
        }


        printf("Culoare masini: VERDE Culoare pietoni: ROSU\n");
        rosu.off();
        galben.off();
        verde.on();
        pieton_rosu.on();
        pieton_verde.off();

        printf("TIMP VERDE PENTRU MASINI = \n" );
        for(int k = 5; k > 0; k --)
        {
          printf("%d SECUNDE \n", k);
          vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        printf("Acum merge sa apesi butonul");

        }
            vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}

void loop() {}