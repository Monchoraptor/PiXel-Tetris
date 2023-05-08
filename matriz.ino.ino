#include <Adafruit_NeoPixel.h>

#define pin 6

Adafruit_NeoPixel matriz = Adafruit_NeoPixel(128, pin, NEO_GRB + NEO_KHZ800);

/*TETRIS*/

int modo = 1; // 0 = tetris, 1 = pixel
int auxm = 0; 
int auxm2 = 0;
int t; 
int pieza = 0; // 0 = I, 1 = J, 2 = L, 3 = O, 4 = S, 5 = T, 6 = Z
int rotacion = 0;
int posicion[4][2]; //posicion de las piezas
long milis = 0; //tiempo
long bajar = 0; //tiempo desde la ultima bajada
long musica = 0; //tiempo desde la ultima nota
int nota = 0; //nota actual
long last_move = 0; //tiempo desde el ultimo movimiento

/*PIXEL*/

int redButton = 8;
int greenButton = 9;
int blueButton = 10;
int down = 2;
int up = 3;
int redButtonState;
int greenButtonState;
int blueButtonState;
int downButtonState;
int upButtonState;
int red;
int green;
int blue;
int value = 0;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int x_pin = A0;
int y_pin = A1;
int sw_pin = 4;
int cursor = 0;
int x = 0;
int y = 0;
int aux[3] = {0, 0, 0};

int tablero[16][8][3] = {
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};


void setup() // se ejecuta al inicio
{
  pinMode(5, INPUT);
  Serial.begin(9600);

  /* Tetris*/

  pinMode(9, OUTPUT);
  matriz.begin();
  matriz.setPixelColor(0, matriz.Color(0, 0, 255));
  matriz.show();
  t = 0;
  randomSeed(analogRead(0));
  pieza = random(1, 7);
  // pieza = 1;
  print_pieza();
  musica = millis();

  /*Pixel*/

  // matriz.begin();
  pinMode(redButton, INPUT);
  pinMode(greenButton, INPUT);
  pinMode(blueButton, INPUT);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(4, INPUT);
  digitalWrite(4, HIGH);
}

void loop()
{
  auxm = digitalRead(5);
  if ((auxm == 0) && (modo == 1))
  {
    nota = 0;
    musica = millis();
    last_move = millis();
    bajar = millis();
    reset_matrix();
    endgame2();
    modo = 0;
  }
  if ((auxm > 0) && (modo == 0))
  {
    endgame2();
    noTone(11);
    reset_matrix();
    modo = 1;
  }
  if (modo == 0)
    tetris();
  else
    pixel();
}

void reset_matrix() // reinicia la matriz
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      for (int k = 0; k < 3; k++)
      {
        tablero[i][j][k] = 0;
      }
    }
  }
}

void tetris() //función principal del tetris
{

  delay(50);
  Serial.println(millis());
  // Serial.println(musica);
  print_matrix();
  // Serial.println(1000 + bajar);
  if (millis() > 1000 + bajar)
  {
    // Serial.println("bajar");
    move_down();
  }
  if (last_move < millis() - 100)
  {
    // Serial.println("check");
    check_joystick();
  }
  if (musica < millis())
  {
    // Serial.println("musica");
    musica += play_note(nota);
    nota++;
    if (nota > 383)
    {
      nota = 0;
    }
  }
}

void check_joystick() //comprueba si se ha movido el joystick o se ha pulsado un botón
{
  int x = analogRead(A0);
  int y = analogRead(A1);
  if (x < 100)
  {
    last_move = millis();
    // Serial.println("left");
    move_left();
  }
  else if (x > 900)
  {
    // Serial.println("right");
    last_move = millis();
    move_right();
  }

  else if (digitalRead(up))
  {
    // Serial.println("rotate");
    last_move = millis();
    rotate();
  }
  else if (digitalRead(down))
  {
    last_move = millis();
    for (int i = 0; i < 8; i++)
    {
      move_down();
    }
  }
  else if (y > 900)
  {
    // Serial.println("down");
    last_move = millis();
    move_down();
  }
}

