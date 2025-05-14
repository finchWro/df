#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdio.h>

static int set_sampling_freq(const struct device *dev)
{
    int ret;
    struct sensor_value odr;

    ret = sensor_attr_get(dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &odr);

    /* If we don't get a frequency > 0, we set one */
    if (ret != 0 || (odr.val1 == 0 && odr.val2 == 0)) {
        odr.val1 = 100; //100MHz
        odr.val2 = 0;

        ret = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY,
                    &odr);

        if (ret != 0) {
            return 1;
        }
    }

    return 0;
}

int main(void) {
    
    // output immediately without buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // get driver for the accelerometer
    const struct device *adi = DEVICE_DT_GET(DT_NODELABEL(accel));
    if (adi == NULL) {
        printf("Could not get ADXL367 device\n");
        return 1;
    }

    if (set_sampling_freq(adi) != 0 ) {
        printf("%s : failed to set sampling frequency\n", adi->name);
        return 1;
    }


    struct sensor_value accel[3];

      while (1) {
        // read data from the sensor
        if (sensor_sample_fetch(adi) < 0) {
            printf("ADXL367 Sensor sample update error\n");
            return 1;
        }

        sensor_channel_get(adi, SENSOR_CHAN_ACCEL_XYZ, accel);

        // print over stdout
        printf("%.3f\t%.3f\t%.3f\r\n",
            sensor_value_to_double(&accel[0]),
            sensor_value_to_double(&accel[1]),
            sensor_value_to_double(&accel[2]));

		/* 10ms period, 100Hz Sampling frequency */
		k_sleep(K_MSEC(10));
    }
    return 0;
}