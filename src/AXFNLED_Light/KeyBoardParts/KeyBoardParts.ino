/*By Novang*/
#include <Keyboard.h>
#define KEY_LEFT_CTRL   0x80   //a
#define KEY_LEFT_SHIFT    0x81  //x
#define KEY_BACKSPACE   0xB2 //f'n



#define BAUDRATE    250000  // baudrate for serial communication 1초동안 전송되는 신호의 수

#define KP_CTRL        A0      // a key pin number  키패드 디지털 핀 지정
#define KP_SHIFT        A1      // x key pin number
#define KP_BACKSPACE       A2      // fn key pin number

#define LP_CTRL        9       // a led pin number led 디지털 핀 지정
#define LP_SHIFT        10      // x led pin number
#define LP_BACKSPACE       11      // fn led pin number

#define PROC_SPD    100       // processing speed (us)  반복 속도
#define LD          255     // led power (0-255) 밝기
#define LD_TM       1     // led minus count (0-255) 밝기를 줄여주기 위한 변수

unsigned short V_CTRL = 0, V_SHIFT = 0, V_BACKSPACE = 0;           // for multi-key input processing (동시 입력값)
unsigned short _ON_CTRL = 0, _ON_SHIFT = 0, _ON_BACKSPACE = 0;     // for prevent unnecessory processing
unsigned short TM_CTRL = LD, TM_SHIFT = LD, TM_BACKSPACE = LD;   // fading tmp value (각 키패드를 눌렀을 때 led값을 저장)



/* 
 * Init
*/
void setup() {
  Serial.begin(BAUDRATE); //
  Serial.print("Arduino based osu keyboard. BY NOVANG! Welcome!\n");
  // 출력 핀으로 설정
  pinMode(KP_CTRL,INPUT_PULLUP);     // define pinmode 핀모드를 정의한다.
  pinMode(KP_SHIFT,INPUT_PULLUP);   // 아두이노 내부에 있는 풀업 저항을 이용하는 함수
  pinMode(KP_BACKSPACE,INPUT_PULLUP);   //내부 풀업저항으로 이용하는 경우, digitalWrite는 필요하지 않다.
  pinMode(LP_CTRL,OUTPUT);     //각각 LP 변수에 저장되어 있는 값 출력 
  pinMode(LP_SHIFT,OUTPUT);     //가변저항을 연결해 사용하기 때문에 digitalwrite를 사용
  pinMode(LP_BACKSPACE,OUTPUT);
  digitalWrite(LP_CTRL,LD);    // indicate start-up
  digitalWrite(LP_SHIFT,LD);    //led의 값을 LP_ 값에 디지털 신호로 입력한다.
  digitalWrite(LP_BACKSPACE,LD);
  delay(1000); 
}

/*digitalWrite(pin, value)는 디지털 출력 핀 pin에 정수 0(LOW) 혹은 정수 1(HIGH) 값을 써 출력 핀의 전압을 0V 혹은 
 * 아두이노 보드의 동작 전압에 따라 +3.3V 혹은 +5V로 만드는 함수입니다. 당연히 이 함수도 출력으로 사용되는 핀들을 
제어하는데 사용하는 것이겠지만, 입력으로 사용하고 있는 핀에 digitalWrite(pin, HIGH)를 사용하여 
 내부 풀업 저항을 사용하게 할 수 있습니다. 
 * 
 */





/* 
 * Debug
*/
void debug() {    //디버그를 할 때 사용하는 함수, 각 변수값을 확인 할 수 있도록 출력한다.
  Serial.print("KEY: ");  
  Serial.print(digitalRead(KP_CTRL));
  Serial.print(" ");
  Serial.print(digitalRead(KP_SHIFT));
  Serial.print(" ");
  Serial.print(digitalRead(KP_BACKSPACE));
  Serial.print("   ");
  Serial.print("LED: ");
  Serial.print(digitalRead(LP_CTRL));
  Serial.print(" ");
  Serial.print(digitalRead(LP_SHIFT));
  Serial.print(" ");
  Serial.print(digitalRead(LP_BACKSPACE));
  Serial.print("\n");
}

/*
 * Read pin connection value
*/
void READPIN() { //동시 입력을 할 때 사용되는 함수로서, 각 변수의 값을 읽어들인다.
  V_BACKSPACE = digitalRead(KP_BACKSPACE);
  V_CTRL = digitalRead(KP_CTRL);
  V_SHIFT = digitalRead(KP_SHIFT);
}

/*
 * Loop
*/
void loop() { //본격적인 메인함수.
  delayMicroseconds(PROC_SPD); //너무 빨리 값을 읽는 것을 방지하기 위한 딜레이.
  READPIN(); 
  //debug(); //디버그 함수
  
  if(V_BACKSPACE == 0) { //exc 값을 검사하는 조건 검사문
    if(_ON_BACKSPACE == 0) { //입력 신호가 0이면 exc값을 입력하도록 한다.
      Keyboard.press(KEY_BACKSPACE);    // fn(`) key (replay)  release 함수가 호출되기 전까지 계속 누르고 있는 동작
      _ON_BACKSPACE = 1;  //신호를 바꾼다.
      analogWrite(LP_BACKSPACE,LD); //아날로그 신호를 출력하기 위한 함수, (신호를 출력할 핀, 출력할 신호)
      TM_BACKSPACE = LD; // TM_FN 변수에 LD 값을 저장한다.(LED가 밝기 값만큼 켜짐)
    }
  }else if(V_BACKSPACE == 1) { //동시 입력을 멈출 것을 검사
    if(_ON_BACKSPACE == 1) { // 입력을 멈추는 것인지 검사.
      Keyboard.release(KEY_BACKSPACE); //멈추면 입력을 멈추는 신호를 보낸다.
      _ON_BACKSPACE = 0; //값 저장
    }
    if(TM_BACKSPACE > 0) { // 한번에 LED가 꺼지지 않고, 서서히 꺼지도록 검사하는 조건문
      TM_BACKSPACE = TM_BACKSPACE - LD_TM; //255 였던 값이 1씩 빠르게 줄어든다.
      analogWrite(LP_BACKSPACE, TM_BACKSPACE); //led의 밝기를 조절할 수 있기 때문에 아날로그 신호를 쓴다.
    }
  }
  
  if(V_CTRL == 0) {  //exc 와 똑같은 기능을 한다.
    if(_ON_CTRL == 0) {
      Keyboard.press(KEY_LEFT_CTRL);    // a key (a)
      _ON_CTRL = 1;
      analogWrite(LP_CTRL,LD);
      TM_CTRL = LD;
    }
  }else if(V_CTRL == 1) {
    if(_ON_CTRL == 1) {
      Keyboard.release(KEY_LEFT_CTRL);
      _ON_CTRL = 0;
    }
    if(TM_CTRL > 0) {
      TM_CTRL = TM_CTRL - LD_TM;
      analogWrite(LP_CTRL, TM_CTRL); 
    }
  }

  if(V_SHIFT == 0) { //마찬가지.
    if(_ON_SHIFT == 0) {
      Keyboard.press(KEY_LEFT_SHIFT);    // x key (x)
      _ON_SHIFT = 1;
      analogWrite(LP_SHIFT,LD);
      TM_SHIFT = LD;
    }
  }else if(V_SHIFT == 1) {
    if(_ON_SHIFT == 1) {
      Keyboard.release(KEY_LEFT_SHIFT);
      _ON_SHIFT = 0;
    }
    if(TM_SHIFT > 0) {
      TM_SHIFT = TM_SHIFT - LD_TM;
      analogWrite(LP_SHIFT, TM_SHIFT);
    }
  }
}