void move_left() // mueve la pieza a la izquierda
{
  if (can_move_l())
  {
    for (int i = 0; i < 4; i++)
    {
      tablero[posicion[i][0]][posicion[i][1]][0] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
      posicion[i][1] -= 1;
      tablero[posicion[i][0]][posicion[i][1]][0] = pieza;
    }
  }
}

void move_right() // mueve la pieza a la derecha
{
  if (can_move_r())
  {
    for (int i = 0; i < 4; i++)
    {
      tablero[posicion[i][0]][posicion[i][1]][0] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
      posicion[i][1] += 1;
      tablero[posicion[i][0]][posicion[i][1]][0] = pieza;
    }
  }
}

void rotate() // rota la pieza que está cayendo
{
  switch (pieza) // donde opción es la variable a comparar
  {
  case 1: // I
    switch (rotacion)
    {
    case 0:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] += 1;
      posicion[0][1] += 1;
      posicion[2][0] -= 1;
      posicion[2][1] -= 1;
      posicion[3][0] -= 2;
      posicion[3][1] -= 2;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 1:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] -= 1;
      posicion[0][1] -= 1;
      posicion[2][0] += 1;
      posicion[2][1] += 1;
      posicion[3][0] += 2;
      posicion[3][1] += 2;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion--;
      break;
    default:
      break;
    }
    break;
  case 2: // T
    switch (rotacion)
    {
    case 0:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[1][0] -= 1;
      posicion[1][1] += 1;
      posicion[2][0] += 1;
      posicion[2][1] -= 1;
      posicion[3][0] -= 1;
      posicion[3][1] -= 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 1:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[1][0] += 1;
      posicion[1][1] += 1;
      posicion[2][0] -= 1;
      posicion[2][1] -= 1;
      posicion[3][0] -= 1;
      posicion[3][1] += 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 2:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[1][0] += 1;
      posicion[1][1] -= 1;
      posicion[2][0] -= 1;
      posicion[2][1] += 1;
      posicion[3][0] += 1;
      posicion[3][1] += 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 3:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[1][0] -= 1;
      posicion[1][1] -= 1;
      posicion[2][0] += 1;
      posicion[2][1] += 1;
      posicion[3][0] += 1;
      posicion[3][1] -= 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion = 0;
      break;
    default:
      break;
    }
    break;
  case 3: // L
    switch (rotacion)
    {
    case 0:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] += 1;
      posicion[0][1] += 2;
      posicion[1][0] += 0;
      posicion[1][1] += 1;
      posicion[2][0] -= 1;
      posicion[2][1] += 0;
      posicion[3][0] += 0;
      posicion[3][1] -= 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 1:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] += 1;
      posicion[0][1] += 0;
      posicion[1][0] += 0;
      posicion[1][1] += 1;
      posicion[2][0] -= 1;
      posicion[2][1] += 2;
      posicion[3][0] -= 2;
      posicion[3][1] += 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 2:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] -= 0;
      posicion[0][1] -= 2;
      posicion[1][0] += 1;
      posicion[1][1] -= 1;
      posicion[2][0] += 2;
      posicion[2][1] += 0;
      posicion[3][0] += 1;
      posicion[3][1] += 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 3:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] -= 2;
      posicion[0][1] += 0;
      posicion[1][0] -= 1;
      posicion[1][1] -= 1;
      posicion[2][0] -= 0;
      posicion[2][1] -= 2;
      posicion[3][0] += 1;
      posicion[3][1] -= 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion = 0;
      break;
    default:
      break;
    }
    break;
  case 4: // J
    switch (rotacion)
    {
    case 0:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] += 2;
      posicion[0][1] += 0;
      posicion[1][0] += 1;
      posicion[1][1] -= 1;
      posicion[2][0] -= 0;
      posicion[2][1] -= 2;
      posicion[3][0] -= 1;
      posicion[3][1] -= 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 1:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] += 0;
      posicion[0][1] -= 2;
      posicion[1][0] -= 1;
      posicion[1][1] -= 1;
      posicion[2][0] -= 2;
      posicion[2][1] += 0;
      posicion[3][0] -= 1;
      posicion[3][1] += 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 2:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] -= 1;
      posicion[0][1] -= 0;
      posicion[1][0] += 0;
      posicion[1][1] += 1;
      posicion[2][0] += 1;
      posicion[2][1] += 2;
      posicion[3][0] += 2;
      posicion[3][1] += 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 3:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] -= 1;
      posicion[0][1] += 2;
      posicion[1][0] -= 0;
      posicion[1][1] += 1;
      posicion[2][0] += 1;
      posicion[2][1] -= 0;
      posicion[3][0] += 0;
      posicion[3][1] -= 1;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion = 0;
      break;
    default:
      break;
    }
    break;
  case 6: // S
    switch (rotacion)
    {
    case 0:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] += 1;
      posicion[0][1] += 1;
      posicion[2][0] += 1;
      posicion[2][1] -= 1;
      posicion[3][0] -= 0;
      posicion[3][1] -= 2;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 1:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] -= 1;
      posicion[0][1] -= 1;
      posicion[2][0] -= 1;
      posicion[2][1] += 1;
      posicion[3][0] += 0;
      posicion[3][1] += 2;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion--;
      break;
    default:
      break;
    }
    break;
  case 7: // Z
    switch (rotacion)
    {
    case 0:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] += 1;
      posicion[0][1] -= 1;
      posicion[2][0] += 1;
      posicion[2][1] += 1;
      posicion[3][0] -= 0;
      posicion[3][1] += 2;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion++;
      break;
    case 1:
      tablero[posicion[0][0]][posicion[0][1]][0] = 0;
      tablero[posicion[1][0]][posicion[1][1]][0] = 0;
      tablero[posicion[2][0]][posicion[2][1]][0] = 0;
      tablero[posicion[3][0]][posicion[3][1]][0] = 0;
      posicion[0][0] -= 1;
      posicion[0][1] += 1;
      posicion[2][0] -= 1;
      posicion[2][1] -= 1;
      posicion[3][0] += 0;
      posicion[3][1] -= 2;
      tablero[posicion[0][0]][posicion[0][1]][0] = pieza;
      tablero[posicion[1][0]][posicion[1][1]][0] = pieza;
      tablero[posicion[2][0]][posicion[2][1]][0] = pieza;
      tablero[posicion[3][0]][posicion[3][1]][0] = pieza;
      rotacion--;
      break;
    default:
      break;
    }
    break;
  }
}

