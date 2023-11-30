#include "irline.h"

uint32_t lineTimings[BUFFER_SIZE];
uint32_t elapsedTimes[BUFFER_SIZE - 1];
char barcodeReading[3];
int timer_index = 0;

float adc_rolling_avg;
float adc_rolling_sum = 0;

// Rolling window to average the current adc reading
static float data[10] = {};
static int rolling_index = 0;
static int rolling_count = 0;

// This is the function to get the time differences between the time taken from each color change in the timer interrupt
// 
void getElapsedTimes(uint32_t arr[]) {
    for (int i = 0; i < BUFFER_SIZE - 1; i++){
        elapsedTimes[i] = arr[i+1] - arr[i];
    }
}

// As code39 barcodes only have 3 wide bars, just have to find the top 3 timings, this function finds the top three timings from a given array
//
void findTopThree(uint32_t arr[], int *max1, int *max2, int *max3) {
    *max1 = 0;
    *max2 = 0;
    *max3 = 0;

    for (int i = 0; i < BUFFER_SIZE; i++){
        if (arr[i] > *max1){
          *max3 = *max2;
          *max2 = *max1;
          *max1 = arr[i];
        }
        else if (arr[i] > *max2){
           *max3 = *max2;
           *max2 = arr[i];
        }
        else if (arr[i] > *max3){
           *max3 = arr[i];
        }
    }
}

// After getting the pulse widths of the barcode, we will then need to convert this into a simpler to read binary format, 1 for wide bars and 0 for small bars
//
void convertTimingToBin(uint32_t arr[], int max1, int max2, int max3, int binBarcode[]) {
    for (int i = 0; i < BUFFER_SIZE; i++){
        if(arr[i] == max1 || arr[i] == max2 || arr[i] == max3){
            binBarcode[i] = 1;
        }
        else{
            binBarcode[i] = 0;
        }
    }
}

// This function contains all the characters in the code39 bacode and decodes the barcode based on the binary barcode given
//
char decodeCode39(int binaryArray[9]) {
    Code39Mapping code39Mappings[] = {
        {{0, 1, 0, 0, 1, 0, 1, 0, 0}, '*'},
        {{1, 0, 0, 0, 0, 1, 0, 0, 1}, 'A'},
        {{0, 0, 1, 0, 0, 1, 0, 0, 1}, 'B'},
        {{1, 0, 1, 0, 0, 1, 0, 0, 0}, 'C'},
        {{0, 0, 0, 0, 1, 1, 0, 0, 1}, 'D'},
        {{1, 0, 0, 0, 1, 1, 0, 0, 0}, 'E'},
        {{0, 0, 1, 0, 1, 1, 0, 0, 0}, 'F'},
        {{0, 0, 0, 0, 0, 1, 1, 0, 1}, 'G'},
        {{1, 0, 0, 0, 0, 1, 1, 0, 0}, 'H'},
        {{0, 0, 1, 0, 0, 1, 1, 0, 0}, 'I'},
        {{0, 0, 0, 0, 1, 1, 1, 0, 0}, 'J'},
        {{1, 0, 0, 0, 0, 0, 0, 1, 1}, 'K'},
        {{0, 0, 1, 0, 0, 0, 0, 1, 1}, 'L'},
        {{1, 0, 1, 0, 0, 0, 0, 1, 0}, 'M'},
        {{0, 0, 0, 0, 1, 0, 0, 1, 1}, 'N'},
        {{1, 0, 0, 0, 1, 0, 0, 1, 0}, 'O'},
        {{0, 0, 1, 0, 1, 0, 0, 1, 0}, 'P'},
        {{0, 0, 0, 0, 0, 0, 1, 1, 1}, 'Q'},
        {{1, 0, 0, 0, 0, 0, 1, 1, 0}, 'R'},
        {{0, 0, 1, 0, 0, 0, 1, 1, 0}, 'S'},
        {{0, 0, 0, 0, 1, 0, 1, 1, 0}, 'T'},
        {{1, 1, 0, 0, 0, 0, 0, 0, 1}, 'U'},
        {{0, 1, 1, 0, 0, 0, 0, 0, 1}, 'V'},
        {{1, 1, 1, 0, 0, 0, 0, 0, 0}, 'W'},
        {{0, 1, 0, 0, 1, 0, 0, 0, 1}, 'X'},
        {{1, 1, 0, 0, 1, 0, 0, 0, 0}, 'Y'},
        {{0, 1, 1, 0, 1, 0, 0, 0, 0}, 'Z'},
        {{0, 1, 0, 0, 0, 0, 1, 0, 1}, '-'},
        {{1, 1, 0, 0, 0, 0, 1, 0, 0}, '.'},
        {{0, 1, 0, 1, 0, 1, 0, 0, 0}, '$'},
        {{0, 1, 0, 1, 0, 0, 0, 1, 0}, '/'},
        {{0, 1, 0, 0, 0, 1, 0, 1, 0}, '+'},
        {{0, 0, 0, 1, 0, 1, 0, 1, 0}, '%'},
        {{0, 0, 0, 1, 1, 0, 1, 0, 0}, '0'},
        {{1, 0, 0, 1, 0, 0, 0, 0, 1}, '1'},
        {{0, 0, 1, 1, 0, 0, 0, 0, 1}, '2'},
        {{1, 0, 1, 1, 0, 0, 0, 0, 0}, '3'},
        {{0, 0, 0, 1, 1, 0, 0, 0, 1}, '4'},
        {{1, 0, 0, 1, 1, 0, 0, 0, 0}, '5'},
        {{0, 0, 1, 1, 1, 0, 0, 0, 0}, '6'},
        {{0, 0, 0, 1, 0, 0, 1, 0, 1}, '7'},
        {{1, 0, 0, 1, 0, 0, 1, 0, 0}, '8'},
        {{0, 0, 1, 1, 0, 0, 1, 0, 0}, '9'},
    };
    
    // Go through the mapping array defined above
    for (int i = 0; i < sizeof(code39Mappings) / sizeof(code39Mappings[0]); i++){
        bool match = true;

        for (int j = 0; j < 9; j++){
            if (code39Mappings[i].binary[j] != binaryArray[j]){
                match = false;
                break; // Binary doesn't match, no need to keep checking the rest of the array
            }
        }
        
        // If match is still true, means that the character matches every binary in the array, return the coressponding character
        if (match){
            return code39Mappings[i].character;
        }
    }
    // If no match, return a ? as no character found, can be changed to other messages in the future.
    return '?';
}

