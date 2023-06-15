#include "usb_lib.h"
#include <wchar.h>
#include "hardware.h"
#include "midi_message_buffer.h"
#include "sysex.h"

#define ENDP_DATA_IN 1
#define ENDP_DATA_OUT 2
#define ENDP_DATA_SIZE 16

#define ENDP_CTL_NUM  3
#define ENDP_CTL_SIZE 8

#define STD_DESCR_LANG 0
#define STD_DESCR_VEND 1
#define STD_DESCR_PROD 2
#define STD_DESCR_SN   3

#define USB_VID 0x16C0
#define USB_PID 0x05DF

#define DEVCLASS_AUDIO        0x01
#define DEVSUBCLASS_MIDI      0x03
#define CDCCLASS_DATA         0x0A
#define CDCPROTOCOL_UNDEF     0x00
#define CDCPROTOCOL_VENDOR    0xFF

USB_ALIGN static const uint8_t USB_DeviceDescriptor[] = {
  ARRLEN1(
  bLENGTH,     // bLength
  USB_DESCR_DEVICE,   // bDescriptorType - Device descriptor
  USB_U16(0x0110), // bcdUSB
  0,   // bDevice Class
  0,   // bDevice SubClass
  0,   // bDevice Protocol
  USB_EP0_BUFSZ,   // bMaxPacketSize0
  USB_U16( USB_VID ), // idVendor
  USB_U16( USB_PID ), // idProduct
  USB_U16( 1 ), // bcdDevice_Ver
  STD_DESCR_VEND,   // iManufacturer
  STD_DESCR_PROD,   // iProduct
  STD_DESCR_SN,   // iSerialNumber
  1    // bNumConfigurations
  )
};

USB_ALIGN static const uint8_t USB_DeviceQualifierDescriptor[] = {
  ARRLEN1(
  bLENGTH,     //bLength
  USB_DESCR_QUALIFIER,   // bDescriptorType - Device qualifier
  USB_U16(0x0200), // bcdUSB
  0,   // bDeviceClass
  0,   // bDeviceSubClass
  0,   // bDeviceProtocol
  USB_EP0_BUFSZ,   // bMaxPacketSize0
  1,   // bNumConfigurations
  0x00    // Reserved
  )
};

USB_ALIGN static const uint8_t USB_ConfigDescriptor[] = {
  ARRLEN34(
  ARRLEN1(
    bLENGTH, // bLength: Configuration Descriptor size
    USB_DESCR_CONFIG,    //bDescriptorType: Configuration
    wTOTALLENGTH, //wTotalLength
    2, // bNumInterfaces
    1, // bConfigurationValue: Configuration value
    0, // iConfiguration: Index of string descriptor describing the configuration
    0x80, // bmAttributes: bus powered
    0x32, // MaxPower 100 mA
  )
  ARRLEN1(//CDC control Interface
    bLENGTH, // bLength
    USB_DESCR_INTERFACE, // bDescriptorType
    0, // bInterfaceNumber
    0, // bAlternateSetting
    1, // bNumEndpoints
    DEVCLASS_AUDIO, // bInterfaceClass: 
    DEVSUBCLASS_MIDI, // bInterfaceSubClass: 
    CDCPROTOCOL_UNDEF, // bInterfaceProtocol: 
    0x00, // iInterface
  )
    ARRLEN1(//MS Header
      bLENGTH, //bLength
      USB_DESCR_CS_INTERFACE, //bDescriptorType
      0x01, //bDescriptorSubType
      USB_U16(0x0100), //bcdMIDI
      USB_U16(0x002C), //wTotalLength
    )
    ARRLEN1( //MIDI In Jack descriptor
      bLENGTH,
      USB_DESCR_CS_INTERFACE,
      0x02, ///bDescriptorSubType - MIDI In Jack
      0x01, //bJackType
      0x01, //bJackID
      0, //iJack
    )
    ARRLEN1( //MIDI Out Jack Descriptor
      bLENGTH,
      USB_DESCR_CS_INTERFACE,
      0x03, //bDescriptorSubType
      0x01, //bJackType
      0x02, //bJackID
      0x01, //bNrInputPins
      0x02, //BaSourceID
      0x01, //BaSourcePin
      0, //iJack
    )
    ARRLEN1(//Standard Bulk IN endpoint descriptor
      bLENGTH, //bLength
      USB_DESCR_ENDPOINT, //bDescriptorType
      0x81,
      USB_ENDP_INTR,
      USB_U16(ENDP_DATA_SIZE),
      0x0A, //bInterval
      0, //bRefresh
      0, //bSyncAddress
    )
    ARRLEN1(//MS Class specific endpoint descriptor
      bLENGTH, //bLength
      0x25,  //bDescriptorType
      0x01,  //bDescriptorSubtype
      0x01,  //bNumEmbMIDIJack (0)
      0x01,  //baAssocJackID (0)
    )
    ARRLEN1(//Standard Bulk OUT endpoint descriptor
      bLENGTH, //bLength
      USB_DESCR_ENDPOINT, //bDescriptorType
      0x01,
      USB_ENDP_INTR,
      USB_U16(ENDP_DATA_SIZE),
      0x0A, //bInterval
      0, //bRefresh
      0, //bSyncAddress
    )
    ARRLEN1(//MS Class specific endpoint descriptor
      bLENGTH, //bLength
      0x25,  //bDescriptorType
      0x01,  //bDescriptorSubtype
      0x01,  //bNumEmbMIDIJack (0)
      0x02,  //baAssocJackID (0)
    )
  )
};