void move_down() // Mueve la pieza hacia abajo
{
  if (can_move())
  {
    for (int i = 0; i < 4; i++)
    {
      tablero[posicion[i][0]][posicion[i][1]][0] = 0;
    }
    for (int i = 0; i < 4; i++)
    {
      posicion[i][0] += 1;
      tablero[posicion[i][0]][posicion[i][1]][0] = pieza;
    }
    bajar = millis();
  }
  else
  {
    check_line();
    check_endgame();
    pieza = random(1, 8);
    print_pieza();
  }
}

void check_endgame() // Revisa si el juego termino
{
  for (int i = 0; i < 8; i++)
  {
    if (tablero[0][i][0] != 0)
    {
      endgame();
    }
  }
}

void endgame() // Termina el juego y lo reinicia
{
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (tablero[i][j][0] != 0)
      {
        tablero[i][j][0] = 6;
      }
    }
    print_matrix();
    delay(200);
  }
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (tablero[i][j][0] != 0)
      {
        tablero[i][j][0] = 0;
      }
    }
    print_matrix();
    delay(200);
  }
  bajar = millis();
  musica = millis();
  nota = 0;
}

void endgame2() // Version de endgame sin delays para el cambio de modo
{
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (tablero[i][j][0] != 0)
      {
        tablero[i][j][0] = 6;
      }
    }
    print_matrix();
  }
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (tablero[i][j][0] != 0)
      {
        tablero[i][j][0] = 0;
      }
    }
  }
  bajar = millis();
  musica = millis();
  nota = 0;
}

