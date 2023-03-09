
#include <linux/platform_device.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/module.h>

#define N_ATTR_GROUPS   3

static struct completion device_release;


static struct device *monmon;

static ssize_t
h_mon_p_hedelek(struct device *d, struct device_attribute *a, char *buf){

	return snprintf(buf, PAGE_SIZE, "%d\n", 6);
}

static ssize_t
h_mon_p_set_hedelek(struct device *d,
		 struct device_attribute *a,
		 const char *buf,
		 size_t count){
	return count;
}

static SENSOR_DEVICE_ATTR(hedelek,
			  0644,
			  h_mon_p_hedelek,
			  h_mon_p_set_hedelek,
			  0);


static struct attribute *hedelek_attrs[] = {
	&sensor_dev_attr_hedelek.dev_attr.attr,
	NULL
};
static const struct attribute_group hedelek_group = {
	.attrs = hedelek_attrs,
};

static const u32 h_mon_p_config_chip[] = {
	HWMON_C_REGISTER_TZ | HWMON_C_UPDATE_INTERVAL,
	0
};

static const u32 h_mon_p_config_temp[] = {
	HWMON_T_INPUT |
	HWMON_T_MAX |
	HWMON_T_MAX_HYST |
	HWMON_T_CRIT |
	HWMON_T_CRIT_HYST |
	HWMON_T_EMERGENCY |
	HWMON_T_EMERGENCY_HYST,
	0
};

static const struct hwmon_channel_info h_mon_p_chip = {
	.type = hwmon_chip,
	.config = h_mon_p_config_chip,
};

static const struct hwmon_channel_info h_mon_p_temp = {
	.type = hwmon_temp,
	.config = h_mon_p_config_temp,
};


static const struct hwmon_channel_info *h_mon_p_info[] = {
	&h_mon_p_chip,
	&h_mon_p_temp,
	NULL
};

static umode_t
h_mon_p_chip_is_visible(const void *data, u32 attr, int channel)
{
	switch (attr) {
	case hwmon_chip_update_interval:
		return 0444;
	default:
		return 0;
	}
}

static umode_t
h_mon_p_temp_is_visible(const void *data, u32 attr, int channel)
{
	switch (attr) {
	case hwmon_temp_input:
		return 0444;
	case hwmon_temp_max:
	case hwmon_temp_max_hyst:
	case hwmon_temp_crit:
	case hwmon_temp_crit_hyst:
	case hwmon_temp_emergency:
	case hwmon_temp_emergency_hyst:
		return 0644;
	default:
		return 0;
	}
}


