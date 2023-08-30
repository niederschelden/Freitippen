/* Dieser Sketch dient zur imitation eines Freitastens für Motorschaltungen
 * Häufig zu finden bei Ladeboardwänden oder Therapieliegen um ein unbeabsichtigtes Starten des Motors zu verhindern.
 * Da ich mit einem Einsatzszenario rechne das Ähnlich einem Medizinprodukt ist, ist Sicherheit sehr wichtig. 
 * Ein Ausfall der Hardware oder ein Softwarefehler darf nicht zu einem start des Motors führen. 
 * Daher wird der Motor nicht direkt angesteuert sondern nur feigegeben.
 * Änderung Version 2.0 -> 2.2 das idleen beim upcount wurde gefixt, zusätzliche Dokumentation in initWD
 */
#define INT_PIN      2
#define OUT_PIN      0


//setup debugging
#define debug        false
#define LED_2        3 //wd INTERRUPT
#define LED_3        4 //PCINTERRUPT
#define LED_6        1 //HWINTERRUPT
//ende setup debugging


#define tippZahl  3//wieviel male Tippen um freizuschalten

volatile int cc = 0; //click counter
volatile int wc = 0; //watchdog counter
volatile unsigned int lastInterrupt;

void freiSchalten(bool schalter){
  if(schalter) PORTB |= (1<<OUT_PIN); //Transistor frei
  else PORTB &= ~(1<<OUT_PIN);//Transistor gesperrt
  }

void initPCI(bool schalter){
  GIFR  |= (1 << PCIF); // Ausgelöste Interrupts löschen
   if(schalter)       {
    initHWI(false);
    GIMSK |= (1 << PCIE); //Generelles Zulassen von PCINT
    PCMSK |= (1 << INT_PIN); //Pin INT_PIN fr Interrupt freigeben
    if(debug) digitalWrite(LED_3,HIGH);
    }
   else if(!schalter)  {
    GIMSK &= ~(1 << PCIE); //Generelles deaktivieren von PCINT
    PCMSK &= ~(1 << INT_PIN); //Pin INT_PIN fr Interrupt sperren
    if(debug) digitalWrite(LED_3,LOW);
    }
   }

void initHWI(bool schalter){
  GIFR  |= (1 << INTF0); // Ausgelöste Interrupts löschen
  if(schalter)       {
    initPCI(false);
    MCUCR |= (1<<ISC01)|(1<<ISC00);//Rising Edge
    GIMSK |= (1 << INT0); //Generelles Zulassen von INT0
    if(debug) digitalWrite(LED_6,HIGH);
    }
   else if(!schalter)  {
    if(debug) digitalWrite(LED_6,LOW);
    GIMSK &= ~(1 << INT0); //Generelles Deaktivieren von INT0
    }
  }

void initWD(int zeit){
  /*
   *8Sek WDP3=1 WDP2=0 WDP1=0 WDP0=1
   *4Sek WDP3=1 WDP2=0 WDP1=0 WDP0=0
   *2Sek WDP3=0 WDP2=1 WDP1=1 WDP0=1
   *1Sek WDP3=0 WDP2=1 WDP1=1 WDP0=0
   */
    WDTCR  |= (1 << WDIF); // Ausgelöste Interrupts löschen
    if   (zeit>1)   {
      asm("WDR"); //Rücksetzen des Watchdog Timer
      MCUSR = 0x00;
      WDTCR |= (1<<WDCE) | (1<<WDE);//Freigabe der Prescaler änderung für 4 Taktzyklen
      WDTCR = (1<<WDIE) | (1<<WDE) | (1<<WDP3) | (1<<WDP0);  //WD Timer auf 8 Sek, enabled: INTERRUPT
      if(debug) digitalWrite(LED_2,HIGH);
    }
    else if (zeit==1) {
      asm("WDR"); //Rücksetzen des Watchdog Timer
      MCUSR = 0x00;
      WDTCR |= (1<<WDCE) | (1<<WDE);//Freigabe der Prescaler änderung für 4 Taktzyklen
      WDTCR = (0<<WDIE) | (1<<WDE) | (1<<WDP2) | (1<<WDP1);  //WD Timer auf 1 Sek, enabled: RESET
      if(debug) digitalWrite(LED_2,HIGH);
    }
    else    {
      asm("WDR"); //Rücksetzen des Watchdog Timer
      MCUSR = 0x00;
      WDTCR |= (1<<WDCE) | (1<<WDE);//Freigabe der Prescaler änderung für 4 Taktzyklen
      WDTCR = 0x00; //Ausschalten des WDT
      if(debug) digitalWrite(LED_2,LOW);
    }
  }
  