void check_line() // Revisa si hay una linea completa
{
  int line;
  for (int i = 0; i < 16; i++)
  {
    line = 0;
    for (int j = 0; j < 8; j++)
    {
      if (tablero[i][j][0] != 0)
        line++;
    }
    if (line == 8)
    {
      for (int k = 0; k < 8; k++)
      {
        tablero[i][k][0] = 8;
      }
      print_matrix();
      delay(200);
      tablero[i][3][0] = 0;
      tablero[i][4][0] = 0;
      print_matrix();
      delay(200);
      tablero[i][2][0] = 0;
      tablero[i][5][0] = 0;
      print_matrix();
      delay(200);
      tablero[i][1][0] = 0;
      tablero[i][6][0] = 0;
      print_matrix();
      delay(200);
      tablero[i][0][0] = 0;
      tablero[i][7][0] = 0;
      print_matrix();
      delay(200);
      for (int k = i; k > 0; k--)
      {
        for (int l = 0; l < 8; l++)
        {
          tablero[k][l][0] = tablero[k - 1][l][0];
        }
      }
      for (int k = 0; k < 8; k++)
      {
        tablero[0][k][0] = 0;
      }
    }
  }
}

bool can_move_l() // Comprueba si se puede mover a la izquierda
{
  for (int i = 0; i < 4; i++)
  {
    if (posicion[i][1] == 0)
    {
      return false;
    }
  }
  for (int i = 0; i < 4; i++)
  {
    if ((tablero[posicion[i][0]][posicion[i][1] - 1][0] != 0) && (!is_pieza(posicion[i][0], posicion[i][1] - 1, i)))
    {
      return false;
    }
  }
  return true;
}

bool can_move_r() // Comprueba si se puede mover a la derecha
{
  for (int i = 0; i < 4; i++)
  {
    if (posicion[i][1] == 7)
    {
      return false;
    }
  }
  for (int i = 0; i < 4; i++)
  {
    if ((tablero[posicion[i][0]][posicion[i][1] + 1][0] != 0) && (!is_pieza(posicion[i][0], posicion[i][1] + 1, i)))
    {
      return false;
    }
  }
  return true;
}

bool can_move() // Comprueba si se puede mover hacia abajo
{
  for (int i = 0; i < 4; i++)
  {
    if (posicion[i][0] == 15)
    {
      return false;
    }
  }
  for (int i = 0; i < 4; i++)
  {
    if ((tablero[posicion[i][0] + 1][posicion[i][1]][0] != 0) && (!is_pieza(posicion[i][0] + 1, posicion[i][1], i)))
    {
      Serial.println("no se puede mover");
      Serial.println(posicion[i][0]);
      Serial.println(posicion[i][1]);
      Serial.println(tablero[posicion[i][0] + 1][posicion[i][1]][0]);

      return false;
    }
  }
  return true;
}

bool is_pieza(int x, int y, int i) // Comprueba si una coordenada pertenece a la pieza en juego
{
  for (int j = 0; j < 4; j++)
  {
    if ((posicion[j][0] == x) && (posicion[j][1] == y) && (j != i))
    {
      return true;
    }
  }
  return false;
}

