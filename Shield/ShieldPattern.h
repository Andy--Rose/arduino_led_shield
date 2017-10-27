#define COLOR_WIPE_INTERVAL 20
#define THEATRE_CHASE_INTERVAL 100
enum direction { FORWARD, REVERSE };

class ShieldPattern : public Adafruit_NeoPixel
{
  private:
    String patterns[3] = { "THEATER_CHASE", "RAINBOW_CYCLE", "COLOR_WIPE" };
  public:
    // Member Variables:
    direction Direction;

    unsigned long Interval;     // milliseconds between updates
    unsigned long lastUpdate;   // last update of position

    uint32_t Color1, Color2;
    uint32_t TotalSteps;        // total number of steps in the pattern
    uint32_t Index;
    uint32_t PatternIndex = 0;

    void (*OnComplete)();       // callback

    // Constructor - calls base-class constructor to initialize strip
    ShieldPattern(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
    }

        // Update the pattern
    void Update()
    {
        if((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();
            String pattern = patterns[PatternIndex];
            if (pattern == "THEATER_CHASE") {
                TheaterChaseUpdate();
            }
            else if (pattern == "RAINBOW_CYCLE") {
              RainbowCycleUpdate();
            }
            else {
              ColorWipeUpdate();
            }
        }
    }

        // Increment the Index and reset at the end
    void Increment()
    {
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
    }

    String GetPattern() {
      return patterns[PatternIndex];
    }

    void NextPattern()
    {
      if (PatternIndex == 0) {// THEATRE
          PatternIndex = 1;
          RainbowCycle(1);
      } else if (PatternIndex == 1) {// RAINBOW
          PatternIndex = 2;
          ColorWipe(Color(255,255,0), COLOR_WIPE_INTERVAL);
      } else {
          PatternIndex = 0;
          TheaterChase(Color(255,255,0), Color(0,0,50), THEATRE_CHASE_INTERVAL);
      }
    }

    // Initialize for a ColorWipe
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
        PatternIndex = 2;
        Interval = interval;
        TotalSteps = numPixels();
        Color1 = color;
        Index = 0;
        Direction = dir;
    }

    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
        if (Direction == REVERSE) {
          setPixelColor(TotalSteps - Index, Color(0,0,0));
        } else {
          setPixelColor(Index, Color1);
        }
        show();
        Increment();
    }

    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
      PatternIndex = 1;
      Interval = interval;
      TotalSteps = 255;
      Index = 0;
      Direction = dir;
    }

    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
        for(int i=0; i< numPixels(); i++)
        {
            setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
        }
        show();
        Increment();
    }

    // Initialize for a Theater Chase
    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
    {
        PatternIndex = 0;
        Interval = interval;
        TotalSteps = numPixels();
        Color1 = color1;
        Color2 = color2;
        Index = 0;
        Direction = dir;
   }

   // Update the Theater Chase Pattern
    void TheaterChaseUpdate()
    {
        for(int i=0; i< numPixels(); i++)
        {
            if ((i + Index) % 3 == 0)
            {
                setPixelColor(i, Color1);
            }
            else
            {
                setPixelColor(i, Color2);
            }
        }
        show();
        Increment();
    }

    // Common Utility Functions
        // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
        return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
        return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
        return color & 0xFF;
    }

    // Return color, dimmed by 75% (used by scanner)
    uint32_t DimColor(uint32_t color)
    {
        uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
        return dimColor;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
        {
            return Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else
        {
            WheelPos -= 170;
            return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }

    // Reverse direction of the pattern
    void Reverse()
    {
        if (Direction == FORWARD)
        {
            Direction = REVERSE;
            Index = TotalSteps-1;
        }
        else
        {
            Direction = FORWARD;
            Index = 0;
        }
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
        for (int i = 0; i < numPixels(); i++)
        {
            setPixelColor(i, color);
        }
        show();
    }
};
