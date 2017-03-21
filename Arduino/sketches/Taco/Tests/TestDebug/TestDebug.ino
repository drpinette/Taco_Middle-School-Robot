#ifdef DEBUG
#define _D(X) Serial.print(#X " "), Serial.print(X), Serial.print("; ") 
#define _DS(X) Serial.print( " "), Serial.print(X), Serial.print("; ")
#define _DC(X) Serial.print(#X "; ")
#define _NL Serial.print("\n")
#else
#define _D
#define _DC
#define _NL
#endif

void setup() {

_DS("Hello World");_NL

}

void loop() {
  // put your main code here, to run repeatedly:

}
