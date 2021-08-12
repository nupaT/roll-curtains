

/*По нажатию на кнопку включается шаговый двигатель и крутиться заданное 
количество шагов либо останавливается сам (достиг конца) либо жмем 
кнопку он останавливается в произвольном месте жмем еще раз продолжает 
движение в том же направлении до конца. Если остановился сам доядя до 
конца, повторное нажатие кнопки запустит мотор в обрантном направлении*/

//#include <AccelStepper.h>
#include <AnalogKey.h>
#include <GyverButton.h>
#include <GyverStepper.h>





GButton butt(12,LOW_PULL, NORM_OPEN);


/*пины к которым подключено контроллер двигателя
ВАЖНО!! Какого-то хрена в моих двигателях (или в библиотеке)
поменяны 1 и 4 выводы. Вместо того чтобы написать по порядку
8 9 10 11 пины я отъе*ался часа 2 пока не нашел правильное 
расположение пинов. Правильный порядок ниже*/
//пины для 1 двигателя
//#define IN1 11 
//#define IN2 9
//#define IN3 10
//#define IN4 8


#define IN1 8   //обратный порядок пинов -> моторы установлены зеркально
#define IN2 10
#define IN3 9
#define IN4 11

//пины для 2 двигателя
#define IN5 5
#define IN6 3
#define IN7 4
#define IN8 2

//задаем имя мотору 8 или 4 (полушаговый - 8, шаговый - 4 режим работы)
// полушаговый мощнее но медленнее

GStepper<STEPPER4WIRE> motor1(1, IN1, IN2, IN3, IN4);
/*AccelStepper motor2(8, IN5, IN6, IN7, IN8);*/

bool moveStatus1 = LOW; //переменная хранит двигается мотор1 или нет
bool moveStatus2 = LOW; //переменная хранит двигается мотор2 или нет
bool reverseStatus1 = LOW; //переменная хранит значение реверса мотора1
bool reverseStatus2 = LOW; //переменная хранит значение реверса мотора2
int setStatus1 = 1; //перменная текущего движения мотора (1 - вниз, -1 - вверх)
int setStatus2 = LOW; //перменная текущего движения мотора (LOW - вниз, HIGH - вверх)

int distance1 = 40; // коичество шагов, на которое должен пройти мотор
//int distance2 = 100; // коичество шагов, на которое должен пройти мотор

void setup() {

  

//Задаем максимальную скорость вращения двигателя(из даташита)
motor1.setMaxSpeed(20);      //максимальная скорость мотора(800)
motor1.setAcceleration(20); //ускорение шаг/сек если нужно плавно стартовать(400)
motor1.setSpeed(20); //скорость двигателя в работе(800)
//motor1.setTarget(distance1); //на сколько шагов повернуться
motor1.setRunMode(FOLLOW_POS); //режим работы мотора (двигаться до цели в шагах)
motor1.autoPower(true);

/*
motor2.setMaxSpeed(800);      //максимальная скорость мотора  
motor2.setAcceleration(400); //ускорение шаг/сек если нужно плавно стартовать
motor2.setSpeed(800); //скорость двигателя в работе
motor2.moveTo(distance2); //на сколько шагов повернуться
motor2.setRunMode(FOLLOW_POS); //режим работы мотора (двигаться до цели в шагах)
*/

//настройка кнопки
butt.setTimeout(500);         //время удержания в мс (по умолчанию 300)
butt.setType(LOW_PULL);       //кнопка подтянута к земле - управляется плюсом
butt.setDirection(NORM_OPEN); //нормально разомкнутая кнопка
butt.setTickMode(AUTO);       //кнопка опрашивается автоматически, если MANUAL то надо в цикле loop самому опрашивать

Serial.begin(9600);

//pinMode(butt, INPUT); //объявляем что кнопка это вход

}

void loop() 
{
  
 if(butt.isClick()) {         //если кнопка нажата - меняем статус мотора на ON (двигается)
  moveStatus1 = !moveStatus1;
 }

  if(moveStatus1) {    //если мотор ON(двигается)
    moveMotor(distance1);     //запускаем мотор от 0 до distance1 (направление меняется в самой функции)
  } 
    
 
 if(butt.isHolded()) {
 }
//  moveMotorReverse(-motor1.getCurrent());
}

void moveMotor(long dist){
    motor1.setTarget(dist*setStatus1);  //устанавливаем конечное положение для мотора и направление setStatus 1 - вниз
  if(motor1.getCurrent() != motor1.getTarget()){  //если текущее положение не равно конечному
    motor1.tick();  //крутим мотор
    Serial.println(motor1.getCurrent());
    Serial.print("Target: ");
    Serial.println(motor1.getTarget());
    } else {      //иначе (мотор достиг конечного положения)
      motor1.stop(); //тормозим мотор
      motor1.reset();  //сбрасываем текущее положение мотора на 0
      moveStatus1 = LOW;  //меняем статус мотора на ВЫКЛ
      setStatus1 = (setStatus1*(-1));   //меняем значение направления движения на противоположное
      Serial.print("setStatus1: ");
      Serial.println(setStatus1);
      return;
    }
}

//void moveMotorReverse(long revDist) {
//    motor1.setTarget(revDist);
//    motor1.reset();
//    if(motor1.getCurrent() != revDist) {
//      motor1.tick();
//      }
//}
