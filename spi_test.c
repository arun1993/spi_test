/*
    spidevlib.c - A user-space program to comunicate using spidev.
                Gustavo Zamboni
*/
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
 
char buf[10];
char buf2[10];
extern int com_serial;
extern int failcount;
 
struct spi_ioc_transfer xfer[2];
 
 
//////////
// Init SPIdev
//////////
int spi_init(char filename[40])
    {
        int file;
    __u8    mode, lsb, bits;
    __u32 speed=2500000;
 
        if ((file = open(filename,O_RDWR)) < 0)
        {
            printf("Failed to open the bus.");
            /* ERROR HANDLING; you can check errno to see what went wrong */
        com_serial=0;
            exit(1);
            }
 
        ///////////////
        // Verifications
        ///////////////
        //possible modes: mode |= SPI_LOOP; mode |= SPI_CPHA; mode |= SPI_CPOL; mode |= SPI_LSB_FIRST; mode |= SPI_CS_HIGH; mode |= SPI_3WIRE; mode |= SPI_NO_CS; mode |= SPI_READY;
        //multiple possibilities using |
        /*
            if (ioctl(file, SPI_IOC_WR_MODE, &mode)<0)   {
                perror("can't set spi mode");
                return;
                }
        */
 
            if (ioctl(file, SPI_IOC_RD_MODE, &mode) < 0)
                {
                perror("SPI rd_mode");
                return;
                }
            if (ioctl(file, SPI_IOC_RD_LSB_FIRST, &lsb) < 0)
                {
                perror("SPI rd_lsb_fist");
                return;
                }
        //sunxi supports only 8 bits
        /*
            if (ioctl(file, SPI_IOC_WR_BITS_PER_WORD, (__u8[1]){8})<0)   
                {
                perror("can't set bits per word");
                return;
                }
        */
            if (ioctl(file, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) 
                {
                perror("SPI bits_per_word");
                return;
                }
        /*
            if (ioctl(file, SPI_IOC_WR_MAX_SPEED_HZ, &speed)<0)  
                {
                perror("can't set max speed hz");
                return;
                }
        */
            if (ioctl(file, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) 
                {
                perror("SPI max_speed_hz");
                return;
                }
     
 
    printf("%s: spi mode %d, %d bits %sper word, %d Hz max\n",filename, mode, bits, lsb ? "(lsb first) " : "", speed);
 
    //xfer[0].tx_buf = (unsigned long)buf;
    xfer[0].len = 3; /* Length of  command to write*/
    xfer[0].cs_change = 0; /* Keep CS activated */
    xfer[0].delay_usecs = 0, //delay in us
    xfer[0].speed_hz = 2500000, //speed
    xfer[0].bits_per_word = 8, // bites per word 8
 
    //xfer[1].rx_buf = (unsigned long) buf2;
    xfer[1].len = 4; /* Length of Data to read */
    xfer[1].cs_change = 0; /* Keep CS activated */
    xfer[0].delay_usecs = 0;
    xfer[0].speed_hz = 2500000;
    xfer[0].bits_per_word = 8;
 
    return file;
    }
 
 
 
//////////
// Read n bytes from the 2 bytes add1 add2 address
//////////
 
char * spi_read(int add1,int add2,int nbytes,int file)
    {
    int status;
 
    memset(buf, 0, sizeof buf);
    memset(buf2, 0, sizeof buf2);
    buf[0] = 0x01;
    buf[1] = add1;
    buf[2] = add2;
    xfer[0].tx_buf = (unsigned long)buf;
    xfer[0].len = 3; /* Length of  command to write*/
    xfer[1].rx_buf = (unsigned long) buf2;
    xfer[1].len = nbytes; /* Length of Data to read */
    status = ioctl(file, SPI_IOC_MESSAGE(2), xfer);
    if (status < 0)
        {
        perror("SPI_IOC_MESSAGE");
        return;
        }
    //printf("env: %02x %02x %02x\n", buf[0], buf[1], buf[2]);
    //printf("ret: %02x %02x %02x %02x\n", buf2[0], buf2[1], buf2[2], buf2[3]);
 
    com_serial=1;
    failcount=0;
    return buf2;
    }
 
//////////
// Write n bytes int the 2 bytes address add1 add2
//////////
void spi_write(int add1,int add2,int nbytes,char value[10],int file)
    {
    unsigned char   buf[32], buf2[32];
    int status;
 
    memset(buf, 0, sizeof buf);
    memset(buf2, 0, sizeof buf2);
    buf[0] = 0x00;
    buf[1] = add1;
    buf[2] = add2;
    if (nbytes>=1) buf[3] = value[0];
    if (nbytes>=2) buf[4] = value[1];
    if (nbytes>=3) buf[5] = value[2];
    if (nbytes>=4) buf[6] = value[3];
    xfer[0].tx_buf = (unsigned long)buf;
    xfer[0].len = nbytes+3; /* Length of  command to write*/
    status = ioctl(file, SPI_IOC_MESSAGE(1), xfer);
    if (status < 0)
        {
        perror("SPI_IOC_MESSAGE");
        return;
        }
    //printf("env: %02x %02x %02x\n", buf[0], buf[1], buf[2]);
    //printf("ret: %02x %02x %02x %02x\n", buf2[0], buf2[1], buf2[2], buf2[3]);
 
    com_serial=1;
    failcount=0;
    }
    
    int main()
    {
    
   char *buffer;
   char buf[10];
 
   file=spi_init("/dev/spidev0.0"); //dev
  
   buf[0] = 0x41;
   buf[1] = 0xFF;
   spi_write(0xE6,0x0E,2,buf,file); //this will write value 0x41FF to the address 0xE60E
 
   buffer=(char *)spi_read(0xE6,0x0E,4,file); //reading the address 0xE60E
 
   close(file);
    
   }
