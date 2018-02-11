deps_config := \
	/Users/iotsuyan/esp/esp-idf/components/app_trace/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/bt/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/esp32/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/ethernet/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/fatfs/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/freertos/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/heap/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/libsodium/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/log/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/lwip/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/openssl/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/pthread/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/spiffs/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/iotsuyan/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/iotsuyan/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/iotsuyan/esp/esp32/blink/main/Kconfig.projbuild \
	/Users/iotsuyan/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/iotsuyan/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
