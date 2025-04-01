
#include <LiquidCrystal_I2C.h> //Install LiquidCrystal_I2C.h
#include <ToneESP32.h>

LiquidCrystal_I2C lcd(0x27,20,4);

#define botaoMais 5
#define botaoMenos 23
#define ledAlerta 2
#define ledMudanca 18
#define buzzer 19

bool modoAlteracao = false;
unsigned long tempoPressionado = 0;

unsigned long ultimoMillis = 0;
const int intervaloBuzzer = 300;

void alterandoLimite();
void atualizarLCD (void);
void tocarMelodia();
void alertaBuzzer(bool ativar);

  int contador = 0;
  int LIMITE = 10;

void setup() 
{
    pinMode(botaoMais, INPUT_PULLUP);
    pinMode(botaoMenos, INPUT_PULLUP);
    pinMode(ledAlerta, OUTPUT);
    pinMode(ledMudanca, OUTPUT);
    pinMode(buzzer, OUTPUT);

    lcd.init();
    lcd.backlight();
    atualizarLCD();
}

void loop() 
{
  //controle do botao para alternar modo
  if (digitalRead(botaoMais) == LOW && digitalRead(botaoMenos) == LOW) 
  {
            lcd.setCursor(11, 0);
            lcd.print("ALTERANDO");
            lcd.setCursor(11, 1);
            lcd.print("MODO...");
        if (tempoPressionado == 0) {
            tempoPressionado = millis();
        } else if (millis() - tempoPressionado >= 3000) {
            modoAlteracao = !modoAlteracao;
            tocarMelodia();
            digitalWrite(ledMudanca, modoAlteracao ? HIGH : LOW);
            atualizarLCD();
    }
  }

  else 
  {
    tempoPressionado = 0;
  }
  //controle do botao / alteracao dos valores

 //alterar o limite
if (millis() - ultimoMillis > 50) {
        ultimoMillis = millis();
  if (modoAlteracao)
  {
    lcd.setCursor(0,3);
    lcd.print("ALTERANDO LIMITE...");
    
    if (digitalRead(botaoMais) == LOW)
    {
      LIMITE++;
      delay(300);
      atualizarLCD();
    }

    if (digitalRead(botaoMenos) == LOW && LIMITE > 0)
    {
      LIMITE--;
      delay(300);
      atualizarLCD();
    }
  }
  // alterar numero de pessoas
    else
    {
        digitalWrite(ledMudanca, LOW);

        if (digitalRead(botaoMais) == LOW)
        {
            contador++;
            delay(300);
            atualizarLCD();
        }

        if (digitalRead(botaoMenos) == LOW && contador > 0)
        {
            contador--;
            delay(300);
            atualizarLCD();
        }
        
        // se o numero de gente for maior que o limite, piscar
        if (contador >= LIMITE)
        {
          digitalWrite(ledAlerta, HIGH);
          alertaBuzzer(true);
        }

        else
        {
          digitalWrite(ledAlerta, LOW);
          alertaBuzzer(false);
        }
    }
  }
}

void atualizarLCD() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Numero: ");
    lcd.print(contador);
    
    lcd.setCursor(0, 1);
    lcd.print("Limite: ");
    lcd.print(LIMITE);
}

void tocarMelodia()
{
      int melodia[] = {293, 293, 349, 440}; 
      int duracao = 150; 

    ledcAttachPin(buzzer, 0);
    for (int i = 0; i < 4; i++)
    {
      ledcWriteTone(0, melodia[i]); 
      delay(duracao + 50);
    }
    ledcDetachPin(buzzer);

  
}

void alertaBuzzer(bool ativar)
{
    if (ativar)
    {
        ledcAttachPin(buzzer, 0);
        ledcWriteTone(0, 1000);
    }
    else
    {
        noTone(buzzer); 
    }
}