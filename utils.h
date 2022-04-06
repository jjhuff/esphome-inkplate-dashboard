#pragma once

#include <vector>

const char* LIGHTBULB = "\U000F0335";
const char* SHADES_DOWN = "\U000F111C";
const char* SHADES_UP = "\U000F111E";
const char* FOOD = "\U000F025A";
const char* BATTERY_HIGH = "\U000F12A3";
const char* BATTERY_MEDIUM = "\U000F12A2";
const char* BATTERY_LOW = "\U000F12A1";
const char* BATTERY_OUTLINE = "\U000F008E";

class Button: public touchscreen::TouchListener {
  public:
    Button(const char* icon, const char* text, binary_sensor::BinarySensor* sensor, Color color=COLOR_OFF) {
      this->icon_ = icon;
      this->text_ = text;
      this->sensor_ = sensor;
      this->color_ = color;
    }

    void layout(int x, int y, int width, int height, touchscreen::Touchscreen* touchscreen) {
      this->x_ = x;
      this->y_ = y;
      this->width_ = width;
      this->height_ = height;
      touchscreen->register_listener(this);
    }

    void render(DisplayBuffer &display) {
      auto x = this->x_;
      auto y = this->y_;
      auto width = this->width_;
      auto height = this->height_;
      auto center_x = x + (width/2);
      auto center_y = y + (height/2);

      display.rectangle(x, y, width, height, COLOR_OFF);

      if (this->icon_) {
        display.print(center_x, center_y-10, &id(icons_90), this->color_, TextAlign::CENTER, this->icon_);
      }
      if (this->text_) {
        display.print(center_x, y + height-5, &id(helvetica_32), COLOR_OFF, TextAlign::BOTTOM_CENTER, this->text_);
      }
    }

    void touch(touchscreen::TouchPoint tp) override {
      bool touched = (tp.x >= this->x_ && tp.x <= this->x_+this->width_ && tp.y >= this->y_ && tp.y <= this->y_+this->height_);

      if (touched) {
        this->sensor_->publish_state(true);
      } else {
        release();
      }
    }

    void release() override {
      this->sensor_->publish_state(false);
    }

  protected:
    int x_, y_, width_, height_;
    Color color_;
    const char* icon_{nullptr};
    const char* text_{nullptr};
    binary_sensor::BinarySensor* sensor_{nullptr};
};

class Row {
  public: 
    Row(int x, int y, int cell_size, const char* label) {
      this->x_ = x;
      this->y_ = y;
      this->cell_size_ = cell_size;
      this->label_ = label;
    }

    void add(Button* btn) {
      this->buttons_.push_back(btn);
    }

    void layout(touchscreen::Touchscreen* touchscreen) {
      int x = this->x_;
      int y = this->y_;

      if(this->label_) {
        y += 50; // Offset for label
      }

      for (auto *btn : this->buttons_) {
        btn->layout(x, y, this->cell_size_, this->cell_size_, touchscreen);
        x += this->cell_size_;
      }
    }

    void render(DisplayBuffer &display) {
      if(this->label_) {
        display.print(this->x_, this->y_, &id(helvetica_48), COLOR_OFF, TextAlign::TOP_LEFT, this->label_);
      }
      for (auto *btn : this->buttons_) {
        btn->render(display);
      }
    }

  protected:
    int x_;
    int y_;
    int cell_size_;
    const char* label_;
    std::vector<Button *> buttons_;
};

