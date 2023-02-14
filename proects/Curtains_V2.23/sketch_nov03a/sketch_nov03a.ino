/*По нажатию на кнопку включается шаговый двигатель и крутиться заданное 
количество шагов либо останавливается сам (достиг конца) либо жмем 
кнопку он останавливается в произвольном месте жмем еще раз продолжает 
движение в том же направлении до конца. Если остановился сам доядя до 
конца, повторное нажатие кнопки запустит мотор в обрантном направлении*/

#include <AccelStepper.h>
//#include <AnalogKey.h>
#include <GyverButton.h>


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
// #define IN5 5
// #define IN6 3
// #define IN7 4
// #define IN8 2

//задаем имя мотору 8 или 4 (полушаговый - 8, шаговый - 4 режим работы)
// полушаговый мощнее но медленнее

AccelStepper motor1(8, IN1, IN2, IN3, IN4);
// AccelStepper motor2(8, IN5, IN6, IN7, IN8);

bool moveStatus1 = LOW; //переменная хранит двигается мотор1 или нет
// bool moveStatus2 = LOW; //переменная хранит двигается мотор2 или нет
bool reverseStatus1 = LOW; //переменная хранит значение реверса мотора1
// bool reverseStatus2 = LOW; //переменная хранит значение реверса мотора2
bool setStatus1 = HIGH; //переменная текущего движения мотора (LOW - вниз, HIGH - вверх)
// bool setStatus2 = LOW; //переменная текущего движения мотора (LOW - вниз, HIGH - вверх)

int distance1 = 500; // количество шагов, на которое должен пройти мотор (полный оборот 4067)
// int distance2 = 2500; // количество шагов, на которое должен пройти мотор

void setup() {

//Задаем максимальную скорость вращения двигателя(из даташита)
motor1.setMaxSpeed(1000);      //максимальная скорость мотора(800)
motor1.setAcceleration(500); //ускорение шаг/сек если нужно плавно стартовать(400)
motor1.setSpeed(1000); //скорость двигателя в работе(800)
motor1.moveTo(distance1); //на сколько шагов повернуться


// motor2.setMaxSpeed(800);      //максимальная скорость мотора  
// motor2.setAcceleration(400); //ускорение шаг/сек если нужно плавно стартовать
// motor2.setSpeed(800); //скорость двигателя в работе
// motor2.moveTo(distance2); //на сколько шагов повернуться

//настройка кнопки
butt.setTimeout(300);         //время удержания в мс (по умолчанию 300)
butt.setType(LOW_PULL);       //кнопка подтянута к земле - управляется плюсом
butt.setDirection(NORM_OPEN); //нормально разомкнутая кнопка
butt.setTickMode(AUTO);       //кнопка опрашивается автоматически, если MANUAL то надо в цикле loop самому опрашивать

Serial.begin(9600);

//pinMode(butt, INPUT); //объявляем что кнопка это вход

}

void loop() 
{
  
  if(butt.isClick()){               //если кнопка нажата
      moveStatus1 = !moveStatus1;   //меняем статус движения1 на противоположный
    } 

  if(butt.isHolded()){                //если кнопка удерживается более 0.5 сек
    reverseStatus1 = !reverseStatus1; //!reverseStatus1; //меняем статус реверса мотора на противоположный
    Serial.print("reverseStatus1 = ");
    Serial.println(reverseStatus1);
  }
    
  if(moveStatus1){ //если статус мотора HIGH - нажали кнопку
    moveMotor(distance1);
    Serial.print("distanceToGO = ");
    Serial.println(motor1.distanceToGo());
  } 

}

void moveMotor(long dist) {
  motor1.moveTo(dist*setStatus1);
  
  Serial.print("targetPosition= ");
  Serial.println(motor1.targetPosition());

  if(motor1.distanceToGo() == 0) {
    Serial.print("Current position = ");
    Serial.println(motor1.currentPosition());
    setStatus1 = !setStatus1;
    moveStatus1 = LOW;
    reverseStatus1 = LOW;
    motor1.disableOutputs();
  }
  // Serial.println(motor1.currentPosition());
  motor1.run();
}



