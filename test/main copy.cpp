class DigitalIO  //ClassName (DigitalIO) is declared/created using class keyword
{
  private:
    int ledPin;  //variable can only be accessed by the functions under public: specifier

  public:
    DigitalIO(int powerpin): ledPin(powerpin) {} //inline constructor to initilize ledPin
    void ioDir();  //member functions
    void ledON();
    void ledOFF();
};

DigitalIO led(13);  //led is called object of type DigitalIO

void setup()
{
  Serial.begin(9600);
  led.ioDir();     //setting up the direction of IO line(13) as output; ioDir() is called method
}

void loop()
{
  led.ledON();
  delay(1000);
  led.ledOFF();
  delay(1000);
}

void DigitalIO::ioDir()   //member function definition; :: (double colon) is called scope resolution operator
{
  pinMode(ledPin, OUTPUT);
}

void DigitalIO::ledON()
{
  digitalWrite(ledPin, HIGH);
}

void DigitalIO::ledOFF()
{
  digitalWrite(ledPin, LOW);
}