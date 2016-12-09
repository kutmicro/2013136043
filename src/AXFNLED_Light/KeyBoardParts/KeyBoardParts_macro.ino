
#include <Keyboard.h>


#define BAUDRATE    250000  // baudrate for serial communication 1초동안 전송되는 신호의 수

#define KP_CTRL        A0      // CTRL key pin number  키패드 디지털 핀 지정
#define KP_SHIFT        A1      // SHIFT key pin number
#define KP_STOP       A2      // STOP key pin number

#define LP_CTRL        9       // CTRL led pin number 
#define LP_SHIFT        10      // SHIFT led pin number
#define LP_STOP       11      // STOP led pin number

#define PROC_SPD    100       // processing speed (us)  반복 속도
#define LD          255     // led power (0-255) 밝기
#define LD_TM       1     // led minus count (0-255) 밝기를 줄여주기 위한 변수

unsigned short V_CTRL = 0, V_SHIFT = 0, V_STOP = 0;           // for multi-key input processing 
unsigned short _ON_CTRL = 0, _ON_SHIFT = 0, _ON_STOP = 0;     // for prevent unnecessory processing
unsigned short TM_CTRL = LD, TM_SHIFT = LD, TM_STOP = LD;   // fading tmp value 
unsigned short ON_OFF = 0;


/* 
 * Init
*/
void setup() {
  Serial.begin(BAUDRATE); //
  Serial.print("Arduino based osu keyboard. BY NOVANG! Welcome!\n");
  // 출력 핀으로 설정
  pinMode(KP_CTRL,INPUT_PULLUP);     // define pinmode 핀모드를 정의한다.
  pinMode(KP_SHIFT,INPUT_PULLUP);   // 아두이노 내부에 있는 풀업 저항을 이용하는 함수
  pinMode(KP_STOP,INPUT_PULLUP);   //내부 풀업저항으로 이용하는 경우, digitalWrite는 필요하지 않다.
  pinMode(LP_CTRL,OUTPUT);         //각각 LP 변수에 저장되어 있는 값 출력 
  pinMode(LP_SHIFT,OUTPUT);         //가변저항을 연결해 사용하기 때문에 digitalwrite를 사용
  pinMode(LP_STOP,OUTPUT);
  //digitalWrite(LP_CTRL,LD);    
  //digitalWrite(LP_SHIFT,LD);   
  digitalWrite(LP_STOP,LD);
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
  Serial.print(digitalRead(KP_STOP));
  Serial.print("   ");
  Serial.print("LED: ");
  Serial.print(digitalRead(LP_CTRL));
  Serial.print(" ");
  Serial.print(digitalRead(LP_SHIFT));
  Serial.print(" ");
  Serial.print(digitalRead(LP_STOP));
  Serial.print("\n");
}

/*
 * Read pin connection value
*/
void READPIN() {  //loop를 통해 디지털값을 계속 읽어들이는 함수
  V_STOP = digitalRead(KP_STOP);
  V_CTRL = digitalRead(KP_CTRL);
  V_SHIFT = digitalRead(KP_SHIFT);
}

void Light_Shift() {  //SHIFT 키를 눌렀을 때 서서히 꺼지도록 하는 함수
       analogWrite(LP_SHIFT,LD);
      TM_SHIFT = LD;
      
      while(TM_SHIFT > 0){
        delay(1);
        TM_SHIFT = TM_SHIFT - LD_TM;
      analogWrite(LP_SHIFT, TM_SHIFT);
        }
}

void Light_CTRL() { //CTRL 키를 눌렀을 때 서서히 꺼지도록 하는 함수
       analogWrite(LP_CTRL,LD);
      TM_CTRL = LD;
      
      while(TM_CTRL > 0){
        delay(1);
        TM_CTRL = TM_CTRL - LD_TM;
      analogWrite(LP_CTRL, TM_CTRL);
        }
}



/*
 * Loop
*/
void loop() { //본격적인 메인함수.
  delayMicroseconds(PROC_SPD); //너무 빨리 값을 읽는 것을 방지하기 위한 딜레이.
  READPIN(); 
  debug(); //디버그 함수
  
  if(V_STOP == 0) {        //STOP 값을 검사하는 조건 검사문
    if(_ON_STOP == 0) {    //입력 신호가 0이면 exc값을 입력하도록 한다.
      _ON_STOP = 1;        //신호를 바꾼다.
      analogWrite(LP_STOP,LD); //아날로그 신호를 출력하기 위한 함수, (신호를 출력할 핀, 출력할 신호)
      TM_STOP = LD; // TM_FN 변수에 LD 값을 저장한다.(LED가 밝기 값만큼 켜짐)
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_RIGHT_ARROW);
      Keyboard.release(KEY_LEFT_SHIFT);
      Keyboard.release(KEY_LEFT_ARROW); 
      Keyboard.release(KEY_INSERT);     
    }
    
  }else if(V_STOP == 1) {
    if(_ON_STOP == 1) {
      _ON_STOP = 0; 
    }
    
    if(TM_STOP > 0) { // 한번에 LED가 꺼지지 않고, 서서히 꺼지도록 검사하는 조건문
      TM_STOP = TM_STOP - LD_TM; //255 였던 값이 1씩 빠르게 줄어든다.
      analogWrite(LP_STOP, TM_STOP); //led의 밝기를 조절할 수 있기 때문에 아날로그 신호를 쓴다.
    }
  }

  
  if(V_CTRL== 0) { 
    if(_ON_CTRL == 0) {
        ON_OFF = 1;
        
        while(ON_OFF == 1){ 
         Light_CTRL(); 
         Keyboard.press(KEY_LEFT_CTRL);    
         Keyboard.press(KEY_LEFT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}
         delay(3000);
              
         Keyboard.release(KEY_LEFT_CTRL);
         Keyboard.release(KEY_LEFT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}    
         delay(1000); 

         Keyboard.press(KEY_LEFT_CTRL);    
         Keyboard.press(KEY_RIGHT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}
         delay(3000);

         Keyboard.release(KEY_LEFT_CTRL);
         Keyboard.release(KEY_RIGHT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}         
         delay(1000);
         }
    }
  }


  if(V_SHIFT == 0) { //마찬가지.
    if(_ON_SHIFT == 0) {
        ON_OFF = 1;
        
        while(ON_OFF == 1){ 
         Light_Shift(); 
         Keyboard.press(KEY_LEFT_SHIFT);    
         Keyboard.press(KEY_LEFT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}
         delay(3000);
              
         Keyboard.release(KEY_LEFT_SHIFT);
         Keyboard.release(KEY_LEFT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}    
  
         Keyboard.press(KEY_LEFT_SHIFT);   
         Keyboard.press(KEY_RIGHT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}
         delay(3000);

         Keyboard.release(KEY_LEFT_SHIFT);
         Keyboard.release(KEY_RIGHT_ARROW);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}  

         Keyboard.press(KEY_INSERT);   
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;}
         delay(500);

         Keyboard.release(KEY_INSERT);
         READPIN(); 
         if(V_STOP == 0){ON_OFF=0; break;} 


            

         }
    }
  }
}