static int
h_mon_p_chip_read(struct device *dev, u32 attr, int channel, long *val)
{
	switch (attr) {
	case hwmon_chip_update_interval:
		*val = 1000;
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static int
h_mon_p_temp_read(struct device *dev, u32 attr, int channel, long *val)
{
	int ret;

	switch (attr) {
	case hwmon_temp_input:
		ret = 55;
		*val = ret < 0 ? ret : (ret * 1000);
		break;
	case hwmon_temp_max:
		*val = 100;
		break;
	case hwmon_temp_max_hyst:
		*val = 101;
		break;
	case hwmon_temp_crit:
		*val = 102;
		break;
	case hwmon_temp_crit_hyst:
		*val = 103;
		break;
	case hwmon_temp_emergency:
		*val = 104;
		break;
	case hwmon_temp_emergency_hyst:
		*val = 105;
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}


static int
h_mon_p_temp_write(struct device *dev, u32 attr, int channel, long val)
{
	switch (attr) {
	case hwmon_temp_max:
		return 0;
	case hwmon_temp_max_hyst:
		return 0;
	case hwmon_temp_crit:
		return 0;
	case hwmon_temp_crit_hyst:
		return 0;
	case hwmon_temp_emergency:
		return 0;
	case hwmon_temp_emergency_hyst:
		return 0;
	default:
		return -EOPNOTSUPP;
	}
}

static umode_t
h_mon_p_is_visible(const void *data,
		enum hwmon_sensor_types type,
		u32 attr,
		int channel)
{
	switch (type) {
	case hwmon_chip:
		return h_mon_p_chip_is_visible(data, attr, channel);
	case hwmon_temp:
		return h_mon_p_temp_is_visible(data, attr, channel);
	case hwmon_fan:
	case hwmon_in:
	case hwmon_pwm:
	case hwmon_power:
	default:
		return 0;
	}
}

static const char input_label[] = "HM_Proto";

static int
h_mon_p_read_string(struct device *dev,
		 enum hwmon_sensor_types type,
		 u32 attr,
		 int channel,
		 const char **buf)
{
	if (type == hwmon_in && attr == hwmon_in_label) {
		*buf = input_label;
		return 0;
	}

	return -EOPNOTSUPP;
}

static int
h_mon_p_read(struct device *dev,
	  enum hwmon_sensor_types type,
	  u32 attr,
	  int channel,
	  long *val)
{
	switch (type) {
	case hwmon_chip:
		return h_mon_p_chip_read(dev, attr, channel, val);
	case hwmon_temp:
		return h_mon_p_temp_read(dev, attr, channel, val);
	case hwmon_fan:
	case hwmon_in:
	case hwmon_pwm:
	case hwmon_power:
	default:
		return -EOPNOTSUPP;
	}
}

static int
h_mon_p_write(struct device *dev,
	   enum hwmon_sensor_types type,
	   u32 attr,
	   int channel,
	   long val)
{
	switch (type) {
	case hwmon_temp:
		return h_mon_p_temp_write(dev, attr, channel, val);
	case hwmon_pwm:
	default:
		return -EOPNOTSUPP;
	}
}

static const struct hwmon_ops h_mon_p_ops = {
	.is_visible = h_mon_p_is_visible,
	.read = h_mon_p_read,
	.read_string = h_mon_p_read_string,
	.write = h_mon_p_write,
};

static const struct hwmon_chip_info h_mon_p_chip_info = {
	.ops = &h_mon_p_ops,
	.info = h_mon_p_info,
};

static int h_mon_p_probe(struct platform_device *pd)
{
	const struct attribute_group *special_groups[N_ATTR_GROUPS];
	int ret = 0;
	int i = 0;

	special_groups[i++] = &hedelek_group;
	special_groups[i] = 0;
	monmon = hwmon_device_register_with_info(&pd->dev,
						 "h_mon_p",
						 pd,
						 &h_mon_p_chip_info,
						 special_groups);
	if (IS_ERR(monmon)) {
		ret = PTR_ERR(monmon);
		pr_err("Unable to register hwmon device: %d\n", ret);
		return ret;
	}

	return 0;
}

static int h_mon_p_remove(struct platform_device *pd)
{
	hwmon_device_unregister(monmon);
	return 0;
}

struct platform_driver h_mon_p_driver = {
	.probe		= h_mon_p_probe,
	.remove		= h_mon_p_remove,
	.driver		= {
		.name	= "h_mon_p-dev",
	},
};

static void h_mon_p_device_release(struct device *dev)
{
	complete(&device_release);
}

static struct platform_device h_mon_p_device = {
	.name		= "h_mon_p-dev",
	.id		= -1,
	.dev = {
		.release = h_mon_p_device_release,
	}
};

int h_mon_p_init(void)
{
	platform_device_register(&h_mon_p_device);
	return platform_driver_register(&h_mon_p_driver);
}
module_init(h_mon_p_init);

void h_mon_p_exit(void)
{
	init_completion(&device_release);

	platform_driver_unregister(&h_mon_p_driver);
	platform_device_unregister(&h_mon_p_device);

	wait_for_completion(&device_release);
}
module_exit(h_mon_p_exit);

MODULE_ALIAS("platform:h_mon_p-dev");
MODULE_DESCRIPTION("HWMON Kernel Module Prototype");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chiyang Lin <chiyanglin@gmail.com>");