void print_pieza() // Genera una nueva pieza en el tablero
{
  rotacion = 0;
  if (pieza == 5) // O
  {
    tablero[0][3][0] = 5;
    tablero[0][4][0] = 5;
    tablero[1][3][0] = 5;
    tablero[1][4][0] = 5;
    posicion[0][0] = 0;
    posicion[0][1] = 3;
    posicion[1][0] = 0;
    posicion[1][1] = 4;
    posicion[2][0] = 1;
    posicion[2][1] = 3;
    posicion[3][0] = 1;
    posicion[3][1] = 4;
    print_matrix();
    return;
  }
  if (pieza == 1) // I
  {
    tablero[0][3][0] = 1;
    tablero[1][3][0] = 1;
    tablero[2][3][0] = 1;
    tablero[3][3][0] = 1;
    posicion[0][0] = 0;
    posicion[0][1] = 3;
    posicion[1][0] = 1;
    posicion[1][1] = 3;
    posicion[2][0] = 2;
    posicion[2][1] = 3;
    posicion[3][0] = 3;
    posicion[3][1] = 3;
    print_matrix();
    return;
  }
  if (pieza == 2) // T
  {
    tablero[0][3][0] = 2;
    tablero[0][2][0] = 2;
    tablero[0][4][0] = 2;
    tablero[1][3][0] = 2;
    posicion[0][0] = 0;
    posicion[0][1] = 3;
    posicion[1][0] = 0;
    posicion[1][1] = 2;
    posicion[2][0] = 0;
    posicion[2][1] = 4;
    posicion[3][0] = 1;
    posicion[3][1] = 3;
    print_matrix();
    return;
  }
  if (pieza == 3) // L
  {
    tablero[0][3][0] = 3;
    tablero[1][3][0] = 3;
    tablero[2][3][0] = 3;
    tablero[2][4][0] = 3;
    posicion[0][0] = 0;
    posicion[0][1] = 3;
    posicion[1][0] = 1;
    posicion[1][1] = 3;
    posicion[2][0] = 2;
    posicion[2][1] = 3;
    posicion[3][0] = 2;
    posicion[3][1] = 4;
    print_matrix();
    return;
  }
  if (pieza == 4) // J
  {
    tablero[0][3][0] = 4;
    tablero[1][3][0] = 4;
    tablero[2][3][0] = 4;
    tablero[2][2][0] = 4;
    posicion[0][0] = 0;
    posicion[0][1] = 3;
    posicion[1][0] = 1;
    posicion[1][1] = 3;
    posicion[2][0] = 2;
    posicion[2][1] = 3;
    posicion[3][0] = 2;
    posicion[3][1] = 2;
    print_matrix();
    return;
  }
  if (pieza == 6) // S
  {
    tablero[0][3][0] = 6;
    tablero[1][3][0] = 6;
    tablero[1][4][0] = 6;
    tablero[2][4][0] = 6;
    posicion[0][0] = 0;
    posicion[0][1] = 3;
    posicion[1][0] = 1;
    posicion[1][1] = 3;
    posicion[2][0] = 1;
    posicion[2][1] = 4;
    posicion[3][0] = 2;
    posicion[3][1] = 4;
    print_matrix();
    return;
  }
  if (pieza == 7) // Z
  {
    tablero[0][3][0] = 7;
    tablero[1][3][0] = 7;
    tablero[1][2][0] = 7;
    tablero[2][2][0] = 7;
    posicion[0][0] = 0;
    posicion[0][1] = 3;
    posicion[1][0] = 1;
    posicion[1][1] = 3;
    posicion[2][0] = 1;
    posicion[2][1] = 2;
    posicion[3][0] = 2;
    posicion[3][1] = 2;
    print_matrix();
    return;
  }
}

void print_matrix() // Imprime la variable matriz en la matriz de leds
{
  int i = 0;
  int j = 0;
  while (i < 16)
  {
    while (j < 8)
    {
      if (tablero[i][j][0] == 1)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(0, 255, 255));
      }
      else if (tablero[i][j][0] == 2)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(128, 0, 128));
      }
      else if (tablero[i][j][0] == 5)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(255, 255, 0));
      }
      else if (tablero[i][j][0] == 4)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(0, 0, 255));
      }
      else if (tablero[i][j][0] == 3)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(255, 128, 0));
      }
      else if (tablero[i][j][0] == 6)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(255, 0, 0));
      }
      else if (tablero[i][j][0] == 7)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(0, 255, 0));
      }
      else if (tablero[i][j][0] == 8)
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(255, 255, 255));
      }
      else
      {
        matriz.setPixelColor((i * 8) + j, matriz.Color(0, 0, 0));
      }
      j++;
    }
    j = 0;
    i++;
  }
  matriz.show();
}

/*PIXEL ART*/

