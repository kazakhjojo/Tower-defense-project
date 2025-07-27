#include <stdio.h>

typedef struct
{
  char head_;
  char body_;
  char legs_;
} EnemyTexture;
typedef struct
{
  char *top_;
  char *middle_;
  char *bottom_;
} TowerTexture;
typedef struct
{
  char path_texture_;
  char placement_space_texture_;
  char map_edges_texture_;
} MapTextures;
void appendEnemyTexture(EnemyTexture *input)
{
  input->head_ = 'o';
  input->body_ = '|';
  input->legs_ = '^';
}
void appendTowerTexture(TowerTexture *input)
{
  input->top_ = "TTT";
  input->middle_ = " | ";
  input->bottom_ = "|||";
}
void appendMapTextures(MapTextures *input)
{
  input->path_texture_ = '#';
  input->placement_space_texture_ = '.';
  input->map_edges_texture_ = '=';
}
void printEnemy(EnemyTexture output)
{
  printf("%c\n%c\n%c\n", output.head_, output.body_, output.legs_);
}
void printTower(TowerTexture output)
{
  printf("%s\n%s\n%s\n", output.top_, output.middle_, output.bottom_);
}
void printMap()
{
  
}
int main(void)
{
  EnemyTexture enemy_general;
  TowerTexture tower_general;
  appendEnemyTexture(&enemy_general);
  appendTowerTexture(&tower_general);
  printEnemy(enemy_general);
  printTower(tower_general);

  return 0;
}