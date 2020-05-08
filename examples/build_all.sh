
#
# build all examples
#    delete any old builds
#    configure and build all projects from their directories
#    gather all binaries (*.elf) in bin directory
#    delete all build direcories
#
rm -rf bin
mkdir bin

rm -rf ex_01a_simple_tx/build
rm -rf ex_01b_tx_sleep/build
rm -rf ex_01c_tx_sleep_auto/build
rm -rf ex_01d_tx_timed_sleep/build
rm -rf ex_01e_tx_with_cca/build
rm -rf ex_02a_simple_rx/build
rm -rf ex_02b_rx_preamble_64/build
rm -rf ex_02c_rx_diagnostics/build
rm -rf ex_02d_rx_sniff/build
rm -rf ex_02e_rx_dbl_buff/build
rm -rf ex_03a_tx_wait_resp/build
rm -rf ex_03b_rx_send_resp/build
rm -rf ex_03c_tx_wait_resp_leds/build
rm -rf ex_03d_tx_wait_resp_interrupts/build
rm -rf ex_04a_cont_wave/build
rm -rf ex_04b_cont_frame/build
rm -rf ex_05a_ds_twr_init/build
rm -rf ex_05b_ds_twr_resp/build
rm -rf ex_05c_ds_twr_resp_ble/build
rm -rf ex_06a_ss_twr_init/build
rm -rf ex_06b_ss_twr_resp/build
rm -rf ex_07a_ack_data_tx/build
rm -rf ex_07b_ack_data_rx/build
rm -rf ex_08a_low_power_listen_rx/build
rm -rf ex_08b_low_power_listen_tx/build
rm -rf ex_09a_bandwidth_power_ref_meas/build
rm -rf ex_09b_bandwidth_power_comp/build
rm -rf ex_10a_gpio/build
rm -rf ex_11a_button/build
rm -rf ex_11b_leds/build
rm -rf ex_12a_ble/build
rm -rf ex_13a_accelerometer/build
rm -rf ex_14a_spi0_master/build
rm -rf ex_14b_spi0_slave/build

pushd .; cd ex_01a_simple_tx                ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_01b_tx_sleep                 ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_01c_tx_sleep_auto            ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_01d_tx_timed_sleep           ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_01e_tx_with_cca              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_02a_simple_rx                ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_02b_rx_preamble_64           ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_02c_rx_diagnostics           ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_02d_rx_sniff                 ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_02e_rx_dbl_buff              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_03a_tx_wait_resp             ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_03b_rx_send_resp             ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_03c_tx_wait_resp_leds        ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_03d_tx_wait_resp_interrupts  ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_04a_cont_wave                ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_04b_cont_frame               ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_05a_ds_twr_init              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_05b_ds_twr_resp              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_05c_ds_twr_resp_ble          ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_06a_ss_twr_init              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_06b_ss_twr_resp              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_07a_ack_data_tx              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_07b_ack_data_rx              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_08a_low_power_listen_rx      ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_08b_low_power_listen_tx      ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_09a_bandwidth_power_ref_meas ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_09b_bandwidth_power_comp     ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_10a_gpio                     ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_11a_button                   ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_11b_leds                     ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_13a_accelerometer            ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_14a_spi0_master              ; ./configure.sh; cd build; make; popd 
pushd .; cd ex_14b_spi0_slave               ; ./configure.sh; cd build; make; popd