void pixel() // Función principal del modo de pixelart
{
  delay(50);
  upButtonState = digitalRead(up);
  downButtonState = digitalRead(down);

  redButtonState = digitalRead(redButton);
  greenButtonState = digitalRead(greenButton);
  blueButtonState = digitalRead(blueButton);
  set_color();
  int x_data = analogRead(A0);
  int y_data = analogRead(A1);

  int sw_data = digitalRead(4);

  if (y_data < 100)
  {
    tablero[x][y][0] = aux[0];
    tablero[x][y][1] = aux[1];
    tablero[x][y][2] = aux[2];
    x--;
    if (x < 0)
    {
      x = 0;
    }
    aux[0] = tablero[x][y][0];
    aux[1] = tablero[x][y][1];
    aux[2] = tablero[x][y][2];
    tablero[x][y][0] = 10;
    tablero[x][y][1] = 10;
    tablero[x][y][2] = 10;
  }
  if (y_data > 900)
  {
    tablero[x][y][0] = aux[0];
    tablero[x][y][1] = aux[1];
    tablero[x][y][2] = aux[2];
    x++;
    if (x > 7)
    {
      x = 7;
    }
    aux[0] = tablero[x][y][0];
    aux[1] = tablero[x][y][1];
    aux[2] = tablero[x][y][2];
    tablero[x][y][0] = 10;
    tablero[x][y][1] = 10;
    tablero[x][y][2] = 10;
  }
  if (x_data < 100)
  {
    tablero[x][y][0] = aux[0];
    tablero[x][y][1] = aux[1];
    tablero[x][y][2] = aux[2];
    y--;
    if (y < 0)
    {
      y = 0;
    }
    aux[0] = tablero[x][y][0];
    aux[1] = tablero[x][y][1];
    aux[2] = tablero[x][y][2];
    tablero[x][y][0] = 10;
    tablero[x][y][1] = 10;
    tablero[x][y][2] = 10;
  }
  if (x_data > 900)
  {
    tablero[x][y][0] = aux[0];
    tablero[x][y][1] = aux[1];
    tablero[x][y][2] = aux[2];
    y++;
    if (y > 7)
    {
      y = 7;
    }
    aux[0] = tablero[x][y][0];
    aux[1] = tablero[x][y][1];
    aux[2] = tablero[x][y][2];
    tablero[x][y][0] = 10;
    tablero[x][y][1] = 10;
    tablero[x][y][2] = 10;
  }
  print_matrix2();
  if (upButtonState == HIGH)
  {
    aux[0] = redValue;
    aux[1] = greenValue;
    aux[2] = blueValue;
  }
  if (downButtonState == HIGH)
  {
    aux[0] = 0;
    aux[1] = 0;
    aux[2] = 0;
  }
}

void set_color() // Cambia el color de la paleta
{
  int i = 9;
  int j = 1;

  if (redButtonState == HIGH)
  {
    tablero[13][1][0] = 255;
    tablero[13][2][0] = 255;
    tablero[14][1][0] = 255;
    tablero[14][2][0] = 255;
    redValue = values(redValue);
    print_matrix2();
  }
  else
  {
    tablero[13][1][0] = 10;
    tablero[13][2][0] = 10;
    tablero[14][1][0] = 10;
    tablero[14][2][0] = 10;
    print_matrix2();
  }
  if (greenButtonState == HIGH)
  {
    tablero[13][3][1] = 255;
    tablero[13][4][1] = 255;
    tablero[14][3][1] = 255;
    tablero[14][4][1] = 255;
    greenValue = values(greenValue);
    print_matrix2();
  }
  else
  {
    tablero[13][3][1] = 10;
    tablero[13][4][1] = 10;
    tablero[14][3][1] = 10;
    tablero[14][4][1] = 10;
    print_matrix2();
  }
  if (blueButtonState == HIGH)
  {
    tablero[13][5][2] = 255;
    tablero[13][6][2] = 255;
    tablero[14][5][2] = 255;
    tablero[14][6][2] = 255;
    blueValue = values(blueValue);
    print_matrix2();
  }
  else
  {
    tablero[13][5][2] = 10;
    tablero[13][6][2] = 10;
    tablero[14][5][2] = 10;
    tablero[14][6][2] = 10;
    print_matrix2();
  }
  while (i < 13)
  {
    while (j < 7)
    {

      tablero[i][j][0] = redValue;
      tablero[i][j][1] = greenValue;
      tablero[i][j][2] = blueValue;
      j++;
    }
    j = 1;
    i++;
  }
  print_matrix2();
}