void schlafe(){
  //if (!debug) digitalWrite(LED_3,HIGH);//Anzeigen das Schalf gleich eingeschaltet wird
  //if (!debug) digitalWrite(LED_2,LOW);//der letzte gewinnt
  //Sleepmode definieren: Power Down
  MCUCR = B10010111;//Vorbereitung BOD disable
  MCUCR = B10110011;//Sleep enable, Mode Power Down, Rising Edge, BOD aus
  sei();
  asm("SLEEP");//Schlafe ....wie cpu_sleep aus <avr/sleep.h>
  MCUCR &= ~(1 << SE); //Sleep Disable setzen
  //if (!debug) digitalWrite(LED_3,LOW);
  }

void idlee(){
  //if (!debug) digitalWrite(LED_2,HIGH);//Anzeigen das Schalf gleich eingeschaltet wird
  //if (!debug) digitalWrite(LED_3,LOW); //der letzte gewinnt
  //Sleepmode definieren: Idle
  MCUCR = B10010111;//Vorbereitung BOD disable
  MCUCR = B10100011;//Sleep enable, Idle, Rising Edge, BOD aus
  sei();
  asm("SLEEP");//Schlafe ....wie cpu_sleep aus <avr/sleep.h>
  MCUCR &= ~(1 << SE); //Sleep Disable setzen
  //if (!debug) digitalWrite(LED_2,LOW);
  }

  
ISR (PCINT0_vect) {//aus Schlaf kommend
  cli();
  //GIFR  |= (1 << PCIF);
  GIFR  |= (1 << INTF0);
  MCUSR = 0x00;
  lastInterrupt=millis();
  cc=1;
  wc=1;
  initWD(wc);//WDI immer zuerst
  initHWI(true);//Zusätzlich auf HWI
}

ISR (INT0_vect) {//im wachem/idle Zustand
  cli();
  GIFR  |= (1 << PCIF);
  //GIFR  |= (1 << INTF0);
  MCUSR = 0x00;
  if((lastInterrupt+200)>millis()) return;//entprellen
  cc++;
  //noch im Upcount
  if(cc<tippZahl){
    //asm("WDR"); //Rücksetzen des Watchdog Timer
    initWD(0);
    initWD(wc);
    return;
    }
   //ab hier wenn 3x freigetippt
   wc=4;//3x 8 Sek + 1 Sek = 25 Sek
   initWD(wc);//WDI immer zuerst
   initHWI(false);//Alle anderen Interrupts aussetzen
   freiSchalten(true);
   schlafe();
  }  

ISR(WDT_vect){
  cli();
  GIFR  |= (1 << PCIF);
  GIFR  |= (1 << INTF0);
  //MCUSR = 0x00;
  wc--;
  if(wc<1 || cc<tippZahl) {//wenn der Countdown oder der Upcount endet, Softreset
    //reset (bei manuellem nullen funktioniert nach erster Freichaltung das Auslesen an INT_PIN und somit auch INT0 nicht mehr, daher kompletter Reset via WD) 
    //Um das zu verstehen bräuchte ich Hardware debugging
  }
  else initWD(wc);//Countdown: Routine immer initialisieren, da der WD ohne neuen Initialisierung beim zweiten Aufruf in Reset geht;
  schlafe();
}

void initRegister(){
    cli();//SREG &= 0x7; //ist gleich cli(); alle innterrupts disable
    //PIN verwendung
    DDRB  = B00111111; // ALLE Pins Ausgang
    DDRB &= ~(1<<INT_PIN);// Pin INT_PIN Eingang
    PORTB = B00000000; //Ausgabe 0 bzw Pullup 0 aller Pins
    //allgemeine Stromsparmaßnahme start
    ACSR &= ~(1<<ACIE);//AC Interrupt aus
    ACSR |= (1<<ACD);//AC Disable = Analog Comparator aus
    ADCSRA &= ~((1 << ADEN) | (1<< ADC) ); //ADC generell ausschalten um Strom zu sparen
    //stromsparen ende

    //Freischaltung deaktivieren
    PORTB &= ~(1<<OUT_PIN); //Transistor ausschalten - falls routine ein 2. mal läuft
    //PCINT
    initPCI(true);//initail ist PCINT an
    //INT0
    initHWI(false);//initail ist INT0 aus
    //WDT 
    initWD(wc); //initial ist WDT aus
    schlafe();
   }

void setup() {
  initRegister();
}

void loop() {
  idlee();//während des upcounts nur Idleen, nicht richtig schlafen damit INT0 richtig funktioniert
}
