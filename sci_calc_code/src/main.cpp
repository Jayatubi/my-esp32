#include <U8g2lib.h>
#include <HWCDC.h>

#define SPI_CS 5
#define SPI_DC 16
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 64

U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC);

template <class T, size_t N>
constexpr size_t count_of(T (&)[N])
{
    return N;
}

static char rowp[] = {33, 32, 35, 34, 39};
static char colp[] = {13, 12, 14, 27, 26, 25};

void setup()
{
    Serial.begin(115200);

    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_b12_t_japanese3);
    u8g2.setFontDirection(0);

    for (int r = 0; r < count_of(rowp); r++)
    {
        pinMode(rowp[r], INPUT);
    }
    for (int c = 0; c < count_of(colp); c++)
    {
        pinMode(colp[c], OUTPUT);
    }
}

const char *text = "こんにちは世界";

void hud(int eplased)
{
    static int frame_count = 0;
    static int total_eplased = 0;
    static int sample_count = 10;
    static float fps = 0;

    frame_count++;
    if (frame_count % sample_count == 0)
    {
        fps = 1000.f / (total_eplased / sample_count);
        total_eplased = 0;
    }
    else
    {
        total_eplased += eplased;
    }
    auto text_width = u8g2.getUTF8Width(text);
    auto move_range = SCREEN_WIDTH - text_width;
    auto loop_range = move_range << 1;
    auto position = move_range - abs(frame_count % loop_range - move_range);
    u8g2.setCursor(position, 12);
    u8g2.print(text);
    u8g2.setCursor(0, SCREEN_HEIGHT);
    u8g2.printf("Frame Count: %d. FPS: %.2f", frame_count, fps);
}

void key_test()
{
    constexpr int offset_x = 10;
    constexpr int offset_y = 20;
    constexpr int width = 4;
    constexpr int margin = 1;
    constexpr int padding = 1;

    for (int c = 0; c < count_of(colp); c++)
    {
        digitalWrite(colp[c], HIGH);
        for (int r = 0; r < count_of(rowp); r++)
        {
            const auto is_high = digitalRead(rowp[r]);
            auto func = is_high ? &U8G2::drawBox : &U8G2::drawFrame;
            (u8g2.*func)(c * (width + margin * 2) + offset_x, r * (width + margin * 2) + offset_y, width, width);
        }
        digitalWrite(colp[c], LOW);
    }
}

void loop()
{
    static int last_time = 0;
    auto now = millis();
    auto eplased = last_time == 0 ? 0 : now - last_time;

    u8g2.firstPage();
    do
    {
        hud(eplased);
        key_test();
    } while (u8g2.nextPage());
    // delay(100);
    last_time = now;
}