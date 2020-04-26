///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//   Sketch para imprimir en una pantalla LCD 16x4 el valor leido en el pin analogico A0         //
//   Conecciones:                                                                                //
//   A0: entrada de señal de 0.07V a 0.5V (obs.: valores distintos cabiar rango en line55 )      //
//   A4: SDA                                                                                     // 
//   A5: SCL                                                                                     //                   
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////


#include<Wire.h>                                                          //inclusion de libreria
#include<LiquidCrystal_I2C.h>                                             //inclusion de libreria
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);                    //configuracion de direcion y del lcd

float entrMando   = A0;                                                   //Definimos el pin analógico A0 para la lectura del voltaje
int lectura       = 0;                                                    //Variable  para recibir el valor leido no pin analg.
int matoria       = 0;                                                    //Variable para recibir el valor de sumatoria
int media         = 0;                                                    //Variable que recibe el valor de media
float ajust       = 0;                                                    //Variable que recibe el volor final ya calculado
int s             = 0;                                                    //Varible auxiliar para los bucles "FOR"
byte flec_d[8]    = {0x0,0x8,0xc,0xe,0x1f,0xe,0xc,0x8};                   //criando caracter en exadecimal "flecha derecha"
byte flec_i[8]    = {0x0,0x2,0x6,0xe,0x1f,0xe,0x6,0x2};                   //criando caracter en exadecimal "flecha izquerda"

void setup(){
  pinMode(entrMando, INPUT);                                              //Activamos el input del pin analógico A0
  lcd.begin(16, 4);                                                       //Iniciamos comunicacion con el lcd
  lcd.setBacklight(LOW);                                                  //Apagamos el led del lcd
  lcd.createChar(1, flec_d);                                              //nombramos el caracter flecha_d como "1"
  lcd.createChar(2, flec_i);                                              //nombramos el caracter flecha_i como "2"
}

//////////////////////////////////////////////////////////////////////////

void loop(){                                                              //Funcion loop
  Lec_ccl();                                                              //Lamamo funcion de lectura do pin y calculo do valor "Lec_ccl"
  if (ajust < 2.4){                                                       //Condicional para que se o resultado sea menor que 2.4V
    flecD ();                                                             //Lamamos funcion flecha_D
  }
  if (ajust > 2.6){                                                       //Condicional para que se o resultado sea mayor que 2.6V
    flecI ();                                                             //Lamamos funcion flecha_I
  }    
}

//////////////////////////////////////////////////////////////////////////

void Lec_ccl(){                                                           //Funcion que hace lectura do pin analogico y calcula o valor
  ajust   = 0;                                                            //Inicio la variable con valor "0"
  media   = 0;                                                            //Inicio la variable con valor "0"
  matoria = 0;                                                            //Inicio la variable con valor "0"
  for (int i = 0; i < 60; i++){                                           //Bucle para leer y sumar 60 los valores leidos 
    lectura = analogRead(entrMando);                                      //Leo el valor do pin y cargo en la variable
    matoria = matoria + lectura;                                          //Se hace una sumatoria y se carga en la variable "matoria"
    delay (2);                                                            //Retardo para lectura
  }
  media = matoria / 60;                                                   //Calculo el valor de media
  ajust = map(media,1 ,105,0,500);                                        //Ajusto los valores
  ajust = ajust/100;                                                      //Ajusto casas decimales
  if (ajust < 0.7){                                                       //Condicional para maten un valor minimo
    ajust = 0.7;                                                          //Mantengo o valor d la variable en 0.7
  }
  Msn();                                                                  //Llamo funcion "Mensagen" "Msn"
  return(ajust);                                                          //Retorno el valor calculado 
}

void Msn (){                                                              //Funcion que imprime en la LCD los textos y valores
  lcd.clear();                                                            //Comando para limpieza de pantalla
  lcd.setCursor(1, 0);                                                    //Comando para ubicar en linea y columna
  lcd.print("MANDO HAULOTTE");                                            //Comando para imprimir en el LCD este mensaje
  lcd.setCursor(1, 1);                                                    //Comando para ubicar en linea y columna
  lcd.print("Valor en EJE Y");                                            //Comando para imprimir en el LCD este mensaje
  lcd.setCursor(1, 3);                                                    //Comando para ubicar en linea y columna
  lcd.print("lectura ");                                                  //Comando para imprimir en el LCD este mensaje
  lcd.print(ajust,1);                                                     //Comando para imprimir en el LCD este mensaje
  lcd.setCursor(13, 3);                                                   //Comando para ubicar en linea y columna
  lcd.print("V");                                                         //Comando para imprimir en el LCD este mensaje
  return(ajust);                                                          //Retorno el valor calculado
}

void flecI (){                                                            //Funcion para imprimer una flecha en la LCD
  for (s = 0; s <= 15;)                                                   //Bucle para recorer todas columnas do LCD
    {                                                                         
      Lec_ccl();                                                          //Llamada de la funcion
      lcd.setCursor(s, 2);                                                //Comando para situamos el cursor
      lcd.print((char)1);                                                 //Comando para escribimos el texto
      s++;                                                                //Acresimo en la variable de control 
    }
}

void flecD (){                                                            //Funcion para imprimer una flecha en la LCD
  for (s = 16; s >= 0;)                                                   //Bucle para recorer todas columnas do LCD
    {
      Lec_ccl();                                                          //Llamada de la funcion 
      lcd.setCursor(s, 2);                                                //Comando para situamos el cursor
      lcd.print((char)2);                                                 //Comando para escribimos el texto
      s--;                                                                //Acresimo en la variable de control
    }  
}