void print_matrix2() // Versión alternativa de print_matrix para la parte de pixelart
{
  int i = 0;
  int j = 0;
  while (i < 16)
  {
    while (j < 8)
    {
      matriz.setPixelColor((i * 8) + j, matriz.Color(tablero[i][j][0], tablero[i][j][1], tablero[i][j][2]));
      j++;
    }
    j = 0;
    i++;
  }
  matriz.show();
}

int values(int valor) // Cambia el valor de los colores
{
  value = valor;
  if (downButtonState == HIGH)
  {
    value -= 10;
    if (value < 0)
    {
      value = 0;
    }
  }
  if (upButtonState == HIGH)
  {
    value += 10;
    if (value > 255)
    {
      value = 255;
    }
  }
  delay(100);
  return value;
}

/*MUSIC*/

int play_note(int i) // Toca la nota i
{
  analogWrite(11, 0);
  // Serial.println(i);
  int noteDuration = 1000 / durations(i);
  tone(11, song(i), noteDuration);
  // analogWrite(9, song(i));
  noteDuration *= 1.30;
  // delay(noteDuration);
  return noteDuration;
}

int song(int i) // Función que almacena como si fuera un array
{
  switch (i)
  {
  case 0:
  case 5:
  case 13:
  case 23:
  case 41:
  case 45:
  case 56:
  case 64:
  case 69:
  case 77:
  case 87:
  case 105:
  case 109:
  case 120:
  case 192:
  case 197:
  case 205:
  case 215:
  case 233:
  case 237:
  case 248:
  case 256:
  case 261:
  case 269:
  case 279:
  case 297:
  case 301:
  case 312:
    return 659;
  case 1:
  case 22:
  case 24:
  case 61:
  case 65:
  case 86:
  case 88:
  case 125:
  case 129:
  case 131:
  case 133:
  case 135:
  case 137:
  case 139:
  case 141:
  case 143:
  case 145:
  case 147:
  case 149:
  case 151:
  case 153:
  case 155:
  case 157:
  case 159:
  case 161:
  case 163:
  case 165:
  case 167:
  case 169:
  case 171:
  case 173:
  case 175:
  case 177:
  case 179:
  case 181:
  case 183:
  case 185:
  case 187:
  case 189:
  case 191:
  case 193:
  case 214:
  case 216:
  case 253:
  case 257:
  case 278:
  case 280:
  case 317:
  case 321:
  case 323:
  case 325:
  case 327:
  case 329:
  case 331:
  case 333:
  case 335:
  case 337:
  case 339:
  case 341:
  case 343:
  case 345:
  case 347:
  case 349:
  case 351:
  case 353:
  case 355:
  case 357:
  case 359:
  case 361:
  case 363:
  case 365:
  case 367:
  case 369:
  case 371:
  case 373:
  case 375:
  case 377:
  case 379:
  case 381:
  case 383:
    return 165;
  case 2:
  case 8:
  case 17:
  case 50:
  case 52:
  case 66:
  case 72:
  case 81:
  case 114:
  case 116:
  case 194:
  case 200:
  case 209:
  case 242:
  case 244:
  case 258:
  case 264:
  case 273:
  case 306:
  case 308:
    return 494;
  case 3:
  case 7:
  case 12:
  case 16:
  case 20:
  case 25:
  case 37:
  case 38:
  case 44:
  case 49:
  case 53:
  case 58:
  case 67:
  case 71:
  case 76:
  case 80:
  case 84:
  case 89:
  case 101:
  case 102:
  case 108:
  case 113:
  case 117:
  case 122:
  case 195:
  case 199:
  case 204:
  case 208:
  case 212:
  case 217:
  case 229:
  case 230:
  case 236:
  case 241:
  case 245:
  case 250:
  case 259:
  case 263:
  case 268:
  case 272:
  case 276:
  case 281:
  case 293:
  case 294:
  case 300:
  case 305:
  case 309:
  case 314:
    return 523;
  case 4:
  case 6:
  case 15:
  case 21:
  case 34:
  case 48:
  case 54:
  case 68:
  case 70:
  case 79:
  case 85:
  case 98:
  case 112:
  case 118:
  case 196:
  case 198:
  case 207:
  case 213:
  case 226:
  case 240:
  case 246:
  case 260:
  case 262:
  case 271:
  case 277:
  case 290:
  case 304:
  case 310:
    return 587;
  case 9:
  case 11:
  case 27:
  case 29:
  case 46:
  case 60:
  case 62:
  case 73:
  case 75:
  case 91:
  case 93:
  case 110:
  case 124:
  case 126:
  case 180:
  case 201:
  case 203:
  case 219:
  case 221:
  case 238:
  case 252:
  case 254:
  case 265:
  case 267:
  case 283:
  case 285:
  case 302:
  case 316:
  case 318:
  case 372:
    return 440;
  case 10:
  case 14:
  case 26:
  case 28:
  case 30:
  case 74:
  case 78:
  case 90:
  case 92:
  case 94:
  case 148:
  case 202:
  case 206:
  case 218:
  case 220:
  case 222:
  case 266:
  case 270:
  case 282:
  case 284:
  case 286:
  case 340:
    return 220;
  case 18:
  case 51:
  case 59:
  case 82:
  case 115:
  case 123:
  case 128:
  case 160:
  case 178:
  case 210:
  case 243:
  case 251:
  case 274:
  case 307:
  case 315:
  case 320:
  case 352:
  case 370:
    return 330;
  case 19:
  case 47:
  case 55:
  case 57:
  case 83:
  case 111:
  case 119:
  case 121:
  case 211:
  case 239:
  case 247:
  case 249:
  case 275:
  case 303:
  case 311:
  case 313:
    return 392;
  case 31:
  case 95:
  case 223:
  case 287:
    return 123;
  case 32:
  case 42:
  case 96:
  case 106:
  case 224:
  case 234:
  case 288:
  case 298:
    return 131;
  case 33:
  case 97:
  case 225:
  case 289:
    return 147;
  case 35:
  case 40:
  case 99:
  case 104:
  case 227:
  case 232:
  case 291:
  case 296:
    return 698;
  case 36:
  case 100:
  case 228:
  case 292:
    return 880;
  case 39:
  case 103:
  case 231:
  case 295:
    return 784;
  case 43:
  case 63:
  case 107:
  case 127:
  case 235:
  case 255:
  case 299:
  case 319:
    return 0;
  case 130:
  case 134:
  case 146:
  case 150:
  case 162:
  case 166:
  case 182:
  case 322:
  case 326:
  case 338:
  case 342:
  case 354:
  case 358:
  case 374:
    return 110;
  case 132:
  case 144:
  case 164:
  case 176:
  case 324:
  case 336:
  case 356:
  case 368:
    return 262;
  case 136:
  case 168:
  case 328:
  case 360:
    return 294;
  case 138:
  case 142:
  case 154:
  case 158:
  case 170:
  case 174:
  case 186:
  case 188:
  case 190:
  case 330:
  case 334:
  case 346:
  case 350:
  case 362:
  case 366:
  case 378:
  case 380:
  case 382:
    return 104;
  case 140:
  case 156:
  case 172:
  case 332:
  case 348:
  case 364:
    return 247;
  case 152:
  case 344:
    return 208;
  case 184:
  case 376:
    return 415;
  }
}

int durations(int i) // Igual que la anterior, pero con las duraciones
{
  switch (i)
  {
  case 5:
  case 6:
  case 37:
  case 38:
  case 46:
  case 47:
  case 69:
  case 70:
  case 101:
  case 102:
  case 110:
  case 111:
  case 197:
  case 198:
  case 229:
  case 230:
  case 238:
  case 239:
  case 261:
  case 262:
  case 293:
  case 294:
  case 302:
  case 303:
    return 16;
  case 34:
  case 62:
  case 63:
  case 98:
  case 126:
  case 127:
  case 226:
  case 254:
  case 255:
  case 290:
  case 318:
  case 319:
    return 4;
  default:
    return 8;
  }
}