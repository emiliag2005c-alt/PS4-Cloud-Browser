#include <orbis/libkernel.h>

/*
 * PS4 Cloud Browser 0.1
 *
 * This is intentionally a boot-test skeleton. It keeps the process alive so
 * we can validate package installation and launching before adding graphics,
 * controller input, networking, or a web engine.
 */
int main(void)
{
    for (;;) {
        sceKernelUsleep(1000000);
    }

    return 0;
}
