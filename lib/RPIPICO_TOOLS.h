#ifndef RPIPICO_TOOLS
#define RPIPICO_TOOLS

void blinkOn() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
}

void blinkOff() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
}

void moveValues(float *data,int size,float newf) {
    for (int i = 1;i<size;i++) {
        data[i-1]=data[i];
    }
    data[size-1] = newf;
} 

float processing_LOWESS (float *data,int size,int center) {
    float value = 0.0;
    float mass = 0.0;
    int local = (size-1)/2;
    for (int i = 0 ; i < size ; i++) {
        int weight = (abs(center-i)>local) ? local+1-abs(center-i)%local : abs(center-i);
        mass += (float)weight;
        value += data[i]*(float)weight;
    }
    return value /  mass;
}

#endif