// Reverses the binary array based on the 2 arrays given
void reverseBinaryArray(int binaryArray[9], int reversedArray[9]) {
    int start = 0;
    int end = 8;
    
    while (start < end) {
        reversedArray[start] = binaryArray[end];
        start++;
        end--;
    }
}

// This is a timer interrupt that is triggered to check the current line value for the barcode
// 
bool timer_callback(struct repeating_timer *t) {
    static bool color_changed = true; // This is a flag to check whether the color has been changed or not

    // Selecting the IR sensor based on the ADC graph
    adc_select_input(0);
    // Reads the current value from the IR sensor
    uint16_t adc_value = adc_read();

    adc_rolling_sum -= data[rolling_index];
    data[rolling_index] = adc_value;
    adc_rolling_sum += data[rolling_index];
    rolling_index = (rolling_index + 1) % 10;

    if (rolling_count < 10) {
        rolling_count++;
    }

    adc_rolling_avg = adc_rolling_sum / rolling_count;

    // Checks if the line is black, means there is a barcode
    if (adc_rolling_avg >= LINE_THRESHOLD){
        if (color_changed){
            color_changed = false;
            lineTimings[timer_index] = time_us_64();
            timer_index++;
            printf("Color changed.\n");
        }
    }

    else{
        if (!color_changed){
            color_changed = true;
            lineTimings[timer_index] = time_us_64();
            timer_index++;
            printf("Color changed.\n");
        }
    }

    // One 9 element character has been read and can be now decoded and returned
    if (timer_index >= BUFFER_SIZE) {
        int max1, max2, max3;
        bool reversed = false;
        int binBarcode[9];
        int reversedBinBarcode[9];
        getElapsedTimes(lineTimings); // Get timing differences between each color change in the barcode

        findTopThree(elapsedTimes, &max1, &max2, &max3); // Find the three wide barcodes

        convertTimingToBin(elapsedTimes, max1, max2, max3, binBarcode); // Convert the timings into binary format for easier viewing

        char charBarcode = decodeCode39(binBarcode); // decode the barcode into a character based on the Code 39 standard

        reverseBinaryArray(binBarcode, reversedBinBarcode); // Reverse the barcode so that able to read barcode backwards as well
        char reversedCharBarcode = decodeCode39(reversedBinBarcode); // decode the barcode into a character based on the Code 39 standard

        // Check if it's the starting or ending asterisk
        if (charBarcode == '*') {
            if (barcodeReading[0] == '*') {
                printf("Valid Barcode\n");
                printf("%c\n", barcodeReading[1]);
            }
            else {
                printf("First Asterisk detected\n");
                barcodeReading[0] = '*';
            }
        }

        // Does the same for the reversed character
        else if (reversedCharBarcode == '*'){
            reversed = true;
            if (barcodeReading[0] == '*') {
                printf("Valid Barcode\n");
                printf("%c\n", barcodeReading[1]);
            }
            else {
                printf("Reversed Asterisk detected\n");
                barcodeReading[0] = '*';
            }
        }

        // It's a valid character
        else {
            // This means that after scanning a character, another character was scanned, thus invalid barcode
            if (barcodeReading[1] != '\0') {
                // cancel_repeating_timer(t);
                printf("Invalid char\n");
            }
            // Check if it's the forward or reversed reading
            else if (!reversed) {
                barcodeReading[1] = charBarcode;
            }
            else {
                barcodeReading[1] = reversedCharBarcode;
            }
        }

        for (int i = 0; i < BUFFER_SIZE - 1; i++) {
            printf("%d ", binBarcode[i]);
            // printf("Elapsed Time %d: %lu us\n", i, elapsed_times[i]);
        }
        printf("\n");
        for (int i = 0; i < BUFFER_SIZE - 1; i++) {
            printf("%d ", reversedBinBarcode[i]);
            // printf("Elapsed Time %d: %lu us\n", i, elapsed_times[i]);
        }
        printf("\n");
        printf("\nChar: %c\n", charBarcode);
        printf("\nReversed Char: %c\n", reversedCharBarcode);
        printf("End of one char\n");
        timer_index = 0;
        color_changed = true;
    }
    return true;
}

// This function takes in the ADC input number based on the PICO W and returns the ADC reading back
int detectLine(int adc_input){
    adc_select_input(adc_input);
    uint16_t adc_value = adc_read();
    printf("ADC Value: %d\n", adc_value);
    return adc_value;
}

// This is the function to scan the barcode during actual runs
int scan_barcode() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(28);

    struct repeating_timer timer;

    add_repeating_timer_ms(1, timer_callback, NULL, &timer);

    while (true) {
        // detectLine();
    }

    return 0;
}
