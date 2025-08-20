#include <stdio.h>
#define IN_RANGE 500
#define OUT_OF_RANGE 501
#define LENGTH_OF_PATH 3

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
  int x_coordinates_;
  int y_coordinates_;
  int top_x_coordinates_;
  int top_y_coordinates_;
  int middle_x_coordinates_;
  int middle_y_coordinates_;
  int bottom_x_coordinates_;
  int bottom_y_coordinates_;
} Tower;
typedef struct
{
  char path_texture_;
  char placement_space_texture_;
  char map_edges_texture_;
  char map_empty_space_textures_;
} MapTextures;
typedef struct 
{
  int height_;
  int width_;
  int beginning_row_;
  int ending_row_;
  int beginning_column_;
  int ending_column_;
  int coordinate_of_path_;
} MapCharacteristics;
int inBetween(int comparable_value, int the_lower_bound, int the_upper_bound)
{
  if(the_lower_bound < comparable_value && comparable_value < the_upper_bound)
  {
    return IN_RANGE;
  }
  else
  {
    return OUT_OF_RANGE;
  }
}
int ceil_My(double number)
{
  double temporary_variable = number - (int)number;
  int the_result;
  if (temporary_variable != 0)
  {
    double the_difference = 1 - temporary_variable;
    number = number + the_difference;
  }
  the_result = (int)number;
  return the_result;
}
void appendEnemyTexture(EnemyTexture *input)
{
  input->head_ = 'o';
  input->body_ = '|';
  input->legs_ = '^';
}
void appendTowerTexture(Tower *input)
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
  input->map_empty_space_textures_ = ' ';
}
void appendTowerCoordinates(Tower *input)
{
  printf("Please insert coordinates at which you want to place your tower\n");
  printf("horizontal coordinates(x axis): ");
  scanf("%d", &input->x_coordinates_);
  printf("vertical coordinates(y axis): ");
  scanf("%d", &input->y_coordinates_);
}
void hitBoxTower(Tower *input)
{
  input->top_x_coordinates_ = input->x_coordinates_ - 1;
  input->top_y_coordinates_ = input->y_coordinates_ - 2;
  input->middle_x_coordinates_ = input->x_coordinates_;
  input->middle_y_coordinates_ = input->y_coordinates_ - 1;
  input->bottom_x_coordinates_ = input->x_coordinates_ - 1;
  input->bottom_y_coordinates_ = input->y_coordinates_;
}
void appendMapCharacteristics(MapCharacteristics *input)
{
  printf("Map height: ");
  scanf("%d", &input->height_);
  // printf("Map width: ");
  // scanf("%d", &input->width_);
  input->width_ = 21;
  input->beginning_row_ = 0;
  input->ending_row_ = input->height_ - 1;
  input->beginning_column_ = 0;
  input->ending_column_ = input->width_ - 1;
}
void printEnemy(EnemyTexture output)
{
  printf("%c\n%c\n%c\n", output.head_, output.body_, output.legs_);
}
void printTower(Tower output)
{
  printf("%s\n%s\n%s\n", output.top_, output.middle_, output.bottom_);
}
void printMap(MapCharacteristics the_map, MapTextures map_textures)
{
  for(int y_axis = 0; y_axis < the_map.height_; y_axis++)
  {
    for(int x_axis = 0; x_axis < the_map.width_; x_axis++)
    {
      if((inBetween(y_axis, the_map.beginning_row_, the_map.ending_row_) == IN_RANGE) && (inBetween(x_axis, the_map.beginning_column_, the_map.ending_column_) == IN_RANGE))
      {
        if(x_axis == (ceil_My((double)the_map.width_ / 2)) - 2)
        {
          for (int index_3 = 0; index_3 < 3; index_3++)
          {
            printf("%c", map_textures.path_texture_);
            if(index_3 == 2)
            {
              x_axis += index_3;
            }
          }
        }
        else
        {
          printf("%c", map_textures.map_empty_space_textures_);
        }

      }
      else
      {
        printf("%c", map_textures.map_edges_texture_);
      }
    }
    printf("\n");
  }
}
int main(void)
{
  EnemyTexture enemy_general;
  Tower tower_general;
  MapCharacteristics map;
  MapTextures the_textures;
  appendEnemyTexture(&enemy_general);
  appendTowerTexture(&tower_general);
  appendMapTextures(&the_textures);
  appendMapCharacteristics(&map);
  // printEnemy(enemy_general);
  // printTower(tower_general);
  printMap(map, the_textures);
  // printf("%d", ceil_My(10));
  return 0;
}