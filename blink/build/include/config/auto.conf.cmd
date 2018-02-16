deps_config := \
	/Users/iotsuyuan/esp/esp-idf/components/app_trace/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/bt/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/esp32/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/ethernet/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/fatfs/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/freertos/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/heap/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/libsodium/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/log/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/lwip/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/openssl/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/pthread/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/spiffs/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/iotsuyuan/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/iotsuyuan/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/iotsuyuan/esp/esp32/blink/main/Kconfig.projbuild \
	/Users/iotsuyuan/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/iotsuyuan/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
