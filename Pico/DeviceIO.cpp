#include "DeviceIO.hpp"

#include <ossia/detail/fmt.hpp>
namespace Pico
{

QString DeviceIO::printInitialization() const
{
  switch (type)
  {
    case GPIO:
    {
      QString str;
      str = QString::fromStdString(fmt::format(
          R"_(
        gpio_config_t io_conf = {{}};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = {};
        //bit mask of the pins that you want to set,e.g.GPIO18/19
        io_conf.pin_bit_mask = (1ULL << GPIO_NUM_{});
        io_conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);
      )_",
          this->direction == Input ? "GPIO_MODE_INPUT" : "GPIO_MODE_OUTPUT",
          this->pin));

      return str;
    }
    case PWM:
    {
      QString str;
      str = QString::fromStdString(fmt::format(
          R"_(
  // Prepare and then apply the LEDC PWM timer configuration
  ledc_timer_config_t ledc_timer = {{
      .speed_mode = LEDC_HIGH_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_8_BIT,
      .timer_num = LEDC_TIMER_0,
      .freq_hz = 3000, // Set output frequency at 5 kHz
      .clk_cfg = LEDC_USE_REF_TICK}};
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  // Prepare and then apply the LEDC PWM channel configuration
  ledc_channel_config_t ledc_channel = {{.gpio_num = GPIO_NUM_{},
                                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                                        .channel = LEDC_CHANNEL_0,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .timer_sel = LEDC_TIMER_0,
                                        .duty = 0, // Set duty to 0%
                                        .hpoint = 0}};
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
)_",
          this->pin));

      return str;
    }
    case ADC:
    {
      QString str;
      str = QString::fromStdString(fmt::format(
          R"_(
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_{}, ADC_ATTEN_DB_0);
)_",
          this->pin));
      return str;
    }
  }
}

}
