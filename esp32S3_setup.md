[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
board_upload.flash_size = 32MB
board_build.partitions = default_16MB.csv
build_flags = 
    -DBOARD_HAS_PSRAM 
    -mfix-esp32-psram-cache-issue
board_build.arduino.memory_type = opi_opi