USB_STRING(USB_StringLangDescriptor, u"\x0409"); //lang US
USB_STRING(USB_StringManufacturingDescriptor, u"piotrosiotr"); //Vendor
USB_STRING(USB_StringProdDescriptor, u"USB MIDI"); //Product
USB_STRING(USB_StringSerialDescriptor, u"1"); //Serial (BCD)

void usb_class_get_std_descr(uint16_t descr, const void **data, uint16_t *size){
  switch(descr & 0xFF00){
    case DEVICE_DESCRIPTOR:
      *data = &USB_DeviceDescriptor;
      *size = sizeof(USB_DeviceDescriptor);
      break;
    case CONFIGURATION_DESCRIPTOR:
      *data = &USB_ConfigDescriptor;
      *size = sizeof(USB_ConfigDescriptor);
      break;
    case DEVICE_QUALIFIER_DESCRIPTOR:
      *data = &USB_DeviceQualifierDescriptor;
      *size = USB_DeviceQualifierDescriptor[0];
      break;
    case STRING_DESCRIPTOR:
      switch(descr & 0xFF){
        case STD_DESCR_LANG:
          *data = &USB_StringLangDescriptor;
          break;
        case STD_DESCR_VEND:
          *data = &USB_StringManufacturingDescriptor;
          break;
        case STD_DESCR_PROD:
          *data = &USB_StringProdDescriptor;
          break;
        case STD_DESCR_SN:
          *data = &USB_StringSerialDescriptor;
          break;
        default:
          return;
      }
      *size = ((uint8_t*)*data)[0]; //data->bLength
      break;
    default:
      break;
  }
}

uint8_t out_buf_data[4*4];
midi_msg_buffer_t out_buf;

char usb_class_ep0_in(config_pack_t *req, void **data, uint16_t *size){
  if( (req->bmRequestType & 0x7F) == (USB_REQ_CLASS | USB_REQ_INTERFACE) ){
    
  }
  return 0;
}

char usb_class_ep0_out(config_pack_t *req, uint16_t offset, uint16_t rx_size){
  if( (req->bmRequestType & 0x7F) == (USB_REQ_CLASS | USB_REQ_INTERFACE) ){
    
  }
  return 0;
}

void ctl_callback(uint8_t epnum){
  
}

void data_out_callback(uint8_t epnum){
  USB_ALIGN uint8_t buf[ ENDP_DATA_SIZE ];
  int len = usb_ep_read_double( ENDP_DATA_OUT, (uint16_t*)buf);
  if(len == 0)return;
  if(buf[0]!= SYSEX_HEADER) return;
  if(buf[1]!= SYSEX_MY_ID) return;
  if(buf[ENDP_DATA_SIZE-1]!= SYSEX_EOM) return;
}

void data_in_callback(uint8_t epnum){
  //USB_ALIGN uint8_t b_on_cmd[] = {0x08, 0x80, 60, 127};
  //usb_ep_write_double( ENDP_DATA_IN | 0x80, (uint16_t*)b_on_cmd, sizeof(b_on_cmd));
}

void usb_class_init(){
  usb_ep_init( ENDP_CTL_NUM | 0x80, USB_ENDP_INTR, ENDP_CTL_SIZE, ctl_callback );
  
  usb_ep_init_double( ENDP_DATA_IN | 0x80, USB_ENDP_BULK, ENDP_DATA_SIZE, data_in_callback );
  usb_ep_init_double( ENDP_DATA_OUT, USB_ENDP_BULK, ENDP_DATA_SIZE, data_out_callback );
  init_message_buff(&out_buf, out_buf_data, 16);
}

void usb_class_poll(){
    while(out_buf.bytes_avail>0) {
        USB_ALIGN uint8_t msg[4];
        get_next_message(&out_buf, msg, 4);
        if(msg[0] != 0x00) usb_ep_write_double( ENDP_DATA_IN | 0x80, (uint16_t*)msg, sizeof(msg));
    }
}
