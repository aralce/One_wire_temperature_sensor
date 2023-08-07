all:
	@echo "=========================================="
	@echo "| One_wire_temperature_sensor UNIT TESTS |"
	@echo "=========================================="	
	@$(MAKE) --no-print-directory -C test_Arduino_implementation/
	@$(MAKE) --no-print-directory -C test_ESP_IDF_implementation/

clean:
	@$(MAKE) clean --no-print-directory -C test_Arduino_implementation/
	@$(MAKE) clean --no-print-directory -C test_ESP_IDF_implementation/