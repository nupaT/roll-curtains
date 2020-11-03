/*По нажатию на кнопку включается шаговый двигатель и крутиться заданное 
количество шагов либо останавливается сам (достиг конца) либо жмем 
кнопку он останавливается в произвольном месте жмем еще раз продолжает 
движение в том же направлении до конца. Если остановился сам доядя до 
конца, повторное нажатие кнопки запустит мотор в обрантном направлении*/

#include <GyverStepper.h>
#include <AnalogKey.h>
#include <GyverButton.h>

GStepper<STEPPER4WIRE> motor1(2048, IN1, IN2, IN3 IN4);
GStepper<STEPPER4WIRE> motor2(2048, IN5, IN6, IN7 IN8);

GButton butt(12,LOW_PULL, NORM_OPEN);


/*пины к которым подключено контроллер двигателя
ВАЖНО!! Какого-то хрена в моих двигателях (или в библиотеке)
поменяны 1 и 4 выводы. Вместо того чтобы написать по порядку
8 9 10 11 пины я отъе*ался часа 2 пока не нашел правильное 
расположение пинов. Правильный порядок ниже*/
//пины для 1 двигателя
#define IN1 11
#define IN2 9
#define IN3 10
#define IN4 8

//пины для 2 двигателя
#define IN5 5
#define IN6 3
#define IN7 4
#define IN8 2

//задаем имя мотору 8 или 4 (полушаговый - 8, шаговый - 4 режим работы)
// полушаговый мощнее но медленнее



byte butt = 12; // пин кнопки
bool moveStatus1 = LOW; //переменная хранит двигается мотор1 или нет
bool moveStatus2 = LOW; //переменная хранит двигается мотор2 или нет
bool reversStatus1 = LOW; //переменная хранит значение реверса мотора1
bool reversStatus2 = LOW; //переменная хранит значение реверса мотора2


int distance = 600; // коичество шагов, на которое должен пройти мотор


void setup() {

//Задаем максимальную скорость вращения двигателя(из даташита)
motor1.setMaxSpeed(800);      //максимальная скорость мотора
motor1.setRunMode(FOLLOW_POS);  //режим слеловния к позиции в шагах
motor1.setAcceleration(400); //ускорение шаг/сек если нужно плавно стартовать
motor1.setSpeed(800); //скорость двигателя в работе
motor1.setTarget(distance); //на сколько шагов повернуться
motot1.autoPower(true); //отключить мотор при достижении цели

motor2.setMaxSpeed(800);      //максимальная скорость мотора  
motor2.setRunMode(FOLLOW_POS);  //режим следования к позиции в шагах
motor2.setAcceleration(400); //ускорение шаг/сек если нужно плавно стартовать
motor2.setSpeed(800); //скорость двигателя в работе
motor2.setTarget(distance); //на сколько шагов повернуться
motot2.autoPower(true); //отключить мотор при достижении цели

butt.setTimeout(500);         //время удержания в мс (по умолчанию 300)
butt.setType(LOW_PULL);       //кнопка подтянута к земле - управляется плюсом
butt.setDirection(NORM_OPEN); //нормально разомкнутая кнопка
butt.setTickMode(AUTO);       //кнопка опрашивается автоматически, если MANUAL то надо в цикле loop самому опрашивать

//pinMode(butt, INPUT); //объявляем что кнопка это вход

}

void loop() 
{
  if(butt.isClick())              //если кнопка нажата
    {
      moveStatus1 = !moveStatus1; //меняем статус движения1 на противоположный
      moveStatus2 = !moveStatus2; //меняем статус движения2 на противоположный
    } 

  if(butt.isHolded()){              //если кнопка удерживается более 0.5 сек
    reversStatus1 = !reversStatus1; //меняем статус реверса мотора на противоположный
    reversStatus2 = !reversStatus2; //меняем статус реверса мотора на противоположный
  }
    
    if(moveStatus1 == HIGH)  //если статус мотора1 ВКЛ
    {
      if (motor1.getTarget() == 0 || reversStatus1 == HIGH) //если дистанция до конца равна 0
      {
      // то меняем направление движения и задаем новую дистанцию
      // если не отнять distance то первый проход будет от 0 до distance
      // а последующие от -distance до +distance
      motor1.moveTo(-(motor1.currentPosition() - distance)); 
      moveStatus1 = LOW;  //останавливаем двигатель
      }
      motor1.tick(); //иначе крутим двигатель до конца дистанции
    }
    
    if(moveStatus2 == HIGH)
       {
        if(motor2.distanceToGo() == 0 || reversStatus2 == HIGH)
        {
          motor2.moveTo(-(motor2.currentPosition() - distance));
          moveStatus2 = LOW;
        }
        motor2.tick();
       }
    
       //отключаем питание с обмоток двигателя если он остановлен (экономия энергии + не греется мотор)
    if(moveStatus1 == LOW)
    {
      motor1.disableOutputs();
    }
    if(moveStatus2 == LOW)
    {
      motor2.disableOutputs();
    }
   lastButt = setButt; //обязательно присваиваем последнему значению кнопки текущее
}

//функция антидребезга
bool debounce (byte button)
{
  bool setButt = digitalRead (button);
  if(last != setButt)
  {
    unsigned long setMillis = millis();
    if(setMillis - lastMillis > periodMillis)
    {
      setButt = digitalRead(butt);
      lastMillis = setMillis;
    }
  }
  return setButt;
}