cp ./ex_04a_cont_wave/build/zephyr/zephyr.elf                ./bin/ex_04a_cont_wave.elf
cp ./ex_01c_tx_sleep_auto/build/zephyr/zephyr.elf            ./bin/ex_01c_tx_sleep_auto.elf
cp ./ex_01d_tx_timed_sleep/build/zephyr/zephyr.elf           ./bin/ex_01d_tx_timed_sleep.elf
cp ./ex_03a_tx_wait_resp/build/zephyr/zephyr.elf             ./bin/ex_03a_tx_wait_resp.elf
cp ./ex_02a_simple_rx/build/zephyr/zephyr.elf                ./bin/ex_02a_simple_rx.elf
cp ./ex_02b_rx_preamble_64/build/zephyr/zephyr.elf           ./bin/ex_02b_rx_preamble_64r.elf
cp ./ex_07b_ack_data_rx/build/zephyr/zephyr.elf              ./bin/ex_07b_ack_data_rx.elf
cp ./ex_01a_simple_tx/build/zephyr/zephyr.elf                ./bin/ex_01a_simple_tx.elf
cp ./ex_14b_spi0_slave/build/zephyr/zephyr.elf               ./bin/ex_14b_spi0_slave.elf
cp ./ex_14a_spi0_master/build/zephyr/zephyr.elf              ./bin/ex_14a_spi0_master.elf
cp ./ex_10a_gpio/build/zephyr/zephyr.elf                     ./bin/ex_10a_gpio.elf
cp ./ex_03b_rx_send_resp/build/zephyr/zephyr.elf             ./bin/ex_03b_rx_send_resp.elf
cp ./ex_08b_low_power_listen_tx/build/zephyr/zephyr.elf      ./bin/ex_08b_low_power_listen_tx.elf
cp ./ex_13a_accelerometer/build/zephyr/zephyr.elf            ./bin/ex_13a_accelerometer.elf
cp ./ex_09b_bandwidth_power_comp/build/zephyr/zephyr.elf     ./bin/ex_09b_bandwidth_power_comp.elf
cp ./ex_03d_tx_wait_resp_interrupts/build/zephyr/zephyr.elf  ./bin/ex_03d_tx_wait_resp_interrupts.elf
cp ./ex_02c_rx_diagnostics/build/zephyr/zephyr.elf           ./bin/ex_02c_rx_diagnostics.elf
cp ./ex_11b_leds/build/zephyr/zephyr.elf                     ./bin/ex_11b_leds.elf
cp ./ex_06b_ss_twr_resp/build/zephyr/zephyr.elf              ./bin/ex_06b_ss_twr_resp.elf
cp ./ex_01e_tx_with_cca/build/zephyr/zephyr.elf              ./bin/ex_01e_tx_with_cca.elf
cp ./ex_06a_ss_twr_init/build/zephyr/zephyr.elf              ./bin/ex_06a_ss_twr_init.elf
cp ./ex_02d_rx_sniff/build/zephyr/zephyr.elf                 ./bin/ex_02d_rx_sniff.elf
cp ./ex_08a_low_power_listen_rx/build/zephyr/zephyr.elf      ./bin/ex_08a_low_power_listen_rx.elf
cp ./ex_05a_ds_twr_init/build/zephyr/zephyr.elf              ./bin/ex_05a_ds_twr_init.elf
cp ./ex_07a_ack_data_tx/build/zephyr/zephyr.elf              ./bin/ex_07a_ack_data_tx.elf
cp ./ex_05b_ds_twr_resp/build/zephyr/zephyr.elf              ./bin/ex_05b_ds_twr_resp.elf
cp ./ex_05c_ds_twr_resp_ble/build/zephyr/zephyr.elf          ./bin/ex_05c_ds_twr_resp_ble.elf 
cp ./ex_11a_button/build/zephyr/zephyr.elf                   ./bin/ex_11a_button.elf
cp ./ex_01b_tx_sleep/build/zephyr/zephyr.elf                 ./bin/ex_01b_tx_sleep.elf
cp ./ex_02e_rx_dbl_buff/build/zephyr/zephyr.elf              ./bin/ex_02e_rx_dbl_buff.elf
cp ./ex_09a_bandwidth_power_ref_meas/build/zephyr/zephyr.elf ./bin/ex_09a_bandwidth_power_ref_meas.elf
cp ./ex_04b_cont_frame/build/zephyr/zephyr.elf               ./bin/ex_04b_cont_frame.elf
cp ./ex_03c_tx_wait_resp_leds/build/zephyr/zephyr.elf        ./bin/ex_03c_tx_wait_resp_leds.elf
                               
rm -rf ex_01a_simple_tx/build
rm -rf ex_01b_tx_sleep/build
rm -rf ex_01c_tx_sleep_auto/build
rm -rf ex_01d_tx_timed_sleep/build
rm -rf ex_01e_tx_with_cca/build
rm -rf ex_02a_simple_rx/build
rm -rf ex_02b_rx_preamble_64/build
rm -rf ex_02c_rx_diagnostics/build
rm -rf ex_02d_rx_sniff/build
rm -rf ex_02e_rx_dbl_buff/build
rm -rf ex_03a_tx_wait_resp/build
rm -rf ex_03b_rx_send_resp/build
rm -rf ex_03c_tx_wait_resp_leds/build
rm -rf ex_03d_tx_wait_resp_interrupts/build
rm -rf ex_04a_cont_wave/build
rm -rf ex_04b_cont_frame/build
rm -rf ex_05a_ds_twr_init/build
rm -rf ex_05b_ds_twr_resp/build
rm -rf ex_05c_ds_twr_resp_ble/build
rm -rf ex_06a_ss_twr_init/build
rm -rf ex_06b_ss_twr_resp/build
rm -rf ex_07a_ack_data_tx/build
rm -rf ex_07b_ack_data_rx/build
rm -rf ex_08a_low_power_listen_rx/build
rm -rf ex_08b_low_power_listen_tx/build
rm -rf ex_09a_bandwidth_power_ref_meas/build
rm -rf ex_09b_bandwidth_power_comp/build
rm -rf ex_10a_gpio/build
rm -rf ex_11a_button/build
rm -rf ex_11b_leds/build
rm -rf ex_12a_ble/build
rm -rf ex_13a_accelerometer/build
rm -rf ex_14a_spi0_master/build
rm -rf ex_14b_spi0_slave/build
