#ifndef USBSERIAL
#define USBERIAL


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <libopencm3/cm3/nvic.h>


#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>

static usbd_device *usbd_devi;






//internal
void usb_init(void);
const usbd_driver * usb_driver(void);



//external
void usbSetup(void);

void usbInLoop(void);





#endif