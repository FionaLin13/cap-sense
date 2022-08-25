#include <mbed.h>



int main(void) {
 BufferedSerial pc(USBTX, USBRX, 9600);

    while (true) {

        // Switch to next state
        char input;
        ssize_t len = pc.read(&input, 1);
        if (len > 0) {
            switch (input) {
                case '1':
                    pc.write("u", 1);
                    break;
                case '2':
                    pc.write("d", 1);
                    break;
                case '3':
                    pc.write("c", 1);
                    break;
                case '4':
                    pc.write("i", 1);
                    break;
                case '0':
                    pc.write("s", 1);
                    break;
                default:
                    printf("[ERROR, %d] Not a command!\n", __LINE__);
                    break;
            }
        }
    }
    return 0;
}