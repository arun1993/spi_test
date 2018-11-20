
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

djb_extra_spi4cs4_default: djb_extra_spi4cs4_default {
        pinmux {
            function = "gpio";
            pins = "gpio62";
        };
        pinconf {
            pins = "gpio62";
            drive-strength = <16>;
            bias-disable;
            output-high;
        };
    };

    djb_extra_spi4cs4_sleep: djb_extra_spi4cs4_sleep {
        pinmux {
            function = "gpio";
            pins = "gpio62";
        };
        pinconf {
            pins = "gpio62";
            drive-strength = <2>;
            bias-pull-down;
        };
    };

spi@78b9000 {
                    // SPI4, GPIO12:15
                    label = "HS-SPI1";
                    status = "okay";
		    compatible = "spi, qcom"
	            spi-mac-frequency = <51000>
	            reg = <0>
                    num-cs = <2>;
                    cs-gpios = <&msmgpio  14 GPIO_ACTIVE_LOW>, //GPIO10  = SPI3_CS0
                             //<&msmgpio 120 GPIO_ACTIVE_LOW>, //GPIO120 = SPI3_CS1
                             //<&msmgpio  37 GPIO_ACTIVE_LOW>, //GPIO37  = SPI3_CS2
                               <&msmgpio  62 GPIO_ACTIVE_LOW>; //GPIO69  = SPI3_CS3
            	    pinctrl-names = "default", "sleep";
                    pinctrl-0 = <&spi4_default>, <&djb_extra_spi4cs4_default>;
                    pinctrl-1 = <&spi4_sleep>, <&djb_extra_spi4cs4_sleep>;
	
                };
#endif


/*3 SPI */
#if 0

Some other changes I made by seeing stuff from online 
https://www.kernel.org/doc/Documentation/devicetree/bindings/spi/spi-bus.txt
https://www.kernel.org/doc/Documentation/devicetree/bindings/spi/qcom%2Cspi-qup.txt


#endif 
