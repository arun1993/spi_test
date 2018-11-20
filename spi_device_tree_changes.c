
/* 1. SPI- GPIO */
#if 0
spi@78b9000 {
		compatible = "spi-gpio";
		#address-cells = <0x1>;
		ranges;

		sck-gpios = <&gpio 15 0>;
		miso-gpios = <&gpio 13 0>;
		mosi-gpios = <&gpio 12 0>;
		cs-gpios = <&gpio 14 0>;
		num-chipselects = <1>;

		/* clients */
	};
#endif  

/* 2. SPI - Louis */
#if 0 



#endif


/*3 SPI */
#if 0



#endif 
